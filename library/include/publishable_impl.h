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



struct GmqPathHelper
{
	struct PathComponents
	{
		GMQ_COLL string authority;
		bool furtherResolution = false;
		bool hasPort = false;
		uint16_t port = 0xFFFF;
		GMQ_COLL string localPart; // TODO: revise
		GMQ_COLL string nodeName;
		GMQ_COLL string statePublisherName;
	};

	static GMQ_COLL string compose( GMQ_COLL string authority, GMQ_COLL string nodeName, GMQ_COLL string statePublisherName )
	{
		// TODO: check components
		GMQ_COLL string ret = "globalmq:";
		if ( !authority.empty() )
		{
			ret += "//";
			ret += authority;
		}
		assert( !nodeName.empty() );
		assert( !statePublisherName.empty() );
		ret += '/';
		ret += localPart( nodeName, statePublisherName );
		return ret;
	}

	static GMQ_COLL string compose( const PathComponents& components )
	{
		// TODO: check components
		GMQ_COLL string ret = "globalmq:";
		if ( !components.authority.empty() )
		{
			ret += "//";
			ret += components.authority;
		}
		if ( components.furtherResolution )
			ret += "!gmq";
		if ( components.hasPort )
		{
			auto str = fmt::format( ":{}", components.port );
			ret += str;
		}
		assert( !components.nodeName.empty() );
		assert( !components.statePublisherName.empty() );
		ret += '/';
		ret += localPart( components.nodeName, components.statePublisherName );
		return ret;
	}

	static GMQ_COLL string localPart( GMQ_COLL string nodeName, GMQ_COLL string statePublisherName )
	{
		return fmt::format( "{}?sp={}", nodeName, statePublisherName );
	}

	static bool parse( GMQ_COLL string path, PathComponents& components )
	{
		size_t pos = path.find( "globalmq:" );
		if ( pos != 0 )
			return false;
		pos += sizeof( "globalmq:" ) - 1;
		if ( path.size() <= pos )
			return false;
		if ( path[pos++] != '/' )
			return false;
		if ( path[pos++] == '/' ) // double-slash, authority component is present
		{
			size_t pos1 = path.find( "/", pos );
			if ( pos1 == GMQ_COLL string::npos )
				return false;
			components.authority = path.substr( pos, pos1 - pos );
			pos = pos1 + 1;
			pos1 = components.authority.find_last_of( ':' );
			if ( pos1 != GMQ_COLL string::npos )
			{
				char* end = nullptr;
				size_t port = strtol( components.authority.c_str() + pos1 + 1, &end, 10 );
				if ( components.authority.c_str() + pos1 + 1 == end )
					return false;
				if ( end - components.authority.c_str() < components.authority.size() ) // there are remaining chars
					return false;
				if ( port >= UINT16_MAX )
					return false;
				components.hasPort = true;
				components.port = (uint16_t)port;
				components.authority.erase( pos1 );
			}
			else
			{
				components.hasPort = false;
				components.port = 0xFFFF;
			}

			size_t pos2 = components.authority.find_last_of( '!' );
			if ( pos2 != GMQ_COLL string::npos )
			{
				if ( components.authority.size() - pos2 < sizeof( "gmq" ) - 1 )
					return false;
				if ( components.authority.substr( pos2 + 1 ) != "gmq" )
					return false;
				components.furtherResolution = true;
				components.authority.erase( pos2 );
			}
			else
			{
				components.furtherResolution = false;
			}
		}
		else
		{
			components.authority = "";
			components.hasPort = false;
			components.furtherResolution = false;
			components.port = 0xFFFF;
		}

		// node name
		size_t pos1 = path.find( '?', pos );
		if ( pos1 == GMQ_COLL string::npos )
			return false;
		components.nodeName = path.substr( pos, pos1 - pos );
		pos = pos1;

		// statePublisherName
		pos = path.find( "sp=", pos );
		if ( pos == GMQ_COLL string::npos )
			return false;
		pos += sizeof( "sp=" ) - 1;
		pos1 = path.find( '&', pos );
		if ( pos1 == GMQ_COLL string::npos )
			components.statePublisherName = path.substr( pos );
		else
			components.statePublisherName = path.substr( pos, pos1 - pos );
		return true;
	}
};

struct PublishableStateMessageHeader
{
	enum MsgType { undefined = 0, subscriptionRequest = 1, subscriptionResponse = 2, stateUpdate = 3 };
	MsgType type;
	uint64_t state_type_id; // Note: may be removed in future versions
	uint64_t priority;
	GMQ_COLL string path;  // subscriptionRequest only
	uint64_t ref_id_at_subscriber; // updatable
	uint64_t ref_id_at_publisher; // updatable

	struct UpdatedData
	{
		uint64_t ref_id_at_subscriber;
		uint64_t ref_id_at_publisher;
		bool update_ref_id_at_subscriber = false;
		bool update_ref_id_at_publisher = false;
	};

	template<class ParserT>
	void parse( ParserT& parser )
	{
		globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &state_type_id, "state_type_id" );
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &priority, "priority" );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			{
				type = MsgType::subscriptionRequest;
				globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &path, "path" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				break;
			}
			case MsgType::subscriptionResponse:
			{
				type = MsgType::subscriptionResponse;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			}
			case MsgType::stateUpdate:
			{
				type = MsgType::stateUpdate;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_subscriber, "ref_id_at_subscriber" );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &ref_id_at_publisher, "ref_id_at_publisher" );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::parsePublishableStructEnd( parser );
	}

	template<class ParserT, class ComposerT>
	static void parseAndUpdate( ParserT& msgStartParser, ParserT& parser, typename ComposerT::BufferType& buff, const UpdatedData& udata )
	{
		ComposerT composer( buff );
//		ParserT parser2 = parser;
		globalmq::marshalling::impl::parsePublishableStructBegin( parser, "hdr" );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		if ( msgType == MsgType::subscriptionRequest )
			throw std::exception(); // inapplicable
		uint64_t dummy;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &dummy, "state_type_id" );
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &dummy, "priority" );
		size_t offset = parser.getCurrentOffset();
		::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( msgStartParser.getIterator(), buff, offset );
		switch ( msgType )
		{
			case MsgType::subscriptionRequest:
			{
				assert( !udata.update_ref_id_at_publisher );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &dummy, "ref_id_at_subscriber" );
				if ( udata.update_ref_id_at_subscriber )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", false );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", false );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			{
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &dummy, "ref_id_at_subscriber" );
				if ( udata.update_ref_id_at_subscriber )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_subscriber, "ref_id_at_subscriber", true );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_subscriber", true );
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &dummy, "ref_id_at_publisher" );
				if ( udata.update_ref_id_at_publisher )
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, udata.ref_id_at_publisher, "ref_id_at_publisher", false );
				else
					globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, dummy, "ref_id_at_publisher", false );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( parser.getIterator(), buff );
	}

	template<class ComposerT>
	void compose(ComposerT& composer, bool addSeparator) const
	{
		globalmq::marshalling::impl::composePublishableStructBegin( composer, "hdr" );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(type), "msg_type", true );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, state_type_id, "state_type_id", true );
		globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, priority, "priority", true );
		switch ( type )
		{
			case MsgType::subscriptionRequest:
			{
				globalmq::marshalling::impl::publishableStructComposeString( composer, path, "path", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", false );
				break;
			}
			case MsgType::subscriptionResponse:
			case MsgType::stateUpdate:
			{
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_subscriber, "ref_id_at_subscriber", false );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, ref_id_at_publisher, "ref_id_at_publisher", false );
				break;
			}
		}
		globalmq::marshalling::impl::composePublishableStructEnd( composer, addSeparator );
	}
};

template<class ComposerT>
void helperComposePublishableStateMessageBegin(ComposerT& composer, const PublishableStateMessageHeader& header)
{
	globalmq::marshalling::impl::composeStructBegin( composer );
	if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate )
	{
		header.compose( composer, true );
		globalmq::marshalling::impl::composeKey( composer, "data" );
		// next call would be generateXXXMessage()
	}
	else
		header.compose( composer, false );
}

template<class ComposerT>
void helperComposePublishableStateMessageEnd(ComposerT& composer)
{
	globalmq::marshalling::impl::composeStructEnd( composer );
}

template<class ParserT>
void helperParsePublishableStateMessageBegin( ParserT& parser, PublishableStateMessageHeader& header ) // leaves parser pos at the beginning of message data part (if any)
{
	globalmq::marshalling::impl::parseStructBegin( parser );
	header.parse( parser );
	if ( header.type == PublishableStateMessageHeader::MsgType::subscriptionResponse || header.type == PublishableStateMessageHeader::MsgType::stateUpdate )
		globalmq::marshalling::impl::parseKey( parser, "data" );
}

template<class ParserT>
void helperParsePublishableStateMessageEnd(ParserT& parser)
{
	globalmq::marshalling::impl::parseStructEnd( parser );
}

template<class ParserT, class ComposerT>
void helperParseAndUpdatePublishableStateMessage( typename ParserT::BufferType& buffFrom, typename ComposerT::BufferType& buffTo, const PublishableStateMessageHeader::UpdatedData& udata )
{
	ParserT parser( buffFrom );
	ParserT parserCurrent( buffFrom );
	PublishableStateMessageHeader header;
	globalmq::marshalling::impl::parseStructBegin( parserCurrent );
	header.parseAndUpdate<ParserT, ComposerT>( parser, parserCurrent, buffTo, udata );
}




enum StateSubscriberStatus { waitingForSubscriptionIni, subscribed };

template<class PlatformSupportT>
struct SubscriberAddress
{
	using NodeAddressT = typename PlatformSupportT::NodeAddressT;
	NodeAddressT nodeAddr;
	uint64_t subscriberAddrInNode;
};

template<class PlatformSupportT>
struct StateSubscriberData
{
	SubscriberAddress<PlatformSupportT> address;
	uint64_t externalID; // for indexing purposes
	StateSubscriberStatus status;
};

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
	template<class PlatformSupportT>
	friend class StatePublisherPool;

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
	uint64_t onSubscriptionRequest( uint64_t externalID, SubscriberAddress<PlatformSupportT> address )
	{
		// TODO: who will check uniqueness?
		subscribers.push_back( StateSubscriberData<PlatformSupportT>({address, externalID, StateSubscriberStatus::waitingForSubscriptionIni}) );
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
	using NodeAddressT = typename PlatformSupportT::NodeAddressT;

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
			res = ID2PublisherAndItsSubscriberMapping.erase( subscriber.externalID );
			assert( res == 1 );
		}
		publishers[publisher->idx].setUnused( publisher );
	}

	void onMessage( ParserT& parser, NodeAddressT nodeAddr )
	{
		PublishableStateMessageHeader mh;
//		ParserT parser( msg );
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionRequest:
			{
				GmqPathHelper::PathComponents pc;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)
				GMQ_COLL string publisherName = pc.statePublisherName;

				auto findres = name2publisherMapping.find( publisherName );
				if ( findres == name2publisherMapping.end() )
					throw std::exception(); // not found / misdirected

				uint64_t id = ++publisherAndItsSubscriberBase;
				size_t refIdAtPublisher = findres->second->onSubscriptionRequest( id, SubscriberAddress<PlatformSupportT>({nodeAddr, mh.ref_id_at_subscriber}) );
				auto ret = ID2PublisherAndItsSubscriberMapping.insert( std::make_pair( id, std::make_pair( findres->second->idx, refIdAtPublisher ) ) );
				assert( ret.second );

				PublishableStateMessageHeader hdrBack;
				hdrBack.type = PublishableStateMessageHeader::MsgType::subscriptionResponse;
				hdrBack.state_type_id = findres->second->stateTypeID();
				hdrBack.priority = mh.priority;
				hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
				hdrBack.ref_id_at_publisher = id;

				BufferT msgBack;
				ComposerT composer( msgBack );
				helperComposePublishableStateMessageBegin( composer, hdrBack );
				findres->second->generateStateSyncMessage( composer );
				helperComposePublishableStateMessageEnd( composer );
				PlatformSupportT::sendMsgFromPublisherToSubscriber( msgBack, nodeAddr );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
		/*globalmq::marshalling::impl::parseStructBegin( parser );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		switch ( msgType )
		{
			case StatePublisherSubscriberMessageType::subscriptionRequest:
			{
				GMQ_COLL string publisherName;
				globalmq::marshalling::impl::publishableParseString<ParserT, GMQ_COLL string>( parser, &publisherName, "publisher_name" );
				auto findres = publishers.find( publisherName );
				if ( findres == publishers.end() )
					throw std::exception(); // not found / misdirected
				uint64_t subscriberID;
				globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &subscriberID, "subscriber_id" );
				findres->second.onSubscriptionRequest( SubscriberAddress<PlatformSupportT>({nodeAddr, subscriberID}) );
				BufferT buff;
				ComposerT composer( buff );
				globalmq::marshalling::impl::composeStructBegin( composer );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(StatePublisherSubscriberMessageType::subscriptionResponse), "msg_type", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, subscriberID, "subscriber_id", true );
				globalmq::marshalling::impl::composeKey( composer, "state" );
				findres->second.generateStateSyncMessage( composer );
				globalmq::marshalling::impl::composeStructEnd( composer );
				PlatformSupportT::sendMsgFromPublisherToSubscriber( buff, nodeAddr );
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		globalmq::marshalling::impl::parseStructEnd( parser );*/
	}

	void postAllUpdates()
	{
		for ( auto& publisher : publishers )
		{
			PublishableStateMessageHeader mhBase;
			mhBase.type = PublishableStateMessageHeader::MsgType::stateUpdate;
			mhBase.state_type_id = publisher.stateTypeID();
			mhBase.priority = 0; // TODO: source
			mhBase.ref_id_at_subscriber = 0; // later
			mhBase.ref_id_at_publisher = 0; // later

			BufferT stateUpdateBuff = publisher.getStateUpdateBuff();

			BufferT msgBase;
			ComposerT composer( msgBase );
			helperComposePublishableStateMessageBegin( composer, mhBase );
//			publisher.generateStateSyncMessage( composer );
			ParserT stateUpdateBuffParser( stateUpdateBuff );
//			globalmq::marshalling::impl::composeKey( composer, "update" );
//			globalmq::marshalling::impl::composeStructBegin( composer );
			copy<typename ParserT::RiterT, typename ComposerT::BufferType>( stateUpdateBuffParser.getIterator(), msgBase );
//			globalmq::marshalling::impl::composeStructEnd( composer );
			helperComposePublishableStateMessageEnd( composer );

			for ( auto& subscriber : publisher.subscribers )
			{
				PublishableStateMessageHeader::UpdatedData ud;
				ud.ref_id_at_publisher = subscriber.externalID; // TODO!!! revise, this is insufficient!
				ud.update_ref_id_at_publisher = true;
				ud.update_ref_id_at_subscriber = true;
//				ud.ref_id_at_subscriber = subscriber.info.ref_id_at_subscriber;
				ud.ref_id_at_subscriber = subscriber.address.subscriberAddrInNode;
				typename ComposerT::BufferType msgForward;
				helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( msgBase, msgForward, ud );

				PlatformSupportT::sendMsgFromPublisherToSubscriber( msgForward, subscriber.address.nodeAddr );
			}
			BufferT newBuff; // just empty
			publisher.startTick( std::move( newBuff ) );
/*			BufferT stateUpdateBuff = publisher.getStateUpdateBuff();
			for ( auto& s : publisher.subscribers )
			{
				BufferT buff;
				ComposerT composer( buff );
				globalmq::marshalling::impl::composeStructBegin( composer );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(StatePublisherSubscriberMessageType::stateUpdate), "msg_type", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, s.address.subscriberAddrInNode, "subscriber_id", true );
				globalmq::marshalling::impl::composeKey( composer, "update" );

				// TODO: consider other ways to insert collected data
				auto riter = stateUpdateBuff.getReadIter();
				size_t availsz = riter.directlyAvailableSize();
				while (availsz)
				{
					const uint8_t* b = riter.directRead( availsz );
					buff.append( b, availsz );					
					availsz = riter.directlyAvailableSize();
				}
				// end of copying

				globalmq::marshalling::impl::composeStructEnd( composer );
				PlatformSupportT::sendMsgFromPublisherToSubscriber( buff, s.address.nodeAddr );
			}

			BufferT newBuff; // just empty
			publisher.startTick( std::move( newBuff ) );*/
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
	friend typename PlatformSupportT;
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;

	GMQ_COLL vector<StateSubscriberT*> subscribers; // TODO: consider mapping ID -> ptr, if states are supposed to be added and removede dynamically

public:
	void add( StateSubscriberT* subscriber )
	{
		subscribers.push_back( subscriber );
	}
	void remove( StateSubscriberT* subscriber )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i] == subscriber )
			{
				subscribers.erase( subscribers.begin() + i );
				return;
			}
		assert( false ); // not found
	}
	void subscribe( StateSubscriberT* subscriber, GMQ_COLL string path )
	{
		for ( size_t i=0; i<subscribers.size(); ++i )
			if ( subscribers[i] == subscriber )
			{
				BufferT buff;
				ComposerT composer( buff );
				PublishableStateMessageHeader mh;
				mh.type = globalmq::marshalling::PublishableStateMessageHeader::subscriptionRequest;
				mh.priority = 0; // TODO: source
				mh.state_type_id = subscriber->stateTypeID();
				mh.path = path;
				mh.ref_id_at_subscriber = i;
				helperComposePublishableStateMessageBegin( composer, mh );
				helperComposePublishableStateMessageEnd( composer );
				/*globalmq::marshalling::impl::composeStructBegin( composer );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, (uint32_t)(StatePublisherSubscriberMessageType::subscriptionRequest), "msg_type", true );
				globalmq::marshalling::impl::publishableStructComposeString( composer, subscriber->name(), "publisher_name", true );
				globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, i, "subscriber_id", false );
				globalmq::marshalling::impl::composeStructEnd( composer );*/
				PlatformSupportT::sendSubscriptionRequest( buff, subscriber->name() );
				return;
			}
		assert( false ); // not found
	}
	void onMessage( ParserT& parser ) 
	{
		PublishableStateMessageHeader mh;
//		ParserT parser( msg );
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::subscriptionResponse:
			{
				if ( mh.ref_id_at_subscriber >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[mh.ref_id_at_subscriber]->applyJsonStateSyncMessage( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[mh.ref_id_at_subscriber]->applyGmqStateSyncMessage( parser );
				}
				break;
			}
			case PublishableStateMessageHeader::MsgType::stateUpdate:
			{
				if ( mh.ref_id_at_subscriber >= subscribers.size() )
					throw std::exception(); // TODO: ... (invalid ID)
				if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
					subscribers[mh.ref_id_at_subscriber]->applyJsonMessageWithUpdates( parser );
				else 
				{
					static_assert( ParserT::proto == globalmq::marshalling::Proto::GMQ );
					subscribers[mh.ref_id_at_subscriber]->applyGmqMessageWithUpdates( parser );
				}
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
		/*globalmq::marshalling::impl::parseStructBegin( parser );
		size_t msgType;
		globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, size_t>( parser, &msgType, "msg_type" );
		switch ( msgType )
		{
			case StatePublisherSubscriberMessageType::subscriptionResponse:
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
			case StatePublisherSubscriberMessageType::stateUpdate:
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
		globalmq::marshalling::impl::parseStructEnd( parser );*/
	}
};

template<class PlatformSupportT>
class MetaPool : public StatePublisherPool<PlatformSupportT>, public StateSubscriberPool<PlatformSupportT>
{
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
	using StatePublisherT = globalmq::marshalling::StatePublisherBase<BufferT>;
	using NodeAddressT = typename PlatformSupportT::NodeAddressT;

public:
	template<class T>
	void add( T* obj )
	{
		if constexpr ( std::is_base_of<StatePublisherBase<ComposerT>, T>::value )
			StatePublisherPool<PlatformSupportT>::add( obj );
		else
		{
			static_assert ( std::is_base_of<StateSubscriberBase<BufferT>, T>::value );
			StateSubscriberPool<PlatformSupportT>::add( obj );
		}
	}

	template<class T>
	void remove( T* obj )
	{
		if constexpr ( std::is_base_of<StatePublisherBase<ComposerT>, T>::value )
			StatePublisherPool<PlatformSupportT>::remove( obj );
		else
		{
			static_assert ( std::is_base_of<StateSubscriberBase<BufferT>, T>::value );
			StateSubscriberPool<PlatformSupportT>::remove( obj );
		}
	}

	void onMessage( ParserT& parser, NodeAddressT nodeAddr )
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
				StatePublisherPool<PlatformSupportT>::onMessage( parser, nodeAddr );
				break;
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
//		globalmq::marshalling::impl::parseStructEnd( parser );
	}

	void onMessage( BufferT& buffer, NodeAddressT nodeAddr )
	{
		ParserT parser( buffer );
		onMessage( parser, nodeAddr );
	}

	void postAllUpdates()
	{
		StatePublisherPool<PlatformSupportT>::postAllUpdates();
	}
};


template<class InputBufferT, class ComposerT>
class StateConcentratorBase
{
	using OutputBufferT = typename ComposerT::BufferType;

public:
	virtual ~StateConcentratorBase() {}
	// as subscriber
	virtual void applyGmqMessageWithUpdates( GmqParser<InputBufferT>& parser ) = 0;
	virtual void applyJsonMessageWithUpdates( JsonParser<InputBufferT>& parser ) = 0;
	virtual void applyGmqStateSyncMessage( GmqParser<InputBufferT>& parser ) = 0;
	virtual void applyJsonStateSyncMessage( JsonParser<InputBufferT>& parser ) = 0;
	// as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) = 0;

	virtual const char* name() = 0;
};

template<class InputBufferT, class ComposerT>
class StateConcentratorFactoryBase
{
public:
	virtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID ) = 0;
};

} // namespace globalmq::marshalling

#endif // PUBLISHABLE_IMPL_H

