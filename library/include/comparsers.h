#ifndef COMPARSERS_H
#define COMPARSERS_H

#define GMQ_COLL std:: // TDO: tmp
#include <vector>
#include <string>
#include <assert.h>
#include <fmt/format.h>

enum Proto { GMQ, JSON };

struct ComposerBase2 {};

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
		t.rw( *this );
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
		implAddNextItemSeparator();
		buff.append( "[\n  ", sizeof("[\n  ") - 1 );
		++stack.back().count;
		stack.push_back({InType::inArray, 0});
	}
	void endArray()
	{
		assert( stack.size() > 0 && stack.back().type == InType::inArray );
		buff.append( "]\n  ", sizeof("]\n  ") - 1 );
		stack.pop_back();
	}
	void beginNamedValue( GMQ_COLL string name )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
	}
	template<class T>
	void processUnsignedInteger( T& t )
	{
		implOnAnyValue();
		implInsertUnsignedInteger( t );
	}
	template<class T>
	void processSignedInteger( T& t )
	{
		implOnAnyValue();
		implInsertSignedInteger( t );
	}
	template<class T>
	void processReal( T& t )
	{
		implOnAnyValue();
		implInsertReal( t );
	}
	template<class T>
	void processStringValue( T& t )
	{
		implOnAnyValue();
		implInsertStringValue( t );
	}
	template<class T>
	void processBooleanValue( T& t )
	{
		implOnAnyValue();
		implInsertBooleanValue( t );
	}
	template<class T>
	void processStructValue( T& t )
	{
		implOnAnyValue();
		implInsertStructValue( t );
	}
	template<class T>
	void processNamedUnsignedInteger( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		implInsertUnsignedInteger( num );
	}
	template<class T>
	void processNamedSignedInteger( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		implInsertSignedInteger( num );
	}
	template<class T>
	void processNamedReal( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		implInsertReal( num );
	}
	template<class T>
	void processNamedBoolean( GMQ_COLL string name, T& b )
	{
		implProcessNamePart( name );
		implInsertBooleanValue( b );
	}
	template<class T>
	void processNamedString( GMQ_COLL string name, T& str )
	{
		implProcessNamePart( name );
		implInsertStringValue( str );
	}
	template<class T>
	void processNamedStruct( GMQ_COLL string name, T& s )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		implInsertStructValue( s );
		stack.pop_back();
	}
	template<class T, class ValT, class LambdaProc>
	void processNamedArray( GMQ_COLL string name, std::vector<T>& v, LambdaProc proc )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		beginArray();
		for ( size_t i=0; i<v.size(); ++i )
			proc( v[i] );
		endArray();
		stack.pop_back();
	}
	template<class T>
	void processNamedArrayOfUnsignedIntegers( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, uint64_t>( name, v, [&](uint64_t& val){processUnsignedInteger( val );} );
	}
	template<class T>
	void processNamedArrayOfSignedIntegers( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, int64_t>( name, v, [&](int64_t& val){processSignedInteger( val );} );
	}
	template<class T>
	void processNamedArrayOfReals( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, double>( name, v, [&](double& val){processReal( val );} );
	}
	template<class T>
	void processNamedArrayOfStrings( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, GMQ_COLL string>( name, v, [&](GMQ_COLL string& val){processString( val );} );
	}
	template<class T>
	void processNamedArrayOfBooleans( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, bool>( name, v, [&](bool& val){processBoolean( val );} );
	}
	template<class T>
	void processNamedArrayOfStructs( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, T>( name, v, [&](T& val){processStructValue(val);} );
	}
};


struct ParserBase2 {};

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
	void beginNamedValue( GMQ_COLL string name )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
	}
	template<class T>
	void processUnsignedInteger( T& t )
	{
		implOnAnyValue();
		readUnsignedInteger( &t );
	}
	template<class T>
	void processSignedInteger( T& t )
	{
		implOnAnyValue();
		readSignedInteger( &t );
	}
	template<class T>
	void processReal( T& t )
	{
		implOnAnyValue();
		readReal( &t );
	}
	template<class T>
	void processStringValue( T& t )
	{
		implOnAnyValue();
		readString( &t );
	}
	template<class T>
	void processBooleanValue( T& t )
	{
		implOnAnyValue();
		readBoolean( &t );
	}
	template<class T>
	void processStructValue( T& t )
	{
		implOnAnyValue();
		t.rw( *this );
	}
	template<class T>
	void processNamedUnsignedInteger( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		readUnsignedInteger( &num );
	}
	template<class T>
	void processNamedSignedInteger( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		readSignedInteger( &num );
	}
	template<class T>
	void processNamedReal( GMQ_COLL string name, T& num )
	{
		implProcessNamePart( name );
		readReal( &num );
	}
	template<class T>
	void processNamedString( GMQ_COLL string name, T& str )
	{
		implProcessNamePart( name );
		readString( &str );
	}
	template<class T>
	void processNamedBoolean( GMQ_COLL string name, T& b )
	{
		implProcessNamePart( name );
		readBoolean( &b );
	}
	template<class T>
	void processNamedStruct( GMQ_COLL string name, T& s )
	{
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		s.rw( *this );
		stack.pop_back();
	}
	template<class T, class ValT, class LambdaProc>
	void processNamedArray( GMQ_COLL string name, std::vector<T>& v, LambdaProc proc )
	{
		v.clear();
		implProcessNamePart( name );
		stack.push_back({InType::inNameVal, 0});
		beginArray();
		if ( !isDelimiter( ']' ) )
		{
			ValT val;
			do {
				proc( val );
				v.push_back( val );
			}
			while ( isComma() );
		}
		endArray();
		stack.pop_back();
	}
	template<class T>
	void processNamedArrayOfUnsignedIntegers( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, uint64_t>( name, v, [&](uint64_t& val){processUnsignedInteger( val );} );
	}
	template<class T>
	void processNamedArrayOfSignedIntegers( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, int64_t>( name, v, [&](int64_t& val){processSignedInteger( val );} );
	}
	template<class T>
	void processNamedArrayOfReals( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, double>( name, v, [&](double& val){processReal( val );} );
	}
	template<class T>
	void processNamedArrayOfStrings( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, GMQ_COLL string>( name, v, [&](GMQ_COLL string& val){processString( val );} );
	}
	template<class T>
	void processNamedArrayOfBooleans( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, bool>( name, v, [&](bool& val){processBoolean( val );} );
	}
	template<class T>
	void processNamedArrayOfStructs( GMQ_COLL string name, std::vector<T>& v )
	{
		processNamedArray<T, T>( name, v, [&](T& val){processStructValue(val);} );
	}
};

#endif // COMPARSERS_H
