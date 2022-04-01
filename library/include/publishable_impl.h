/* -------------------------------------------------------------------------------
* Copyright (c) 2020-2021, OLogN Technologies AG
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

namespace globalmq::marshalling2 {
	class ParserBase;
}

namespace globalmq::marshalling {

// VECTOR support

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

	void insert_before( size_t idx, typename VectorT::value_type what ) { 
		GMQ_ASSERT( idx <= b.size());
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
						ElemTypeT::parseForStateSyncOrMessageInDepth( parser, what );
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
						ElemTypeT::parseForStateSyncOrMessageInDepth( parser, what );
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
			else if constexpr ( std::is_base_of<impl::DiscriminatedUnionType, ElemTypeT>::value )
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



// DICTIONARY support

enum ActionOnDictionary { update_value = 1, insert = 2, remove = 3 };

class PublishableDictionaryProcessor
{
public:
	template<class ParserT, class DictionaryT, class ProcType>
	static
	void parseKey( ParserT& parser, typename DictionaryT::key_type& key ) { 
		if constexpr ( std::is_same<ProcType, impl::SignedIntegralType>::value )
			impl::publishableParseInteger( parser, &key, "key" );
		else if constexpr ( std::is_same<ProcType, impl::UnsignedIntegralType>::value )
			impl::publishableParseUnsignedInteger( parser, &key, "key" );
		else if constexpr ( std::is_same<ProcType, impl::StringType>::value )
			impl::publishableParseString( parser, &key, "key" );
		else
			static_assert( std::is_same<ProcType, AllowedDataType>::value, "unsupported type" );
	}

	template<class ParserT, class DictionaryT, class ProcType>
	static
	void parseValue( ParserT& parser, typename DictionaryT::mapped_type& value ) { 
		if constexpr ( std::is_same<ProcType, impl::SignedIntegralType>::value )
			impl::publishableParseInteger( parser, &value, "value" );
		else if constexpr ( std::is_same<ProcType, impl::UnsignedIntegralType>::value )
			impl::publishableParseUnsignedInteger( parser, &value, "value" );
		else if constexpr ( std::is_same<ProcType, impl::RealType>::value )
			impl::publishableParseReal( parser, &value, "value" );
		else if constexpr ( std::is_same<ProcType, impl::StringType>::value )
			impl::publishableParseString( parser, &value, "value" );
		else if constexpr ( std::is_base_of<impl::StructType, ProcType>::value )
		{
			impl::parsePublishableStructBegin( parser, "value" );
			ProcType::parse( parser, value );
			impl::parsePublishableStructEnd( parser );
		}
		else
			static_assert( std::is_same<ProcType, AllowedDataType>::value, "unsupported type" );
	}

	template<class ParserT, class DictionaryT, class ProcType>
	static
	bool parseValueAndCompare( ParserT& parser, typename DictionaryT::mapped_type& value, const typename DictionaryT::mapped_type& oldValue ) { 
		if constexpr ( std::is_base_of<impl::StructType, ProcType>::value )
		{
			impl::parsePublishableStructBegin( parser, "value" );
			ProcType::parse( parser, value );
			impl::parsePublishableStructEnd( parser );
			return !ProcType::isSame( value, oldValue );
		}
		else 
		{
			if constexpr ( std::is_same<ProcType, impl::SignedIntegralType>::value )
				impl::publishableParseInteger( parser, &value, "value" );
			else if constexpr ( std::is_same<ProcType, impl::UnsignedIntegralType>::value )
				impl::publishableParseUnsignedInteger( parser, &value, "value" );
			else if constexpr ( std::is_same<ProcType, impl::RealType>::value )
				impl::publishableParseReal( parser, &value, "value" );
			else if constexpr ( std::is_same<ProcType, impl::StringType>::value )
				impl::publishableParseString( parser, &value, "value" );
			else
				static_assert( std::is_same<ProcType, AllowedDataType>::value, "unsupported type" );
			return value != oldValue;
		}
	}

	template<class ComposerTT, class DictionaryT, class KeyTypeT, class ValueTypeT>
	static
	void compose( ComposerTT& composer, const DictionaryT& what ) { 
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
		using key_type = typename DictionaryT::key_type;
		using value_type = typename DictionaryT::mapped_type;
		size_t collSz = what.size();
		if constexpr ( ComposerT::proto == Proto::GMQ )
		{
			impl::composeUnsignedInteger( composer, collSz );
			for ( const auto& it: what )
			{
				// key
				if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
					impl::composeSignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
					impl::composeUnsignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
					impl::composeString( composer, it.first );
				else
					static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

				// value
				if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
					impl::composeSignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
					impl::composeUnsignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
					impl::composeReal( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
					impl::composeString( composer, it.second );
				else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
				{
					impl::composeStructBegin( composer );
					ValueTypeT::compose( composer, it.second );
					impl::composeStructEnd( composer );
				}
				else
					static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );
			}
		}
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			composer.buff.append( "{", 1 );
			size_t commaCtr = 0;
			for ( const auto& it: what )
			{
				composer.buff.append( "{", 1 );

				// key
				if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
					impl::json::composeSignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
					impl::json::composeUnsignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
					impl::json::composeString( composer, it.first );
				else
					static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

				composer.buff.append( ",", 1 );

				// value
				if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
					impl::json::composeSignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
					impl::json::composeUnsignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
					impl::json::composeReal( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
					impl::json::composeString( composer, it.second );
				else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
				{
					impl::composeStructBegin( composer );
					ValueTypeT::compose( composer, it.second );
					impl::composeStructEnd( composer );
				}
				else
					static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );

				composer.buff.append( "}", 1 );

				if ( commaCtr + 1 < collSz )
				{
					composer.buff.append( ",", 1 );
					++commaCtr;
				}
			}
			assert( collSz == 0 || commaCtr + 1 == collSz );
			composer.buff.append( "}", 1 );
		}
	}

	template<class ComposerT, class DictionaryT, class KeyTypeT, class ValueTypeT, typename NameT>
	static
	void compose( ComposerT& composer, const DictionaryT& what, NameT name, bool addListSeparator ) { 
		if constexpr ( ComposerT::proto == Proto::GMQ )
			compose<ComposerT, DictionaryT, KeyTypeT, ValueTypeT>( composer, what );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			impl::json::addNamePart( composer, name );
			compose<ComposerT, DictionaryT, KeyTypeT, ValueTypeT>( composer, what );
			if ( addListSeparator )
				composer.buff.append( ",", 1 );
		}
	}

	template<class ParserT, class DictionaryT, class KeyTypeT, class ValueTypeT, bool suppressNotifications = false>
	static
	void parse( ParserT& parser, DictionaryT& dest ) { 
		using key_type = typename DictionaryT::key_type;
		using value_type = typename DictionaryT::mapped_type;
		dest.clear();
		if constexpr ( ParserT::proto == Proto::GMQ )
		{
			size_t collSz;
			parser.parseUnsignedInteger( &collSz );
			for ( size_t i=0; i<collSz; ++i )
			{
				key_type key;
				value_type value;

				// key
				if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
					parser.parseSignedInteger( &key );
				else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
					parser.parseUnsignedInteger( &key );
				else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
					parser.parseString( &key );
				else
					static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

				// value
				if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
					parser.parseSignedInteger( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
					parser.parseUnsignedInteger( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
					parser.parseReal( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
					parser.parseString( &value );
				else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
				{
					impl::parseStructBegin( parser );
					if constexpr( suppressNotifications )
						ValueTypeT::parseForStateSyncOrMessageInDepth( parser, value );
					else
						ValueTypeT::parse( parser, value );
					impl::parseStructEnd( parser );
				}
				else
					static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );

				dest.insert( std::make_pair( key, value ) );
			}
		}
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			parser.skipDelimiter( '{' );
			if ( parser.isDelimiter( '}' ) )
			{
				parser.skipDelimiter( '}' );
				if ( parser.isDelimiter( ',' ) )
					parser.skipDelimiter( ',' );
				return;
			}
			for( ;; )
			{
				key_type key;
				value_type value;

				parser.skipDelimiter( '{' );

				// key
				if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
					parser.readSignedIntegerFromJson( &key );
				else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
					parser.readUnsignedIntegerFromJson( &key );
				else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
					parser.readStringFromJson( &key );
				else
					static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

				parser.skipDelimiter( ',' );

				// value
				if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
					parser.readSignedIntegerFromJson( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
					parser.readUnsignedIntegerFromJson( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
					parser.readRealFromJson( &value );
				else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
					parser.readStringFromJson( &value );
				else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
				{
					impl::parseStructBegin( parser );
					if constexpr( suppressNotifications )
						ValueTypeT::parseForStateSyncOrMessageInDepth( parser, value );
					else
						ValueTypeT::parse( parser, value );
					impl::parseStructEnd( parser );
				}
				else
					static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );

				parser.skipDelimiter( '}' );
				dest.insert( std::make_pair( key, value ) );

				if ( parser.isDelimiter( ',' ) )
				{
					parser.skipDelimiter( ',' );
					continue;
				}
				if ( parser.isDelimiter( '}' ) )
				{
					parser.skipDelimiter( '}' );
					break;
				}
			}
			if ( parser.isDelimiter( ',' ) )
				parser.skipDelimiter( ',' );
		}
	}
};

template<class DictionaryT>
class DictionaryOfSimpleTypeRefWrapper
{
	DictionaryT& b;
public:
	using key_type = typename DictionaryT::key_type;
	using value_type = typename DictionaryT::mapped_type;
public:
	DictionaryOfSimpleTypeRefWrapper( DictionaryT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	bool get_value( const key_type key, value_type& value )
	{
		auto f = b.find( key );
		if ( f != b.end() )
		{
			value = f->second;
			return true;
		}
		return false;
	}
};

template<class DictionaryT>
class DictionaryOfCompositeTypeRefWrapper
{
	DictionaryT& b;
public:
	using key_type = typename DictionaryT::key_type;
	using value_type = typename DictionaryT::mapped_type;
public:
	DictionaryOfCompositeTypeRefWrapper( DictionaryT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	bool get_value( const key_type key, value_type& value )
	{
		auto f = b.find( key );
		if ( f != b.end() )
		{
			value = f->second;
			return true;
		}
		return false;
	}
};

template<class RefWrapperT, class DictionaryT>
class DictionaryOfStructRefWrapper
{
	DictionaryT& b;
public:
	using key_type = typename DictionaryT::key_type;
	using value_type = typename DictionaryT::mapped_type;
public:
	DictionaryOfStructRefWrapper( DictionaryT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
};

template<class DictionaryT, class KeyTypeT, class ValueTypeT, class RootT>
class DictionaryRefWrapper4Set
{
public:
	using key_type = typename DictionaryT::key_type;
	using value_type = typename DictionaryT::mapped_type;

protected:
	DictionaryT& b;
	RootT& root;
	GMQ_COLL vector<size_t> address;

private:
	/*void finalizeInsertOrUpdateOperation( KeyTypeT key, ValueTypeT value ) { 
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
	}*/
	void composeKey( key_type key, bool followedByValue ) { 
		if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
			impl::publishableStructComposeInteger( root.getComposer(), key, "key", followedByValue );
		else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
			impl::publishableStructComposeUnsignedInteger( root.getComposer(), key, "key", followedByValue );
		else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
			impl::publishableStructComposeString( root.getComposer(), key, "key", followedByValue );
		else
			static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );
	}

	void composeValue( const value_type& value ) { 
		if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
			impl::publishableStructComposeInteger( root.getComposer(), value, "value", false );
		else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
			impl::publishableStructComposeUnsignedInteger( root.getComposer(), value, "value", false );
		else if constexpr ( std::is_same<KeyTypeT, impl::RealType>::value )
			impl::publishableStructComposeReal( root.getComposer(), value, "value", false );
		else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
			impl::publishableStructComposeString( root.getComposer(), value, "value", false );
		else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
		{
			impl::composePublishableStructBegin( root.getComposer(), "value" );
			ValueTypeT::compose( root.getComposer(), value );
			impl::publishableComposeLeafeStructEnd( root.getComposer() );
		}
		else
			static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );
	}

public:
	DictionaryRefWrapper4Set( DictionaryT& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : b( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}

	size_t remove( const key_type& key ) { 
		size_t ret = b.erase( key );
		if ( ret )
		{
			impl::composeAddressInPublishable( root.getComposer(), address, ActionOnDictionary::remove );
//			impl::composeActionInPublishable( root.getComposer(), ActionOnDictionary::remove, true );
			composeKey( key, false );
			impl::composeStateUpdateBlockEnd(root.getComposer());
		}
		return ret;
	}

	bool insert( const key_type& key, const value_type& value ) { 
		auto insret = b.insert( std::make_pair( key, value ) );
		if ( insret.second )
		{
			impl::composeAddressInPublishable( root.getComposer(), address, ActionOnDictionary::insert );
//			impl::composeActionInPublishable( root.getComposer(), ActionOnDictionary::insert, false );
//			finalizeInsertOrUpdateOperation( key, value );
//			PublishableDictionaryProcessor::compose<decltype(root.getComposer()), DictionaryT, KeyTypeT, ValueTypeT>( root.getComposer(), b, "data", true );
			composeKey( key, true );
			composeValue( value );
			impl::composeStateUpdateBlockEnd(root.getComposer());
		}
		return insret.second;
	}

	bool update_value( const key_type& key, const value_type& newValue ) {
		auto f = b.find( key );
		if ( f != b.end() )
		{
			f->second = newValue;
			impl::composeAddressInPublishable( root.getComposer(), address, ActionOnDictionary::update_value );
//			impl::composeActionInPublishable( root.getComposer(), ActionOnDictionary::update_value, false );
//			finalizeInsertOrUpdateOperation( key, newValue );
//			PublishableDictionaryProcessor::compose<typename RootT::ComposerT, DictionaryT, KeyTypeT, ValueTypeT>( root.getComposer(), b, "data", true );
			composeKey( key, true );
			composeValue( newValue );
			impl::composeStateUpdateBlockEnd(root.getComposer());
			return true;
		}
		return false;
	}
};

template<class DictionaryT, class KeyTypeT, class ValueTypeT, class RootT, class RefWrapper4SetT>
class DictionaryOfStructRefWrapper4Set : public DictionaryRefWrapper4Set<DictionaryT, KeyTypeT, ValueTypeT, RootT>
{
public:
	DictionaryOfStructRefWrapper4Set( DictionaryT& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : 
		DictionaryRefWrapper4Set<DictionaryT, KeyTypeT, ValueTypeT, RootT>( actual, root_, address_, idx ) {}
//	auto get4set_at( size_t idx ) { return RefWrapper4SetT(DictionaryRefWrapper4Set<DictionaryT, ElemTypeT, RootT>::b[idx], DictionaryRefWrapper4Set<DictionaryT, ElemTypeT, RootT>::root, DictionaryRefWrapper4Set<DictionaryT, ElemTypeT, RootT>::address, idx); }
};

namespace impl {
	template<class DictionaryT, class KeyTypeT, class ValueTypeT>
	void copyDictionary( const DictionaryT& src, DictionaryT& dst )
	{
		for ( const auto& it: src )
		{
			if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
				dst.insert( std::make_pair( it.first, it.second ) );
			else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
				dst.insert( std::make_pair( it.first, it.second ) );
			else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
				dst.insert( std::make_pair( it.first, it.second ) );
			else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
				dst.insert( std::make_pair( it.first, it.second ) );
			else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
			{
				typename DictionaryT::mapped_type value;
				ValueTypeT::copy( it.second, value );
				dst.insert( std::make_pair( it.first, value ) ); // TODO: revise
			}
			else if constexpr ( std::is_base_of<impl::DiscriminatedUnionType, ValueTypeT>::value )
			{
				typename DictionaryT::mapped_type value;
				ValueTypeT::copy( it.second, value );
				dst.insert( std::make_pair( it.first, value ) ); // TODO: revise
			}
			else
				static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );
		}
	}

	template<class DictionaryT, class ValueTypeT>
	bool isSameDictionary( const DictionaryT& v1, const DictionaryT& v2 )
	{
		if ( v1.size() != v2.size() )
			return false;
		auto it1 = v1.begin();
		auto it2 = v2.begin();
		while ( it1 != v1.end() )
		{
			if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
			{
				if ( it1->first != it2->first || it1->second != it2->second ) 
					return false;
			}
			else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
			{
				if ( it1->first != it2->first || it1->second != it2->second ) 
					return false;
			}
			else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
			{
				if ( it1->first != it2->first || it1->second != it2->second ) 
					return false;
			}
			else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
			{
				if ( it1->first != it2->first || it1->second != it2->second ) 
					return false;
			}
			else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
			{
				if ( it1->first != it2->first || !ValueTypeT::isSame( it1->second, it2->second ) ) 
					return false;
			}
			else if constexpr ( std::is_base_of<impl::DiscriminatedUnionType, ValueTypeT>::value )
			{
				if ( it1->first != it2->first || !ValueTypeT::isSame( it1->second, it2->second ) ) 
					return false;
			}
			else
				static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );
			
			++it1;
			++it2;
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

template<class ComposerTT>
class StatePublisherBase
{
	template<class PlatformSupportT>
	friend class StatePublisherPool;

public:
	using ComposerT = ComposerTT;
	using BufferT = typename ComposerT::BufferType;

public:
	uint64_t idx; // for use in pools, etc

public:
	virtual ~StatePublisherBase() {}
	// interaction with state wrapper
	virtual void generateStateSyncMessage( ComposerT& composer ) = 0;
	virtual void startTick( BufferT&& buff ) = 0;
	virtual BufferT&& endTick() = 0;
	virtual const char* publishableName() = 0;
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
				auto ins = name2publisherMapping.insert( std::move( std::make_pair(publisher->publishableName(), &(publishers[i] ) ) ) );
				assert( ins.second ); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
				return i;
			}
		publishers.push_back( std::move( StatePublisherData<PlatformSupportT>(publishers.size(), publisher) ) );
		auto ins = name2publisherMapping.insert( std::move( std::make_pair(publisher->publishableName(), &(publishers[publishers.size() - 1] ) ) ) );
		assert( ins.second ); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
		return publishers.size() - 1;
	}
	void remove( globalmq::marshalling::StatePublisherBase<ComposerT>* publisher )
	{
		size_t res = name2publisherMapping.erase( publisher->publishableName() );
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
				pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)
				GMQ_COLL string publisherName = pc.statePublisherOrConnectionType;

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
			auto riter = stateUpdateBuff.getReadIter();
			ParserT stateUpdateBuffParser( riter );
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

	virtual const char* publishableName() = 0;
	virtual uint64_t stateTypeID() = 0;

	// new interface with default implementation to avoid breaking old code
	virtual void applyMessageWithUpdates( globalmq::marshalling2::ParserBase& parser ) { throw std::exception(); }
	virtual void applyStateSyncMessage( globalmq::marshalling2::ParserBase& parser ) { throw std::exception(); }

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
				else if constexpr ( ParserT::proto == globalmq::marshalling::Proto::GMQ )
					subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqStateSyncMessage( parser );
				else
					subscribers[mh.ref_id_at_subscriber].subscriber->applyStateSyncMessage( parser );
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
				else if constexpr ( ParserT::proto == globalmq::marshalling::Proto::GMQ )
					subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqMessageWithUpdates( parser );
				else
					subscribers[mh.ref_id_at_subscriber].subscriber->applyMessageWithUpdates( parser );

				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}
};


template<class T> class ClientSimpleConnectionPool;
template<class T> class ServerSimpleConnectionPool;
template<class PlatformSupportT, class PoolT>
class ConnectionBase
{
protected:
	template<class T> friend class ClientSimpleConnectionPool;
	template<class T> friend class ServerSimpleConnectionPool;

	enum Status {uninitialized, addedToPool, connRequestSent, connected, removedFromPool }; // TODO: add states as necessary; consider making specific to a particular connection
	Status status = Status::uninitialized;

	PoolT* pool = nullptr;
	uint64_t connID = 0;
	uint64_t getConnID() { return connID; }

	ConnectionBase() {}

public:
	using BufferT = typename PlatformSupportT::BufferT;

public:
	bool isConnected() { return status == Status::connected; }
	virtual void onMessage( typename BufferT::ReadIteratorT& riter ) = 0;
	void postMessage( BufferT&& buff )
	{
		assert( this->status == Status::connected );
		assert( pool != nullptr );
		pool->postMessage( connID, std::move( buff ) );
	}
};

template<class PlatformSupportT>
class ClientConnectionBase : public ConnectionBase<PlatformSupportT, ClientSimpleConnectionPool<PlatformSupportT>>
{
public:
	void connect( GMQ_COLL string path )
	{
		assert( this->status == (ConnectionBase<PlatformSupportT, ClientSimpleConnectionPool<PlatformSupportT>>::Status::addedToPool) );
		assert( this->pool != nullptr );
		this->pool->connect( this->connID, path );
	}
	virtual void onConnectionAccepted() {};
	virtual ~ClientConnectionBase() {
		// TODO: check status for being disconnected
	}
};

template<class PlatformSupportT>
class ServerConnectionBase : public ConnectionBase<PlatformSupportT, ServerSimpleConnectionPool<PlatformSupportT>>
{
public:
	virtual ~ServerConnectionBase() {}
};

template<class PlatformSupportT>
class ClientSimpleConnectionPool
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

public:
	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }

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
		connection->connID = cc.ref_id_at_client;
		connection->status = ConnectionT::Status::addedToPool;
		return cc.ref_id_at_client;
	}
	void remove( ConnectionT* connection )
	{
		assert( connection != nullptr );
		connections.erase( connection->getConnID() );
		connection->status = ConnectionT::Status::removedFromPool;
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
		conn.connection->status = ConnectionT::Status::connRequestSent;
		return;
	}

	void postMessage( uint64_t connID, BufferT&& msgBuff )
	{
		auto f = connections.find( connID );
		assert( f != connections.end() );
		auto& conn = f->second;
		assert( conn.ref_id_at_client == connID ); // self-consistency
		BufferT buff;
		ComposerT composer( buff );
		PublishableStateMessageHeader mh;
		mh.type = globalmq::marshalling::PublishableStateMessageHeader::connectionMessage;
		mh.priority = 0; // TODO: source
		mh.state_type_id_or_direction = PublishableStateMessageHeader::ConnMsgDirection::toServer;
		assert( conn.ref_id_at_client == connID );
		mh.ref_id_at_subscriber = conn.ref_id_at_client;
		assert( conn.ref_id_at_server != 0 );
		mh.ref_id_at_publisher = conn.ref_id_at_server;
		helperComposePublishableStateMessageBegin( composer, mh );
		auto riter = msgBuff.getReadIter();
		::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( riter, buff );

		helperComposePublishableStateMessageEnd( composer );
		assert( transport != nullptr );
		transport->postMessage( std::move( buff ) );
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
				if ( conn.connection->status != ConnectionT::Status::connRequestSent )
					throw std::exception(); // well, we have not requested connection yet
				conn.connection->status = ConnectionT::Status::connected;
				conn.connection->onConnectionAccepted();
				break;
			}
			case PublishableStateMessageHeader::MsgType::connectionMessage:
			{
				assert( mh.state_type_id_or_direction == PublishableStateMessageHeader::ConnMsgDirection::toClient );
				auto f = connections.find( mh.ref_id_at_subscriber );
				if ( f == connections.end() )
					throw std::exception();
				auto& conn = f->second;
				if ( conn.connection->status != ConnectionT::Status::connected )
					throw std::exception(); // TODO: revise
				assert( conn.ref_id_at_server == mh.ref_id_at_publisher ); // self-consistency
				auto riter = parser.getIterator();
				conn.connection->onMessage( riter );
				parser = riter;
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
	using OwningPtrToConnection = typename PlatformSupportT::template OwningPtrT<ServerConnectionBase<PlatformSupportT>>;
	virtual OwningPtrToConnection create() = 0;
};

template<class PlatformSupportT>
class ServerSimpleConnectionPool
{
protected:
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using ConnectionT = globalmq::marshalling::ServerConnectionBase<PlatformSupportT>;
	using OwningPtrToConnection = typename PlatformSupportT::template OwningPtrT<ServerConnectionBase<PlatformSupportT>>;

	struct ServerConnection
	{
		OwningPtrToConnection connection;
		uint64_t ref_id_at_server = 0; // that is, local id
		uint64_t ref_id_at_client = 0;
	};

	GMQ_COLL unordered_map<uint64_t, ServerConnection> connections;
	uint64_t connIdxBase = 0;

	GMQ_COLL unordered_map<GMQ_COLL string, ConnectionFactoryBase<PlatformSupportT>*> connFactories;
	GMQTransportBase<PlatformSupportT>* transport = nullptr;

public:
	ServerSimpleConnectionPool() {}
	~ServerSimpleConnectionPool() {}

	void addSimpleConnectionFactory( ConnectionFactoryBase<PlatformSupportT>* connFactory, GMQ_COLL string name )
	{
		assert( connFactory != nullptr );
		auto ins = connFactories.insert( std::make_pair( name, connFactory ) );
		assert( ins.second );
	}
	void setTransport( GMQTransportBase<PlatformSupportT>* tr ) { transport = tr; }

	void postMessage( uint64_t connID, BufferT&& msgBuff )
	{
		auto f = connections.find( connID );
		assert( f != connections.end() );
		auto& conn = f->second;
		assert( conn.ref_id_at_server == connID ); // self-consistency
		BufferT buff;
		ComposerT composer( buff );
		PublishableStateMessageHeader mh;
		mh.type = globalmq::marshalling::PublishableStateMessageHeader::connectionMessage;
		mh.priority = 0; // TODO: source
		mh.state_type_id_or_direction = PublishableStateMessageHeader::ConnMsgDirection::toClient;
		mh.ref_id_at_subscriber = conn.ref_id_at_client;
		mh.ref_id_at_publisher = conn.ref_id_at_server;
		helperComposePublishableStateMessageBegin( composer, mh );

		auto riter = msgBuff.getReadIter();
		::globalmq::marshalling::copy<typename ParserT::RiterT, typename ComposerT::BufferType>( riter, buff );

		helperComposePublishableStateMessageEnd( composer );
		assert( transport != nullptr );
		transport->postMessage( std::move( buff ) );
	}

	void onMessage( ParserT& parser ) 
	{
		PublishableStateMessageHeader mh;
		helperParsePublishableStateMessageBegin( parser, mh );
		switch ( mh.type )
		{
			case PublishableStateMessageHeader::MsgType::connectionRequest:
			{
				GmqPathHelper::PathComponents pc;
				pc.type = PublishableStateMessageHeader::MsgType::connectionRequest;
				bool pathOK = GmqPathHelper::parse( mh.path, pc );
				if ( !pathOK )
					throw std::exception(); // TODO: ... (bad path)

				auto f = connFactories.find( pc.statePublisherOrConnectionType );
				if ( f == connFactories.end() )
					throw std::exception(); // TODO:  ... (no factory for conn name)
				auto& connFactory = *f->second;

				ServerConnection sc;
				sc.connection = connFactory.create();
				sc.ref_id_at_client = mh.ref_id_at_subscriber;
				sc.ref_id_at_server = ++connIdxBase;
				sc.connection->pool = this;
				sc.connection->connID = sc.ref_id_at_server;
				sc.connection->status = ConnectionT::Status::connected;
				auto ins = connections.insert( std::move( std::make_pair( sc.ref_id_at_server, std::move(sc) ) ) );
				assert( ins.second );

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
				assert( conn.ref_id_at_server == mh.ref_id_at_publisher ); // self-consistency
				auto riter = parser.getIterator();
				conn.connection->onMessage( riter );
				parser = riter;
				break;
			}
			default:
				throw std::exception(); // TODO: ... (unknown msg type)
		}
		helperParsePublishableStateMessageEnd( parser );
	}
};

template<class PlatformSupportT>
class MetaPool : public StatePublisherPool<PlatformSupportT>, public StateSubscriberPool<PlatformSupportT>, public ClientSimpleConnectionPool<PlatformSupportT>, public ServerSimpleConnectionPool<PlatformSupportT>
{
	using BufferT = typename PlatformSupportT::BufferT;
	using ParserT = typename PlatformSupportT::ParserT;
	using ComposerT = typename PlatformSupportT::ComposerT;
	using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
	using StatePublisherT = globalmq::marshalling::StatePublisherBase<BufferT>;
//	using ConnectionT = globalmq::marshalling::ConnectionBase<BufferT>;

public:
	void setTransport( GMQTransportBase<PlatformSupportT>* tr )
	{
		StatePublisherPool<PlatformSupportT>::setTransport( tr );
		StateSubscriberPool<PlatformSupportT>::setTransport( tr );
		ClientSimpleConnectionPool<PlatformSupportT>::setTransport( tr );
		ServerSimpleConnectionPool<PlatformSupportT>::setTransport( tr );
	}

	void addSimpleConnectionFactory( ConnectionFactoryBase<PlatformSupportT>* connFactory, GMQ_COLL string name )
	{
		ServerSimpleConnectionPool<PlatformSupportT>::addSimpleConnectionFactory( connFactory, name );
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
			ClientSimpleConnectionPool<PlatformSupportT>::add( obj );
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
			static_assert ( std::is_base_of<ConnectionBase<BufferT, ClientSimpleConnectionPool<PlatformSupportT>>, T>::value );
			ClientSimpleConnectionPool<PlatformSupportT>::remove( obj );
		}
	}

	void onMessage( ParserT& parser )
	{
		typename ParserT::RiterT riter( parser.getIterator() );
		ParserT parser1( riter );
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
				ServerSimpleConnectionPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::connectionAccepted:
				ClientSimpleConnectionPool<PlatformSupportT>::onMessage( parser );
				break;
			case PublishableStateMessageHeader::MsgType::connectionMessage:
				switch ( mh.state_type_id_or_direction )
				{
					case PublishableStateMessageHeader::ConnMsgDirection::toClient:
						ClientSimpleConnectionPool<PlatformSupportT>::onMessage( parser );
						break;
					case PublishableStateMessageHeader::ConnMsgDirection::toServer:
						ServerSimpleConnectionPool<PlatformSupportT>::onMessage( parser );
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
		auto riter = buffer.getReadIter();
		ParserT parser( riter );
		onMessage( parser );
	}

	void postAllUpdates()
	{
		StatePublisherPool<PlatformSupportT>::postAllUpdates();
	}
};

} // namespace globalmq::marshalling

#endif // PUBLISHABLE_IMPL_H

