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
#include "gmqueue.h"


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

	template<class ParserT, class VectorT, class ProcType>
	static
	bool parseSingleValueAndCompare( ParserT& parser, typename VectorT::value_type& value, const typename VectorT::value_type& oldValue ) { 
		if constexpr ( std::is_base_of<impl::StructType, ProcType>::value )
		{
			impl::parseStructBegin( parser );
			ProcType::parse( parser, value );
			impl::parseStructEnd( parser );
			return !ProcType::isSame( value, oldValue );
		}
		else 
		{
			if constexpr ( std::is_same<ProcType, impl::SignedIntegralType>::value )
				impl::IntegerProcessor::parse<ParserT, typename VectorT::value_type>( parser, &value );
			else if constexpr ( std::is_same<ProcType, impl::UnsignedIntegralType>::value )
				impl::UnsignedIntegerProcessor::parse( parser, &value );
			else if constexpr ( std::is_same<ProcType, impl::RealType>::value )
				impl::RealProcessor::parse( parser, &value );
			else if constexpr ( std::is_same<ProcType, impl::StringType>::value )
				impl::StringProcessor::parse( parser, &value );
			else
				static_assert( std::is_same<ProcType, AllowedDataType>::value, "unsupported type" );
			return value != oldValue;
		}
	}

	template<class ComposerTT, class VectorT, class ElemTypeT>
	static
	void compose( ComposerTT& composer, const VectorT& what ) { 
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
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

	template<class ParserT, class VectorT, class ElemTypeT, bool suppressNotifications = false>
	static
	void parse( ParserT& parser, VectorT& dest ) { 
		dest.clear();
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
					if constexpr( suppressNotifications )
						ElemTypeT::parseForStateSync( parser, what );
					else
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
				if ( parser.isDelimiter( ',' ) )
					parser.skipDelimiter( ',' );
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
					if constexpr( suppressNotifications )
						ElemTypeT::parseForStateSync( parser, what );
					else
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
			if ( parser.isDelimiter( ',' ) )
				parser.skipDelimiter( ',' );
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
				ElemTypeT::copy( src[i], dst[i] );
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

template<class PlatformSupportT>
struct StateSubscriberData
{
	uint64_t IdInPool; // for indexing purposes
	uint64_t ref_id_at_subscriber;
};

template<class PlatformSupportT>
class StatePublisherPool;

template<class ComposerT>
class StatePublisherBase
{
	template<class PlatformSupportT>
	friend class StatePublisherPool;
	using BufferT = typename ComposerT::BufferType;

public:
	uint64_t idx; // for use in pools, etc

public:
	virtual ~StatePublisherBase() {}
	// interaction with state wrapper
	virtual void generateStateSyncMessage( ComposerT& composer ) = 0;
	virtual void startTick( BufferT&& buff ) = 0;
	virtual BufferT&& endTick() = 0;
	virtual const char* name() = 0;
	virtual uint64_t stateTypeID() = 0;
};

template<class PlatformSupportT>
class StatePublisherData
{
	friend class StatePublisherPool<PlatformSupportT>;

	using BufferT = typename PlatformSupportT::BufferT;
	using ComposerT = typename PlatformSupportT::ComposerT;

	globalmq::marshalling::StatePublisherBase<ComposerT>* publisher = nullptr;
	GMQ_COLL vector<StateSubscriberData<PlatformSupportT>> subscribers;
	uint64_t idx; // in pool

public:
	StatePublisherData( uint64_t idx_, globalmq::marshalling::StatePublisherBase<ComposerT>* publisher_ ) : publisher( publisher_ ) {
		assert( publisher != nullptr );
		idx = idx_;
		publisher->idx = idx_;
		BufferT buff; // just empty
		publisher->startTick( std::move( buff ) );
	}
	void setPublisher( globalmq::marshalling::StatePublisherBase<ComposerT>* publisher_ ) {
		assert( publisher == nullptr );
		assert( publisher_ != nullptr );
		publisher = publisher_;
		publisher->idx = idx;
		BufferT buff; // just empty
		publisher->startTick( std::move( buff ) );
	}
	void setUnused( globalmq::marshalling::StatePublisherBase<ComposerT>* publisher_ ) { 
		assert( publisher == publisher_ );
		publisher = nullptr;
	}
	bool isUsed() { return publisher != nullptr; }
	uint64_t stateTypeID() {
		assert( publisher != nullptr );
		return publisher->stateTypeID();
	}

	// processing requests (by now they seem to be independent on state wrappers)
	uint64_t onSubscriptionRequest( uint64_t IdInPool, uint64_t ref_id_at_subscriber )
	{
		// TODO: who will check uniqueness?
		subscribers.push_back( StateSubscriberData<PlatformSupportT>({IdInPool, ref_id_at_subscriber}) );
		return subscribers.size() - 1;
	}
	void generateStateSyncMessage( ComposerT& composer ) { assert( publisher != nullptr ); publisher->generateStateSyncMessage( composer ); }
	BufferT&& getStateUpdateBuff() { return publisher->endTick(); }
	void startTick( BufferT&& buff ) { assert( publisher != nullptr ); publisher->startTick( std::move( buff ) ); }
	BufferT&& endTick() { assert( publisher != nullptr ); return std::move( publisher->endTick() ); }
};

template<class PlatformSupportT>
class StatePublisherPool
{
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;

	GMQTransportBase<PlatformSupportT>* transport = nullptr;

public: // TODO: just a tmp approach to continue immediate dev
	GMQ_COLL vector<StatePublisherData<PlatformSupportT>> publishers;
	GMQ_COLL unordered_map<GMQ_COLL string, StatePublisherData<PlatformSupportT>*> name2publisherMapping;
	GMQ_COLL unordered_map<uint64_t, std::pair<uint64_t, uint64_t>> ID2PublisherAndItsSubscriberMapping;
	uint64_t publisherAndItsSubscriberBase = 0;

public:
	uint64_t add( globalmq::marshalling::StatePublisherBase<ComposerT>* publisher )
	{
		for ( size_t i=0; i<publishers.size(); ++i )
			if ( !publishers[i].isUsed() )
			{
				publishers[i].setPublisher( publisher );
				auto ins = name2publisherMapping.insert( std::move( std::make_pair(publisher->name(), &(publishers[i] ) ) ) );
				assert( ins.second ); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
				return i;
			}
		publishers.push_back( std::move( StatePublisherData<PlatformSupportT>(publishers.size(), publisher) ) );
		auto ins = name2publisherMapping.insert( std::move( std::make_pair(publisher->name(), &(publishers[publishers.size() - 1] ) ) ) );
		assert( ins.second ); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
		return publishers.size() - 1;
	}
	void remove( globalmq::marshalling::StatePublisherBase<ComposerT>* publisher )
	{
		size_t res = name2publisherMapping.erase( publisher->name() );
		assert( res == 1 );
		assert( publisher->idx < publishers.size() );
		for ( auto& subscriber : publishers[publisher->idx].subscribers )
		{
			res = ID2PublisherAndItsSubscriberMapping.erase( subscriber.IdInPool );
			assert( res == 1 );
		}
		publishers[publisher->idx].setUnused( publisher );
	}

	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }

	void onMessage( ParserT& parser )
	{
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( GmqPathHelper::Type::subscriptionRequest, mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)
				GMQ_COLL string publisherName = pc.statePublisherOrConnPeerName;

				auto findres = name2publisherMapping.find( publisherName );
				if ( findres == name2publisherMapping.end() )
					throw std::exception(); // not found / misdirected

				uint64_t id = ++publisherAndItsSubscriberBase;
				size_t refIdAtPublisher = findres->second->onSubscriptionRequest( id, mh.ref_id_at_subscriber );
				auto ret = ID2PublisherAndItsSubscriberMapping.insert( std::make_pair( id, std::make_pair( findres->second->idx, refIdAtPublisher ) ) );
				assert( ret.second );

				PublishableStateMessageHeader hdrBack;
				hdrBack.type = PublishableStateMessageHeader::MsgType::subscriptionResponse;
				hdrBack.state_type_id_or_direction = findres->second->stateTypeID();
				hdrBack.priority = mh.priority;
				hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
				hdrBack.ref_id_at_publisher = id;

				BufferT msgBack;
				ComposerT composer( msgBack );
				helperComposePublishableStateMessageBegin( composer, hdrBack );
				findres->second->generateStateSyncMessage( composer );
				helperComposePublishableStateMessageEnd( composer );
				assert( transport != nullptr );
				transport->postMessage( std::move( msgBack ) );

				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}

	void postAllUpdates()
	{
		for ( auto& publisher : publishers )
		{
			PublishableStateMessageHeader mhBase;
			mhBase.type = PublishableStateMessageHeader::MsgType::stateUpdate;
			mhBase.state_type_id_or_direction = publisher.stateTypeID();
			mhBase.priority = 0; // TODO: source
			mhBase.ref_id_at_subscriber = 0; // later
			mhBase.ref_id_at_publisher = 0; // later

			BufferT stateUpdateBuff = publisher.getStateUpdateBuff();

			BufferT msgBase;
			ComposerT composer( msgBase );
			helperComposePublishableStateMessageBegin( composer, mhBase );
			ParserT stateUpdateBuffParser( stateUpdateBuff );
			copy<typename ParserT::RiterT, typename ComposerT::BufferType>( stateUpdateBuffParser.getIterator(), msgBase );
			helperComposePublishableStateMessageEnd( composer );

			for ( auto& subscriber : publisher.subscribers )
			{
				PublishableStateMessageHeader::UpdatedData ud;
				ud.ref_id_at_publisher = subscriber.IdInPool;
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
//				ud.ref_id_at_subscriber = subscriber.info.ref_id_at_subscriber;
				ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
				typename ComposerT::BufferType msgForward;
				helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msgBase, msgForward, ud );

//				PlatformSupportT::sendMsgFromPublisherToSubscriber( msgForward, subscriber.address.nodeAddr );
				assert( transport != nullptr );
				transport->postMessage( std::move( msgForward ) );
			}
			BufferT newBuff; // just empty
			publisher.startTick( std::move( newBuff ) );
		}
	}
};

template<class BufferT>
class StateSubscriberBase
{
public:
	virtual void applyGmqMessageWithUpdates( GmqParser<BufferT>& parser ) = 0;
	virtual void applyJsonMessageWithUpdates( JsonParser<BufferT>& parser ) = 0;
	virtual void applyGmqStateSyncMessage( GmqParser<BufferT>& parser ) = 0;
	virtual void applyJsonStateSyncMessage( JsonParser<BufferT>& parser ) = 0;
	virtual const char* name() = 0;
	virtual uint64_t stateTypeID() = 0;
	virtual ~StateSubscriberBase() {}
};

template<class PlatformSupportT>
class StateSubscriberPool
{
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;

	struct Subscriber
	{
		StateSubscriberT* subscriber;
		uint64_t ref_id_at_subscriber;
		uint64_t ref_id_at_publisher;
	};

	GMQ_COLL vector<Subscriber> subscribers; // TODO: consider mapping ID -> ptr, if states are supposed to be added and removede dynamically

	GMQTransportBase<PlatformSupportT>* transport = nullptr;

public:
	void add( StateSubscriberT* subscriber )
	{
		// TODO: revise for performance
		Subscriber s;
		s.subscriber = subscriber;
		s.ref_id_at_subscriber = subscribers.size();
		s.ref_id_at_publisher = 0;
		subscribers.push_back( s );
	}
	void remove( StateSubscriberT* subscriber )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i].subscriber == subscriber )
			{
				// TODO: if unsubscribeMessaage is to be sent, it's probably the right place
				subscribers.erase( subscribers.begin() + i );
				return;
			}
		assert( false ); // not found
	}

	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }

	void subscribe( StateSubscriberT* subscriber, GMQ_COLL string path )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i].subscriber == subscriber )
			{
				BufferT buff;
				ComposerT composer( buff );
				PublishableStateMessageHeader mh;
				mh.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::subscriptionRequest;
				mh.priority = 0; // TODO: source
				mh.state_type_id_or_direction = subscriber->stateTypeID();
				mh.path = path;
				assert( subscribers[i].ref_id_at_subscriber == i );
				mh.ref_id_at_subscriber = subscribers[i].ref_id_at_subscriber;
				helperComposePublishableStateMessageBegin( composer, mh );
				helperComposePublishableStateMessageEnd( composer );
				assert( transport != nullptr );
				transport->postMessage( std::move( buff ) );
				return;
			}
		assert( false ); // not found
	}
	void onMessage( ParserT& parser ) 
	{
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionResponse:
			{
				if ( mh.ref_id_at_subscriber >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				subscribers[mh.ref_id_at_subscriber].ref_id_at_publisher = mh.ref_id_at_publisher;
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[mh.ref_id_at_subscriber].subscriber->applyJsonStateSyncMessage( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqStateSyncMessage( parser );
				}
				break;
			}
			case PublishableStateMessageHeader::MsgType::stateUpdate:
			{
				if ( mh.ref_id_at_subscriber >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				// TODO: consider the following:
				//if ( subscribers[mh.ref_id_at_subscriber].ref_id_at_publisher != mh.ref_id_at_publisher )
				//	throw std::exception(); // TODO: ... (invalid source)
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[mh.ref_id_at_subscriber].subscriber->applyJsonMessageWithUpdates( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqMessageWithUpdates( parser );
				}
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}
};


template<class PlatformSupportT>
struct ConnectionBase
{
};

template<class PlatformSupportT> class ClientConnectionPool;
template<class PlatformSupportT>
class ClientConnectionBase
{
	template<class PlatformSupportT> friend class ClientConnectionPool;
	ClientConnectionPool<PlatformSupportT>* pool = nullptr;

public:
	void connect( GMQ_COLL string path )
	{
		assert( pool != nullptr );
		pool->connect( this, path );
	}
};

struct ConnectionNotifierBase
{
};

template<class PlatformSupportT>
class ClientNotifierBase : public ConnectionNotifierBase
{
public:
	using ParserT = typename PlatformSupportT::ParserT;
	using ConnectionT = globalmq::marshalling::ClientConnectionBase<PlatformSupportT>;

	virtual void onConnectionAccepted( uint64_t connID ) {};
	virtual void onMessage( uint64_t connID, ParserT& parser ) {};
};

template<class PlatformSupportT>
class ClientConnectionPool
{
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using ConnectionT = globalmq::marshalling::ClientConnectionBase<PlatformSupportT>;

	struct ClientConnection
	{
		ConnectionT* connection;
		uint64_t ref_id_at_server;
		uint64_t ref_id_at_client; // that is, local id
	};

	GMQ_COLL unordered_map<uint64_t, ClientConnection> connections;
	uint64_t connIdxBase = 0;

	GMQTransportBase<PlatformSupportT>* transport = nullptr;

	ClientNotifierBase<PlatformSupportT>* notifier = nullptr;

public:
	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }
	void setNotifier( ClientNotifierBase<PlatformSupportT>* notifier_ )
	{
		assert( notifier != nullptr );
		notifier = notifier_;
	}

	uint64_t add( ConnectionT* connection ) // returns connection ID
	{
		// TODO: revise for performance
		ClientConnection cc;
		cc.connection = connection;
		cc.ref_id_at_client = ++connIdxBase;
		cc.ref_id_at_server = 0;
		auto ins = connections.insert( std::make_pair( cc.ref_id_at_client, cc ) );
		assert( ins.second );
		connection->pool = this;
		return cc.ref_id_at_client;
	}
	void remove( uint64_t connID )
	{
		connections.erase( connID );
	}

	void connect( uint64_t connID, GMQ_COLL string path )
	{
		auto f = connections.find( connID );
		assert( f != connections.end() );
		auto& conn = f->second;
		assert( conn.ref_id_at_client == connID ); // self-consistency
		BufferT buff;
		ComposerT composer( buff );
		PublishableStateMessageHeader mh;
		mh.type = globalmq::marshalling::PublishableStateMessageHeader::connectionRequest;
		mh.priority = 0; // TODO: source
		mh.state_type_id_or_direction = PublishableStateMessageHeader::ConnMsgDirection::toServer;
		mh.path = path;
		assert( conn.ref_id_at_client == connID );
		mh.ref_id_at_subscriber = conn.ref_id_at_client;
		helperComposePublishableStateMessageBegin( composer, mh );
		helperComposePublishableStateMessageEnd( composer );
		assert( transport != nullptr );
		transport->postMessage( std::move( buff ) );
		return;
	}
	void onMessage( ParserT& parser ) 
	{
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::connectionAccepted:
			{
				auto f = connections.find( mh.ref_id_at_subscriber );
				if ( f == connections.end() )
					throw std::exception();
				auto& conn = f->second;
				assert( conn.ref_id_at_client == mh.ref_id_at_subscriber ); // self-consistency
				conn.ref_id_at_server = mh.ref_id_at_publisher;
				assert( notifier != nullptr );
				notifier->onConnectionAccepted( conn.ref_id_at_client );
				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionMessage:
			{
				assert( mh.state_type_id_or_direction == PublishableStateMessageHeader::ConnMsgDirection::toClient );
				auto f = connections.find( mh.ref_id_at_publisher );
				if ( f == connections.end() )
					throw std::exception();
				auto& conn = f->second;
				assert( notifier != nullptr );
				assert( conn.ref_id_at_server == mh.ref_id_at_publisher ); // self-consistency
				notifier->onMessage( conn.ref_id_at_client, parser );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}
};

template<class PlatformSupportT>
class ConnectionFactoryBase
{
public:
	virtual ConnectionBase<PlatformSupportT>* create() = 0;
};

template<class PlatformSupportT>
class ServerNotifierBase : public ConnectionNotifierBase
{
public:
	using ParserT = typename PlatformSupportT::ParserT;
	using ConnectionT = globalmq::marshalling::ConnectionBase<PlatformSupportT>;

	virtual void onNewConnection( uint64_t connID ) {};
	virtual void onMessage( uint64_t connID, ConnectionT* connection, ParserT& parser ) {};
};

template<class PlatformSupportT>
class ServerConnectionPool
{
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using ConnectionT = globalmq::marshalling::ConnectionBase<PlatformSupportT>;

	struct ServerConnection
	{
		ConnectionT* connection = nullptr;
		uint64_t ref_id_at_server; // that is, local id
		uint64_t ref_id_at_client;
	};

	GMQ_COLL unordered_map<uint64_t, ServerConnection> connections; // TODO: consider mapping ID -> ptr, if states are supposed to be added and removede dynamically
	uint64_t connIdxBase = 0;

	ConnectionFactoryBase<PlatformSupportT>* connFactory = nullptr;
	GMQTransportBase<PlatformSupportT>* transport = nullptr;
	ServerNotifierBase<PlatformSupportT>* notifier = nullptr;

public:
	ServerConnectionPool() {}
	~ServerConnectionPool()
	{
		for ( auto conn : connections )
			if ( conn.second.connection != nullptr )
				delete conn.second.connection;
	}
	void setConnectionFactory( ConnectionFactoryBase<PlatformSupportT>* connFactory_ )
	{
		assert( connFactory_ != nullptr );
		connFactory = connFactory_;
	}
	void setNotifier( ServerNotifierBase<PlatformSupportT>* notifier_ )
	{
		assert( notifier != nullptr );
		notifier = notifier_;
	}
	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }

	void onMessage( ParserT& parser ) 
	{
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::connectionRequest:
			{
				assert( connFactory != nullptr );
				ServerConnection sc;
				sc.connection = connFactory->create();
				sc.ref_id_at_client = mh.ref_id_at_subscriber;
				sc.ref_id_at_server = ++connIdxBase;
				auto ins = connections.insert( std::make_pair( sc.ref_id_at_server, sc ) );
				assert( ins.second );

				assert( notifier != nullptr );
				notifier->onNewConnection( sc.ref_id_at_server );

				PublishableStateMessageHeader hdrBack;
				hdrBack.type = PublishableStateMessageHeader::MsgType::connectionAccepted;
				hdrBack.state_type_id_or_direction = PublishableStateMessageHeader::ConnMsgDirection::toClient;
				hdrBack.priority = mh.priority; // TODO: source?
				hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
				hdrBack.ref_id_at_publisher = sc.ref_id_at_server;

				BufferT msgBack;
				ComposerT composer( msgBack );
				helperComposePublishableStateMessageBegin( composer, hdrBack );
				helperComposePublishableStateMessageEnd( composer );
				assert( transport != nullptr );
				transport->postMessage( std::move( msgBack ) );

				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionMessage:
			{
				assert( mh.state_type_id_or_direction == PublishableStateMessageHeader::ConnMsgDirection::toServer );
				auto f = connections.find( mh.ref_id_at_publisher );
				if ( f == connections.end() )
					throw std::exception();
				auto& conn = f->second;
				assert( notifier != nullptr );
				assert( conn.ref_id_at_server == mh.ref_id_at_publisher ); // self-consistency
				notifier->onMessage( conn.ref_id_at_server, conn.connection, parser );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}
};

template<class PlatformSupportT>
class MetaPool : public StatePublisherPool<PlatformSupportT>, public StateSubscriberPool<PlatformSupportT>, public ClientConnectionPool<PlatformSupportT>, public ServerConnectionPool<PlatformSupportT>
{
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
	using StatePublisherT = globalmq::marshalling::StatePublisherBase<BufferT>;
	using ConnectionT = globalmq::marshalling::ConnectionBase<BufferT>;

public:
	void setTransport( GMQTransportBase<PlatformSupportT>* tr )
	{
		StatePublisherPool<PlatformSupportT>::setTransport( tr );
		StateSubscriberPool<PlatformSupportT>::setTransport( tr );
		ClientConnectionPool<PlatformSupportT>::setTransport( tr );
		ServerConnectionPool<PlatformSupportT>::setTransport( tr );
	}

	void setConnectionFactory( ConnectionFactoryBase<PlatformSupportT>* connFactory )
	{
		ServerConnectionPool<PlatformSupportT>::setConnectionFactory( connFactory );
	}

	template<class T>
	void setNotifier( T* notifier )
	{
		static_assert ( std::is_base_of<ConnectionNotifierBase, T>::value );
		if constexpr ( std::is_base_of<ClientNotifierBase<PlatformSupportT>, T>::value )
			ClientConnectionPool<PlatformSupportT>::setNotifier( notifier );
		else
		{
			static_assert ( std::is_base_of<ServerNotifierBase<PlatformSupportT>, T>::value );
			ServerConnectionPool<PlatformSupportT>::setNotifier( notifier );
		}
	}

	template<class T>
	void add( T* obj )
	{
		if constexpr ( std::is_base_of<StatePublisherBase<ComposerT>, T>::value )
			StatePublisherPool<PlatformSupportT>::add( obj );
		else if constexpr ( std::is_base_of<StateSubscriberBase<BufferT>, T>::value )
			StateSubscriberPool<PlatformSupportT>::add( obj );
		else
		{
			static_assert ( std::is_base_of<ClientConnectionBase<PlatformSupportT>, T>::value );
			ClientConnectionPool<PlatformSupportT>::add( obj );
		}
	}

	template<class T>
	void remove( T* obj )
	{
		if constexpr ( std::is_base_of<StatePublisherBase<ComposerT>, T>::value )
			StatePublisherPool<PlatformSupportT>::remove( obj );
		else if constexpr ( std::is_base_of<StateSubscriberBase<BufferT>, T>::value )
			StateSubscriberPool<PlatformSupportT>::remove( obj );
		else
		{
			static_assert ( std::is_base_of<ConnectionBase<BufferT>, T>::value );
			ClientConnectionPool<PlatformSupportT>::remove( obj );
		}
	}

	void onMessage( ParserT& parser )
	{
		ParserT parser1( parser );
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser1, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionResponse:
			case PublishableStateMessageHeader::MsgType::stateUpdate:
				StateSubscriberPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
				StatePublisherPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::connectionRequest:
				ServerConnectionPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::connectionAccepted:
				ClientConnectionPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::connectionMessage:
				switch ( mh.state_type_id_or_direction )
				{
					case PublishableStateMessageHeader::ConnMsgDirection::toClient:
						ClientConnectionPool<PlatformSupportT>::onMessage( parser );
						break;
					case PublishableStateMessageHeader::ConnMsgDirection::toServer:
						ServerConnectionPool<PlatformSupportT>::onMessage( parser );
						break;
					default:
						throw std::exception(); // TODO: ... (unknown msg type)
				}
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
//		globalmq::marshalling::impl::parseStructEnd( parser );
	}

	void onMessage( BufferT& buffer )
	{
		ParserT parser( buffer );
		onMessage( parser );
	}

	void postAllUpdates()
	{
		StatePublisherPool<PlatformSupportT>::postAllUpdates();
	}
};

} // namespace globalmq::marshalling

#endif // PUBLISHABLE_IMPL_H

