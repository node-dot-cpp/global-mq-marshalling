/* -------------------------------------------------------------------------------
* Copyright (c) 2018, OLogN Technologies AG
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

#ifndef COMPOSE_AND_PARSE_IMPL_H
#define COMPOSE_AND_PARSE_IMPL_H

#include "common.h"
#include <tuple>
#include <cstddef>


namespace m {

struct FloatingParts
{
	int64_t fraction;
	int64_t exponent;
	FloatingParts( double d ) { fromFloating( d ); }
	void fromFloating( double d ) { 
		uint64_t fraction_ = *(uint64_t*)(&d) & 0x800fffffffffffffULL; 
		fraction = *(int64_t*)(&fraction_);
		uint64_t exponent_ = ( *(uint64_t*)(&d) << 1 ) >> 53;
		exponent = *(uint64_t*)(&exponent_) - 1023;
	}
	double value() { 
		int64_t exp_ = exponent + 1023;
		uint64_t res = (*(uint64_t*)(&exp_) << 52) | *(uint64_t*)(&fraction);
		GMQ_ASSERT( ( *(uint64_t*)(&fraction) & 0x7ff0000000000000 ) == 0 );
		GMQ_ASSERT( ( exp_ & ~0x7ffLLU ) == 0 );
		return *(double*)(&res);
	}
};

template<int64_t fraction_, int64_t exponent_>
struct FloatingDefault
{
	static_assert( ( (uint64_t)(fraction_) & 0x7ff0000000000000 ) == 0 );
	static_assert( ( exponent_ & ~0x7ff ) == 0 );
	static constexpr int64_t fraction = fraction_;
	static constexpr int64_t exponent = exponent_;
	constexpr FloatingDefault() {}
	static double value() {
		int64_t exp_ = exponent + 1023;
		uint64_t res = (*(uint64_t*)(&exp_) << 52) | *(uint64_t*)(&fraction);
		GMQ_ASSERT( ( *(uint64_t*)(&fraction) & 0x7ff0000000000000 ) == 0 );
		GMQ_ASSERT( ( exp_ & ~0x7ff ) == 0 );
		return *(double*)(&res);
	}
};


enum Proto { GMQ, JSON };

struct ComposerBase {};

template<class BufferT>
class JsonComposer : public ComposerBase
{
public: // just temporary TODO: rework!
	static constexpr Proto proto = Proto::JSON;
	BufferT& buff;

public:
	JsonComposer( BufferT& buff_ ) : buff( buff_ ) {}
};

template<class BufferT>
class GmqComposer : public ComposerBase
{
public: // just temporary TODO: rework!
	static constexpr Proto proto = Proto::GMQ;
	BufferT& buff;

public:
	GmqComposer( BufferT& buff_ ) : buff( buff_ ) {}
};

} // namespace m



namespace m::impl {

static constexpr size_t integer_max_size = 8;

// supported types
struct SignedIntegralType {static constexpr bool dummy = false;};
struct UnsignedIntegralType {static constexpr bool dummy = false;};
struct RealType {static constexpr bool dummy = false;};
struct StringType {static constexpr bool dummy = false;};
struct BlobType {static constexpr bool dummy = false;};
struct ByteArrayType {static constexpr bool dummy = false;};
struct EnumType {static constexpr bool dummy = false;};
struct VectorType {static constexpr bool dummy = false;};
struct VectorOfSympleTypesBase : public VectorType {static constexpr bool dummy = false;};
template<class value_type_>
struct VectorOfSympleTypes : public VectorOfSympleTypesBase {static constexpr bool dummy = false; using value_type = value_type_;};
struct VectorOfMessageType : public VectorType {static constexpr bool dummy = false;};
struct VectorOfNonextMessageTypesBase : public VectorType {static constexpr bool dummy = false;};
//template<class value_type_>
//struct VectorOfNonextMessageTypes : public VectorOfNonextMessageTypesBase {static constexpr bool dummy = false; using value_type = value_type_;};
struct VectorOfNonextMessageTypes : public VectorOfNonextMessageTypesBase {static constexpr bool dummy = false;};

struct MessageType {static constexpr bool dummy = false;};
struct NonextMessageType : public MessageType {static constexpr bool dummy = false;};

struct NoDefaultValueType {static constexpr bool dummy = false;};
// helper types

struct StringLiteralForComposing
{
	const char* const str;
	const size_t size;
	constexpr StringLiteralForComposing( const char* const str_, const size_t size_ ) : str( str_ ), size( size_ ) {}
};


// composing

template<typename ComposerT, typename T>
void composeSignedInteger(ComposerT& composer, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		GMQ_ASSERT( num <= INT64_MAX );
	}
	/*temporary solution TODO: actual implementation*/ { 
		int64_t val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

template<typename ComposerT, typename T>
void composeUnsignedInteger(ComposerT& composer, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		GMQ_ASSERT( num >= 0 );
	}
	/*temporary solution TODO: actual implementation*/ { 
		uint64_t val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

template<typename ComposerT, typename T>
void composeReal(ComposerT& composer, T num )
{
	/*temporary solution TODO: actual implementation*/ { 
		double val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

template<typename ComposerT>
void composeString(ComposerT& composer, const GMQ_COLL string& str )
{
	composer.buff.appendString( str );
	composer.buff.appendUint8( 0 );
}

template<typename ComposerT>
void composeString(ComposerT& composer, const StringLiteralForComposing* str )
{
	composer.buff.append( str->str, str->size );
	composer.buff.appendUint8( 0 );
}

template<typename ComposerT>
void composeString(ComposerT& composer, const char* str )
{
	size_t sz = strlen( str );
	composer.buff.append( str, sz );
	composer.buff.appendUint8( 0 );
}

namespace json
{

template<typename ComposerT, typename T>
void composeSignedInteger(ComposerT& composer, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		GMQ_ASSERT( num <= INT64_MAX );
	}
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template<typename ComposerT, typename T>
void composeUnsignedInteger(ComposerT& composer, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		GMQ_ASSERT( num >= 0 );
	}
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template<typename ComposerT, typename T>
void composeReal(ComposerT& composer, T num )
{
	static_assert ( std::is_arithmetic<T>::value );
	composer.buff.appendString( fmt::format( "{}", num ) );
}

template<typename ComposerT>
void composeString(ComposerT& composer, const GMQ_COLL string& str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( str );
	composer.buff.appendUint8( '\"' );
}

template<typename ComposerT>
void composeString(ComposerT& composer, const StringLiteralForComposing* str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str->str, str->size );
	composer.buff.appendUint8( '\"' );
}

template<typename ComposerT>
void composeString(ComposerT& composer, GMQ_COLL string str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str.c_str(), str.size() );
	composer.buff.appendUint8( '\"' );
}

template<typename ComposerT>
void composeString(ComposerT& composer, const char* str )
{
	size_t sz = strlen( str );
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str, sz );
	composer.buff.appendUint8( '\"' );
}

template<typename ComposerT>
void addNamePart(ComposerT& composer, GMQ_COLL string name )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( name );
	composer.buff.appendUint8( '\"' );
	composer.buff.appendUint8( ':' );
	composer.buff.appendUint8( ' ' );
}

template<typename ComposerT, typename T>
void composeNamedSignedInteger(ComposerT& composer, GMQ_COLL string name, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		GMQ_ASSERT( num <= INT64_MAX );
	}
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template<typename ComposerT, typename T>
void composeNamedUnsignedInteger(ComposerT& composer, GMQ_COLL string name, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		GMQ_ASSERT( num >= 0 );
	}
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template<typename ComposerT, typename T>
void composeNamedReal(ComposerT& composer, GMQ_COLL string name, T num )
{
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", num ) );
}

template<typename ComposerT>
void composeNamedString(ComposerT& composer, GMQ_COLL string name, const GMQ_COLL string& str )
{
	addNamePart( composer, name );
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( str );
	composer.buff.appendUint8( '\"' );
}

template<typename ComposerT>
void composeNamedString(ComposerT& composer, GMQ_COLL string name, const StringLiteralForComposing* str )
{
	addNamePart( composer, name );
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str->str, str->size );
	composer.buff.appendUint8( '\"' );
}

} // namespace json

} // namespace m::impl

// parsing

namespace m {

class ParserBase
{
protected:
	uint8_t* begin = nullptr;
	uint8_t* end = nullptr;

public:
	void adjustParsingPos( size_t toSkip )
	{
		begin += toSkip;
	}

};

template<class BufferT>
class GmqParser : public ParserBase
{
public:
	static constexpr Proto proto = Proto::GMQ;

public:
	GmqParser( BufferT& b ) { begin = b.begin(); end = b.begin() + b.size(); }
	GmqParser( uint8_t* buff, size_t size ) { begin = buff; end = buff + size; }
	GmqParser( const GmqParser& other, size_t size ) { begin = other.begin; end = other.begin + size; }

	template <typename T>
	void parseSignedInteger( T* num )
	{
		static_assert( sizeof( T ) <= impl::integer_max_size );
		static_assert( std::is_integral<T>::value );
		/*temporary solution TODO: actual implementation*/ int64_t val = *reinterpret_cast<int64_t*>(begin); begin += sizeof( val );
		static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
		if constexpr ( std::is_signed< T >::value )
		{
			if constexpr ( sizeof( T ) == 8 )
				*num = (T)(val);
			else if constexpr ( sizeof( T ) == 4 )
			{
				GMQ_ASSERT( val >= INT32_MIN && val <= INT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				GMQ_ASSERT( val >= INT16_MIN && val <= INT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				GMQ_ASSERT( val >= INT8_MIN && val <= INT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
		else
		{
			if constexpr ( sizeof( T ) == 8 )
			{
				GMQ_ASSERT( val >= 0 );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 4 )
			{
				GMQ_ASSERT( val >= 0 && val <= UINT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				GMQ_ASSERT( val >= 0 && val <= UINT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				GMQ_ASSERT( val >= 0 && val <= UINT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
	}
	void skipSignedInteger()
	{
		/*temporary solution TODO: actual implementation*/ begin += impl::integer_max_size;
		static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
	}

	template <typename T>
	void parseUnsignedInteger( T* num )
	{
		static_assert( sizeof( T ) <= impl::integer_max_size );
		static_assert( std::is_integral<T>::value );
		/*temporary solution TODO: actual implementation*/ uint64_t val = *reinterpret_cast<uint64_t*>(begin); begin += sizeof( val );
		static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
		if constexpr ( std::is_unsigned< T >::value )
		{
			if constexpr ( sizeof( T ) == 8 )
				*num = (T)(val);
			else if constexpr ( sizeof( T ) == 4 )
			{
				GMQ_ASSERT( val <= UINT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				GMQ_ASSERT( val <= UINT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				GMQ_ASSERT( val <= UINT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
		else
		{
			if constexpr ( sizeof( T ) == 8 )
			{
				GMQ_ASSERT( val <= INT64_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 4 )
			{
				GMQ_ASSERT( val <= INT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				GMQ_ASSERT( val <= INT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				GMQ_ASSERT( val <= INT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
	}
	void skipUnsignedInteger()
	{
		/*temporary solution TODO: actual implementation*/ uint64_t val = *reinterpret_cast<uint64_t*>(begin); begin += impl::integer_max_size;
		static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
	}

	template <typename T>
	void parseReal( T* num )
	{
		static_assert( sizeof( T ) <= impl::integer_max_size );
		/*temporary solution TODO: actual implementation*/ double val = *reinterpret_cast<double*>(begin); begin += sizeof( val );
		if constexpr ( std::is_integral<T>::value )
		{
			static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
			if constexpr ( std::is_signed< T >::value )
			{
				if constexpr ( sizeof( T ) == 8 )
					*num = (T)(val);
				else if constexpr ( sizeof( T ) == 4 )
				{
					GMQ_ASSERT( val >= INT32_MIN && val <= INT32_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 2 )
				{
					GMQ_ASSERT( val >= INT16_MIN && val <= INT16_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 1 )
				{
					GMQ_ASSERT( val >= INT8_MIN && val <= INT8_MAX );
					*num = (T)(val);
				}
				else
					static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
			}
			else
			{
				if constexpr ( sizeof( T ) == 8 )
				{
					GMQ_ASSERT( val >= 0 );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 4 )
				{
					GMQ_ASSERT( val >= 0 && val <= UINT32_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 2 )
				{
					GMQ_ASSERT( val >= 0 && val <= UINT16_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 1 )
				{
					GMQ_ASSERT( val >= 0 && val <= UINT8_MAX );
					*num = (T)(val);
				}
				else
					static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
			}
		}
		else
		{
			static_assert( std::is_floating_point<T>::value );
			*num = val; // TODO: check limits for T = float
		}
	}
	void skipReal()
	{
		/*temporary solution TODO: actual implementation*/ begin += impl::integer_max_size;
		static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
	}

	void parseString( const char** str )
	{
		*str = reinterpret_cast<char*>(begin);
		while( *begin++ != 0 );
	}

	void parseString( GMQ_COLL string* str )
	{
		*str = reinterpret_cast<char*>(begin);
		while( *begin++ != 0 );
	}

	void parseString( GMQ_COLL string_view* str )
	{
		const char* start = reinterpret_cast<char*>(begin);
		while( *begin++ != 0 );
		*str = GMQ_COLL string_view( start, reinterpret_cast<char*>(begin) - start - 1 );
	}

	void skipString()
	{
		while( *begin++ != 0 );
	}
};

template<class BufferT>
class JsonParser : public ParserBase
{
public:
	static constexpr Proto proto = Proto::JSON;

	void impl_skipBlockFromJson( char left, char right )
	{
		skipSpacesEtc();
		if ( *begin++ != left )
			throw std::exception(); // TODO
		size_t ctr = 1;
		while ( begin < end )
		{
			if ( *begin == left )
				++ctr;
			else if ( *begin == right )
			{
				--ctr;
				if ( ctr == 0 )
				{
					++begin;
					return;
				}
			}
			++begin;
		}
	}

public:
	JsonParser( BufferT& b ) { begin = b.begin(); end = b.begin() + b.size(); }
	JsonParser( uint8_t* buff, size_t size ) { begin = buff; end = buff + size; }
	JsonParser( const JsonParser& other, size_t size ) { begin = other.begin; end = other.begin + size; }

	void skipSpacesEtc()
	{
		while ( begin < end && ( *begin == ' ' || *begin == '\t' || *begin == '\r' || *begin == '\n' ) ) ++begin;
	}

	bool isComma()
	{
		skipSpacesEtc();
		return *begin == ',';
	}

	void skipComma()
	{
		if ( *begin++ != ',' )
			throw std::exception(); // TODO
	}

	bool isDelimiter( char delim )
	{
		skipSpacesEtc();
		return *begin == delim;
	}

	void skipDelimiter( char delim )
	{
		skipSpacesEtc();
		if ( *begin++ != delim )
			throw std::exception(); // TODO
	}

	bool isData() { return begin != end && *begin != 0;  }

	void readStringFromJson(GMQ_COLL string* s)
	{
		skipSpacesEtc();
		if ( *begin++ != '\"' )
			throw std::exception(); // TODO
		const char* start = reinterpret_cast<const char*>( begin );
		while ( begin < end && *begin != '\"' ) ++begin;
		*s = GMQ_COLL string( start, reinterpret_cast<const char*>( begin ) - start );
		if ( begin == end )
			throw std::exception(); // TODO
		++begin;
	}
	void readStringFromJson(GMQ_COLL string_view* s)
	{
		skipSpacesEtc();
		if ( *begin++ != '\"' )
			throw std::exception(); // TODO
		const char* start = reinterpret_cast<const char*>( begin );
		while ( begin < end && *begin != '\"' ) ++begin;
		*s = GMQ_COLL string_view( start, reinterpret_cast<const char*>( begin ) - start );
		if ( begin == end )
			throw std::exception(); // TODO
		++begin;
	}
	void skipStringFromJson()
	{
		skipSpacesEtc();
		if ( *begin++ != '\"' )
			throw std::exception(); // TODO
		while ( begin < end && *begin != '\"' ) ++begin;
		if ( begin == end )
			throw std::exception(); // TODO
		++begin;
	}

	void skipVectorFromJson()
	{
		impl_skipBlockFromJson( '[', ']' );
	}
	void skipMessageFromJson()
	{
		impl_skipBlockFromJson( '{', '}' );
	}

	template <typename T>
	void readUnsignedIntegerFromJson( T* num )
	{
		skipSpacesEtc();
		if ( *begin == '-' )
			throw std::exception(); // TODO: (negative is unexpected)
		auto start = begin;
		uint64_t ret = strtoull( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ), 10 );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}
	void skipUnsignedIntegerFromJson()
	{
		skipSpacesEtc();
		if ( *begin == '-' )
			throw std::exception(); // TODO: (negative is unexpected)
		auto start = begin;
		uint64_t ret = strtoull( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ), 10 );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
	}

	template <typename T>
	void readSignedIntegerFromJson( T* num )
	{
		skipSpacesEtc();
		auto start = begin;
		int64_t ret = strtoll( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ), 10 );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}
	void skipSignedIntegerFromJson()
	{
		skipSpacesEtc();
		auto start = begin;
		int64_t ret = strtoll( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ), 10 );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
	}

	template <typename T>
	void readRealFromJson( T* num )
	{
		skipSpacesEtc();
		auto start = begin;
		double ret = strtod( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ) );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}
	void skipRealFromJson()
	{
		skipSpacesEtc();
		auto start = begin;
		double ret = strtod( reinterpret_cast<const char*>( begin ), reinterpret_cast<char**>( &begin ) );
		if ( start == begin )
			throw std::exception(); // TODO: (NaN)
	}

	void readKey(GMQ_COLL string* s)
	{
		skipSpacesEtc();
		readStringFromJson(s);
		skipSpacesEtc();
		if ( *begin++ != ':' )
			throw std::exception(); // TODO (expected ':')
	}
	void readKey(GMQ_COLL string_view* s)
	{
		skipSpacesEtc();
		readStringFromJson(s);
		skipSpacesEtc();
		if ( *begin++ != ':' )
			throw std::exception(); // TODO (expected ':')
	}

};

} // namespace m

#endif // COMPOSE_AND_PARSE_IMPL_H
