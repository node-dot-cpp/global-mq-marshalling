/* -------------------------------------------------------------------------------
* Copyright (c) 2021, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#ifndef GMQUEUE_H
#define GMQUEUE_H

#include "global_mq_common.h"
#include "marshalling.h"


namespace globalmq::marshalling {

struct GMQPublicAddress
{
	std::string machine;
	std::string node;
	std::string object;
};

class InProcessMessagePostmanBase
{
public:
	InProcessMessagePostmanBase() {};
	virtual void postMessage( InterThreadMsg&& ) = 0;
	virtual ~InProcessMessagePostmanBase() {}
};

class AddressableLocation
{
	InProcessMessagePostmanBase* postman = nullptr;
	uint64_t reincarnation = 0;
};

struct SlotIdx
{
	static constexpr size_t invalid_idx = (size_t)(-1);
	static constexpr size_t invalid_reincarnation = (size_t)(-1);
	size_t idx = invalid_idx;
	uint64_t reincarnation = invalid_reincarnation;
};

class AddressableLocations // one per process
{
	std::vector<AddressableObject> slots; // mx-protected!
public:
	SlotIdx add( InProcessMessagePostmanBase* postman )
	{ 
		// TODO: essentially add to slots and return its idx
	}
	void remove( SlotIdx idx )
	{ 
		// TODO: find by idx.idx, check reincarnaion, set postman to null
	}
};

extern AddressableLocations addressableLocations;
extern AddressableLocations& getAddressableLocations() { return addressableLocations; }

class GMQueue
{
	std::unordered_map<GMQPublicAddress, AddressableLocation> recipients; // mx-protected

public:
	GMQueue() {}

	void add( GMQPublicAddress addr, SlotIdx idx ) {}
	void remove( GMQPublicAddress addr, SlotIdx idx ) {}
};

class GMQTransportBase
{
protected:
	GMQueue& gmq;
	SlotIdx idx;
	GMQ_COLL string_literal name;

public:
	GMQTransportBase( GMQueue& queue, GMQ_COLL string_literal name_, InProcessMessagePostmanBase* postman ) : gmq( queue ), name( name_ ) {
		idx = getAddressableLocations().add( postman );
		GMQPublicAddress addr;
		addr.node = name;
		queue.add( addr, idx );
	};
	virtual ~GMQTransportBase() {
		getAddressableLocations().remove( idx );
		queue.remove( addr, idx );
	}

	virtual void postMessage( InterThreadMsg&& ) = 0;
};

/*class GMQHwndTransport : public GMQTransportBase
{
//	GMQHwndTransport( GMQueue& gmq, HWND hwnd ) {}
};

class GMQQTTransport : public GMQTransportBase
{
};*/

class GMQInProcessQueueTransport : public GMQTransportBase
{
	GMQInProcessQueueTransport( GMQueue& queue ) : GMQTransportBase( queue ) {
	}
	GMQInProcessQueueTransport( GMQueue& queue, string_literal name ) : GMQTransportBase( queue ) {
	}
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
