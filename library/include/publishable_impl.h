/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
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

#ifndef PUBLISHABLE_IMPL_H
#define PUBLISHABLE_IMPL_H

#include "global_mq_common.h"


namespace m {

template<class TB>
class VectorOfSimpleTypeRefWrapper
{
	TB& b;
public:
	VectorOfSimpleTypeRefWrapper( TB& actual ) : b( actual ) {}
	auto get_at( size_t idx ) { 
		if constexpr ( std::is_arithmetic<typename TB::value_type>::value )
			return b[idx];
		else
		{
			const auto& ret = b[idx];
			return ret;
		}
	}
};

template<class TB>
class VectorOfCompositeTypeRefWrapper
{
	TB& b;
public:
	VectorOfCompositeTypeRefWrapper( TB& actual ) : b( actual ) {}
	const auto& get_at( size_t idx ) { return b[idx]; }
};

template<class RefWrapperT, class TB>
class VectorOfStructRefWrapper
{
	TB& b;
public:
	VectorOfStructRefWrapper( TB& actual ) : b( actual ) {}
	auto get_at( size_t idx ) { return RefWrapperT(b[idx]); }
};

template<class TB, class RootT>
class VectorOfSimpleTypeRefWrapper4Set
{
	TB& b;
	RootT& root;
	GMQ_COLL vector<size_t> address;
public:
	VectorOfSimpleTypeRefWrapper4Set( TB& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : b( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	void remove( size_t idx ) { GMA_ASSERT( idx < b.size()); b.erase( b.begin() + idx ); }
	void insert_bafore( TB& what, size_t idx ) { GMA_ASSERT( idx < b.size()); b.insert( what, b.begin() + idx ); }
};

template<class RefWrapper4SetT, class TB, class RootT>
class VectorOfStructRefWrapper4Set
{
	TB& b;
	RootT& root;
	GMQ_COLL vector<size_t> address;
public:
	VectorOfStructRefWrapper4Set( TB& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : b( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get4set_at( size_t idx ) { return RefWrapper4SetT(b[idx], root, address, idx); }
	void remove( size_t idx ) { GMA_ASSERT( idx < b.size()); b.erase( b.begin() + idx ); }
	void insert_bafore( TB& what, size_t idx ) { GMA_ASSERT( idx < b.size()); b.insert( what, b.begin() + idx ); }
};


} // namespace m

#endif // PUBLISHABLE_IMPL_H
