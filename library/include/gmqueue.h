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
#include <thread>
#include <mutex>
#include <condition_variable>
#include "publishable_impl.h"


namespace globalmq::marshalling {

struct GMQBaseAddress
{
	std::string machine;
	std::string node;
};

struct GMQFullAddress : public GMQBaseAddress
{
	std::string object;
};

class InterThreadMsg;

class InProcessMessagePostmanBase
{
public:
	InProcessMessagePostmanBase() {};
	virtual void postMessage( InterThreadMsg&& ) = 0;
	virtual ~InProcessMessagePostmanBase() {}
};

struct AddressableLocation
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

class AddressableLocations // one per process; provides process-unique Slot with Postman and returns its SlotIdx
{
	std::mutex mx;
	std::vector<AddressableLocation> slots; // mx-protected!
public:
	SlotIdx add( InProcessMessagePostmanBase* postman )
	{ 
		std::unique_lock<std::mutex> lock(mx);
		// TODO: essentially add to slots and return its idx
		for ( size_t i = 0; i<slots.size(); ++i )
			if ( slots[i].postman == nullptr )
			{
				slots[i].postman = postman;
				++(slots[i].reincarnation);
				return SlotIdx({i,slots[i].reincarnation});
			}
		slots.push_back({postman, 0});
		return SlotIdx({slots.size() - 1, 0});
	}
	void remove( SlotIdx idx )
	{ 
		std::unique_lock<std::mutex> lock(mx);
		// find by idx.idx, check reincarnaion, set postman to null
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		slots[idx.idx].postman = nullptr;
	}
	InProcessMessagePostmanBase* getPostman( SlotIdx idx )
	{
		std::unique_lock<std::mutex> lock(mx);
		// access, verify, return
		assert ( idx.idx < slots.size() ); 
		assert ( idx.reincarnation == slots[idx.idx].reincarnation ); 
		return slots[idx.idx].postman;
	}
};

extern AddressableLocations& getAddressableLocations();

template<class PlatformSupportT>
class GMQueue
{
	using InputBufferT = typename PlatformSupportT::BufferT;
	using ComposerT = typename PlatformSupportT::ComposerT;

	class ConcentratorWrapper
	{
		StateConcentratorBase<InputBufferT, ComposerT>* ptr = nullptr;
	public:
		ConcentratorWrapper( StateConcentratorBase<InputBufferT, ComposerT>* ptr_ ) : ptr( ptr_ ) {}
		ConcentratorWrapper( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper& operator = ( const ConcentratorWrapper& ) = delete;
		ConcentratorWrapper( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; }
		ConcentratorWrapper& operator = ( ConcentratorWrapper&& other ) { ptr = other.ptr; other.ptr = nullptr; return *this; }
		~ConcentratorWrapper() { if ( ptr ) delete ptr; }
		StateConcentratorBase<InputBufferT, ComposerT>* operator -> () { return ptr; }
	};

	std::unordered_map<GMQ_COLL string, ConcentratorWrapper> concentrators; // address to concentrator, mx-protected
	std::unordered_map<GMQ_COLL string, AddressableLocation> recipients; // node name to location, mx-protected

public:
	GMQueue() {}

	/*void add( GMQBaseAddress addr, SlotIdx idx ) {}
	void addLocal( GMQ_COLL string name, SlotIdx idx ) {
		if ( name != "" )
		{
			add( name, idx );
		}
	}
	void remove( GMQBaseAddress addr, SlotIdx idx ) {}
	void removeLocal( GMQ_COLL string name, SlotIdx idx ) {
		if ( name != "" )
		{
			remove( name, idx );
		}
	}*/
};

template<class PlatformSupportT>
class GMQTransportBase
{
protected:
	GMQueue<PlatformSupportT>& gmq;
	SlotIdx idx;
	GMQ_COLL string name;

public:
	GMQTransportBase( GMQueue<PlatformSupportT>& queue, GMQ_COLL string name_, InProcessMessagePostmanBase* postman ) : gmq( queue ), name( name_ ) {
		idx = getAddressableLocations().add( postman );
		gmq.add( name, idx );
	};
	virtual ~GMQTransportBase() {
		getAddressableLocations().remove( idx );
		gmq.remove( name, idx );
	}

	virtual void postMessage( InterThreadMsg&& ) = 0;
};



} // namespace globalmq::marshalling

#endif // GMQUEUE_H
