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

#ifndef GMQ_GT_QUEUE_H
#define GMQ_GT_QUEUE_H

#include "global_mq_common.h"
#include "marshalling.h"


namespace globalmq::marshalling {

class QtGmqEvent : public QEvent
{
	InterThreadMsgPtr ptr;
public:
	QtGmqEvent( int eventType, InterThreadMsgPtr ptr_) : QEvent( (Type)eventType ), ptr(ptr_) {}
	InterThreadMsgPtr getPtr() { return ptr; }
	void setPtr(InterThreadMsgPtr ptr_) {
		ptr = ptr_;
	}
};


class Widget;
class QtPostman : public InProcessMessagePostmanBase
{
	Widget* widget = nullptr;
	int eventType; // typically QEvent::Type::User + XXX
public:
	QtPostman( Widget* widget_, int eventType_ ) : widget( widget_ ), eventType( eventType_ ) {}
	virtual ~QtPostman() {}
	virtual void postMessage( InterThreadMsg&& msg ) override
	{
		// create a move-copy in heap, otherwise the msg will be destructed at the end of this call (and, potentially, before it will be received and processed)
		QtGmqEvent* mEvent = nodecpp::stdalloc<QtGmqEvent>( 1 );
		new(mEvent) QtGmqEvent( eventType, msg.convertToPointer() );
		QApplication::postEvent( widget, mEvent );
	}
};

class GMQQtTransport : public GMQTransportBase
{
	QtPostman postman;
	GMQQtTransport( GMQueue& gmq, GMQ_COLL string_literal name, HWND hwnd, uint32_t msgType ) : GMQTransportBase( gmq, &postman ), postman( hwnd, msgType ) {}
};

} // namespace globalmq::marshalling

#endif // GMQ_GT_QUEUE_H
