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

#ifndef INPROC_QUEUE_H
#define INPROC_QUEUE_H

#include "global_mq_common.h"
#include "marshalling.h"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace globalmq::marshalling {


template<class T, size_t maxsz_bits>
class CircularBuffer {
	static constexpr size_t bufsz = 1 << maxsz_bits;
	static constexpr size_t maxsz = bufsz - 1;
	//-1 to make sure that head==tail always means 'empty'
	static constexpr size_t mask = maxsz;
	size_t head = 0;
	size_t tail = 0;
	alignas(T) uint8_t buffer[bufsz * sizeof(T)];
	//Having buffer as T[bufsz] is possible 
	//  IF we'll replace placement move constructors with move assignments
	//  AND drop explicit destructor calls
	//However, it will require T to have a default constructor,
	//  so at the moment I prefer to deal with pure buffers
	//  and to have the only requirement that T is move-constructible

public:
	using value_type = T;
public:
	size_t size() {
		return head - tail +
			(((size_t)(head >= tail) - (size_t)1) & bufsz);
		//trickery to avoid pipeline stalls via arithmetic
		//supposedly equivalent to:
		//if(head >= tail)
		//  return head - tail;
		//else
		//  return head + bufsz - tail;
	}

	bool is_full() { return size() == maxsz; }

	void push_back(T&& t) {
//		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, size() < maxsz);
		GMQ_ASSERT( size() < maxsz);
		new(tbuffer(head)) T(std::move(t));
		head = (head + 1) & mask;
	}

	T pop_front() {
//		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, size() > 0);
		GMQ_ASSERT( size() > 0);
		T* ttail = tbuffer(tail);
		T ret = std::move(*ttail);
		ttail->~T();
		tail = (tail + 1) & mask;
		return ret;
	}

private:
	T* tbuffer(size_t idx) {
		return reinterpret_cast<T*>(buffer + (idx * sizeof(T)));
	}
};

template <class FixedSizeCollection>
class MWSRFixedSizeQueueWithFlowControl {
private:
	std::mutex mx;
	std::condition_variable waitrd;
	std::condition_variable waitwr;
	FixedSizeCollection coll;
	bool killflag = false;

	//stats:
	int nfulls = 0;
	size_t hwmsize = 0;//high watermark on queue size

public:
	using T = typename FixedSizeCollection::value_type;

	MWSRFixedSizeQueueWithFlowControl() {}
	MWSRFixedSizeQueueWithFlowControl( const MWSRFixedSizeQueueWithFlowControl& ) = delete;
	MWSRFixedSizeQueueWithFlowControl& operator = ( const MWSRFixedSizeQueueWithFlowControl& ) = delete;
	MWSRFixedSizeQueueWithFlowControl( MWSRFixedSizeQueueWithFlowControl&& ) = delete;
	MWSRFixedSizeQueueWithFlowControl& operator = ( MWSRFixedSizeQueueWithFlowControl&& ) = delete;
	void push_back(T&& it) {
		//if the queue is full, BLOCKS until some space is freed
		{//creating scope for lock
			std::unique_lock<std::mutex> lock(mx);
			while (coll.is_full() && !killflag) {
				waitwr.wait(lock);
				++nfulls;
				//this will also count spurious wakeups,
				//  but they're supposedly rare
			}

			if (killflag)
				return;
//			NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, !coll.is_full());
			GMQ_ASSERT( !coll.is_full());
			coll.push_back(std::move(it));
			size_t sz = coll.size();
			hwmsize = std::max(hwmsize, sz);
		}//unlocking mx

		waitrd.notify_one();
	}

	std::pair<bool, T> pop_front() {
		std::unique_lock<std::mutex> lock(mx);
		while (coll.size() == 0 && !killflag) {
			waitrd.wait(lock);
		}
		if (killflag)
			return std::pair<bool, T>(false, T());

//		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, coll.size() > 0);
		GMQ_ASSERT( coll.size() > 0);
//		T ret = std::move(coll.front());
//		coll.pop_front();
		T ret = std::move(coll.pop_front());
		lock.unlock();
		waitwr.notify_one();

		return std::pair<bool, T>(true, std::move(ret));
	}

	size_t pop_front( T* messages, size_t count ) {
		std::unique_lock<std::mutex> lock(mx);
		while (coll.size() == 0 && !killflag) {
			waitrd.wait(lock);
		}
		if (killflag)
			return 0;

//		NODECPP_ASSERT( nodecpp::module_id, ::nodecpp::assert::AssertLevel::critical, coll.size() > 0);
		GMQ_ASSERT( coll.size() > 0);
		size_t sz2move = count <= coll.size() ? count : coll.size();
		for ( size_t i=0; i<sz2move; ++i )
			messages[i] = std::move(coll.pop_front());
		lock.unlock();
		waitwr.notify_one();

		return sz2move;
	}

	size_t pop_front( T* messages, size_t count, uint64_t timeout ) {
		std::unique_lock<std::mutex> lock(mx);
		bool expired = false;
		while (coll.size() == 0 && !expired && !killflag) {
			expired = waitrd.wait_for(lock, std::chrono::milliseconds(timeout)) == std::cv_status::timeout;
		}
		if (killflag)
			return 0;

		size_t sz2move = count <= coll.size() ? count : coll.size();
		for ( size_t i=0; i<sz2move; ++i )
			messages[i] = std::move(coll.pop_front());
		lock.unlock();
		waitwr.notify_one();

		return sz2move;
	}

	void kill() {
		{//creating scope for lock
			std::unique_lock<std::mutex> lock(mx);
			killflag = true;
		}//unlocking mx

		waitrd.notify_all();
		waitwr.notify_all();
	}
};

template<class InterThreadMsgT>
struct ThreadQueueItem
{
	static constexpr uint64_t invalidRecipientID = (uint64_t)(-1);
	InterThreadMsgT msg;
	uint64_t recipientID = invalidRecipientID;
	ThreadQueueItem() {}
	ThreadQueueItem( InterThreadMsgT&& msg_, uint64_t recipientID_ ) : msg( std::move( msg_ ) ), recipientID( recipientID_ ) {}
	ThreadQueueItem( const ThreadQueueItem& other ) = delete;
	ThreadQueueItem& operator = ( const ThreadQueueItem& other ) = delete;
	ThreadQueueItem( ThreadQueueItem&& other ) {
		msg = std::move( other.msg );
		recipientID = other.recipientID;
		other.recipientID = invalidRecipientID;
	}
	ThreadQueueItem& operator = ( ThreadQueueItem&& other ) {
		msg = std::move( other.msg );
		recipientID = other.recipientID;
		other.recipientID = invalidRecipientID;
		return *this;
	}
};

template<class InterThreadMsgT>
class ThreadQueuePostman : public InProcessMessagePostmanBase
{
public:
	using MsgQueue = MWSRFixedSizeQueueWithFlowControl<CircularBuffer<ThreadQueueItem<InterThreadMsgT>, 4>>; // TODO: revise the second param value

private:
	MsgQueue& msgQueue;
	uint64_t recipientID;

public:
	ThreadQueuePostman( MsgQueue& msgQueue_, uint64_t recipientID_ ) : msgQueue( msgQueue_ ), recipientID( recipientID_ ) {}
	virtual ~ThreadQueuePostman() {}
	virtual void postMessage( InterThreadMsgT&& msg ) override
	{
		msgQueue.push_back( ThreadQueueItem<InterThreadMsgT>({std::move( msg ), recipientID}) );
	}
};

template<class PlatformSupportT>
class GMQThreadQueueTransport : public GMQTransportBase<PlatformSupportT>
{
	using BufferT = typename PlatformSupportT::BufferT;

public:
	GMQThreadQueueTransport( GMQueue<PlatformSupportT>& gmq, GMQ_COLL string name, typename ThreadQueuePostman<BufferT>::MsgQueue& queue, uint64_t recipientID ) : GMQTransportBase( gmq, name, gmq.template allocPostman<ThreadQueuePostman<BufferT>>( queue, recipientID ) )/*, postman( queue, recipientID )*/ {}
	GMQThreadQueueTransport( GMQueue<PlatformSupportT>& gmq, typename ThreadQueuePostman<BufferT>::MsgQueue& queue, int recipientID ) : GMQTransportBase( gmq, gmq.template allocPostman<ThreadQueuePostman<BufferT>>( queue, recipientID ) )/*, postman( queue, recipientID )*/ {}
	virtual ~GMQThreadQueueTransport() {}
};

} // namespace globalmq::marshalling

#endif // INPROC_QUEUE_H
