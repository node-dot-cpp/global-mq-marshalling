#ifndef COMPARSERS_H
#define COMPARSERS_H

#define GMQ_COLL std:: // TDO: tmp
#include <vector>
#include <string>
#include <assert.h>
#include <fmt/format.h>

enum Proto { GMQ, JSON };

struct ComparserBase
{
	// supported value types (exhaustive list)
	struct INT {static constexpr bool dummy = false;};
	struct UINT {static constexpr bool dummy = false;};
	struct REAL {static constexpr bool dummy = false;};
	struct STRING {static constexpr bool dummy = false;};
	struct BOOLEAN {static constexpr bool dummy = false;};
	struct ENUM {static constexpr bool dummy = false;};
	struct STRUCT {static constexpr bool dummy = false;};
};

struct ComposerBase2 : public ComparserBase
{
	static constexpr bool isComposer() { return true; }
	static constexpr bool isParser() { return false; }
	static void checkPrecondition( bool cond ) { assert( cond ); }
	static void checkPostcondition( bool cond ) {}
};

template<class BufferT, bool SC_friendly = false>
class JsonComposer2 : public ComposerBase2
{
	enum class InType {inUnknown, inStruct, inArray, inNameVal};
	struct In { InType type = InType::inUnknown; uint32_t count = 0; };
	std::vector<In> stack;
	void implAddNextItemSeparator()
	{
		assert( stack.size() > 0 );
		// TODO: offsets, (no) new lines, etc
		if ( stack.back().count != 0 )
		{
			buff.append( ",\n", 2 );
		}
	}
	template<class StringT>
	GMQ_COLL string implString2JsonString( const StringT& str )
	{
		GMQ_COLL string out;
		for ( auto& ch : str )
		{
			switch ( ch )
			{
				case '\\':
					out += "\\\\";
					break;
				case '\n':
					out += "\\n";
					break;
				case '\r':
					out += "\\r";
					break;
				case '\t':
					out += "\\t";
					break;
				case '\"':
					out += "\\\"";
					break;
				default:
					out.push_back( ch );
			}
		}
		return out;
	}
	void implInsertNamePart( GMQ_COLL string name )
	{
		auto str = fmt::format( "\"{}\":", name );
		buff.append( str.c_str(), str.size() );
	}
	void implOnAnyValue()
	{
		assert( stack.size() > 0 && ( stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) ) );

		implAddNextItemSeparator();

		if ( stack.back().type == InType::inNameVal )
		{
			stack.pop_back();
			assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		}
		else
		{
			assert( stack.size() > 0 && stack.back().type == InType::inArray );
			++(stack.back().count);
		}
	}
	void implProcessNamePart( GMQ_COLL string name )
	{
		assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		implAddNextItemSeparator();
		implInsertNamePart( name );
		++stack.back().count;
	}
	template<class T>
	void implInsertUnsignedInteger( T& t )
	{
		static_assert( std::is_arithmetic<T>::value );
		auto str = fmt::format( "{}", t );
		buff.append( str.c_str(), str.size() );
	}
	template<class T>
	void implInsertSignedInteger( T& t )
	{
		static_assert( std::is_arithmetic<T>::value );
		auto str = fmt::format( "{}", t );
		buff.append( str.c_str(), str.size() );
	}
	template<class T>
	void implInsertReal( T& t )
	{
		static_assert( std::is_arithmetic<T>::value );
		auto str = fmt::format( "{}", t );
		buff.append( str.c_str(), str.size() );
	}
	template<class T>
	void implInsertEnumValue( T& t )
	{
		using PureT = typename std::remove_reference<T>::type;
		using UnderT = typename std::underlying_type_t<PureT>;
		static_assert( sizeof( PureT ) == sizeof( UnderT ) );
		if constexpr ( std::is_signed< UnderT >::value )
		{
			UnderT num = (UnderT)(t);
			implInsertSignedInteger( num );
		}
		else
		{
			static_assert( std::is_unsigned<UnderT>::value );
			UnderT num = (UnderT)(t);
			implInsertUnsignedInteger( num );
		}
	}
	template<class T>
	void implInsertBooleanValue( T& t )
	{
		static_assert( std::is_arithmetic<T>::value );
		if ( t )
			buff.append( "true", 4 );
		else
			buff.append( "false", 5 );
	}
	template<class T>
	void implInsertStringValue( T& str )
	{
		if constexpr ( std::is_same<T, std::string>::value )
		{
			buff.appendUint8( '\"' );
			auto str1 = implString2JsonString( str );
			buff.append( str1.c_str(), str1.size() );
			buff.appendUint8( '\"' );
		}
		else 
		{
			static_assert( std::is_same<T, const char*>::value );
			std::string s(str);
			buff.appendUint8( '\"' );
			auto str1 = implString2JsonString( s );
			buff.append( str1.c_str(), str1.size() );
			buff.appendUint8( '\"' );
		}
	}
	template<class T>
	void implInsertStructValue( T& t )
	{
//		t._rw( *this );
		t.rw( *this );
	}
	template<class T, class ItemProcT>
	void implInsertStructValue( T& t, ItemProcT proc )
	{
		proc( t );
	}

	template<class T, class ItemProcT>
	void implInsertArray( std::vector<T>& v, ItemProcT proc )
	{
		beginArray();
		for ( size_t i=0; i<v.size(); ++i )
			proc( v[i] );
		endArray();
	}
	
	template<class ValueTypeT, class ValueT>
	void implInsertValue( ValueT& val )
	{
		if constexpr ( std::is_same<INT, ValueTypeT>::value )
			implInsertSignedInteger( val );
		else if constexpr ( std::is_same<UINT, ValueTypeT>::value )
			implInsertUnsignedInteger( val );
		else if constexpr ( std::is_same<REAL, ValueTypeT>::value )
			implInsertReal( val );
		else if constexpr ( std::is_same<STRING, ValueTypeT>::value )
			implInsertStringValue( val );
		else if constexpr ( std::is_same<BOOLEAN, ValueTypeT>::value )
			implInsertBooleanValue( val );
		else if constexpr ( std::is_same<ENUM, ValueTypeT>::value )
			implInsertEnumValue( val );
		else
		{
			static_assert ( std::is_same<STRUCT, ValueTypeT>::value );
			implInsertStructValue( val );
		}
	}



public: // just temporary TODO: rework!
	static constexpr bool SCFriendly = SC_friendly;
	static constexpr Proto proto = Proto::JSON;
	using BufferType = BufferT;
private:
	BufferT& buff;

public:
	JsonComposer2( BufferT& buff_ ) : buff( buff_ ) {}
	void reset() {} // TODO: add extra functionality, if starting over requires that. Do not clear buff!!! - it may already have some data in it

	void beginStruct()
	{
		assert( stack.size() == 0 || stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) );
		buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		if ( stack.size() ) // yes, it might also be the root object
			++stack.back().count;
		stack.push_back({InType::inStruct, 0});
	}
	void endStruct()
	{
		assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		buff.append( "}\n  ", sizeof("}\n  ") - 1 );
		stack.pop_back();
	}
	void beginArray()
	{
		assert( stack.size() > 0 && ( stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) ) );
//		implAddNextItemSeparator();
		buff.append( "[\n  ", sizeof("[\n  ") - 1 );
		++stack.back().count;
		stack.push_back({InType::inArray, 0});
	}
	void endArray()
	{
		assert( stack.size() > 0 && stack.back().type == InType::inArray );
		buff.append( "]\n  ", sizeof("]\n  ") - 1 );
		stack.pop_back();
		if ( stack.back().type == InType::inNameVal )
		{
			stack.pop_back();
			assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		}
		else
		{
			assert( stack.size() > 0 && stack.back().type == InType::inArray );
			++(stack.back().count);
		}
	}
	void beginNamedValue( GMQ_COLL string name )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
	}

	template<class ValueTypeT, class ValueT>
	void rw( ValueT& val )
	{
		implOnAnyValue();
		implInsertValue<ValueTypeT>( val );
	}

	template<class ValueTypeT, class ValueT>
	void rw( GMQ_COLL string name, ValueT& val )
	{
		static_assert( !std::is_invocable<ValueT, JsonComposer2, ValueT>::value );
		implProcessNamePart( name );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.push_back({InType::inNameVal, 0});

		implInsertValue<ValueTypeT>( val );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.pop_back();
	}

	template<class ValueTypeT, class ValueT, class ItemProcT>
	void rw( GMQ_COLL string name, ValueT& val, ItemProcT proc )
	{
		implProcessNamePart( name );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.push_back({InType::inNameVal, 0});

		proc( val );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.pop_back();
	}

	template<class ValueTypeT, class ValueT>
	void rw( std::vector<ValueT>& v )
	{
		implOnAnyValue();
		implInsertArray( v, [&](ValueT& val){rw<ValueTypeT>( val );} );
	}

	template<class ValueTypeT, class ValueT, class ItemProcT>
	void rw( std::vector<ValueT>& v, ItemProcT proc )
	{
		implOnAnyValue();
		implInsertArray( v, proc );
	}
	
	template<class ValueTypeT, class ValueT>
	void rw( GMQ_COLL string name, std::vector<ValueT>& v )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		implInsertArray( v, [&](ValueT& val){rw<ValueTypeT>( val );} );
	}
	
	template<class ValueTypeT, class ValueT, class LambdaProc>
	void rw( GMQ_COLL string name, std::vector<ValueT>& v, LambdaProc proc )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		implInsertArray<ValueT>( v, [&](ValueT& val){proc( val );} );
	}
};


struct ParserBase2 : public ComparserBase
{
	static constexpr bool isComposer() { return false; }
	static constexpr bool isParser() { return true; }
	static void checkPrecondition( bool cond ) {}
	static void checkPostcondition( bool cond ) { if( !cond ) throw std::exception(); }
};

template<class MessageT>
class JsonParser2 : public ParserBase2
{
public:
	static constexpr Proto proto = Proto::JSON;
	using BufferType = MessageT;
	using RiterT = typename MessageT::ReadIteratorT;

private:
	RiterT& riter;

	enum class InType {inUnknown, inStruct, inArray, inNameVal};
	struct In { InType type = InType::inUnknown; uint32_t count = 0; };
	std::vector<In> stack;

	void implOnAnyValue()
	{
		assert( stack.size() > 0 && ( stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) ) );

		if ( stack.back().type == InType::inNameVal )
		{
			stack.pop_back();
			assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		}
		else
		{
			assert( stack.size() > 0 && stack.back().type == InType::inArray );
			if ( stack.back().count > 0 )
				skipComma();
			++(stack.back().count);
		}
	}

	void skipSpacesEtc()
	{
		while ( riter.isData() && ( *riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' ) ) ++riter;
	}

	bool isComma()
	{
		skipSpacesEtc();
		return *riter == ',';
	}

	void skipComma()
	{
		skipSpacesEtc();
		if ( *riter != ',' )
			throw std::exception(); // TODO
		++riter;
	}

	bool isDelimiter( char delim )
	{
		skipSpacesEtc();
		return *riter == delim;
	}

	void skipDelimiter( char delim )
	{
		skipSpacesEtc();
		if ( *riter != delim )
			throw std::exception(); // TODO
		++riter;
	}

	void readString(GMQ_COLL string* s)
	{
		s->clear();
		skipSpacesEtc();
		if ( *riter != '\"' )
			throw std::exception(); // TODO
		++riter;
		bool done = false;
		while ( (!done) && riter.isData()  )
		{
			switch ( *riter )
			{
				case '\"':
					done = true;
					break;
				case '\\':
					++riter;
					if ( !riter.isData() )
						throw std::exception(); // TODO
					switch ( *riter )
					{
						case '\\':
							*s += '\\';
							break;
						case 't':
							*s += '\t';
							break;
						case 'r':
							*s += '\r';
							break;
						case 'n':
							*s += '\n';
							break;
						case '\"':
							*s += '\"';
							break;
						default:
							throw std::exception(); // TODO (unexpected)
					}
					++riter;
					break;
				default:
					*s += *riter;
					++riter;
					break;
			}
		}
		if ( !riter.isData() )
			throw std::exception(); // TODO
		++riter;
	}

	void readKey(GMQ_COLL string* s)
	{
		s->clear();
		skipSpacesEtc();
		readString(s);
		skipSpacesEtc();
		if ( *riter != ':' )
			throw std::exception(); // TODO (expected ':')
		++riter;
	}

	template <typename T>
	void readUnsignedInteger( T* num )
	{
		skipSpacesEtc();
		if ( *riter == '-' )
			throw std::exception(); // TODO: (negative is unexpected)
		GMQ_COLL string s;
		while ( riter.isData() && (*riter >= '0' && *riter <= '9') )
		{
			s += *riter;
			++riter;
		}
		if ( !riter.isData() )
			throw std::exception(); // TODO
		char* end = const_cast<char*>(s.c_str());
		uint64_t ret = strtoull( s.c_str(), &end, 10 );
		if ( s.c_str() == end )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}

	template <typename T>
	void readSignedInteger( T* num )
	{
		skipSpacesEtc();
		GMQ_COLL string s;
		if ( *riter == '-' )
		{
			s += *riter;
			++riter;
		}
		while ( riter.isData() && (*riter >= '0' && *riter <= '9') )
		{
			s += *riter;
			++riter;
		}
		if ( !riter.isData() )
			throw std::exception(); // TODO
		char* end = const_cast<char*>(s.c_str());
		uint64_t ret = strtoull( s.c_str(), &end, 10 );
		if ( s.c_str() == end )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}

	template <typename T>
	void readReal( T* num )
	{
		skipSpacesEtc();
		GMQ_COLL string s;
//		while ( riter.isData() && ((*riter >= '0' && *riter <= '9') || *riter == '.') ) // TODO: more than one '.' in json with errors
		while ( riter.isData() && !(*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' || *riter == ',' || *riter == ']' || *riter == '}') ) // expected terminators
		{
			s += *riter;
			++riter;
		}
		if ( !riter.isData() )
			throw std::exception(); // TODO
		char* end = const_cast<char*>(s.c_str());
		double ret = strtod( s.c_str(), &end );
		if ( s.c_str() == end )
			throw std::exception(); // TODO: (NaN)
		*num = (T)ret; // TODO: add boundary checking
	}

	template<class T>
	void readEnumValue( T* t )
	{
		using PureT = typename std::remove_pointer<T>::type;
		using UnderT = typename std::underlying_type_t<PureT>;
		static_assert( sizeof( PureT ) == sizeof( UnderT ) );
		if constexpr ( std::is_signed< UnderT >::value )
			readSignedInteger( (UnderT*)(t) );
		else
		{
			static_assert( std::is_unsigned<UnderT>::value );
			readUnsignedInteger( (UnderT*)(t) );
		}
	}

	template <typename T>
	void readBoolean( T* b )
	{
		skipSpacesEtc();
		GMQ_COLL string s;
		while ( riter.isData() && !(*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' || *riter == ',' || *riter == ']' || *riter == '}') ) // expected terminators
		{
			s += *riter;
			++riter;
		}
		if ( !riter.isData() )
			throw std::exception(); // TODO
		if ( s == "true" )
			*b = true;
		else if ( s == "false" )
			*b = false;
		else
			throw std::exception(); // TODO
	}

	void implProcessNamePart( GMQ_COLL string name )
	{
		assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		if ( stack.back().count > 0 )
			skipComma();
		GMQ_COLL string s;
		readKey(&s);
		if ( s != name )
			throw std::exception(); // TODO
		++stack.back().count;
	}

	
	template<class T>
	void readStructValue( T& t )
	{
		t.rw( *this );
	}
	template<class T, class ItemProcT>
	void readStructValue( T& t, ItemProcT proc )
	{
		proc( t );
	}
	template<class ValueTypeT, class ValueT>
	void implReadValue( ValueT& val )
	{
		if constexpr ( std::is_same<INT, ValueTypeT>::value )
			readSignedInteger( &val );
		else if constexpr ( std::is_same<UINT, ValueTypeT>::value )
			readUnsignedInteger( &val );
		else if constexpr ( std::is_same<REAL, ValueTypeT>::value )
			readReal( &val );
		else if constexpr ( std::is_same<STRING, ValueTypeT>::value )
			readString( &val );
		else if constexpr ( std::is_same<BOOLEAN, ValueTypeT>::value )
			readBoolean( &val );
		else if constexpr ( std::is_same<ENUM, ValueTypeT>::value )
			readEnumValue( &val );
		else
		{
			static_assert ( std::is_same<STRUCT, ValueTypeT>::value );
			readStructValue( val );
		}
	}
	template<class T, class ItemProcT>
	void implProcessArray( std::vector<T>& v, ItemProcT proc )
	{
		beginArray();
		if ( !isDelimiter( ']' ) )
		{
			do {
				T val;
				proc( val );
				v.push_back( std::move( val ) );
			}
			while ( isComma() );
		}
		endArray();
	}


public:
	JsonParser2( RiterT& riter_ ) : riter( riter_ ) {}
	JsonParser2( const JsonParser2& other ) : riter( other.riter ) {}
	JsonParser2& operator = ( const JsonParser2& other ) { riter = other.riter; return *this; }
	JsonParser2( JsonParser2&& other ) noexcept { riter = std::move( other.riter ); }
	JsonParser2& operator = ( JsonParser2&& other ) noexcept { riter = std::move( other.riter ); return *this; }
	~JsonParser2() {}
	void reset() {} // TODO: add extra functionality, if starting over requires that. Do not clear buff!!! - it may already have some data in it

	void beginStruct()
	{
		assert( stack.size() == 0 || stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) );
		skipDelimiter( '{' );
		if ( stack.size() )
			++stack.back().count;
		stack.push_back({InType::inStruct, 0});
	}
	void endStruct()
	{
		assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		skipDelimiter( '}' );
		stack.pop_back();
	}
	void beginArray()
	{
		assert( stack.size() > 0 && ( stack.back().type == InType::inArray || ( stack.back().type == InType::inNameVal && stack.back().count == 0 ) ) );
		skipDelimiter( '[' );
		++stack.back().count;
		stack.push_back({InType::inArray, 0});
	}
	void endArray()
	{
		assert( stack.size() > 0 && stack.back().type == InType::inArray );
		skipDelimiter( ']' );
		stack.pop_back();
		if ( stack.back().type == InType::inNameVal )
		{
			stack.pop_back();
			assert( stack.size() > 0 && stack.back().type == InType::inStruct );
		}
		else
		{
			assert( stack.size() > 0 && stack.back().type == InType::inArray );
			++(stack.back().count);
		}
	}
	void beginNamedValue( GMQ_COLL string name )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
	}

	template<class ValueTypeT, class ValueT>
	void rw( ValueT& val )
	{
		implOnAnyValue();
		implReadValue<ValueTypeT>( val );
	}

	template<class ValueTypeT, class ValueT>
	void rw( GMQ_COLL string name, ValueT& val )
	{
		static_assert( !std::is_invocable<ValueT, JsonParser2, ValueT>::value );
		implProcessNamePart( name );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.push_back({InType::inNameVal, 0});

		implReadValue<ValueTypeT>( val );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.pop_back();
	}

	template<class ValueTypeT, class ValueT, class ItemProcT>
	void rw( GMQ_COLL string name, ValueT& val, ItemProcT proc )
	{
		implProcessNamePart( name );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.push_back({InType::inNameVal, 0});

		proc( val );

		if constexpr ( std::is_same<STRUCT, ValueTypeT>::value )
			stack.pop_back();
	}

	template<class ValueTypeT, class ValueT>
	void rw( std::vector<ValueT>& v )
	{
		implOnAnyValue();
		implProcessArray( v, [&](ValueT& val){rw<ValueTypeT>( val );} );
	}

	template<class ValueTypeT, class ValueT, class ItemProcT>
	void rw( std::vector<ValueT>& v, ItemProcT proc )
	{
		implOnAnyValue();
		implProcessArray( v, proc );
	}
	
	template<class ValueTypeT, class ValueT>
	void rw( GMQ_COLL string name, std::vector<ValueT>& v )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		implProcessArray( v, [&](ValueT& val){rw<ValueTypeT>( val );} );
	}
	
	template<class ValueTypeT, class ValueT, class LambdaProc>
	void rw( GMQ_COLL string name, std::vector<ValueT>& v, LambdaProc proc )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		implProcessArray<ValueT>( v, [&](ValueT& val){proc( val );} );
	}
};

#endif // COMPARSERS_H
