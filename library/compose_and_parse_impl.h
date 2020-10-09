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

#include <type_traits>
#include <assert.h> // TODO: replace by nodecpp assertion system
#include <fmt/format.h>

namespace m {

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
			assert( ( *(uint64_t*)(&fraction) & 0x7ff0000000000000 ) == 0 );
			assert( ( exp_ & ~0x7ff ) == 0 );
			return *(double*)(&res);
		}
	};

	static constexpr size_t MIN_BUFFER = 1024;
	class Buffer {
	private:
		size_t _size = 0;
		size_t _capacity = 0;
		std::unique_ptr<uint8_t[]> _data;

	private:
		void ensureCapacity(size_t sz) { // NOTE: may invalidate pointers
			if (_data == nullptr) {
				reserve(sz);
			}
			else if (sz > _capacity) {
				size_t cp = std::max(sz, MIN_BUFFER);
				std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);
				memcpy(tmp.get(), _data.get(), _size);
				_capacity = cp;
				_data = std::move(tmp);
			}
		}

	public:
		Buffer() {}
		Buffer(size_t res) { reserve(res); }
		Buffer(Buffer&& p) {
			std::swap(_size, p._size);
			std::swap(_capacity, p._capacity);
			std::swap(_data, p._data);
		}
		Buffer& operator = (Buffer&& p) {
			std::swap(_size, p._size);
			std::swap(_capacity, p._capacity);
			std::swap(_data, p._data);
			return *this;
		}
		Buffer(const Buffer&) = delete;
		Buffer& operator = (const Buffer& p) = delete;

		void reserve(size_t sz) {
			assert( _size == 0 ); 
			assert( _capacity == 0 );
			assert( _data == nullptr );

			size_t cp = std::max(sz, MIN_BUFFER);
			std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);

			_capacity = cp;
			_data = std::move(tmp);
		}

		void append(const void* dt, size_t sz) { // NOTE: may invalidate pointers
			ensureCapacity(_size + sz);
			memcpy(end(), dt, sz);
			_size += sz;
		}

		void trim(size_t sz) { // NOTE: keeps pointers
			assert( sz <= _size );
			assert( _data != nullptr || (_size == 0 && sz == 0) );
			_size -= sz;
		}

		void clear() {
			trim(size());
		}

		void set_size(size_t sz) { // NOTE: keeps pointers
			assert( sz <= _capacity );
			assert( _data != nullptr );
			_size = sz;
		}

		size_t size() const { return _size; }
		bool empty() const { return _size == 0; }
		size_t capacity() const { return _capacity; }
		uint8_t* begin() { return _data.get(); }
		const uint8_t* begin() const { return _data.get(); }
		uint8_t* end() { return _data.get() + _size; }
		const uint8_t* end() const { return _data.get() + _size; }

		// TODO: revise and add other relevant calls
		size_t writeInt8( int8_t val, size_t pos ) {
			ensureCapacity(pos + 1);
			*reinterpret_cast<uint8_t*>(begin() + pos ) = val;
			if ( _size < pos + 1 )
				_size = pos + 1;
			return pos + 1;
		}
		size_t appendUint8( int8_t val ) {
			ensureCapacity(size() + 1);
			*reinterpret_cast<uint8_t*>(begin() + size() ) = val;
			return ++_size;
		}

		uint32_t readUInt8(size_t offset) const {
			assert( offset + 1 <= _size );
			return *(begin() + offset);
		}

		size_t appendString( const std::string& str ) {
			append( str.c_str(), str.size() );
			return _size;
		}
	};
} // namespace m



//using Buffer = nodecpp::Buffer;
using String = std::string;

namespace m {

enum Proto { GMQ, JSON };

class Composer
{
public: // just temporary TODO: rework!
	Proto proto;
	Buffer& buff;

public:
	Composer( Proto proto_, Buffer& buff_ ) : proto( proto_ ), buff( buff_ ) {}
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

struct NoDefaultValueType {static constexpr bool dummy = false;};
// helper types

struct StringLiteralForComposing
{
	const char* const str;
	const size_t size;
	constexpr StringLiteralForComposing( const char* const str_, const size_t size_ ) : str( str_ ), size( size_ ) {}
};


// composing

template <typename T>
void composeSignedInteger(Composer& composer, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		assert( num <= INT64_MAX );
	}
	/*temporary solution TODO: actual implementation*/ { 
		int64_t val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

template <typename T>
void composeUnsignedInteger(Composer& composer, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		assert( num >= 0 );
	}
	/*temporary solution TODO: actual implementation*/ { 
		uint64_t val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

template <typename T>
void composeReal(Composer& composer, T num )
{
	/*temporary solution TODO: actual implementation*/ { 
		double val = num; 
		composer.buff.append( &val, sizeof( val ) );
	}
}

inline
void composeString(Composer& composer, const std::string& str )
{
	composer.buff.appendString( str );
	composer.buff.appendUint8( 0 );
}

inline
void composeString(Composer& composer, const StringLiteralForComposing* str )
{
	composer.buff.append( str->str, str->size );
	composer.buff.appendUint8( 0 );
}

inline
void composeString(Composer& composer, const char* str )
{
	size_t sz = strlen( str );
	composer.buff.append( str, sz );
	composer.buff.appendUint8( 0 );
}

namespace json
{

template <typename T>
void composeSignedInteger(Composer& composer, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		assert( num <= INT64_MAX );
	}
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template <typename T>
void composeUnsignedInteger(Composer& composer, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		assert( num >= 0 );
	}
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

inline
void composeString(Composer& composer, const std::string& str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( str );
	composer.buff.appendUint8( '\"' );
}

inline
void composeString(Composer& composer, const StringLiteralForComposing* str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str->str, str->size );
	composer.buff.appendUint8( '\"' );
}

inline
void composeString(Composer& composer, std::string str )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str.c_str(), str.size() );
	composer.buff.appendUint8( '\"' );
}

inline
void composeString(Composer& composer, const char* str )
{
	size_t sz = strlen( str );
	composer.buff.appendUint8( '\"' );
	composer.buff.append( str, sz );
	composer.buff.appendUint8( '\"' );
}

inline
void addNamePart(Composer& composer, std::string name )
{
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( name );
	composer.buff.appendUint8( '\"' );
	composer.buff.appendUint8( ':' );
	composer.buff.appendUint8( ' ' );
}

template <typename T>
void composeNamedSignedInteger(Composer& composer, std::string name, T num )
{
	static_assert( std::is_integral<T>::value );
	if constexpr ( std::is_unsigned<T>::value && sizeof( T ) >= integer_max_size )
	{
		assert( num <= INT64_MAX );
	}
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template <typename T>
void composeNamedUnsignedInteger(Composer& composer, std::string name, T num )
{
	if constexpr ( std::is_signed<T>::value )
	{
		assert( num >= 0 );
	}
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", (int64_t)num ) );
}

template <typename T>
void composeNamedReal(Composer& composer, std::string name, T num )
{
	addNamePart( composer, name );
	composer.buff.appendString( fmt::format( "{}", num ) );
}

inline
void composeNamedString(Composer& composer, std::string name, const std::string& str )
{
	addNamePart( composer, name );
	composer.buff.appendUint8( '\"' );
	composer.buff.appendString( str );
	composer.buff.appendUint8( '\"' );
}

inline
void composeNamedString(Composer& composer, std::string name, const StringLiteralForComposing* str )
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

class Parser
{
	uint8_t* begin = nullptr;
	uint8_t* end = nullptr;

public: // TODO: Revise!
	Proto proto;

public:
	Parser( Proto proto_, Buffer& b ) : proto( proto_ ) { begin = b.begin(); end = b.begin() + b.size(); }
	Parser( Proto proto_, uint8_t* buff, size_t size ) : proto( proto_ ) { begin = buff; end = buff + size; }
	Parser( const Parser& other, size_t size ) : proto( other.proto ) { begin = other.begin; end = other.begin + size; }

	void adjustParsingPos( size_t toSkip )
	{
		begin += toSkip;
	}

	// JSON  ///////////////////////////////////////////////////////////////////

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

	void readStringFromJson(std::string* s)
	{
		skipSpacesEtc();
		if ( *begin++ != '\"' )
			throw std::exception(); // TODO
		const char* start = reinterpret_cast<const char*>( begin );
		while ( begin < end && *begin != '\"' ) ++begin;
		*s = std::string( start, reinterpret_cast<const char*>( begin ) - start );
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

	void readKey(std::string* s)
	{
		skipSpacesEtc();
		readStringFromJson(s);
		skipSpacesEtc();
		if ( *begin++ != ':' )
			throw std::exception(); // TODO (expected ':')
	}

	// GMQ  ///////////////////////////////////////////////////////////////////

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
				assert( val >= INT32_MIN && val <= INT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				assert( val >= INT16_MIN && val <= INT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				assert( val >= INT8_MIN && val <= INT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
		else
		{
			if constexpr ( sizeof( T ) == 8 )
			{
				assert( val >= 0 );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 4 )
			{
				assert( val >= 0 && val <= UINT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				assert( val >= 0 && val <= UINT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				assert( val >= 0 && val <= UINT8_MAX );
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
				assert( val <= UINT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				assert( val <= UINT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				assert( val <= UINT8_MAX );
				*num = (T)(val);
			}
			else
				static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
		}
		else
		{
			if constexpr ( sizeof( T ) == 8 )
			{
				assert( val <= INT64_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 4 )
			{
				assert( val <= INT32_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 2 )
			{
				assert( val <= INT16_MAX );
				*num = (T)(val);
			}
			else if constexpr ( sizeof( T ) == 1 )
			{
				assert( val <= INT8_MAX );
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
		if ( std::is_integral<T>::value )
		{
			static_assert( impl::integer_max_size == 8, "revise implementation otherwise" );
			if constexpr ( std::is_signed< T >::value )
			{
				if constexpr ( sizeof( T ) == 8 )
					*num = (T)(val);
				else if constexpr ( sizeof( T ) == 4 )
				{
					assert( val >= INT32_MIN && val <= INT32_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 2 )
				{
					assert( val >= INT16_MIN && val <= INT16_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 1 )
				{
					assert( val >= INT8_MIN && val <= INT8_MAX );
					*num = (T)(val);
				}
				else
					static_assert( sizeof( T ) > impl::integer_max_size ); // kinda chitting with a compiler, which treats static_assert( false ) here as an unconditional error
			}
			else
			{
				if constexpr ( sizeof( T ) == 8 )
				{
					assert( val >= 0 );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 4 )
				{
					assert( val >= 0 && val <= UINT32_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 2 )
				{
					assert( val >= 0 && val <= UINT16_MAX );
					*num = (T)(val);
				}
				else if constexpr ( sizeof( T ) == 1 )
				{
					assert( val >= 0 && val <= UINT8_MAX );
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

	void parseString( std::string* str )
	{
		*str = reinterpret_cast<char*>(begin);
		while( *begin++ != 0 );
	}

	void skipString()
	{
		while( *begin++ != 0 );
	}
};

} // namespace m

#endif // COMPOSE_AND_PARSE_IMPL_H
