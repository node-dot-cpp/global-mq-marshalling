/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
* Copyright (c) 2018-2021, OLogN Technologies AG
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

#ifndef GLOBALMQ_MARSHALLING2_MARSHALLING2_H
#define GLOBALMQ_MARSHALLING2_MARSHALLING2_H

#include <functional>
#include <action_on.h>
#include <marshalling_impl.h>

namespace globalmq::marshalling2 {

// common base to be able to use RTTI
class ComposerBase
{
public:
	virtual ~ComposerBase() {}
};

/// mb: this is a reference interface for a composer. not really to be used 
template<class BufferT>
class IComposer2 : public ComposerBase
{
public:
	virtual ~IComposer2() {}

	using BufferType = BufferT;
	static constexpr int proto = -1;

	virtual void reset() = 0;
	virtual void appendRaw(typename BufferT::ReadIteratorT it, size_t count = SIZE_MAX) = 0;

	virtual void composeSignedInteger(int64_t val) = 0;
	virtual void composeUnsignedInteger(uint64_t val) = 0;
	virtual void composeReal(double val) = 0;
	virtual void composeString(const GMQ_COLL string& val) = 0;

	virtual void structBegin() = 0;
	virtual void structEnd() = 0;

	virtual void vectorBegin(uint64_t sz) = 0;
	virtual void vectorEnd() = 0;

	virtual void dictionaryBegin(uint64_t sz) = 0;
	virtual void dictionaryEnd() = 0;

	virtual void namedParamBegin(const char* name) = 0;
	virtual void leafeBegin() = 0;
	virtual void nextElement() = 0;

	virtual void changeBegin(const GMQ_COLL vector<uint64_t>& addr, uint64_t last) = 0;
	virtual void changeEnd() = 0;
	virtual void stateUpdateBegin() = 0;
	virtual void stateUpdateEnd() = 0;
	virtual void stateSyncBegin() = 0;
	virtual void stateSyncEnd() = 0;

	virtual void composeAction(uint64_t action) = 0;
};

template<class BufferT>
class JsonComposer2 : public ComposerBase
{
public:
	BufferT& buff; //public because of impl::json::composeXXX, remove

	JsonComposer2(BufferT& buff) : buff(buff) {}
	virtual ~JsonComposer2() {}

	using BufferType = BufferT;
	static constexpr int proto = -1;

	void reset() {}
	void appendRaw(typename BufferT::ReadIteratorT it, size_t count = SIZE_MAX) { buff.append(it, count); }

	void composeSignedInteger(int64_t val) { globalmq::marshalling::impl::json::composeSignedInteger(*this, val); }
	void composeUnsignedInteger(uint64_t val) { globalmq::marshalling::impl::json::composeUnsignedInteger(*this, val); }
	void composeReal(double val) { globalmq::marshalling::impl::json::composeReal(*this, val); }
	void composeString(const GMQ_COLL string& val) { globalmq::marshalling::impl::json::composeString(*this, val); }

	void structBegin() { buff.appendUint8( '{' ); }
	void structEnd() { buff.appendUint8( '}' ); }

	void vectorBegin(uint64_t sz) { buff.appendUint8( '[' ); }
	void vectorEnd() { buff.appendUint8( ']' ); }

	void dictionaryBegin(uint64_t sz) { buff.appendUint8( '{' ); }
	void dictionaryEnd() { buff.appendUint8( '}' ); }

	void namedParamBegin(const char* name) { globalmq::marshalling::impl::json::addNamePart(*this, name); }
	void leafeBegin() { namedParamBegin("value"); }
	void nextElement() { buff.appendUint8( ',' ); }

	void changeBegin(const GMQ_COLL vector<uint64_t>& addr, uint64_t last)
	{
		structBegin();
		composeAddressInPublishable2(*this, addr, last);
	}

	void changeEnd() { structEnd(); nextElement(); }

	void stateUpdateBegin()
	{
		structBegin();
		namedParamBegin("changes");
		buff.appendUint8( '[' );
	}
	void stateUpdateEnd()
	{
		buff.appendUint8( '{' );
		buff.appendUint8( '}' );
		buff.appendUint8( ']' );
		structEnd();
	}

	void stateSyncBegin() { structBegin(); namedParamBegin("hdr"); }
	void stateSyncEnd() { structEnd(); }

	void composeAction(uint64_t action)
	{
		namedParamBegin("action");
		composeUnsignedInteger(action);
	}
};

template<class BufferT>
class GmqComposer2 : public ComposerBase
{
public:
	BufferT& buff; //public because of impl::json::composeXXX, remove

	GmqComposer2( BufferT& buff ) : buff(buff) {}
	virtual ~GmqComposer2() {}

	using BufferType = BufferT;
	static constexpr int proto = -1;

	void reset() {}
	void appendRaw(typename BufferT::ReadIteratorT it, size_t count = SIZE_MAX) { buff.append(it, count); }

	void composeSignedInteger(int64_t val) { globalmq::marshalling::impl::composeSignedInteger(*this, val); }
	void composeUnsignedInteger(uint64_t val) { globalmq::marshalling::impl::composeUnsignedInteger(*this, val); }
	void composeReal(double val) { globalmq::marshalling::impl::composeReal(*this, val); }
	void composeString(const GMQ_COLL string& val) { globalmq::marshalling::impl::composeString(*this, val); }

	void structBegin() {}
	void structEnd() {}

	void vectorBegin(uint64_t sz) { composeUnsignedInteger(sz); }
	void vectorEnd() {}

	void dictionaryBegin(uint64_t sz) { composeUnsignedInteger(sz); }
	void dictionaryEnd() {}

	void namedParamBegin(const char* name) {}
	void leafeBegin() {}
	void nextElement() {}

	void changeBegin(const GMQ_COLL vector<uint64_t>& addr, uint64_t last) { composeAddressInPublishable2(*this, addr, last); }
	void changeEnd() {}

	void stateUpdateBegin() {}
	void stateUpdateEnd() { composeUnsignedInteger(0); }

	void stateSyncBegin() {}
	void stateSyncEnd() {}

	void composeAction(uint64_t action) { composeUnsignedInteger(action); }
};


inline
GMQ_COLL vector<uint64_t> makeAddress(const GMQ_COLL vector<uint64_t>& addr, uint64_t last)
{
	GMQ_COLL vector<uint64_t> result{addr};
	result.push_back(last);
	return result;
}

template<typename ComposerT>
void composeAddressInPublishable2( ComposerT& composer, const GMQ_COLL vector<uint64_t>& addr, uint64_t last )
{
	composer.namedParamBegin("addr");
	composer.vectorBegin(addr.size() + 1);

	for (uint64_t each : addr )
	{
		composer.composeUnsignedInteger(each);
		composer.nextElement();
	}

	composer.composeUnsignedInteger(last);
	// composer.nextElement();
	composer.vectorEnd();
}

//mb: copy and paste from marshalling.h to avoid #include
template <typename T, typename NameTag>
class NamedParameterWithType;

template <typename NameTag>
class NamedParameter
{
public:
	explicit NamedParameter() {}

	using Name = NameTag;

	struct TypeConverter
	{
		template<typename DataType>
		NamedParameterWithType<DataType, NameTag> operator=(DataType&& value) const
		{
			return NamedParameterWithType<DataType, NameTag>(std::forward<DataType>(value));
		}
		TypeConverter() = default;
		TypeConverter(TypeConverter const&) = delete;
		TypeConverter(TypeConverter&&) = delete;
		TypeConverter& operator=(TypeConverter const&) = delete;
		TypeConverter& operator=(TypeConverter&&) = delete;
	};
};

template <typename T, typename NameTag_>
class NamedParameterWithType : public NamedParameter<NameTag_>
{
private:
	T value_;

public:
	explicit NamedParameterWithType(T const& value) : value_(value) {}
	T& get() { return value_; }
	T const& get() const { return value_; }

	using NameBase = NamedParameter<NameTag_>;
	using Name = typename NamedParameter<NameTag_>::Name;
	using Type = T;
	using typename NamedParameter<NameTag_>::TypeConverter;

	template<class NameT, class DataT>
	struct FullType
	{
		using NameTag = NameT;
		using DataType = DataT;
	};

	using NameAndTypeID = FullType<NameTag_, T>;
	static constexpr NameAndTypeID nameAndTypeID = {};
};

template <class T>
struct unwrap_refwrapper
{
	using type = T;
};
 
template <class T>
struct unwrap_refwrapper<std::reference_wrapper<T>>
{
	using type = T&;
};
 
template <class T>
using special_decay_t = typename unwrap_refwrapper<typename std::decay<T>::type>::type;


template <typename NameTag_, typename Proc_>
class NamedParameterWithProcessor : public NamedParameter<NameTag_>
{
// private:
	// T value_;

public:
	// explicit NamedParameterWithProcessor(T const& value) : value_(value) {}
	// T& get() { return value_; }
	// T const& get() const { return value_; }

	using NameBase = NamedParameter<NameTag_>;
	using Name = typename NamedParameter<NameTag_>::Name;
	using Proc = Proc_;
	using typename NamedParameter<NameTag_>::TypeConverter;

	template<class NameT, class DataT>
	struct FullType
	{
		using NameTag = NameT;
		// using DataType = DataT;
	};

	using NameAndTypeID = FullType<NameTag_, Proc_>;
	static constexpr NameAndTypeID nameAndTypeID = {};
};



template<typename ComposerT, typename ProcessorCppType, typename ArgT>
void composeEntry2(ComposerT& composer, const char* name, ArgT& arg)
{
	ProcessorCppType::compose(composer, arg.get());
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue>
void composeParam2(ComposerT& composer, GMQ_COLL string name, const typename TypeToPick::NameAndTypeID expected)
{
	static_assert( !required, "required parameter" );
	//TODO
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue, typename Arg0, typename ... Args>
void composeParam2(ComposerT& composer, const char* name, const typename TypeToPick::NameAndTypeID expected, Arg0&& arg0, Args&& ... args)
{
	using Agr0Type = special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename special_decay_t<Arg0>::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		composeEntry2<ComposerT, typename TypeToPick::Proc, Arg0>(composer, name, arg0);
	}
	else
		composeParam2<ComposerT, TypeToPick, required, AssumedDefaultT, DefaultT, defaultValue>(composer, name, expected, args...);
}


// common base to be able to use RTTI
class ParserBase
{
public:
	virtual ~ParserBase() {}
};

/// mb: this is a reference interface for a parser. not really to be used 
template<class BufferT>
class IParser2 : public ParserBase
{
public:
	virtual ~IParser2() {}

	using BufferType = BufferT;
	using RiterT = typename BufferT::ReadIteratorT;
	static constexpr int proto = -1;

	virtual size_t getCurrentOffset() const = 0;
	virtual RiterT& getIterator() = 0;

	virtual int64_t parseSignedInteger() = 0;
	virtual uint64_t parseUnsignedInteger() = 0;
	virtual double parseReal() = 0;
	virtual GMQ_COLL string parseString() = 0;

	virtual void structBegin() = 0;
	virtual void structEnd() = 0;

	virtual uint64_t vectorBegin() = 0;
	virtual void vectorEnd() = 0;
	virtual bool isVectorEnd() = 0;

	virtual uint64_t dictionaryBegin() = 0;
	virtual void dictionaryEnd() = 0;
	virtual bool isDictionaryEnd() = 0;

	virtual void namedParamBegin(const char* name) = 0;
	virtual void leafeBegin() = 0;
	virtual void nextElement() = 0;

	virtual bool changeBegin(GMQ_COLL vector<uint64_t>& addr) = 0;
	virtual void changeEnd() = 0;
	virtual void stateUpdateBegin() = 0;
	virtual void stateUpdateEnd() = 0;
	virtual void stateSyncBegin() = 0;
	virtual void stateSyncEnd() = 0;

	virtual uint64_t parseAction() = 0;
};

template<class BufferT>
class JsonParser2 : public ParserBase
{
	globalmq::marshalling::JsonParser<BufferT> p;

public:

	JsonParser2(typename BufferT::ReadIteratorT& iter) :p(iter) {}
	virtual ~JsonParser2() {}

	using BufferType = BufferT;
	using RiterT = typename BufferT::ReadIteratorT;
	static constexpr int proto = -1;


	size_t getCurrentOffset() const { return p.getCurrentOffset(); }
	RiterT& getIterator() { return p.getIterator(); }

	int64_t parseSignedInteger() { int64_t v; p.readSignedIntegerFromJson(&v); return v; }
	uint64_t parseUnsignedInteger() { uint64_t v; p.readUnsignedIntegerFromJson(&v); return v; }
	double parseReal() { double v; p.readRealFromJson(&v); return v; }
	GMQ_COLL string parseString() { GMQ_COLL string v; p.readStringFromJson(&v); return v; }

	void structBegin() { p.skipDelimiter('{'); }
	void structEnd() { p.skipDelimiter('}'); }

	uint64_t vectorBegin() { p.skipDelimiter('['); return UINT64_MAX; }
	void vectorEnd() { p.skipDelimiter(']'); }
	bool isVectorEnd() { return p.isDelimiter(']'); }

	uint64_t dictionaryBegin() { p.skipDelimiter('{'); return UINT64_MAX; }
	void dictionaryEnd() { p.skipDelimiter('}'); }
	bool isDictionaryEnd() { return p.isDelimiter('}'); }

	void namedParamBegin(const char* name)
	{
		GMQ_COLL string v;
		p.readKey(&v);
		if(v != name)
			throw std::exception();
	}

	void leafeBegin() { namedParamBegin("value"); }
	void nextElement() { p.skipDelimiter(','); }

	bool changeBegin(GMQ_COLL vector<uint64_t>& addr)
	{
		addr.clear();

		structBegin();
		if(p.isDelimiter('}'))
		{
			p.skipDelimiter('}');
			return false;
		}

		namedParamBegin("addr");
		uint64_t sz = vectorBegin();

		for (uint64_t i = 0; !isVectorEnd(); ++i)
		{
			if(i != 0)
				nextElement();

			uint64_t v = parseUnsignedInteger();
			addr.push_back(v);
		}

		vectorEnd();
		return !addr.empty();
	}

	void changeEnd() { structEnd(); nextElement(); }

	void stateUpdateBegin()
	{
		structBegin();
		namedParamBegin("changes");
		p.skipDelimiter('[');
	}
	void stateUpdateEnd()
	{
		p.skipDelimiter(']');
		structEnd();
	}
	void stateSyncBegin() { structBegin(); namedParamBegin("hdr"); }
	void stateSyncEnd() { structEnd(); }

	uint64_t parseAction()
	{
		namedParamBegin("action");
		return parseUnsignedInteger();
	}
};


template<class BufferT>
class GmqParser2 : public ParserBase
{
	globalmq::marshalling::GmqParser<BufferT> p;

public:
	using RiterT = typename BufferT::ReadIteratorT;
	static constexpr int proto = -1;

	GmqParser2(typename BufferT::ReadIteratorT& iter) :p(iter) {}
	virtual ~GmqParser2() {}

	size_t getCurrentOffset() const { return p.getCurrentOffset(); }
	RiterT& getIterator() { return p.getIterator(); }

	int64_t parseSignedInteger() { int64_t v; p.parseSignedInteger(&v); return v; }
	uint64_t parseUnsignedInteger() { uint64_t v; p.parseUnsignedInteger(&v); return v; }
	double parseReal() { double v; p.parseReal(&v); return v; }
	GMQ_COLL string parseString() { GMQ_COLL string v; p.parseString(&v); return v; }

	void structBegin() { }
	void structEnd() { }

	uint64_t vectorBegin() { return parseUnsignedInteger(); }
	void vectorEnd() { }
	bool isVectorEnd() { return false; }

	uint64_t dictionaryBegin() { return parseUnsignedInteger(); }
	void dictionaryEnd() { }
	bool isDictionaryEnd() { return false; }

	void namedParamBegin(const char* name) { }
	void leafeBegin() { }
	void nextElement() { }

	bool changeBegin(GMQ_COLL vector<uint64_t>& addr)
	{
		addr.clear();
		uint64_t sz = vectorBegin();

		if(sz == 0)
			return false;

		for (uint64_t i = 0; i != sz; ++i)
		{
			uint64_t v = parseUnsignedInteger();
			addr.push_back(v);
		}

		return true;
	}

	void changeEnd() {}

	void stateUpdateBegin() {}
	void stateUpdateEnd() {}
	void stateSyncBegin() {}
	void stateSyncEnd() {}

	uint64_t parseAction() { return parseUnsignedInteger();	}
};

template<typename ParserT>
uint64_t parseMessageId(ParserT& parser)
{
	parser.namedParamBegin("msgid");
	uint64_t v = parser.parseUnsignedInteger();
	parser.nextElement();

	return v;
}




///////////////////////////////////////////
// Processors so primitives have similar
// syntax to structs
// This processors are written in terms of the
// default mapped C++ type
// custom mappings should emit its own processors 
///////////////////////////////////////////
class Int64Processor
{
public:
	using CppType = int64_t;

	template<typename T>
	static
	void checkType(const T& t)
	{
		//TODO improve
		static_assert(std::is_integral<T>::value);
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, int64_t arg)
	{
		composer.composeSignedInteger( arg );
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, uint64_t arg)
	{
		if(arg > static_cast<uint64_t>(INT64_MAX))
			throw std::exception();

		composer.composeSignedInteger( static_cast<int64_t>(arg) );
	}

	template<typename ParserT>
	static
	int64_t parse(ParserT& parser)
	{
		return parser.parseSignedInteger();
	}

	template<typename ParserT>
	static
	void parse(ParserT& parser, int64_t& arg)
	{
		arg = parse(parser);
	}

	template<typename ParserT>
	static
	void parseForStateSyncOrMessageInDepth(ParserT& parser, int64_t& arg)
	{
		parse(parser, arg);
	}

	template<typename ParserT>
	static
	void parse_state_sync(ParserT& parser, int64_t& arg)
	{
		parse(parser, arg);
	}

	static
	void copy( const int64_t& src, int64_t& dst ) { dst = src; }
	static
	int64_t copy( const int64_t& src ) { return src; }

	static
	bool isSame(int64_t l, int64_t r) { return l == r;}

	static
	GMQ_COLL vector<uint64_t> makeAddress(const GMQ_COLL vector<uint64_t>& baseAddress, int64_t address)
	{
		GMQ_COLL vector<uint64_t> result{baseAddress};
		uint64_t val = globalmq::marshalling::impl::IntegralVlq::zigzagEncode(address);
		result.push_back(val);
		return result;
	}

	static
	int64_t fromAddress(const GMQ_COLL vector<uint64_t>& address, size_t& index)
	{
		uint64_t val = address[index];
		int64_t result = globalmq::marshalling::impl::IntegralVlq::zigzagDecode(val);
		++index;
		return result;
	}
};

class UInt64Processor
{
public:
	using CppType = uint64_t;

	template<typename T>
	static
	void checkType(const T& t)
	{
		//TODO improve
		static_assert(std::is_integral<T>::value);
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, uint64_t arg)
	{
		composer.composeUnsignedInteger(arg);
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, int64_t arg)
	{
		if(arg < 0)
			throw std::exception();

		composer.composeUnsignedInteger(static_cast<uint64_t>(arg));
	}

	template<typename ParserT>
	static
	uint64_t parse(ParserT& parser)
	{
		return parser.parseUnsignedInteger();
	}

	template<typename ParserT>
	static
	void parse(ParserT& parser, uint64_t& arg)
	{
		arg = parse(parser);
	}

	template<typename ParserT>
	static
	void parseForStateSyncOrMessageInDepth(ParserT& parser, uint64_t& arg)
	{
		parse(parser, arg);
	}

	template<typename ParserT>
	static
	void parse_state_sync(ParserT& parser, uint64_t& arg)
	{
		parse(parser, arg);
	}

	static
	void copy( const uint64_t& src, uint64_t& dst ) { dst = src; }

	static
	uint64_t copy( const uint64_t& src ) { return src; }

	static
	bool isSame(uint64_t l, uint64_t r) { return l == r;}

	static
	GMQ_COLL vector<uint64_t> makeAddress(const GMQ_COLL vector<uint64_t>& baseAddress, uint64_t address)
	{
		GMQ_COLL vector<uint64_t> result{baseAddress};
		result.push_back(address);
		return result;
	}

	static
	uint64_t fromAddress(const GMQ_COLL vector<uint64_t>& address, size_t& index)
	{
		uint64_t result = address[index];
		++index;
		return result;
	}
};

class DoubleProcessor
{
public:
	using CppType = double;

	template<typename T>
	static
	void checkType(const T& t)
	{
		//TODO improve
		static_assert(std::is_arithmetic<T>::value);
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, double arg)
	{
		composer.composeReal(arg);
	}

	template<typename ParserT>
	static
	double parse(ParserT& parser)
	{
		return parser.parseReal();
	}

	template<typename ParserT>
	static
	void parse(ParserT& parser, double& arg)
	{
		arg = parse(parser);
	}

	template<typename ParserT>
	static
	void parseForStateSyncOrMessageInDepth(ParserT& parser, double& arg)
	{
		parse(parser, arg);
	}

	template<typename ParserT>
	static
	void parse_state_sync(ParserT& parser, double& arg)
	{
		parse(parser, arg);
	}

	static
	void copy( const double& src, double& dst ) { dst = src; }
	static
	double copy( const double& src ) { return src; }

	static
	bool isSame(double l, double r) { return l == r;}
};

class StringProcessor
{
public:
	using CppType = GMQ_COLL string;

	template<typename T>
	static
	void checkType(const T& t)
	{
		//TODO improve
		static_assert(std::is_same<T, GMQ_COLL string>::value || std::is_same<T, char*>::value);
	}

	template<typename ComposerT>
	static
	void compose(ComposerT& composer, const GMQ_COLL string& arg)
	{
		composer.composeString(arg);
	}

	template<typename ParserT>
	static
	GMQ_COLL string parse(ParserT& parser)
	{
		return parser.parseString();
	}

	template<typename ParserT>
	static
	void parse(ParserT& parser, GMQ_COLL string& arg)
	{
		arg = parse(parser);
	}

	template<typename ParserT>
	static
	void parseForStateSyncOrMessageInDepth(ParserT& parser, GMQ_COLL string& arg)
	{
		parse(parser, arg);
	}

	template<typename ParserT>
	static
	void parse_state_sync(ParserT& parser, GMQ_COLL string& arg)
	{
		parse(parser, arg);
	}

	static
	void copy( const GMQ_COLL string& src, GMQ_COLL string& dst ) { dst = src; }
	static
	GMQ_COLL string copy( const GMQ_COLL string& src ) { return src; }

	static
	bool isSame(GMQ_COLL string l, GMQ_COLL string r) { return l == r;}

	static
	GMQ_COLL vector<uint64_t> makeAddress(const GMQ_COLL vector<uint64_t>& baseAddress, GMQ_COLL string address)
	{
		if(address.find_first_of('\0') != GMQ_COLL string::npos)
			throw std::exception();

		GMQ_COLL vector<uint64_t> result{baseAddress};
		result.insert(result.end(), address.begin(), address.end());
		result.push_back(0);
		return result;
	}

	static
	GMQ_COLL string fromAddress(const GMQ_COLL vector<uint64_t>& address, size_t& index)
	{
		GMQ_COLL string result;
		while(address[index] != 0)
		{
			result.push_back(static_cast<char>(address[index]));
			++index;
		}

		++index; // skip nul terminator
		return result;
	}
};

template<class ElemTypeT, class RootT>
class VectorRefWrapper4Set
{
public:
	using VectorT = GMQ_COLL vector<typename ElemTypeT::CppType>;
	using CppType = VectorT;
private:
	void finalizeInsertOrUpdateAt( size_t idx ) { 
		ElemTypeT::compose( root.getComposer(), b[idx] );
	}

protected:

	VectorT& b;
	RootT& root;
	GMQ_COLL vector<uint64_t> address;

public:
	VectorRefWrapper4Set( VectorT& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ )
		: b( actual ), root( root_ ), address( std::move(address_) ) { }

	void remove( size_t idx ) { 
		GMQ_ASSERT( idx < b.size());
		b.erase( b.begin() + idx );
		root.getComposer().changeBegin(address, idx);
		root.getComposer().nextElement();
		root.getComposer().composeAction(globalmq::marshalling::ActionOnVector::remove_at);
		root.getComposer().changeEnd();
	}

	void insert_before( size_t idx, typename VectorT::value_type what ) {
		GMQ_ASSERT( idx <= b.size());
		b.insert( b.begin() + idx, what );
		root.getComposer().changeBegin(address, idx);
		root.getComposer().nextElement();
		root.getComposer().composeAction(globalmq::marshalling::ActionOnVector::insert_single_before);
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		finalizeInsertOrUpdateAt( idx );
		root.getComposer().changeEnd();
	}

	void set_at( typename VectorT::value_type what, size_t idx ) {
		GMQ_ASSERT( idx < b.size());
		b[idx] = what;
		root.getComposer().changeBegin(address, idx);
		root.getComposer().nextElement();
		root.getComposer().composeAction(globalmq::marshalling::ActionOnVector::update_at);
		root.getComposer().nextElement();
		root.getComposer().leafeBegin();
		finalizeInsertOrUpdateAt( idx );
		root.getComposer().changeEnd();
	}
};

template<class ElemTypeT, class RootT, class RefWrapper4SetT>
class VectorOfStructRefWrapper4Set : public VectorRefWrapper4Set<ElemTypeT, RootT>
{
public:
	using base_type = VectorRefWrapper4Set<ElemTypeT, RootT>;
	using VectorT = typename base_type::VectorT;
	using CppType = VectorT;

	VectorOfStructRefWrapper4Set( VectorT& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ ) : 
		base_type( actual, root_, std::move(address_) ) {}
	auto get4set_at( size_t idx ) { return RefWrapper4SetT(base_type::b[idx], base_type::root, makeAddress(base_type::address, idx)); }
};

template<class ElemTypeT>
class VectorOfSimpleTypeRefWrapper
{
public:
	using VectorT = GMQ_COLL vector<typename ElemTypeT::CppType>;
	using CppType = VectorT;
private:
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

template<class ElemTypeT, class RefWrapperT>
class VectorOfStructRefWrapper
{
public:
	using VectorT = GMQ_COLL vector<typename ElemTypeT::CppType>;
	using CppType = VectorT;
private:
	VectorT& b;
public:
	VectorOfStructRefWrapper( VectorT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	auto get_at( size_t idx ) { return RefWrapperT(b[idx]); }
};


template<class ElemProcT>
class PublishableVectorProcessor2
{
public:
	using ElementT = typename ElemProcT::CppType;
	using CppType = GMQ_COLL vector<ElementT>;

	template<class ParserT>
	static
	void parseSingleValue( ParserT& parser, ElementT& value ) { 
		ElemProcT::parse( parser, value );
	}

	template<class ParserT>
	static
	bool parseSingleValueAndCompare( ParserT& parser,  ElementT& value, const ElementT& oldValue ) { 
		ElemProcT::parse( parser, value );
		return !ElemProcT::isSame( value, oldValue );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what ) { 
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
		size_t collSz = what.size();
		composer.vectorBegin( collSz );
		for ( size_t i=0; i<collSz; ++i )
		{
			if(i != 0)
				composer.nextElement();

			ElemProcT::compose( composer, what[i] );
		}
		composer.vectorEnd();
	}

	template<class ParserT>
	static
	void parse( ParserT& parser, CppType& dest ) { 
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			ElementT what;
			ElemProcT::parse( parser, what );
			dest.push_back( what );

			if(!parser.isVectorEnd())
				parser.nextElement();
		}
		parser.vectorEnd();
	}

	template<class ParserT>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, CppType& dest ) { 
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			ElementT what;
			ElemProcT::parseForStateSyncOrMessageInDepth( parser, what );
			dest.push_back( what );

			if(!parser.isVectorEnd())
				parser.nextElement();

		}
		parser.vectorEnd();
	}

	template<class ParserT>
	static
	void parse_state_sync( ParserT& parser, CppType& dest ) { 
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			ElementT what;
			ElemProcT::parse_state_sync( parser, what );
			dest.push_back( what );

			if(!parser.isVectorEnd())
				parser.nextElement();

		}
		parser.vectorEnd();
	}

	static
	void copy( const CppType& src, CppType& dst )
	{
		dst = src;
	}
	static
	CppType copy( const CppType& src )
	{
		return src;
	}

	template<class UserT1, class UserT2>
	static
	bool isSame( const GMQ_COLL vector<UserT1>& v1, const GMQ_COLL vector<UserT2>& v2 )
	{
		if ( v1.size() != v2.size() )
			return false;
		for ( size_t i=0; i<v1.size(); ++i )
		{
			if ( !ElemProcT::isSame( v1[i], v2[i] ) ) 
				return false;
		}
		return true;
	}
};


template<class ElemProcT>
class ValueVectorProcessor2
{
public:
	using CppType = GMQ_COLL vector<typename ElemProcT::CppType>;
	using ElementType = typename ElemProcT::CppType;

	template<class ParserT>
	static
	void parseSingleValue( ParserT& parser, ElementType& value ) { 
		ElemProcT::parse( parser, value );
	}

	template<class ParserT>
	static
	ElementType parseSingleValue( ParserT& parser ) { 
		return ElemProcT::parse( parser );
	}

	template<class ParserT>
	static
	bool parseSingleValueAndCompare( ParserT& parser,  ElementType& value, const ElementType& oldValue ) { 
		ElemProcT::parse( parser, value );
		return !ElemProcT::isSame( value, oldValue );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what )
	{
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
		size_t collSz = what.size();
		composer.vectorBegin( collSz );
		for ( size_t i=0; i<collSz; ++i )
		{
			if(i != 0)
				composer.nextElement();

			ElemProcT::compose( composer, what[i] );
		}
		composer.vectorEnd();
	}

	template<class ParserT>
	static
	void parse( ParserT& parser, CppType& dest )
	{ 
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			ElementType what;
			ElemProcT::parse( parser, what );
			dest.push_back( what );

			if(!parser.isVectorEnd())
				parser.nextElement();
		}
		parser.vectorEnd();
	}

	template<class ParserT>
	static
	CppType parse( ParserT& parser )
	{
		CppType dest;
		parse(parser, dest);
		return dest;
	}

	template<class ParserT>
	static
	void parse_state_sync( ParserT& parser, CppType& dest )
	{
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			ElementType what;
			ElemProcT::parse_state_sync( parser, what );
			dest.push_back( what );

			if(!parser.isVectorEnd())
				parser.nextElement();

		}
		parser.vectorEnd();
	}

	static
	void copy( const CppType& src, CppType& dst )
	{
		dst = src;
	}
	static
	CppType copy( const CppType& src )
	{
		return src;
	}

	static
	bool isSame( const CppType& v1, const CppType& v2 )
	{
		return v1 == v2;
	}
};
template<class ElemProcT>
class PtrVectorProcessor2
{
public:
	using CppType = GMQ_COLL vector<GMQ_COLL unique_ptr<typename ElemProcT::CppType>>;
	using PtrType = GMQ_COLL unique_ptr<typename ElemProcT::CppType>;
	using ElementType = typename ElemProcT::CppType;

	template<class ParserT>
	static
	bool parse_element( ParserT& parser, ElementType& value ) { 
		return ElemProcT::parse( parser, value );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what ) { 

		size_t collSz = what.size();
		composer.vectorBegin( collSz );
		for ( size_t i=0; i<collSz; ++i )
		{
			if(i != 0)
				composer.nextElement();

			ElemProcT::compose( composer, *(what[i]) );
		}
		composer.vectorEnd();
	}

	template<class ParserT>
	static
	void parse_notify( ParserT& parser, CppType& dest, std::function<PtrType()> factory ) { 
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			PtrType elem = factory();
			ElemProcT::parse_notify( parser, *elem );
			dest.push_back( std::move(elem) );

			if(!parser.isVectorEnd())
				parser.nextElement();
		}
		parser.vectorEnd();
	}

	template<class ParserT>
	static
	void parse_state_sync( ParserT& parser, CppType& dest, std::function<PtrType()> factory ) {
		dest.clear();
		uint64_t collSz = parser.vectorBegin();
		if(collSz != UINT64_MAX)
			dest.reserve( collSz );

		for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
		{
			PtrType elem = factory();
			ElemProcT::parse_state_sync( parser, *elem );
			dest.push_back( std::move(elem) );

			if(!parser.isVectorEnd())
				parser.nextElement();

		}
		parser.vectorEnd();
	}

	static
	bool isSame( const CppType& v1, const CppType& v2 )
	{
		return v1 == v2;
	}

	template<class UserT>
	static
	bool isSame( const CppType& v1, const GMQ_COLL vector<UserT>& v2 )
	{
		if ( v1.size() != v2.size() )
			return false;
		for ( size_t i=0; i<v1.size(); ++i )
		{
			if ( !ElemProcT::isSame( *(v1[i]), v2[i] ) ) 
				return false;
		}
		return true;
	}
};

template<class KeyProcT, class ValueProcT, class RootT>
class DictionaryRefWrapper4Set
{
public:
	using key_type = typename KeyProcT::CppType;
	using value_type = typename ValueProcT::CppType;
	using DictionaryT = GMQ_COLL unordered_map<key_type, value_type>;
	using CppType = DictionaryT;
protected:
	DictionaryT& b;
	RootT& root;
	GMQ_COLL vector<size_t> address;

private:

public:
	DictionaryRefWrapper4Set( DictionaryT& actual, RootT& root_, GMQ_COLL vector<size_t>&& address_ ) :
		b( actual ), root( root_ ), address( std::move(address_) )
	{ }

	auto get_at( const typename KeyProcT::CppType& key )
	{
		return b.at(key);
	}

	size_t remove( const key_type& key ) { 
		size_t ret = b.erase( key );
		if ( ret != 0 )
		{
			root.getComposer().changeBegin(address, globalmq::marshalling::ActionOnDictionary::remove);
			root.getComposer().nextElement();
			root.getComposer().namedParamBegin("key");
			KeyProcT::compose( root.getComposer(), key );
			root.getComposer().changeEnd();
		}
		return ret;
	}

	bool insert( const key_type& key, const value_type& value ) { 
		auto insret = b.insert( std::make_pair( key, value ) );
		if ( insret.second )
		{
			root.getComposer().changeBegin(address, globalmq::marshalling::ActionOnDictionary::insert);
			root.getComposer().nextElement();
			root.getComposer().namedParamBegin("key");
			KeyProcT::compose( root.getComposer(), key );
			root.getComposer().nextElement();
			root.getComposer().namedParamBegin("value");
			ValueProcT::compose( root.getComposer(), value );
			root.getComposer().changeEnd();
		}
		return insret.second;
	}

	bool update_value( const key_type& key, const value_type& newValue ) {
		auto f = b.find( key );
		if ( f != b.end() )
		{
			f->second = newValue;

			root.getComposer().changeBegin(address, globalmq::marshalling::ActionOnDictionary::update_value);
			root.getComposer().nextElement();
			root.getComposer().namedParamBegin("key");
			KeyProcT::compose( root.getComposer(), key );
			root.getComposer().nextElement();
			root.getComposer().namedParamBegin("value");
			ValueProcT::compose( root.getComposer(), newValue );
			root.getComposer().changeEnd();

			return true;
		}
		return false;
	}
};

template<class KeyProcT, class ValueProcT, class RootT, class RefWrapper4SetT>
class DictionaryOfStructRefWrapper4Set : public DictionaryRefWrapper4Set<KeyProcT, ValueProcT, RootT>
{
public:
	using base_type = DictionaryRefWrapper4Set<KeyProcT, ValueProcT, RootT>;
	using DictionaryT = typename base_type::DictionaryT;

	DictionaryOfStructRefWrapper4Set( DictionaryT& actual, RootT& root_, GMQ_COLL vector<uint64_t>&& address_ ) : 
		base_type( actual, root_, std::move(address_) ) {}
	auto get4set_at( const typename KeyProcT::CppType& key )
	{
		return RefWrapper4SetT(base_type::b[key], base_type::root, KeyProcT::makeAddress(base_type::address, key));
	}
};

template<class KeyProcT, class ValueProcT>
class DictionaryOfSimpleTypeRefWrapper
{
public:
	using key_type = typename KeyProcT::CppType;
	using value_type = typename ValueProcT::CppType;
	using DictionaryT = GMQ_COLL unordered_map<key_type, value_type>;
	using CppType = DictionaryT;
private:
	DictionaryT& b;
public:
	DictionaryOfSimpleTypeRefWrapper( DictionaryT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	auto get_at( const typename KeyProcT::CppType& key )
	{
		return b.at(key);
	}
};


template<class KeyProcT, class ValueProcT, class RefWrapperT>
class DictionaryOfStructRefWrapper
{
public:
	using key_type = typename KeyProcT::CppType;
	using value_type = typename ValueProcT::CppType;
	using DictionaryT = GMQ_COLL unordered_map<key_type, value_type>;
	using CppType = DictionaryT;
private:
	DictionaryT& b;
public:
	DictionaryOfStructRefWrapper( DictionaryT& actual ) : b( actual ) {}
	size_t size() { return b.size(); }
	auto get_at( const typename KeyProcT::CppType& key )
	{
		return RefWrapperT(b.at(key));
	}
};

template<class KeyProcT, class ValueProcT>
class PublishableDictionaryProcessor2
{
public:
	using CppType = GMQ_COLL unordered_map<typename KeyProcT::CppType, typename ValueProcT::CppType>;

	template<class ParserT>
	static
	void parseKey( ParserT& parser, typename KeyProcT::CppType& key )
	{
		parser.namedParamBegin("key");
		KeyProcT::parse( parser, key );
	}

	template<class ParserT>
	static
	void parseValue( ParserT& parser, typename ValueProcT::CppType& value )
	{
		parser.namedParamBegin("value");
		ValueProcT::parse( parser, value );
	}

	template<class ParserT>
	static
	bool parseValueAndCompare( ParserT& parser, typename ValueProcT::CppType& value, const typename ValueProcT::CppType& oldValue ) { 
		parser.namedParamBegin("value");
		ValueProcT::parse( parser, value );
		return !ValueProcT::isSame( value, oldValue );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what ) {
		size_t collSz = what.size();
		composer.dictionaryBegin(collSz);
		size_t commaCtr = 0;
		for ( const auto& it: what )
		{
			if(commaCtr != 0)
				composer.nextElement();

			++commaCtr;

			composer.structBegin();

			// key
			KeyProcT::compose( composer, it.first );

			composer.nextElement();

			// value
			ValueProcT::compose( composer, it.second );

			composer.structEnd();
		}
		composer.dictionaryEnd();
	}


	template<class ParserT>
	static
	void parse( ParserT& parser, CppType& dest ) { 
		dest.clear();
		size_t collSz = parser.dictionaryBegin();

		for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
		{
			if(i != 0)
				parser.nextElement();

			parser.structBegin();

			// key
			typename KeyProcT::CppType key;
			KeyProcT::parse( parser, key );

			parser.nextElement();

			// value
			typename ValueProcT::CppType value;
			ValueProcT::parse( parser, value );

			parser.structEnd();

			dest.insert( std::make_pair( key, value ) );
		}
		parser.dictionaryEnd();
	}

	

	template<class ParserT>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, CppType& dest ) { 
		dest.clear();
		size_t collSz = parser.dictionaryBegin();

		for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
		{
			if(i != 0)
				parser.nextElement();

			parser.structBegin();

			// key
			typename KeyProcT::CppType key;
			KeyProcT::parseForStateSyncOrMessageInDepth( parser, key );

			parser.nextElement();

			// value
			typename ValueProcT::CppType value;
			ValueProcT::parseForStateSyncOrMessageInDepth( parser, value );

			parser.structEnd();

			dest.insert( std::make_pair( key, value ) );
		}
		parser.dictionaryEnd();
	}


	static
	void copy( const CppType& src, CppType& dst )
	{
		for ( const auto& it: src )
		{
			typename ValueProcT::CppType value;
			ValueProcT::copy( it.second, value );
			dst.insert( GMQ_COLL make_pair( it.first, value ) );
		}
	}
	CppType copy( const CppType& src )
	{
		CppType dst;
		copy(src, dst);
		return dst;
	}

	static
	bool isSame( const CppType& v1, const CppType& v2 )
	{
		if ( v1.size() != v2.size() )
			return false;
		auto it1 = v1.begin();
		auto it2 = v2.begin();
		while ( it1 != v1.end() )
		{
			if ( !KeyProcT::isSame(it1->first, it2->first) || !ValueProcT::isSame( it1->second, it2->second ) ) 
				return false;
			
			++it1;
			++it2;
		}

		return true;
	}

};

template<class KeyProcT, class ValueProcT>
class ValueDictionaryProcessor2
{
public:
	using KeyType = typename KeyProcT::CppType;
	using ValueType = typename ValueProcT::CppType;
	using CppType = GMQ_COLL unordered_map<KeyType, ValueType>;

	template<class ParserT>
	static
	KeyType parse_key( ParserT& parser )
	{
		parser.namedParamBegin("key");
		return KeyProcT::parse( parser );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what ) {
		size_t collSz = what.size();
		composer.dictionaryBegin(collSz);
		size_t commaCtr = 0;
		for ( const auto& it: what )
		{
			if(commaCtr != 0)
				composer.nextElement();

			++commaCtr;

			composer.structBegin();

			// key
			KeyProcT::compose( composer, it.first );

			composer.nextElement();

			// value
			ValueProcT::compose( composer, it.second );

			composer.structEnd();
		}
		composer.dictionaryEnd();
	}

	template<class ParserT>
	static
	CppType parse( ParserT& parser )
	{
		CppType dest;
		parse(parser, dest);
		return dest;
	}

	template<class ParserT>
	static
	void parse( ParserT& parser, CppType& dest ) { 
		dest.clear();
		size_t collSz = parser.dictionaryBegin();

		for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
		{
			if(i != 0)
				parser.nextElement();

			parser.structBegin();

			// key
			auto key = KeyProcT::parse( parser );

			parser.nextElement();

			// value
			auto value = ValueProcT::parse( parser );

			parser.structEnd();

			dest.insert( GMQ_COLL make_pair( std::move(key), std::move(value) ) );
		}
		parser.dictionaryEnd();
	}

	static
	void copy( const CppType& src, CppType& dst )
	{
		for ( const auto& it: src )
		{
			typename ValueProcT::CppType value;
			ValueProcT::copy( it.second, value );
			dst.insert( GMQ_COLL make_pair( it.first, value ) );
		}
	}
	CppType copy( const CppType& src )
	{
		CppType dst;
		copy(src, dst);
		return dst;
	}

	static
	bool isSame( const CppType& v1, const CppType& v2 )
	{
		return v1 == v2;
	}

};

template<class KeyProcT, class ValueProcT>
class PtrDictionaryProcessor2
{
public:
	using KeyType = typename KeyProcT::CppType;
	using ValueType = typename ValueProcT::CppType;
	using PtrValueType = GMQ_COLL unique_ptr<ValueType>;
	using CppType = GMQ_COLL unordered_map<KeyType, PtrValueType>;

	template<class ParserT>
	static
	KeyType parse_key( ParserT& parser )
	{
		parser.namedParamBegin("key");
		return KeyProcT::parse( parser );
	}

	template<class ComposerTT>
	static
	void compose( ComposerTT& composer, const CppType& what ) {
		size_t collSz = what.size();
		composer.dictionaryBegin(collSz);
		size_t commaCtr = 0;
		for ( const auto& it: what )
		{
			if(commaCtr != 0)
				composer.nextElement();

			++commaCtr;

			composer.structBegin();

			// key
			KeyProcT::compose( composer, it.first );

			composer.nextElement();

			// value
			ValueProcT::compose( composer, *(it.second) );

			composer.structEnd();
		}
		composer.dictionaryEnd();
	}


	template<class ParserT>
	static
	void parse_notify( ParserT& parser, CppType& dest, std::function<PtrValueType()> factory ) { 
		dest.clear();
		size_t collSz = parser.dictionaryBegin();

		for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
		{
			if(i != 0)
				parser.nextElement();

			parser.structBegin();

			// key
			auto key = KeyProcT::parse( parser );

			parser.nextElement();

			// value
			auto value = factory();
			ValueProcT::parse_notify( parser, *value );

			parser.structEnd();

			dest.insert( GMQ_COLL make_pair( std::move(key), std::move(value) ) );
		}
		parser.dictionaryEnd();
	}

	

	template<class ParserT>
	static
	void parse_state_sync( ParserT& parser, CppType& dest, std::function<PtrValueType()> factory ) { 
		dest.clear();
		size_t collSz = parser.dictionaryBegin();

		for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
		{
			if(i != 0)
				parser.nextElement();

			parser.structBegin();

			// key
			KeyType key = KeyProcT::parse( parser );

			parser.nextElement();

			// value
			PtrValueType value = factory();
			ValueProcT::parse_state_sync( parser, *value );

			parser.structEnd();

			dest.insert( GMQ_COLL make_pair( key, std::move(value) ) );
		}
		parser.dictionaryEnd();
	}


	static
	void copy( const CppType& src, CppType& dst )
	{
		for ( const auto& it: src )
		{
			typename ValueProcT::CppType value;
			ValueProcT::copy( it.second, value );
			dst.insert( GMQ_COLL make_pair( it.first, value ) );
		}
	}
	CppType copy( const CppType& src )
	{
		CppType dst;
		copy(src, dst);
		return dst;
	}

	static
	bool isSame( const CppType& v1, const CppType& v2 )
	{
		return v1 == v2;
	}

	template<class UserT>
	static
	bool isSame( const CppType& d1, const GMQ_COLL unordered_map<KeyType, UserT>& d2 )
	{
		if (d1.size() != d2.size())
			return false;
		
		auto it1 = d1.begin();

		while ( it1 != d1.end() )
		{
			auto it2 = d2.find(it1->first);

			if(it2 == d2.end())
				return false;

			if (!ValueProcT::isSame( *(it1->second), it2->second )) 
				return false;

			++it1;
		}

		return true;
	}
};


} // namespace globalmq::marshalling2

#endif // GLOBALMQ_MARSHALLING2_MARSHALLING2_H
