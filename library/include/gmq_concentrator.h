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

#ifndef GMQ_CONCENTRATOR_H
#define GMQ_CONCENTRATOR_H

#include "global_mq_common.h"
#include "marshalling.h"
#include <thread>
#include <mutex>
#include <condition_variable>


namespace globalmq::marshalling {

template<class ExpectedType, class ... ExpectedTypes>
class Concentrator
{
	// TODO:
	// 1. new/delete usage
	// 2. unexpected type

	void* data = nullptr;
	size_t type;

	template<class Type>
	bool init( uint64_t type_, Type** data_ )
	{
		if ( Type::numTypeID == type_ )
		{
			*data_ = new Type;
			type = type_;
			return true;
		}
		return false;
	}

	template<class Type, class NextT, class ... Types>
	bool init( uint64_t type_, Type** data_ )
	{
		if ( Type::numTypeID == type_ )
		{
			*data_ = new Type;
			type = type_;
			return true;
		}
		else
			return init<NextT, Types...>( type_, (NextT**)(data_) );
	}

	void deleteData() {
//		if ( data != nullptr )
	}
public:
	Concentrator( uint64_t type_ ) { 
		bool ok = init<ExpectedType, ExpectedTypes...>( type_, (ExpectedType**)(&data) ); 
		assert( ok );
	}
	~Concentrator() { deleteData(); }
};

} // namespace globalmq::marshalling

#endif // GMQ_CONCENTRATOR_H
