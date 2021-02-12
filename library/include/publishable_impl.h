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
#include "marshalling.h"


namespace globalmq::marshalling {

enum ActionOnVector { update_at = 1, insert_single_before = 2, remove_at = 3 };

template<class VectorT>
class VectorOfSimpleTypeRefWrapper
{
	VectorT& b;
public:
	VectorOfSimpleTypeRefWrapper( VectorT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	auto get_at( size_t idx ) { 
		if constexpr ( std::is_arithmetic<typename VectorT::value_type>::value )
			return b[idx];
		else
		{
			const auto& ret = b[idx];
			return ret;
		}
	}
};

template<class VectorT>
class VectorOfCompositeTypeRefWrapper
{
	VectorT& b;
public:
	VectorOfCompositeTypeRefWrapper( VectorT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	const auto& get_at( size_t idx ) { return b[idx]; }
};

template<class RefWrapperT, class VectorT>
class VectorOfStructRefWrapper
{
	VectorT& b;
public:
	VectorOfStructRefWrapper( VectorT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	auto get_at( size_t idx ) { return RefWrapperT(b[idx]); }
};

template<class VectorT, class ElemTypeT, class RootT>
class VectorRefWrapper4Set
{
	void finalizeInsertOrUpdateAt( size_t idx ) { 
		if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
			impl::publishableComposeLeafeInteger( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
			impl::publishableComposeLeafeUnsignedInteger( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
			impl::publishableComposeLeafeReal( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
			impl::publishableComposeLeafeString( root.getComposer(), b[idx] );
		else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
		{
			impl::publishableComposeLeafeStructBegin( root.getComposer() );
			ElemTypeT::compose( root.getComposer(), b[idx] );
			impl::publishableComposeLeafeStructEnd( root.getComposer() );
		}
		else
			static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
	}

protected:
	VectorT& b;
	RootT& root;
	GMQ_COLL vector<size_t> address;

public:
	VectorRefWrapper4Set( VectorT& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : b( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}

	void remove( size_t idx ) { 
		GMQ_ASSERT( idx < b.size()); 
		b.erase( b.begin() + idx );
		impl::composeAddressInPublishable( root.getComposer(), address, idx );
		impl::composeActionInPublishable( root.getComposer(), ActionOnVector::remove_at, true );
	}

	void insert_before( size_t idx, typename VectorT::value_type& what ) { 
		GMQ_ASSERT( idx < b.size());
		b.insert( b.begin() + idx, what );
		impl::composeAddressInPublishable( root.getComposer(), address, idx );
		impl::composeActionInPublishable( root.getComposer(), ActionOnVector::insert_single_before, false );
		finalizeInsertOrUpdateAt( idx );
	}

	void set_at( typename VectorT::value_type what, size_t idx ) {
		GMQ_ASSERT( idx < b.size());
		b[idx] = what;
		impl::composeAddressInPublishable( root.getComposer(), address, idx );
		impl::composeActionInPublishable( root.getComposer(), ActionOnVector::update_at, false );
		finalizeInsertOrUpdateAt( idx );
	}
};

template<class VectorT, class ElemTypeT, class RootT, class RefWrapper4SetT>
class VectorOfStructRefWrapper4Set : public VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>
{
public:
	VectorOfStructRefWrapper4Set( VectorT& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : 
		VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>( actual, root_, address_, idx ) {}
	auto get4set_at( size_t idx ) { return RefWrapper4SetT(VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::b[idx], VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::root, VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::address, idx); }
};

class PublishableVectorProcessor
{
public:
	template<class ParserT, class VectorT, class ProcType>
	static
	void parseSingleValue( ParserT& parser, typename VectorT::value_type& value ) { 
		if constexpr ( std::is_same<ProcType, impl::SignedIntegralType>::value )
			impl::IntegerProcessor::parse<ParserT, typename VectorT::value_type>( parser, &value );
		else if constexpr ( std::is_same<ProcType, impl::UnsignedIntegralType>::value )
			impl::UnsignedIntegerProcessor::parse( parser, &value );
		else if constexpr ( std::is_same<ProcType, impl::RealType>::value )
			impl::RealProcessor::parse( parser, &value );
		else if constexpr ( std::is_same<ProcType, impl::StringType>::value )
			impl::StringProcessor::parse( parser, &value );
		else if constexpr ( std::is_base_of<impl::StructType, ProcType>::value )
		{
			impl::parseStructBegin( parser );
			ProcType::parse( parser, value );
			impl::parseStructEnd( parser );
		}
		else
			static_assert( std::is_same<ProcType, AllowedDataType>::value, "unsupported type" );
	}

	template<class ComposerT, class VectorT, class ElemTypeT>
	static
	void compose( ComposerT& composer, const VectorT& what ) { 
		size_t collSz = what.size();
		if constexpr ( ComposerT::proto == Proto::GMQ )
		{
			impl::composeUnsignedInteger( composer, collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
					impl::composeSignedInteger( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
					impl::composeUnsignedInteger( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
					impl::composeReal( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
					impl::composeString( composer, what[i] );
				else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
				{
					impl::composeStructBegin( composer );
					ElemTypeT::compose( composer, what[i] );
					impl::composeStructEnd( composer );
				}
				else
					static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
			}
		}
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			composer.buff.append( "[", 1 );
			for ( size_t i=0; i<collSz; ++i )
			{
				if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
					impl::json::composeSignedInteger( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
					impl::json::composeUnsignedInteger( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
					impl::json::composeReal( composer, what[i] );
				else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
					impl::json::composeString( composer, what[i] );
				else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
				{
					impl::composeStructBegin( composer );
					ElemTypeT::compose( composer, what[i] );
					impl::composeStructEnd( composer );
				}
				else
					static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
				if ( i + 1 < collSz ) 
					composer.buff.append( ", ", 2 );
			}
			composer.buff.append( "]", 1 );
		}
	}

	template<class ComposerT, class VectorT, class ElemTypeT, typename NameT>
	static
	void compose( ComposerT& composer, const VectorT& what, NameT name, bool addListSeparator ) { 
		if constexpr ( ComposerT::proto == Proto::GMQ )
			compose<ComposerT, VectorT, ElemTypeT>( composer, what );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			impl::json::addNamePart( composer, name );
			compose<ComposerT, VectorT, ElemTypeT>( composer, what );
			if ( addListSeparator )
				composer.buff.append( ",", 1 );
		}
	}

	template<class ParserT, class VectorT, class ElemTypeT>
	static
	void parse( ParserT& parser, VectorT& dest ) { 
		if constexpr ( ParserT::proto == Proto::GMQ )
		{
			size_t collSz;
			parser.parseUnsignedInteger( &collSz );
			dest.reserve( collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				typename VectorT::value_type what;
				if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
					parser.parseSignedInteger( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
					parser.parseUnsignedInteger( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
					parser.parseReal( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
					parser.parseString( &what );
				else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
				{
					impl::parseStructBegin( parser );
					ElemTypeT::parse( parser, what );
					impl::parseStructEnd( parser );
				}
				else
					static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
				dest.push_back( what );
			}
		}
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			parser.skipDelimiter( '[' );
			if ( parser.isDelimiter( ']' ) )
			{
				parser.skipDelimiter( ']' );
				return;
			}
			for( ;; )
			{
				typename VectorT::value_type what;
				if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
					parser.readSignedIntegerFromJson( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
					parser.readUnsignedIntegerFromJson( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
					parser.readRealFromJson( &what );
				else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
					parser.readStringFromJson( &what );
				else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
				{
					impl::parseStructBegin( parser );
					ElemTypeT::parse( parser, what );
					impl::parseStructEnd( parser );
				}
				else
					static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
				dest.push_back( what );
				if ( parser.isDelimiter( ',' ) )
				{
					parser.skipDelimiter( ',' );
					continue;
				}
				if ( parser.isDelimiter( ']' ) )
				{
					parser.skipDelimiter( ']' );
					break;
				}
			}
		}
	}
};

namespace impl {
	template<class VectorT, class ElemTypeT>
	void copyVector( const VectorT& src, VectorT& dst )
	{
		dst.resize( src.size() );
		for ( size_t i=0; i<src.size(); ++i )
		{
			if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
				dst[i] = src[i];
			else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
				dst[i] = src[i];
			else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
				dst[i] = src[i];
			else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
				dst[i] = src[i];
			else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
				ElemTypeT::copy( src, dst );
			else
				static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
		}
	}

	template<class VectorT, class ElemTypeT>
	bool isSameVector( const VectorT& v1, const VectorT& v2 )
	{
		if ( v1.size() != v2.size() )
			return false;
		for ( size_t i=0; i<v1.size(); ++i )
		{
			if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
			{
				if ( v1[i] != v2[i] ) 
					return false;
			}
			else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
			{
				if ( v1[i] != v2[i] ) 
					return false;
			}
			else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
			{
				if ( v1[i] != v2[i] ) 
					return false;
			}
			else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
			{
				if ( v1[i] != v2[i] ) 
					return false;
			}
			else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
			{
				if ( !ElemTypeT::isSame( v1[i], v2[i] ) ) 
					return false;
			}
			else
				static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
		}
		return true;
	}
} // namespace impl

#if 0
class VectorOfStructBody
{
public:
	template<class ComposerT, class VectorT, class ElemTypeT, class ProcType/*, class RootT*/>
	static
	void compose( ComposerT& composer, VectorT& what ) { 
		size_t collSz = what.size();
		if constexpr ( ComposerT::proto == Proto::GMQ )
		{
			composeUnsignedInteger( composer, collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				m::impl::composePublishableStructBegin( composer, "Size" );
				ProcType::compose( composer, what[i] );
				m::impl::composePublishableStructEnd( composer, false );
			}
		}
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			composer.buff.append( "[", 1 );
			for ( size_t i=0; i<collSz; ++i )
			{
				m::impl::composePublishableStructBegin( composer, "Size" );
				ProcType::compose( composer, what[i] );
				m::impl::composePublishableStructEnd( composer, false );
			}
			composer.buff.append( "]", 1 );
		}
	}

	template<class ParserT, class VectorT, class ElemTypeT, class ProcType/*, class RootT*/>
	static
	void parse( ParserT& parser, VectorT& dest ) { 
		if constexpr ( ParserT::proto == Proto::GMQ )
		{
			size_t collSz;
			parseUnsignedInteger( parser, &collSz );
			dest.reserve( collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				typename VectorT::value_type what;
				ProcType::parse( parser, what );
				dest.push_back( what );
			}
		}
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			parser.skipDelimiter( '[' );
			if ( parser.isDelimiter( ']' ) )
			{
				parser.skipDelimiter( ']' );
				return;
			}
			for( ;; )
			{
				typename VectorT::value_type what;
				parser.skipDelimiter( '{' );
				ProcType::parse( parser, what );
				parser.skipDelimiter( '}' );
				dest.push_back( what );
				if ( parser.isDelimiter( ',' ) )
				{
					parser.skipDelimiter( ',' );
					continue;
				}
				if ( parser.isDelimiter( ']' ) )
				{
					parser.skipDelimiter( ']' );
					break;
				}
			}
		}
	}

	template<class ParserT, class VectorT, class ElemTypeT, class ProcType/*, class RootT*/>
	static
	void parse( ParserT& parser, VectorT& dest, GMQ_COLL vector<size_t>& addr, size_t offset, size_t action ) { 
		switch ( action )
		{
			case ActionOnVector::remove_at:
				parseStateUpdateBlockEnd( parser );
				assert( addr.size() == offset + 1 );
				size_t pos = addr[size];
				assert( pos < dest.size() );
				dest.erase( dest.begin() + pos );
				break;
			case ActionOnVector::update_at:
				assert( addr.size() == offset + 1 );
				size_t pos = addr[size];
				assert( pos < dest.size() );
				dest.erase( dest.begin() + pos );
				break;
		}
		if constexpr ( ParserT::proto == Proto::GMQ )
		{
			size_t collSz;
			parseUnsignedInteger( parser, &collSz );
			dest.reserve( collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				typename VectorT::value_type what;
				ProcType::parse( parser, what );
				dest.push_back( what );
			}
		}
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			parser.skipDelimiter( '[' );
			for( ;; )
			{
				typename VectorT::value_type what;
				parser.skipDelimiter( '{' );
				ProcType::parse( parser, what );
				parser.skipDelimiter( '}' );
				dest.push_back( what );
				if ( parser.isDelimiter( ',' ) )
				{
					parser.skipDelimiter( ',' );
					continue;
				}
				if ( parser.isDelimiter( ']' ) )
				{
					parser.skipDelimiter( ']' );
					break;
				}
			}
		}
	}
};
#endif // 0

enum SubscriberStatus { waitingForSubscriptionIni };
enum PublisherSubscriberMessageType { undefined = 0, subscriptionRequest = 1, subscriptionResponse = 2, stateUpdate = 3 };

struct SubscriberAddress
{
	std::string nodeAddr;
	std::string subscriberinNodeAddr;
};

struct SubscriberData
{
	SubscriberAddress address;
	SubscriberStatus status;
};

template<class ComposerT>
class PublisherBase
{
protected:
	GMQ_COLL vector<SubscriberAddress> subscribers;

public:
	virtual ~PublisherBase() {}
	virtual ComposerT& getComposer() = 0;
	virtual void resetComposer( ComposerT* composer_ ) = 0;
	virtual void finalizeComposing() = 0;
	virtual void generateStateSyncMessage( ComposerT& composer ) = 0;
};

template<class PlatformSupportT>
class PublisherPool
{
public: // TODO: just a tmp approach to continue immediate dev
	std::vector<globalmq::marshalling::PublisherBase<typename PlatformSupportT::ComposerT>*> publishers;

public:
	void registerPublisher( globalmq::marshalling::PublisherBase<typename PlatformSupportT::ComposerT>* publisher )
	{
		publishers.push_back( publisher );
	}
	void unregisterPublisher( globalmq::marshalling::PublisherBase<typename PlatformSupportT::ComposerT>* publisher )
	{
		for ( size_t i=0; i<publishers.size(); ++i )
			if ( publishers[i] == publisher )
			{
				publishers.erase( publishers.begin() + i );
				return;
			}
		assert( false ); // not found
	}
};

template<class BufferT>
class SubscriberBase
{
public:
	virtual void applyGmqMessageWithUpdates( GmqParser<BufferT>& parser ) = 0;
	virtual void applyJsonMessageWithUpdates( JsonParser<BufferT>& parser ) = 0;
	virtual void applyGmqStateSyncMessage( GmqParser<BufferT>& parser ) = 0;
	virtual void applyJsonStateSyncMessage( JsonParser<BufferT>& parser ) = 0;
	virtual ~SubscriberBase() {}
};

template<class PlatformSupportT>
class SubscriberPool
{
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;

	GMQ_COLL vector<globalmq::marshalling::SubscriberBase<BufferT>*> subscribers; // TODO: consider mapping ID -> ptr, if states are supposed to be added and removede dynamically

public:
	void registerSubscriber( globalmq::marshalling::SubscriberBase<BufferT>* subscriber )
	{
		subscribers.push_back( subscriber );
	}
	void unregisterSubscriber( globalmq::marshalling::SubscriberBase<BufferT>* subscriber )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i] == subscriber )
			{
				subscribers.erase( subscribers.begin() + i );
				return;
			}
		assert( false ); // not found
	}
	void onMessage( ParserT& parser ) 
	{
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		switch ( msgType )
		{
			case PublisherSubscriberMessageType::subscriptionResponse:
			{
				size_t subscriberID;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &subscriberID, "subscriber_id" );
				if ( subscriberID >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				globalmq::marshalling::impl::parseKey( parser, "state" );
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[subscriberID]->applyJsonStateSyncMessage( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[subscriberID]->applyGmqStateSyncMessage( parser );
				}
				break;
			}
			case PublisherSubscriberMessageType::stateUpdate:
			{
				size_t subscriberID;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &subscriberID, "subscriber_id" );
				if ( subscriberID >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				globalmq::marshalling::impl::parseKey( parser, "update" );
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[subscriberID]->applyJsonMessageWithUpdates( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[subscriberID]->applyGmqMessageWithUpdates( parser );
				}
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}

	}
};


} // namespace globalmq::marshalling

#endif // PUBLISHABLE_IMPL_H
