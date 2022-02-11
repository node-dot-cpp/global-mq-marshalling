/* -------------------------------------------------------------------------------
* Copyright (c) 2022, OLogN Technologies AG
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

#ifndef DLLMAIN_H
#define DLLMAIN_H

#include <fmt/format.h>
#include <gmqueue.h>
#include <cstdint>

using namespace globalmq::marshalling;

typedef void (*GmqPostMessage)(uint64_t recipientID, uint64_t size, uint64_t capacity, uint8_t* data);

extern "C" __declspec(dllexport) void GmqInit(uint64_t id, uint64_t slotId, uint64_t reincarnation, GmqPostMessage postFunc);
extern "C" __declspec(dllexport) void GmqHandleMessage(uint64_t size, uint64_t capacity, uint8_t* data);

extern "C" __declspec(dllexport) void TestUpdatePublisher(int64_t number);
extern "C" __declspec(dllexport) void EndTick();




	class InteropPostman : public globalmq::marshalling::InProcessMessagePostmanBase
	{
	public:

	private:
		GmqPostMessage postFunc;
		uint64_t recipientID;

	public:
		InteropPostman( uint64_t recipientID_, GmqPostMessage postFunc ) : recipientID( recipientID_ ), postFunc(postFunc) {}
		virtual ~InteropPostman() {}
		virtual void postMessage( globalmq::marshalling::MessageBufferT&& msg ) override
		{
			globalmq::marshalling::BufferMemento memento = msg.releaseToMemento();
			postFunc(recipientID, memento.size, memento.capacity, memento.buffer);
		}
	};

	template<class PlatformSupportT>
	class InteropTransport : public globalmq::marshalling::GMQTransportBase<PlatformSupportT>
	{
		uint64_t id;
		SlotIdx idx;
		GmqPostMessage postFunc;

	public:
		InteropTransport( globalmq::marshalling::GMQueue<PlatformSupportT>& gmq, GMQ_COLL string name, uint64_t recipientID, GmqPostMessage postFunc, uint64_t id, SlotIdx idx )
			: globalmq::marshalling::GMQTransportBase<PlatformSupportT>( gmq, name, gmq.template allocPostman<InteropPostman>( recipientID, postFunc ) ), id(id), idx(idx), postFunc(postFunc) {}
		virtual ~InteropTransport() {}
		virtual void postMessage( MessageBufferT&& msg ){
			printf("GMQTransportBase::postMessage 0x%x\n", this);
		
			assert( idx.isInitialized() );
			globalmq::marshalling::BufferMemento memento = msg.releaseToMemento();
			postFunc(0, memento.size, memento.capacity, memento.buffer);

			// postFunc( std::move( msg ), id, idx );
		}

	};
	

#endif // DLLMAIN_H
