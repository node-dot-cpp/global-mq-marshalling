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

#ifndef GMQ_HWND_QUEUE_H
#define GMQ_HWND_QUEUE_H

#include "../global_mq_common.h"
#include "../gmqueue.h"
//#include "marshalling.h"


namespace globalmq::marshalling {

class HwndPostman : public InProcessMessagePostmanBase
{
	HWND hWnd;
	uint32_t msgType; // typically WM_USER + XXX
public:
	HwndPostman( HWND hWnd_, uint32_t msgType_ ) : hWnd( hWnd_ ), msgType( msgType_ ) {}
	virtual ~HwndPostman() {}
	virtual void postMessage( InterThreadMsg&& msg ) override
	{
		// create a move-copy in heap, otherwise the msg will be destructed at the end of this call (and, potentially, before it will be received and processed)
		PostMessage(hWnd, msgType, WPARAM(msg.convertToPointer()), 0 );
	}
};

class GMQHwndTransport : public GMQTransportBase
{
	HwndPostman postman;

public:
	GMQHwndTransport( GMQueue& gmq, GMQ_COLL string_literal name, HWND hwnd, uint32_t msgType ) : GMQTransportBase( gmq, name, &postman ), postman( hwnd, msgType ) {}
};

} // namespace globalmq::marshalling

#endif //  GMQ_HWND_QUEUE_H
