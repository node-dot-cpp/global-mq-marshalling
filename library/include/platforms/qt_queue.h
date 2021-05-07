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

#ifndef GMQ_QT_QUEUE_H
#define GMQ_QT_QUEUE_H

#include "global_mq_common.h"
#include "marshalling.h"


namespace globalmq::marshalling {

template<class InterThreadMsgT>
class QtGmqEvent : public QEvent
{
	uintptr_t uptr = 0;
public:
	QtGmqEvent( int eventType, InterThreadMsgT&& msg) : QEvent( (Type)eventType ), uptr((uintptr_t)(msg.convertToPointer())) {}
	InterThreadMsgT&& getMsg() {
		InterThreadMsgT ret;
		ret.restoreFromPointer( static_cast<InterThreadMsgT>( uptr ) );
		return ret; 
	}
};


template<class InterThreadMsgT>
class QtPostman : public InProcessMessagePostmanBase
{
	QWidget* widget = nullptr;
	int eventType; // typically QEvent::Type::User + XXX
public:
	QtPostman( QWidget* widget_, int eventType_ ) : widget( widget_ ), eventType( eventType_ ) {}
	virtual ~QtPostman() {}
	virtual void postMessage( InterThreadMsgT&& msg ) override
	{
		// create a move-copy in heap, otherwise the msg will be destructed at the end of this call (and, potentially, before it will be received and processed)
		QtGmqEvent<InterThreadMsgT>* mEvent = nodecpp::stdalloc<QtGmqEvent<InterThreadMsgT>>( 1, eventType, std::move(msg) );
		QApplication::postEvent( widget, mEvent );
	}
};

template<class PlatformSupportT>
class GMQQtTransport : public GMQTransportBase<PlatformSupportT>
{
	using BufferT = typename PlatformSupportT::BufferT;

public:
	GMQQtTransport( GMQueue<PlatformSupportT>& gmq_, GMQ_COLL string name, QWidget* widget, int qtEventType ) : GMQTransportBase( gmq_, name, GMQueue<PlatformSupportT>::allocPostman<QtPostman<BufferT>, QWidget*, int>( widget, qtEventType ) )/*, postman( widget, qtEventType )*/ {}
	GMQQtTransport( GMQueue<PlatformSupportT>& gmq_, QWidget* widget, int qtEventType ) : GMQTransportBase( gmq_, GMQueue<PlatformSupportT>::allocPostman<QtPostman<BufferT>, QWidget*, int>( std::move(widget), std::move(qtEventType) ) )/*, postman( widget, qtEventType )*/ {}
	virtual ~GMQQtTransport() {}
};

} // namespace globalmq::marshalling

#endif // GMQ_QT_QUEUE_H
