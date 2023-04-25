#ifndef COMPARSERS_H
#define COMPARSERS_H

#include <assert.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <functional>
#include <array>

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
	template<typename T> concept has_equality_comparison_with_struct_default_value = requires(T t) { { t.operator == (StructDefaultValueType()) }; };
	template<typename T> concept has_equality_comparison_with_struct_itself = requires(T t) { { t.operator == (T()) }; };

    struct EmptyVectorValueType
    {
        uint8_t dummy = 0;
    };
	static constexpr EmptyVectorValueType emptyVectorValue;

	struct StringLiteralHelperIndicator {};
	template<unsigned int charCnt>
	struct StringLiteralHelper : public StringLiteralHelperIndicator
	{
		char str[charCnt]{};
		constexpr StringLiteralHelper(const char (&s)[charCnt]) 
		{
			for (unsigned int i=0; i<charCnt; ++i) 
				str[i] = s[i];
		}
		constexpr operator const char*() const
		{
			return str;
		}
	};

	template<class T>
	struct is_std_array:std::is_array<T>{};
	template<class T, std::size_t N>
	struct is_std_array<std::array<T,N>>:std::true_type{};
    
	struct FloatingConstIndicator {};
	template <class _Ty>
	class FloatingConst : public FloatingConstIndicator {
	public:
		using value_type      = _Ty;	
		constexpr FloatingConst( _Ty f ) { _Elem = f; }
		constexpr _Ty val() const { return _Elem; }
		_Ty _Elem;
	};
	using FLOAT = FloatingConst<float>;
	using DOUBLE = FloatingConst<double>;

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
				if constexpr ( std::is_same<decltype( defaultValue ), EmptyVectorValueType>::value || 
					std::is_same<decltype( defaultValue ), decltype( emptyVectorValue )>::value ) // NOTE: the second component is due to f..ing GCC which sees types differently
					return val.empty();
				else if constexpr ( is_std_array<std::remove_cvref_t<decltype( defaultValue )>>::value )
				{
					if ( val.size() != defaultValue.size() )
						return false;
					for ( size_t i=0; i<val.size(); ++i )
						if ( val[i] != defaultValue[i] )
							return false;
					return true;
				}
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
					if constexpr ( std::is_same<decltype( defaultValue ), StructDefaultValueType>::value || 
						std::is_same<decltype( defaultValue ), decltype( structDefaultValue)>::value ) // NOTE: the second component is due to f..ing GCC which sees types differently
					{
						if constexpr ( has_equality_comparison_with_struct_default_value<ValueT> )
							return val == structDefaultValue;
						else
						{
							static_assert ( has_equality_comparison_with_struct_itself<ValueT>, "one of comparison operators required" );
							return val == ValueT(structDefaultValue);
						}
					}
					else
						return val == defaultValue();
				}
				else 
				{
					if constexpr ( std::is_invocable<decltype( defaultValue )>::value )
						return val == defaultValue();
					else
					{
						if constexpr ( std::is_base_of<StringLiteralHelperIndicator, decltype( defaultValue )>::value )
							return val == (const char*)defaultValue;
						else if constexpr ( std::is_base_of<FloatingConstIndicator, decltype( defaultValue )>::value )
							return val == defaultValue.val();
						else
							return val == defaultValue;
					}
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

		template <typename ValueTypeT, typename ValueT, StringLiteralHelper defaultValue>
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

		template <typename ValueTypeT, typename ValueT, StringLiteralHelper defaultValue, typename Proc>
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
		using CharT = typename RiterT::CharT;

		enum class InType
		{
			inUnknown,
			inStruct,
			inArray,
			inNameVal
		};

		class LastReadChar
		{
			CharT ch = 0;
			bool set = false;
		public:
			void save( CharT ch_ )
			{
				assert( !set );
				ch = ch_;
				set = true;
			}
			bool isSaved()
			{
				return set;
			}
			CharT consume()
			{
				assert( set );
				CharT ret = ch;
				ch = 0;
				set = false;
				return ret;
			}
		};
		LastReadChar lastChar;

	  protected:
		RiterT& riter;

		CharT skipSpacesEtc( CharT firstChar )
		{
			while (riter.isData() && (firstChar == ' ' || firstChar == '\t' || firstChar == '\r' || firstChar == '\n'))
				firstChar = riter.readChar();
			return firstChar;
		}

		CharT skipSpacesEtc()
		{
			return skipSpacesEtc(riter.readChar());
		}

		/*bool isComma()
		{
			CharT ret = skipSpacesEtc();
			return ret == ',';
		}

		bool isComma( CharT ch )
		{
			CharT ret = skipSpacesEtc(ch);
			return ret == ',';
		}*/

		void skipComma()
		{
			CharT ret = skipSpacesEtc();
			if (ret != ',')
				throw std::exception(); // TODO
		}

		void skipComma( CharT ch )
		{
			CharT ret = skipSpacesEtc(ch);
			if (ret != ',')
				throw std::exception(); // TODO
		}

		/*bool isDelimiter(char delim)
		{
			CharT ret = skipSpacesEtc();
			return ret == delim;
		}

		bool isDelimiter(char delim, CharT ch)
		{
			CharT ret = skipSpacesEtc(ch);
			return ret == delim;
		}*/

		void skipDelimiter( char delim )
		{
			CharT ret = skipSpacesEtc();
			if ( ret != delim )
				throw std::exception(); // TODO
		}

		void skipDelimiter( char delim, CharT ch )
		{
			CharT ret = skipSpacesEtc(ch);
			if ( ret != delim )
				throw std::exception(); // TODO
		}

		template<class StringT>
		void implContinueReadingString(StringT* s)
		{
			bool done = false;
			while ((!done) && riter.isData())
			{
				CharT ret = riter.readChar();
				switch (ret)
				{
				case '\"':
					done = true;
					break;
				case '\\':
					ret = riter.readChar();
					if (!riter.isData())
						throw std::exception(); // TODO
					switch (ret)
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
					break;
				default:
					*s += ret;
					break;
				}
			}
			if (!riter.isData())
				throw std::exception(); // TODO
//			++riter;
		}

		void readString(std::string* s)
		{
			CharT ret = skipSpacesEtc();
			if (ret != '\"')
				throw std::exception(); // TODO
			implContinueReadingString( s );
		}

		void readString(std::string* s, CharT ch)
		{
			CharT ret = skipSpacesEtc(ch);
			if (ret != '\"')
				throw std::exception(); // TODO
			implContinueReadingString( s );
		}

		void readKey(std::string* s, CharT ch)
		{
			s->clear();
			readString(s, ch);
			CharT ret = skipSpacesEtc();
			if (ret != ':')
				throw std::exception(); // TODO (expected ':')
		}

		void readKey(std::string* s)
		{
			s->clear();
			readString(s);
			CharT ret = skipSpacesEtc();
			if (ret != ':')
				throw std::exception(); // TODO (expected ':')
		}

		template <typename T>
		CharT implReadUnsignedInteger(T* num, CharT retch)
		{
			if (retch == '-')
				throw std::exception(); // TODO: (negative is unexpected)
			std::string s;
			while (riter.isData() && (retch >= '0' && retch <= '9'))
			{
				s += retch;
				if (!riter.isData())
					throw std::exception(); // TODO
				retch = riter.readChar();
			}
			char* end = const_cast<char*>(s.c_str());
			uint64_t retnum = strtoull(s.c_str(), &end, 10);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)retnum;              // TODO: add boundary checking
			return retch;
		}

		template <typename T>
		CharT readUnsignedInteger(T* num)
		{
			return implReadUnsignedInteger(num, skipSpacesEtc());
		}

		template <typename T>
		CharT readUnsignedInteger(T* num, CharT ch)
		{
			return implReadUnsignedInteger(num, skipSpacesEtc(ch));
		}

		template <typename T>
		CharT implReadSignedInteger(T* num, CharT retch)
		{
			std::string s;
			if (retch == '-')
			{
				s += retch;
				retch = riter.readChar();
			}
			while (riter.isData() && (retch >= '0' && retch <= '9'))
			{
				s += retch;
				if (!riter.isData())
					throw std::exception(); // TODO
				retch = riter.readChar();
			}
			char* end = const_cast<char*>(s.c_str());
			uint64_t retnum = strtoull(s.c_str(), &end, 10);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)retnum;              // TODO: add boundary checking
			return retch;
		}

		template <typename T>
		CharT readSignedInteger(T* num)
		{
			return implReadSignedInteger(num, skipSpacesEtc());
		}

		template <typename T>
		CharT readSignedInteger(T* num, CharT ch)
		{
			return implReadSignedInteger(num, skipSpacesEtc(ch));
		}

		template <typename T>
		CharT implReadReal(T* num, CharT retch)
		{
			std::string s;
			while (riter.isData() && !(retch == ' ' || retch == '\t' || retch == '\r' || retch == '\n' ||
									   retch == ',' || retch == ']' || retch == '}')) // expected terminators
			{
				s += retch;
				if (!riter.isData())
					throw std::exception(); // TODO
				retch = riter.readChar();
			}
			char* end = const_cast<char*>(s.c_str());
			double retnum = strtod(s.c_str(), &end);
			if (s.c_str() == end)
				throw std::exception(); // TODO: (NaN)
			*num = (T)retnum;              // TODO: add boundary checking
			return retch;
		}

		template <typename T>
		CharT readReal(T* num)
		{
			return implReadReal(num, skipSpacesEtc());
		}

		template <typename T>
		CharT readReal(T* num, CharT ch)
		{
			return implReadReal(num, skipSpacesEtc(ch));
		}

		template <typename T>
		CharT readEnumValue(T* t)
		{
			using PureT = typename std::remove_pointer<T>::type;
			using UnderT = typename std::underlying_type_t<PureT>;
			static_assert(sizeof(PureT) == sizeof(UnderT));
			if constexpr (std::is_signed<UnderT>::value)
				return readSignedInteger((UnderT*)(t));
			else
			{
				static_assert(std::is_unsigned<UnderT>::value);
				return readUnsignedInteger((UnderT*)(t));
			}
		}

		template <typename T>
		CharT readEnumValue(T* t, CharT ch)
		{
			using PureT = typename std::remove_pointer<T>::type;
			using UnderT = typename std::underlying_type_t<PureT>;
			static_assert(sizeof(PureT) == sizeof(UnderT));
			if constexpr (std::is_signed<UnderT>::value)
				return readSignedInteger((UnderT*)(t), ch);
			else
			{
				static_assert(std::is_unsigned<UnderT>::value);
				return readUnsignedInteger((UnderT*)(t), ch);
			}
		}

		template <typename T>
		CharT implReadBoolean(T* b, CharT retch)
		{
			std::string s;
			while (riter.isData() && !(retch == ' ' || retch == '\t' || retch == '\r' || retch == '\n' ||
									   retch == ',' || retch == ']' || retch == '}')) // expected terminators
			{
				s += retch;
				retch = riter.readChar();
			}
			if (!riter.isData())
				throw std::exception(); // TODO
			if (s == "true")
				*b = true;
			else if (s == "false")
				*b = false;
			else
				throw std::exception(); // TODO
			return retch;
		}

		template <typename T>
		CharT readBoolean(T* num)
		{
			return implReadBoolean(num, skipSpacesEtc());
		}

		template <typename T>
		CharT readBoolean(T* num, CharT ch)
		{
			return implReadBoolean(num, skipSpacesEtc(ch));
		}

		template<class StringT>
		void implReadObjectAsJson( StringT& buf )
		{
			buf.append(1, '{');
			size_t depth = 1;
			do
			{
				CharT retch = riter.readChar();
				buf.append( 1, retch );
				switch ( retch )
				{
					case '}': 
						--depth;
						if ( depth == 0 )
							return;
						break;
					case '{':
						++depth;
						break;
					case '\"':
						implContinueReadingString( &buf );
						buf.append(1, '\"');
						break;
					default:
						break;
				}
			}
			while ( 1 );
		}

		template<class StringT>
		void implReadArrayAsJson( StringT& buf )
		{
			buf.append(1, '[');
			size_t depth = 1;
			do
			{
				CharT ch = riter.readChar();
				buf.append( 1, ch );
				switch ( ch )
				{
					case ']': 
						--depth;
						if ( depth == 0 )
							return;
						break;
					case '[':
						++depth;
						break;
					case '\"':
						implContinueReadingString( &buf );
						buf.append(1, '\"');
						break;
					default:
						break;
				}
			}
			while ( 1 );
		}

		template<class StringT>
		CharT implReadValueAsJson( StringT& buf, CharT ch )
		{
			switch ( ch )
			{
				case '[' :
					implReadArrayAsJson( buf );
					ch = riter.readChar();
					break;
				case '{' :
					implReadObjectAsJson( buf );
					ch = riter.readChar();
					break;
				case '\"' :
				{
					buf.append(1, '\"');
					implContinueReadingString( &buf );
					buf.append(1, '\"');
					ch = riter.readChar();
					break;
				}
				default: // true, false, none, number
				{
					while (riter.isData() && !(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ||
											   ch == ',' || ch == ']' || ch == '}')) // expected terminators
					{
						buf.append( 1, ch );
						ch = riter.readChar();
					}
					if (!riter.isData())
						throw std::exception(); // TODO
				}
			}
			buf.append(2, ' '); // to avoid end-of-data while further parsing of numbers and identifiers
			return ch;
		}

		template<class StringT>
		CharT readValueAsJson( StringT& buf )
		{
			return implReadValueAsJson( buf, skipSpacesEtc() );
		}

		template<class StringT>
		CharT readValueAsJson( StringT& buf, CharT ch )
		{
			return implReadValueAsJson( buf, ch );
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

	template <typename RiterT, class DataT=void>
	class JsonParser2 : public JsonParserBase<RiterT, DataT>
	{
	  public:
		static constexpr Proto proto = JsonParserBase<RiterT, DataT>::proto;
		using InType = typename JsonParserBase<RiterT, DataT>::InType;
		using CharT = typename JsonParserBase<RiterT, DataT>::CharT;

		template <typename RiterT_, class DataT_>
		friend class JsonParser2;

	  private:
		template<class StringT>
		class StringAsRiter
		{
			const StringT& str;
			size_t pos = 0;

		public:
			using CharT = typename StringT::value_type;
			StringAsRiter( const StringT& str_ ) : str( str_ ) {}
			bool isData()
			{
				return pos < str.size();
			}
			/*typename StringT::value_type operator* () 
			{
				assert( pos < str.size() );
				return str[pos];
			}
			void operator ++ () 
			{
				++pos;
			}*/
			CharT readChar()
			{
				assert( pos < str.size() );
				return str[pos++];
			}
		};

	  private:
		struct In
		{
			InType type = InType::inUnknown;
			uint32_t count = 0;
			std::unordered_map<std::string, std::basic_string<CharT>> ooo;
		};
		std::vector<In> stack;
		std::vector<std::string> structNameStack; // Note: at present we silently assume that all CharT = char and std::string works as such TODO: if CharT != char convert names to utf8 first
		std::function<void(const std::vector<std::string>&, const std::string&)> unknownFieldsReporter;

		void implBeforeAnyValue()
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));

			if (stack.back().type == InType::inArray)
			{
				if (stack.back().count > 0)
				{
					if ( this->lastChar.isSaved() )
						this->skipComma(this->lastChar.consume());
					else
						this->skipComma();
				}
				++(stack.back().count);
			}
		}

		void implAfterAnyValue()
		{
		}

		std::string implProcessNamePart( CharT ch )
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			std::string s;
			if (stack.back().count > 0)
			{
				this->skipComma(ch);
				this->readKey(&s);
			}
			else
				this->readKey(&s, ch);
			return s;
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

		void implProcessNamePart(const std::string_view& name)
		{
			std::string s = implProcessNamePart();
			if (s != name)
				throw std::exception(); // TODO
			++stack.back().count;
		}

		void implProcessNamePart(const std::string_view& name, CharT ch)
		{
			std::string s = implProcessNamePart(ch);
			if (s != name)
				throw std::exception(); // TODO
			++stack.back().count;
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
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			if constexpr (std::is_same<INT, ValueTypeT>::value)
				ch = this->readSignedInteger(&val, ch);
			else if constexpr (std::is_same<UINT, ValueTypeT>::value)
				ch = this->readUnsignedInteger(&val, ch);
			else if constexpr (std::is_same<REAL, ValueTypeT>::value)
				ch = this->readReal(&val, ch);
			else if constexpr (std::is_same<STRING, ValueTypeT>::value)
			{
				this->implContinueReadingString(&val);
				ch = this->skipSpacesEtc();
			}
			else if constexpr (std::is_same<BOOLEAN, ValueTypeT>::value)
				ch = this->readBoolean(&val, ch);
			else if constexpr (std::is_same<ENUM, ValueTypeT>::value)
				ch = this->readEnumValue(&val, ch);
			else
			{
				static_assert(std::is_same<STRUCT, ValueTypeT>::value);
				this->lastChar.save(ch);
				readStructValue(val);
				ch = this->skipSpacesEtc();
			}
			this->lastChar.save(ch);
		}
		template <typename T, typename ItemProcT>
		void implProcessArray(std::vector<T>& v, ItemProcT&& proc)
		{
			beginArray();
			CharT ch = this->skipSpacesEtc();
			if ( ch !=']' )
			{
				size_t ctr = 0;
				do
				{
					this->lastChar.save(ch);
					T val;
					structNameStack.push_back( fmt::format( "[{}]", ctr ) );
					proc(val);
					v.push_back(std::move(val));
					structNameStack.pop_back();
					ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
					++ctr;
				}
				while (ch == ',');
			}
			endArray(ch);
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

		void setUnknownFieldsReporter( std::function<void(const std::vector<std::string>&, const std::string&)>& unknownFieldsReporter_ )
		{
			unknownFieldsReporter = unknownFieldsReporter_;
		}

		void resetUnknownFieldsReporter()
		{
			unknownFieldsReporter = std::function<void(const std::vector<std::string>&, const std::string&)>();
		}

		void beginStruct([[maybe_unused]] const std::string_view& name)
		{
			assert(stack.size() == 0 || stack.back().type == InType::inArray ||
				   (stack.back().type == InType::inNameVal && stack.back().count == 0));
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			this->skipDelimiter('{', ch);
			if (stack.size())
				++stack.back().count;
			stack.push_back({InType::inStruct, 0});
		}
		void endStruct()
		{
			assert(stack.size() > 0 && stack.back().type == InType::inStruct);
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			this->skipDelimiter('}', ch);
			if ( stack.back().ooo.size() && unknownFieldsReporter )
			{
				for ( auto& it: stack.back().ooo )
					unknownFieldsReporter( structNameStack, it.first );
			}
			stack.pop_back();
		}
		void beginArray(CharT ch)
		{
			assert(stack.size() > 0 && (stack.back().type == InType::inArray ||
										(stack.back().type == InType::inNameVal && stack.back().count == 0)));
			this->skipDelimiter('[', ch);
			++stack.back().count;
			stack.push_back({InType::inArray, 0});
		}
		void beginArray()
		{
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			beginArray( ch );
		}
		void endArray(CharT ch)
		{
			assert(stack.size() > 0 && stack.back().type == InType::inArray);
			this->skipDelimiter(']', ch);
			stack.pop_back();
			if (stack.back().type != InType::inNameVal)
			{
				assert(stack.size() > 0 && stack.back().type == InType::inArray);
				++(stack.back().count);
			}
		}
		void endArray()
		{
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			endArray( ch );
		}
		void beginNamedValue(const std::string_view& name)
		{
			CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
			implProcessNamePart(name, ch);
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
				if constexpr ( std::is_same<decltype( defaultValue ), EmptyVectorValueType>::value || 
					std::is_same<decltype( defaultValue ), decltype( emptyVectorValue )>::value ) // NOTE: the second component is due to f..ing GCC which sees types differently
					return;
				else if constexpr ( is_std_array<std::remove_cvref_t<decltype( defaultValue )>>::value )
				{
					val.resize( defaultValue.size() );
					for ( size_t i=0; i<val.size(); ++i )
						val[i] = defaultValue[i];
				}
				else
					val = defaultValue();
			}
			else
			{
				if constexpr (std::is_same<STRUCT, TypeHint>::value)
				{
					if constexpr ( std::is_same<decltype( defaultValue ), StructDefaultValueType>::value || 
						std::is_same<decltype( defaultValue ), decltype( structDefaultValue )>::value ) // NOTE: the second component is due to f..ing GCC which sees types differently
						val = ValueT( defaultValue );
					else
						val = defaultValue();
				}
				else 
				{
					if constexpr ( std::is_invocable<decltype( defaultValue )>::value )
						val = defaultValue();
					else
					{
						if constexpr ( std::is_base_of<StringLiteralHelperIndicator, decltype( defaultValue )>::value )
							val = (const char*)defaultValue;
						else if constexpr ( std::is_base_of<FloatingConstIndicator, decltype( defaultValue )>::value )
							val = defaultValue.val();
						else
							val = defaultValue;
					}
				}
			}
		}

		template <typename TypeHint, typename ValueT, typename ValueProc, bool hasDefault, auto DefaultValue>
		void implRWNameValue(const std::string_view& name, ValueT& val, ValueProc&& proc)
		{
			auto f = stack.back().ooo.find( std::string( name ) );
			if ( f != stack.back().ooo.end() )
			{
				StringAsRiter<std::basic_string<CharT>> sr( f->second );
				JsonParser2<StringAsRiter<std::basic_string<CharT>>, DataT> p( sr );
				p.unknownFieldsReporter = unknownFieldsReporter;
				p.structNameStack = structNameStack;
				p.structNameStack.push_back( std::string( name ) ); // "popping" is in dtor
				if constexpr ( !std::is_same<DataT, void>::value )
					p.userdata = std::move( this->userdata );
				p.stack.push_back({JsonParser2<StringAsRiter<std::basic_string<CharT>>, DataT>::InType::inNameVal, 0});
				if constexpr ( std::is_same<ValueProc, VoidPlaceholder>::value )
					p.template implRWValue<TypeHint, ValueT>(val);
				else
					p.template implRWValue<TypeHint, ValueT, ValueProc>(val, std::move( proc ) );
				p.endNamedValue();
				if constexpr ( !std::is_same<DataT, void>::value )
					this->userdata = std::move( p.userdata );
				stack.back().ooo.erase( f );
			}
			else
			{
				CharT ch = this->lastChar.isSaved() ? this->skipSpacesEtc(this->lastChar.consume()) : this->skipSpacesEtc();
				if ( ch == '}' )
				{
					if constexpr ( hasDefault )
					{
						this->lastChar.save(ch);
						implApplyDefault<TypeHint, ValueT, DefaultValue>(val);
					}
					else
						throw std::exception(); // not found; rwWithWireDefault() might be helpful
					return;
				}
				std::string s = implProcessNamePart(ch);
				while ( s != name )
				{
					std::basic_string<CharT> buf;
					ch = this->readValueAsJson( buf );
					++stack.back().count;
					stack.back().ooo.insert( std::make_pair( s, buf ) );
					ch = this->skipSpacesEtc(ch);
					if ( ch == '}' )
					{
						if constexpr ( hasDefault )
						{
							this->lastChar.save(ch);
							implApplyDefault<TypeHint, ValueT, DefaultValue>(val);
						}
						else
							throw std::exception(); // not found; rwWithWireDefault() might be helpful
						return;
					}
					s = implProcessNamePart(ch);
				}
				++stack.back().count;
				stack.push_back({InType::inNameVal, 0});
				structNameStack.push_back( std::string( name ) );
				if constexpr ( std::is_same<ValueProc, VoidPlaceholder>::value )
					implRWValue<TypeHint, ValueT>(val);
				else
					implRWValue<TypeHint, ValueT, ValueProc>(val, std::move( proc ) );
				structNameStack.pop_back();
				stack.pop_back();
			}
		}

		template <typename TypeHint, typename ValueT>
		void rw(const std::string_view& name, ValueT& val)
		{
			VoidPlaceholder dummyProc;
			implRWNameValue<TypeHint, ValueT, VoidPlaceholder, false, voidPlaceholderValue>(name, val, std::move( dummyProc ));
		}

		template <typename TypeHint, typename ValueT, auto defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val)
		{
			VoidPlaceholder dummyProc;
			implRWNameValue<TypeHint, ValueT, VoidPlaceholder, true, defaultValue>(name, val, std::move( dummyProc ));
		}

		template <typename TypeHint, typename ValueT, StringLiteralHelper defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val)
		{
			VoidPlaceholder dummyProc;
			implRWNameValue<TypeHint, ValueT, VoidPlaceholder, true, defaultValue>(name, val, std::move( dummyProc ));
		}

		template <typename TypeHint, typename ValueT, typename ItemProc>
		void rw(const std::string_view& name, ValueT& val, ItemProc&& proc)
		{
			implRWNameValue<TypeHint, ValueT, ItemProc, false, voidPlaceholderValue>(name, val, std::move( proc ));
		}

		template <typename TypeHint, typename ValueT, typename ItemProc, auto defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val, ItemProc&& proc)
		{
			implRWNameValue<TypeHint, ValueT, ItemProc, true, defaultValue>(name, val, std::move( proc ));
		}

		template <typename TypeHint, typename ValueT, typename ItemProc, StringLiteralHelper defaultValue>
		void rwWithWireDefault(const std::string_view& name, ValueT& val, ItemProc&& proc)
		{
			implRWNameValue<TypeHint, ValueT, ItemProc, true, defaultValue>(name, val, std::move( proc ));
		}
	};

} // namespace comparsers

#endif // COMPARSERS_H
