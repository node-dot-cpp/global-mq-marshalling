#ifndef COMPARSERS_H
#define COMPARSERS_H

#include <assert.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <string_view>

namespace comparsers
{
	template<class DataT>
	struct UserData
	{
		using UserDataT = DataT;
		static_assert( std::is_default_constructible<DataT>::value );
		static_assert( std::is_move_constructible<DataT>::value );
		DataT userdata;
		UserData()
		{
			if constexpr ( std::is_pointer<DataT>::value )
				userdata = nullptr;
		}
	};

	template<>
	struct UserData<void>
	{
		using UserDataT = void;
	};

	template <typename Type, template <typename...> typename Template>
	struct is_specialization : std::false_type
	{
	};

	template <template <typename...> typename Template, typename... Args>
	struct is_specialization<Template<Args...>, Template> : std::true_type
	{
	};

	template <typename Type, template <typename...> typename Template>
	inline constexpr bool is_specialization_v = is_specialization<Type, Template>::value;

	template <typename T>
	inline constexpr bool is_vector_v = is_specialization_v<std::decay_t<T>, std::vector>;

    template <int>
    struct ScalarValueType
    {
        static constexpr bool isScalar = true;
    };

    template <typename T>
    struct VectorValueType
    {
        using ValueT = T;
        static constexpr bool isScalar = false;
    };

    using INT = ScalarValueType<0>;
    using UINT = ScalarValueType<1>;
    using REAL = ScalarValueType<2>;
    using STRING = ScalarValueType<3>;
    using BOOLEAN = ScalarValueType<4>;
    using ENUM = ScalarValueType<5>;
    using STRUCT = ScalarValueType<6>;
    using VofINT = VectorValueType<INT>;
    using VofUINT = VectorValueType<UINT>;
    using VofREAL = VectorValueType<REAL>;
    using VofSTRING = VectorValueType<STRING>;
    using VofBOOLEAN = VectorValueType<BOOLEAN>;
    using VofENUM = VectorValueType<ENUM>;
    using VofSTRUCT = VectorValueType<STRUCT>;

	enum class Proto
	{
		GMQ,
		JSON
	};

    struct StructDefaultValueType
    {
        uint8_t dummy = 0;
    };
	static constexpr StructDefaultValueType structDefaultValue;

    struct EmptyVectorValueType
    {
        uint8_t dummy = 0;
    };
	static constexpr EmptyVectorValueType emptyVectorValue;

    struct VoidPlaceholder
    {
        uint8_t dummy = 0;
    };
	static constexpr VoidPlaceholder voidPlaceholderValue;

	template<class DataT=void>
	struct ComparserBase : public UserData<DataT>
	{
		// checks
		template <typename ValueTypeT>
		static constexpr void assertScalarType()
		{
			static_assert(ValueTypeT::isScalar);
		}
		template <typename ValueTypeT>
		static constexpr void assertVectorType()
		{
			static_assert(!ValueTypeT::isScalar);
		}
	};

	template<class DataT=void>
	struct ComposerBase2 : public ComparserBase<DataT>
	{
		static constexpr bool isComposer()
		{
			return true;
		}
		static constexpr bool isParser()
		{
			return false;
		}
		static void checkPrecondition(bool cond)
		{
			assert(cond);
		}
		static void checkPostcondition(bool cond)
		{
		}
	};

	template <typename BufferT, bool Beautify = true, class DataT=void>
	class JsonComposer2 : public ComposerBase2<DataT>
	{
		enum class InType
		{
			inUnknown,
			inStruct,
			inArray,
			inNameVal
		};
		struct In
		{
			InType type = InType::inUnknown;
			uint32_t count = 0;
		};
		std::vector<In> stack;
		size_t offset = 0;
		void implAddNextItemSeparator()
		{
			assert(stack.size() > 0);
			// TODO: offsets, (no) new lines, etc
			if (stack.back().count != 0)
			{
				buff.append(",", 1);
			}
			if constexpr (Beautify)
			{
				buff.append("\n", 1);
				for (size_t i = 0; i < offset; ++i)
					buff.append("  ", 2);
			}
		}
		template <typename StringT>
		std::string implString2JsonString(const StringT& str)
		{
			std::string out;
			for (auto& ch : str)
			{
				switch (ch)
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
					out.push_back(ch);
				}
			}
			return out;
		}
		void implInsertNamePart(const std::string_view& name)
		{
			auto str = fmt::format("\"{}\":", name);
			buff.append(str.c_str(), str.size());
			if constexpr (Beautify)
				buff.append(" ", 1);
		}
		void implBeforeAnyValue()
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));
			implAddNextItemSeparator();
		}
		void implAfterAnyValue()
		{
			if (stack.back().type != InType::inNameVal)
			{
				assert(stack.size() > 0 && stack.back().type == InType::inArray);
				++(stack.back().count);
			}
		}
		void implProcessNamePart(const std::string_view& name)
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			implAddNextItemSeparator();
			implInsertNamePart(name);
			++stack.back().count;
		}
		template <typename T>
		void implInsertUnsignedInteger(T& t)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto str = fmt::format("{}", t);
			buff.append(str.c_str(), str.size());
		}
		template <typename T>
		void implInsertSignedInteger(T& t)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto str = fmt::format("{}", t);
			buff.append(str.c_str(), str.size());
		}
		template <typename T>
		void implInsertReal(T& t)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto str = fmt::format("{}", t);
			buff.append(str.c_str(), str.size());
		}
		template <typename T>
		void implInsertEnumValue(T& t)
		{
			using PureT = typename std::remove_reference<T>::type;
			using UnderT = typename std::underlying_type_t<PureT>;
			static_assert(sizeof(PureT) == sizeof(UnderT));
			if constexpr (std::is_signed<UnderT>::value)
			{
				UnderT num = (UnderT)(t);
				implInsertSignedInteger(num);
			}
			else
			{
				static_assert(std::is_unsigned<UnderT>::value);
				UnderT num = (UnderT)(t);
				implInsertUnsignedInteger(num);
			}
		}
		template <typename T>
		void implInsertBooleanValue(T& t)
		{
			static_assert(std::is_arithmetic<T>::value);
			if (t)
				buff.append("true", 4);
			else
				buff.append("false", 5);
		}

		void implInsertStringValue(const std::string_view& str)
		{
			buff.append("\"", 1);
			auto str1 = implString2JsonString(str);
			buff.append(str1.c_str(), str1.size());
			buff.append("\"", 1);
		}
		template <typename T>
		void implInsertStructValue(T& t)
		{
			std::remove_const_t<T>::rw(t, *this);
		}
		template <typename T, typename ItemProcT>
		void implInsertStructValue(T& t, ItemProcT&& proc)
		{
			proc(t);
		}

		template <typename Vector, typename ItemProcT, typename Enable = std::enable_if_t<is_vector_v<Vector>>>
		void implInsertArray(Vector& v, ItemProcT&& proc)
		{
			beginArray();
			for (size_t i = 0; i < v.size(); ++i)
				proc(v[i]);
			endArray();
		}

		template <typename ValueTypeT, typename ValueT>
		void implInsertScalarValue(ValueT& val)
		{
			if constexpr (std::is_same<INT, ValueTypeT>::value)
				implInsertSignedInteger(val);
			else if constexpr (std::is_same<UINT, ValueTypeT>::value)
				implInsertUnsignedInteger(val);
			else if constexpr (std::is_same<REAL, ValueTypeT>::value)
				implInsertReal(val);
			else if constexpr (std::is_same<STRING, ValueTypeT>::value)
				implInsertStringValue(val);
			else if constexpr (std::is_same<BOOLEAN, ValueTypeT>::value)
				implInsertBooleanValue(val);
			else if constexpr (std::is_same<ENUM, ValueTypeT>::value)
				implInsertEnumValue(val);
			else
			{
				static_assert(std::is_same<STRUCT, ValueTypeT>::value);
				implInsertStructValue(val);
			}
		}

		template <typename TypeHint, typename ValueT, auto defaultValue>
		bool implIsDefaultValue(const ValueT& val)
		{
			if constexpr (is_vector_v<ValueT>)
			{
				if constexpr ( std::is_same<decltype( defaultValue ), EmptyVectorValueType>::value )
					return val.empty();
				else
				{
					auto defval = defaultValue();
					if ( val.size() != defval.size() )
						return false;
					for ( size_t i=0; i<val.size(); ++i )
						if ( val[i] != defval[i] )
							return false;
					return true;
				}
			}
			else
			{
				if constexpr (std::is_same<STRUCT, TypeHint>::value)
				{
					if constexpr ( std::is_same<decltype( defaultValue ), StructDefaultValueType>::value )
						return val == ValueT( defaultValue );
					else
						return val == defaultValue();
				}
				else 
				{
					if ( std::is_invocable<decltype( defaultValue ), ValueT>::value )
						return val == defaultValue();
					else
						return val == defaultValue;
				}
			}
		}

	  public: // just temporary TODO: rework!
		static constexpr Proto proto = Proto::JSON;
		using BufferType = BufferT;

	  private:
		BufferT& buff;

	  public:
		JsonComposer2(BufferT& buff_) : buff(buff_)
		{
		}
		void reset()
		{
		} // TODO: add extra functionality, if starting over requires that. Do not clear buff!!! - it may already have some
		  // data in it

		void beginStruct([[maybe_unused]] const std::string_view& name)
		{
			assert(stack.size() == 0 || stack.back().type == InType::inArray ||
				   (stack.back().type == InType::inNameVal && stack.back().count == 0));
			buff.append("{", 1);
			if constexpr (Beautify)
				++offset;
			if (stack.size()) // yes, it might also be the root object
				++stack.back().count;
			stack.push_back({InType::inStruct, 0});
		}
		void endStruct()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			stack.pop_back();
			if constexpr (Beautify)
			{
				--offset;
				buff.append("\n  ", 1);
				for (size_t i = 0; i < offset; ++i)
					buff.append("  ", 2);
			}
			buff.append("}", 1);
		}
		void beginArray()
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));
			buff.append("[", 1);
			if constexpr (Beautify)
				++offset;
			++stack.back().count;
			stack.push_back({InType::inArray, 0});
		}
		void endArray()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inArray);
			stack.pop_back();
			if constexpr (Beautify)
			{
				--offset;
				buff.append("\n  ", 1);
				for (size_t i = 0; i < offset; ++i)
					buff.append("  ", 2);
			}
			buff.append("]", 1);
			if (stack.back().type != InType::inNameVal)
			{
				assert(stack.size() > 0 && stack.back().type == InType::inArray);
				++(stack.back().count);
			}
		}
		void beginNamedValue(const std::string_view& name)
		{
			implProcessNamePart(name);
			stack.push_back({InType::inNameVal, 0});
		}
		void endNamedValue()
		{
			stack.pop_back();
		}

		template <typename ValueTypeT, typename ValueT, typename ItemProcT,
				  typename Enable = std::enable_if_t<is_vector_v<ValueT>>>
		void rw(ValueT& v, ItemProcT&& proc)
		{
			ComparserBase<DataT>::template assertVectorType<ValueTypeT>();
			implBeforeAnyValue();
			implInsertArray(v, std::forward<ItemProcT>(proc));
			implAfterAnyValue();
		}

		template <typename ValueTypeT, typename ValueT>
		void rw(ValueT& val)
		{
			implBeforeAnyValue();
			if constexpr (is_vector_v<ValueT>)
			{
				ComparserBase<DataT>::template assertVectorType<ValueTypeT>();
				implInsertArray(val, [&](auto& e) { rw<typename ValueTypeT::ValueT>(e); });
			}
			else
			{
				ComparserBase<DataT>::template assertScalarType<ValueTypeT>();
				implInsertScalarValue<ValueTypeT>(val);
			}
			implAfterAnyValue();
		}

		template <typename ValueTypeT, typename ValueT>
		void rw(const std::string_view& name, ValueT& val)
		{
			implProcessNamePart(name);
			stack.push_back({InType::inNameVal, 0});
			if constexpr (is_vector_v<ValueT>)
			{
				ComparserBase<DataT>::template assertVectorType<ValueTypeT>();
				using ElementType = typename ValueT::value_type;
				if constexpr (std::is_same_v<ElementType, bool>)
					implInsertArray(val, [&](bool e) { rw<typename ValueTypeT::ValueT>(e); });
				else
					implInsertArray(val, [&](auto& e) { rw<typename ValueTypeT::ValueT>(e); });
				assert(stack.back().type == InType::inNameVal);
				assert(stack.back().count == 1);
			}
			else
			{
				ComparserBase<DataT>::template assertScalarType<ValueTypeT>();
				static_assert(!std::is_invocable_v<ValueT, JsonComposer2, ValueT>);
				implInsertScalarValue<ValueTypeT>(val);
			}
			stack.pop_back();
		}

		template <typename ValueTypeT, typename ValueT, auto defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val)
		{
			if ( !implIsDefaultValue<ValueTypeT, ValueT, defaultValue>( val ) )
				rw<ValueTypeT, ValueT>(name, val);
		}

		template <typename ValueTypeT, typename ValueT, typename Proc>
		void rw(const std::string_view& name, ValueT& val, Proc&& proc)
		{
			implProcessNamePart(name);
			stack.push_back({InType::inNameVal, 0});

			if constexpr (is_vector_v<ValueT>)
			{
				ComparserBase<DataT>::template assertVectorType<ValueTypeT>();
				implInsertArray<ValueT>(val, std::forward<Proc>(proc));
				assert(stack.back().type == InType::inNameVal);
				assert(stack.back().count == 1);
			}
			else
			{
				ComparserBase<DataT>::template assertScalarType<ValueTypeT>();
				proc(val);
			}

			stack.pop_back();
		}

		template <typename ValueTypeT, typename ValueT, auto defaultValue, typename Proc>
		void rwWithWireDefault(const std::string_view& name, ValueT& val, Proc&& proc)
		{
			if ( !implIsDefaultValue<ValueTypeT, ValueT, defaultValue>( val ) )
				rw<ValueTypeT, ValueT, Proc>(name, val, std::move(proc));
		}
	};

	template <class DataT=void>
	struct ParserBase2 : public ComparserBase<DataT>
	{
		static constexpr bool isComposer()
		{
			return false;
		}
		static constexpr bool isParser()
		{
			return true;
		}
		static void checkPrecondition(bool cond)
		{
		}
		static void checkPostcondition(bool cond)
		{
			if (!cond)
				throw std::exception();
		}
	};

	template <typename RiterT_, class DataT=void>
	class JsonParserBase : public ParserBase2<DataT>
	{
	  public:
		static constexpr Proto proto = Proto::JSON;
		static constexpr bool isPositionBased()
		{
			return false;
		}
		static constexpr bool isTagBased()
		{
			return true;
		}
		void fence()
		{
			static_assert( isTagBased() ); // we do nothing in this case (unlike in case of isPositionBased())
		}


		using RiterT = RiterT_;

		enum class InType
		{
			inUnknown,
			inStruct,
			inArray,
			inNameVal
		};

	  protected:
		RiterT& riter;

		void skipSpacesEtc()
		{
			while (riter.isData() && (*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n'))
				++riter;
		}

		bool isComma()
		{
			skipSpacesEtc();
			return *riter == ',';
		}

		void skipComma()
		{
			skipSpacesEtc();
			if (*riter != ',')
				throw std::exception(); // TODO
			++riter;
		}

		bool isDelimiter(char delim)
		{
			skipSpacesEtc();
			return *riter == delim;
		}

		void skipDelimiter(char delim)
		{
			skipSpacesEtc();
			if (*riter != delim)
				throw std::exception(); // TODO
			++riter;
		}

		void readString(std::string* s)
		{
			skipSpacesEtc();
			if (*riter != '\"')
				throw std::exception(); // TODO
			++riter;
			bool done = false;
			while ((!done) && riter.isData())
			{
				switch (*riter)
				{
				case '\"':
					done = true;
					break;
				case '\\':
					++riter;
					if (!riter.isData())
						throw std::exception(); // TODO
					switch (*riter)
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
			if (!riter.isData())
				throw std::exception(); // TODO
			++riter;
		}

		void readKey(std::string* s)
		{
			s->clear();
			skipSpacesEtc();
			readString(s);
			skipSpacesEtc();
			if (*riter != ':')
				throw std::exception(); // TODO (expected ':')
			++riter;
		}

		template <typename T>
		void readUnsignedInteger(T* num)
		{
			skipSpacesEtc();
			if (*riter == '-')
				throw std::exception(); // TODO: (negative is unexpected)
			std::string s;
			while (riter.isData() && (*riter >= '0' && *riter <= '9'))
			{
				s += *riter;
				++riter;
			}
			if (!riter.isData())
				throw std::exception(); // TODO
			char* end = const_cast<char*>(s.c_str());
			uint64_t ret = strtoull(s.c_str(), &end, 10);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)ret;              // TODO: add boundary checking
		}

		template <typename T>
		void readSignedInteger(T* num)
		{
			skipSpacesEtc();
			std::string s;
			if (*riter == '-')
			{
				s += *riter;
				++riter;
			}
			while (riter.isData() && (*riter >= '0' && *riter <= '9'))
			{
				s += *riter;
				++riter;
			}
			if (!riter.isData())
				throw std::exception(); // TODO
			char* end = const_cast<char*>(s.c_str());
			uint64_t ret = strtoull(s.c_str(), &end, 10);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)ret;              // TODO: add boundary checking
		}

		template <typename T>
		void readReal(T* num)
		{
			skipSpacesEtc();
			std::string s;
			while (riter.isData() && !(*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' ||
									   *riter == ',' || *riter == ']' || *riter == '}')) // expected terminators
			{
				s += *riter;
				++riter;
			}
			if (!riter.isData())
				throw std::exception(); // TODO
			char* end = const_cast<char*>(s.c_str());
			double ret = strtod(s.c_str(), &end);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)ret;              // TODO: add boundary checking
		}

		template <typename T>
		void readEnumValue(T* t)
		{
			using PureT = typename std::remove_pointer<T>::type;
			using UnderT = typename std::underlying_type_t<PureT>;
			static_assert(sizeof(PureT) == sizeof(UnderT));
			if constexpr (std::is_signed<UnderT>::value)
				readSignedInteger((UnderT*)(t));
			else
			{
				static_assert(std::is_unsigned<UnderT>::value);
				readUnsignedInteger((UnderT*)(t));
			}
		}

		template <typename T>
		void readBoolean(T* b)
		{
			skipSpacesEtc();
			std::string s;
			while (riter.isData() && !(*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' ||
									   *riter == ',' || *riter == ']' || *riter == '}')) // expected terminators
			{
				s += *riter;
				++riter;
			}
			if (!riter.isData())
				throw std::exception(); // TODO
			if (s == "true")
				*b = true;
			else if (s == "false")
				*b = false;
			else
				throw std::exception(); // TODO
		}

		void readObjectAsJson( std::string& buf )
		{
			assert( *riter == '{' );
			buf.append("{", 1);
			++riter;
			size_t depth = 1;
			do
			{
				char ch = *riter;
				buf.append( &ch, 1 );
				switch ( ch )
				{
					case '}': 
						++riter;
						--depth;
						if ( depth == 0 )
							return;
						break;
					case '{':
						++riter;
						++depth;
						break;
					case '\"':
						readString( &buf );
						buf.append("\"", 1);
						break;
					default:
						++riter;
						break;
				}
			}
			while ( 1 );
		}

		void readArrayAsJson( std::string& buf )
		{
			assert( *riter == '[' );
			buf.append("[", 1);
			++riter;
			size_t depth = 1;
			do
			{
				char ch = *riter;
				buf.append( &ch, 1 );
				switch ( ch )
				{
					case ']': 
						++riter;
						--depth;
						if ( depth == 0 )
							return;
						break;
					case '[':
						++riter;
						++depth;
						break;
					case '\"':
						readString( &buf );
						buf.append("\"", 1);
						break;
					default:
						++riter;
						break;
				}
			}
			while ( 1 );
		}

		void readValueAsJson( std::string& buf )
		{
			skipSpacesEtc();
			switch ( *riter )
			{
				case '[' :
					readArrayAsJson( buf );
					break;
				case '{' :
					readObjectAsJson( buf );
					break;
				case '\"' :
				{
					buf.append("\"", 1);
					readString( &buf );
					buf.append("\"", 1);
					break;
				}
				default: // true, false, none, number
				{
					while (riter.isData() && !(*riter == ' ' || *riter == '\t' || *riter == '\r' || *riter == '\n' ||
											   *riter == ',' || *riter == ']' || *riter == '}')) // expected terminators
					{
						char ch = *riter;
						buf.append( &ch, 1 );
						++riter;
					}
					if (!riter.isData())
						throw std::exception(); // TODO
				}
			}
		}

		JsonParserBase(RiterT& riter_) : riter(riter_)
		{
		}
		JsonParserBase(const JsonParserBase& other) : riter(other.riter)
		{
		}
		JsonParserBase& operator=(const JsonParserBase& other)
		{
			riter = other.riter;
			return *this;
		}
		JsonParserBase(JsonParserBase&& other) noexcept
		{
			riter = std::move(other.riter);
		}
		JsonParserBase& operator=(JsonParserBase&& other) noexcept
		{
			riter = std::move(other.riter);
			return *this;
		}
		~JsonParserBase()
		{
		}
		void reset()
		{
		} // TODO: add extra functionality, if starting over requires that. Do not clear buff!!! - it may already have some
	};

#include <unordered_map>

	template <typename RiterT, class DataT=void>
	class JsonParser2 : public JsonParserBase<RiterT, DataT>
	{
		template <typename RiterT_, class DataT_>
		friend class JsonParser2;
		template<class StringT>
		class StringAsRiter
		{
			const StringT& str;
			size_t pos = 0;

		public:
			StringAsRiter( const StringT& str_ ) : str( str_ ) {}
			typename StringT::value_type operator* () 
			{
				assert( pos < str.size() );
				return str[pos];
			}
			bool isData()
			{
				return pos < str.size();
			}
			void operator ++ () 
			{
				++pos;
			}
		};

	  public:
		static constexpr Proto proto = JsonParserBase<RiterT, DataT>::proto;
//		using BufferType = MessageT;
		using InType = JsonParserBase<RiterT, DataT>::InType;

	  private:
		struct In
		{
			InType type = InType::inUnknown;
			uint32_t count = 0;
			std::unordered_map<std::string, std::string> ooo;
		};
		std::vector<In> stack;

		void implBeforeAnyValue()
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));

			if (stack.back().type == InType::inArray)
			{
				if (stack.back().count > 0)
					this->skipComma();
				++(stack.back().count);
			}
		}

		void implAfterAnyValue()
		{
		}

		void implProcessNamePart(const std::string_view& name)
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			if (stack.back().count > 0)
				this->skipComma();
			std::string s;
			this->readKey(&s);
			if (s != name)
				throw std::exception(); // TODO
			++stack.back().count;
		}

		std::string implProcessNamePart()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			if (stack.back().count > 0)
				this->skipComma();
			std::string s;
			this->readKey(&s);
			return s;
		}

		template <typename T>
		void readStructValue(T& t)
		{
			std::remove_const_t<T>::rw(t, *this);
		}
		template <typename T, typename ItemProcT>
		void readStructValue(T& t, ItemProcT&& proc)
		{
			proc(t);
		}
		template <typename ValueTypeT, typename ValueT>
		void implReadValue(ValueT& val)
		{
			if constexpr (std::is_same<INT, ValueTypeT>::value)
				this->readSignedInteger(&val);
			else if constexpr (std::is_same<UINT, ValueTypeT>::value)
				this->readUnsignedInteger(&val);
			else if constexpr (std::is_same<REAL, ValueTypeT>::value)
				this->readReal(&val);
			else if constexpr (std::is_same<STRING, ValueTypeT>::value)
				this->readString(&val);
			else if constexpr (std::is_same<BOOLEAN, ValueTypeT>::value)
				this->readBoolean(&val);
			else if constexpr (std::is_same<ENUM, ValueTypeT>::value)
				this->readEnumValue(&val);
			else
			{
				static_assert(std::is_same<STRUCT, ValueTypeT>::value);
				readStructValue(val);
			}
		}
		template <typename T, typename ItemProcT>
		void implProcessArray(std::vector<T>& v, ItemProcT&& proc)
		{
			beginArray();
			if (!this->isDelimiter(']'))
			{
				do
				{
					T val;
					proc(val);
					v.push_back(std::move(val));
				} while (this->isComma());
			}
			endArray();
		}

	  public:
		JsonParser2(RiterT& riter_) : JsonParserBase<RiterT, DataT>(riter_)
		{
		}
		JsonParser2(const JsonParser2& other) : JsonParserBase<RiterT, DataT>(other.riter)
		{
		}
		JsonParser2& operator=(const JsonParser2& other)
		{
			this->riter = other.riter;
			return *this;
		}
		JsonParser2(JsonParser2&& other) noexcept
		{
			this->riter = std::move(other.riter);
		}
		JsonParser2& operator=(JsonParser2&& other) noexcept
		{
			this->riter = std::move(other.riter);
			return *this;
		}
		~JsonParser2()
		{
		}
		void reset()
		{
		} // TODO: add extra functionality, if starting over requires that. Do not clear buff!!! - it may already have some
		  // data in it

		void beginStruct([[maybe_unused]] const std::string_view& name)
		{
			assert(stack.size() == 0 || stack.back().type == InType::inArray ||
				   (stack.back().type == InType::inNameVal && stack.back().count == 0));
			this->skipDelimiter('{');
			if (stack.size())
				++stack.back().count;
			stack.push_back({InType::inStruct, 0});
		}
		void endStruct()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			this->skipDelimiter('}');
			stack.pop_back();
		}
		void beginArray()
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));
			this->skipDelimiter('[');
			++stack.back().count;
			stack.push_back({InType::inArray, 0});
		}
		void endArray()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inArray);
			this->skipDelimiter(']');
			stack.pop_back();
			if (stack.back().type != InType::inNameVal)
			{
				assert(stack.size() > 0 && stack.back().type == InType::inArray);
				++(stack.back().count);
			}
		}
		void beginNamedValue(const std::string_view& name)
		{
			implProcessNamePart(name);
			stack.push_back({InType::inNameVal, 0});
		}
		void endNamedValue()
		{
			stack.pop_back();
		}

		template <typename TypeHint, typename ValueT>
		void rw(ValueT& val)
		{
			implBeforeAnyValue();
			if constexpr (is_vector_v<ValueT>)
			{
				ParserBase2<DataT>::template assertVectorType<TypeHint>();
				implProcessArray(val, [&](auto& element) { rw<typename TypeHint::ValueT>(element); });
			}
			else
			{
				this->implReadValue<TypeHint>(val);
			}
			implAfterAnyValue();
		}

		template <typename TypeHint, typename Vector, typename ItemProcT,
				  typename Enable = std::enable_if_t<is_vector_v<Vector>>>
		void rw(Vector& v, ItemProcT&& proc)
		{
			ParserBase2<DataT>::template assertVectorType<TypeHint>();
			implBeforeAnyValue();
			implProcessArray(v, std::forward<ItemProcT>(proc));
			implAfterAnyValue();
		}

		template <typename TypeHint, typename ValueT>
		void implRWValue(ValueT& val)
		{
			if constexpr (is_vector_v<ValueT>)
			{
				ParserBase2<DataT>::template assertVectorType<TypeHint>();
				implProcessArray(val, [&](auto& element) { rw<typename TypeHint::ValueT>(element); });
			}
			else
			{
				static_assert(!std::is_invocable<ValueT, JsonParser2, ValueT>::value);
				this->implReadValue<TypeHint>(val);
			}
		}

		template <typename TypeHint, typename ValueT, typename ItemProc>
		void implRWValue(ValueT& val, ItemProc&& proc)
		{
			if constexpr (is_vector_v<ValueT>)
			{
				ParserBase2<DataT>::template assertVectorType<TypeHint>();
				implProcessArray(val, std::forward<ItemProc>(proc));
			}
			else
			{
				proc(val);
			}
		}

		template <typename TypeHint, typename ValueT, auto defaultValue>
		void implApplyDefault(ValueT& val)
		{
			if constexpr (is_vector_v<ValueT>)
			{
				if constexpr ( std::is_same<decltype( defaultValue ), EmptyVectorValueType>::value )
					return;
				else
					val = defaultValue();
			}
			else
			{
				if constexpr (std::is_same<STRUCT, TypeHint>::value)
				{
					if constexpr ( std::is_same<decltype( defaultValue ), StructDefaultValueType>::value )
						val = ValueT( defaultValue );
					else
						val = defaultValue();
				}
				else 
				{
					if ( std::is_invocable<decltype( defaultValue ), ValueT>::value )
						val = defaultValue();
					else
						val = defaultValue;
				}
			}
		}

		template <typename TypeHint, typename ValueT, typename ValueProc, auto DefaultValue>
		void implRWNameValue(const std::string_view& name, ValueT& val, ValueProc&& proc)
		{
			auto f = stack.back().ooo.find( std::string( name ) );
			if ( f != stack.back().ooo.end() )
			{
				StringAsRiter<std::string> sr( f->second );
				JsonParser2<StringAsRiter<std::string>, DataT> p( sr );
				if constexpr ( !std::is_same<DataT, void>::value )
					p.userdata = std::move( this->userdata );
				p.stack.push_back({JsonParser2<StringAsRiter<std::string>, DataT>::InType::inNameVal, 0});
				if constexpr ( std::is_same<ValueProc, VoidPlaceholder>::value )
					p.implRWValue<TypeHint, ValueT>(val);
				else
					p.implRWValue<TypeHint, ValueT, ValueProc>(val, std::move( proc ) );
				p.endNamedValue();
				if constexpr ( !std::is_same<DataT, void>::value )
					this->userdata = std::move( p.userdata );
			}
			else
			{
				std::string s = implProcessNamePart();
				while ( s != name )
				{
					std::string buf;
					this->readValueAsJson( buf );
					++stack.back().count;
					stack.back().ooo.insert( std::make_pair( s, buf ) );
					this->skipSpacesEtc();
					if ( *(this->riter) == '}' )
					{
						if constexpr ( std::is_same<decltype(DefaultValue), VoidPlaceholder>::value )
							throw std::exception(); // not found; rwWithWireDefault() might be helpful
						else
							implApplyDefault<TypeHint, ValueT, DefaultValue>(val);
					}
					s = implProcessNamePart();
				}
				++stack.back().count;
				stack.push_back({InType::inNameVal, 0});
				if constexpr ( std::is_same<ValueProc, VoidPlaceholder>::value )
					implRWValue<TypeHint, ValueT>(val);
				else
					implRWValue<TypeHint, ValueT, ValueProc>(val, std::move( proc ) );
				stack.pop_back();
			}
		}

		template <typename TypeHint, typename ValueT>
		void rw(const std::string_view& name, ValueT& val)
		{
			VoidPlaceholder dummyProc;
			implRWNameValue<TypeHint, ValueT, VoidPlaceholder, voidPlaceholderValue>(name, val, std::move( dummyProc ));
		}

		template <typename TypeHint, typename ValueT, auto defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val)
		{
			VoidPlaceholder dummyProc;
			implRWNameValue<TypeHint, ValueT, VoidPlaceholder, defaultValue>(name, val, std::move( dummyProc ));
		}

		template <typename TypeHint, typename ValueT, typename ItemProc>
		void rw(const std::string_view& name, ValueT& val, ItemProc&& proc)
		{
			implRWNameValue<TypeHint, ValueT, ItemProc, voidPlaceholderValue>(name, val, std::move( proc ));
		}

		template <typename TypeHint, typename ValueT, typename ItemProc, auto defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val, ItemProc&& proc)
		{
			implRWNameValue<TypeHint, ValueT, ItemProc, defaultValue>(name, val, std::move( proc ));
		}
	};

} // namespace comparsers

#endif // COMPARSERS_H
