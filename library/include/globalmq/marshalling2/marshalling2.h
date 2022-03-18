/* -------------------------------------------------------------------------------
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

#include <marshalling_impl.h>

namespace globalmq::marshalling2 {

// common base to be able to use RTTI
class ComposerBase
{
public:
	virtual ~ComposerBase() {}
};

// mb: this is a reference interface for a composer. not really to be used virtually 
template<class BufferT>
class IComposer2 : public ComposerBase
{
public:
	using BufferType = BufferT;
	virtual BufferT&& getBuffer() = 0;
	virtual void setBuffer(BufferT&&) = 0;

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

	virtual void composeAction(uint64_t action) = 0;

	virtual ~IComposer2() {}
};

template<class BufferT>
class JsonComposer2 : public IComposer2<BufferT>
{
public:
	BufferT buff; //public because of impl::json::composeXXX, remove

	// JsonComposer2() : buff( buff_ ) {}

	virtual BufferT&& getBuffer() override { return std::move(buff); }
	virtual void setBuffer(BufferT&& b) override { buff = std::move( b ); }

	void composeSignedInteger(int64_t val) override { globalmq::marshalling::impl::json::composeSignedInteger(*this, val); }
	void composeUnsignedInteger(uint64_t val) override { globalmq::marshalling::impl::json::composeUnsignedInteger(*this, val); }
	void composeReal(double val) override { globalmq::marshalling::impl::json::composeReal(*this, val); }
	void composeString(const GMQ_COLL string& val) override { globalmq::marshalling::impl::json::composeString(*this, val); }

	void structBegin() override { buff.appendUint8( '{' ); }
	void structEnd() override { buff.appendUint8( '}' ); }

	void vectorBegin(uint64_t sz) override { buff.appendUint8( '[' ); }
	void vectorEnd() override { buff.appendUint8( ']' ); }

	void dictionaryBegin(uint64_t sz) override { buff.appendUint8( '{' ); }
	void dictionaryEnd() override { buff.appendUint8( '}' ); }

	void namedParamBegin(const char* name) override { globalmq::marshalling::impl::json::addNamePart(*this, name); }
	void leafeBegin() override { namedParamBegin("value"); }
	void nextElement() override { buff.appendUint8( ',' ); }

	void changeBegin(const GMQ_COLL vector<uint64_t>& addr, uint64_t last) override
	{
		structBegin();
		composeAddressInPublishable2(*this, addr, last);
	}

	void changeEnd() override { structEnd(); nextElement(); }

	void stateUpdateBegin() override
	{
		structBegin();
		namedParamBegin("changes");
		buff.appendUint8( '[' );
	}
	void stateUpdateEnd() override
	{
		buff.appendUint8( '{' );
		buff.appendUint8( '}' );
		buff.appendUint8( ']' );
		structEnd();
	}

	void composeAction(uint64_t action) override
	{
		namedParamBegin("action");
		composeUnsignedInteger(action);
	}

	virtual ~JsonComposer2() {}
};

template<class BufferT>
class GmqComposer2 : public ComposerBase
{
public:
	BufferT buff; //public because of impl::json::composeXXX, remove

	// GmqComposer2( BufferT& buff_ ) : buff( buff_ ) {}

	using BufferType = BufferT;
	BufferT&& getBuffer() { return std::move(buff); }
	void setBuffer(BufferT&& b) { buff = std::move( b ); }

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

	void composeAction(uint64_t action) { composeUnsignedInteger(action); }
};


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

template <typename NameTag_, typename Proc_>
class NamedParameterWithProcessor : public globalmq::marshalling::NamedParameter<NameTag_>
{
// private:
	// T value_;

public:
	// explicit NamedParameterWithProcessor(T const& value) : value_(value) {}
	// T& get() { return value_; }
	// T const& get() const { return value_; }

	using NameBase = globalmq::marshalling::NamedParameter<NameTag_>;
	using Name = typename globalmq::marshalling::NamedParameter<NameTag_>::Name;
	using Proc = Proc_;
	using typename globalmq::marshalling::NamedParameter<NameTag_>::TypeConverter;

	template<class NameT, class DataT>
	struct FullType
	{
		using NameTag = NameT;
		// using DataType = DataT;
	};

	using NameAndTypeID = FullType<NameTag_, Proc_>;
	static constexpr NameAndTypeID nameAndTypeID = {};
};



template<typename ComposerT, typename ProcessorType, typename ArgT>
void composeEntry2(ComposerT& composer, const char* name, ArgT& arg)
{
	ProcessorType::compose(composer, arg.get());
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
	using Agr0Type = globalmq::marshalling::special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename globalmq::marshalling::special_decay_t<Arg0>::Name, typename TypeToPick::Name>::value ) // same parameter name
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

// mb: this is a reference interface for a composer. not really to be used virtually 
class IParser2 : public ParserBase
{
public:
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
	virtual uint64_t parseAction() = 0;

	virtual ~IParser2() {}
};

// mb: this is a reference interface for a composer. not really to be used virtually
template<class BufferT>
class JsonParser2 : public IParser2
{
	globalmq::marshalling::JsonParser<BufferT> p;

public:
	JsonParser2(typename BufferT::ReadIteratorT iter) :p(iter) {}

	int64_t parseSignedInteger() override { int64_t v; p.readSignedIntegerFromJson(&v); return v; }
	uint64_t parseUnsignedInteger() override { uint64_t v; p.readUnsignedIntegerFromJson(&v); return v; }
	double parseReal() override { double v; p.readRealFromJson(&v); return v; }
	GMQ_COLL string parseString() override { GMQ_COLL string v; p.readStringFromJson(&v); return v; }

	void structBegin() override { p.skipDelimiter('{'); }
	void structEnd() override { p.skipDelimiter('}'); }

	uint64_t vectorBegin() override { p.skipDelimiter('['); return UINT64_MAX; }
	void vectorEnd() override { p.skipDelimiter(']'); }
	bool isVectorEnd() override { return p.isDelimiter(']'); }

	uint64_t dictionaryBegin() override { p.skipDelimiter('{'); return UINT64_MAX; }
	void dictionaryEnd() override { p.skipDelimiter('}'); }
	bool isDictionaryEnd() override { return p.isDelimiter('}'); }

	void namedParamBegin(const char* name) override
	{
		GMQ_COLL string v;
		p.readKey(&v);
		if(v != name)
			throw std::exception();
	}

	void leafeBegin() override { namedParamBegin("value"); }
	void nextElement() override { p.skipDelimiter(','); }

	void stateUpdateBegin() override
	{
		structBegin();
		namedParamBegin("changes");
		p.skipDelimiter('[');
	}
	void stateUpdateEnd() override
	{
		p.skipDelimiter(']');
		structEnd();
	}
	bool changeBegin(GMQ_COLL vector<uint64_t>& addr) override
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

	void changeEnd() override { structEnd(); nextElement(); }
	
	uint64_t parseAction() override
	{
		namedParamBegin("action");
		return parseUnsignedInteger();
	}


	virtual ~JsonParser2() {}
};


template<class BufferT>
class GmqParser2 : public ParserBase
{
	globalmq::marshalling::GmqParser<BufferT> p;

public:
	GmqParser2(typename BufferT::ReadIteratorT iter) :p(iter) {}

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
	void parse(ParserT& parser, int64_t& arg)
	{
		arg = parser.parseSignedInteger();
	}

	template<typename ParserT>
	static
	int64_t parse(ParserT& parser)
	{
		return parser.parseSignedInteger();
	}

	static
	bool isSame(int64_t l, int64_t r) { return l == r;}
};

class UInt64Processor
{
public:
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
	void parse(ParserT& parser, uint64_t& arg)
	{
		arg = parser.parseUnsignedInteger();
	}

	template<typename ParserT, typename ArgT>
	static
	uint64_t parse(ParserT& parser)
	{
		return parser.parseUnsignedInteger();
	}

	static
	bool isSame(uint64_t l, uint64_t r) { return l == r;}
};

class DoubleProcessor
{
public:
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
	void parse(ParserT& parser, double& arg)
	{
		arg = parser.parseReal();
	}

	template<typename ParserT, typename ArgT>
	static
	double parse(ParserT& parser)
	{
		return parser.parseReal();
	}

	static
	bool isSame(double l, double r) { return l == r;}
};

class StringProcessor
{
public:
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
	void parse(ParserT& parser, GMQ_COLL string& arg)
	{
		arg = parser.parseString();
	}

	template<typename ParserT, typename ArgT>
	static
	GMQ_COLL string parse(ParserT& parser)
	{
		return parser.parseString();
	}

	static
	bool isSame(GMQ_COLL string l, GMQ_COLL string r) { return l == r;}
};


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
		if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
			Int64Processor::compose( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
			UInt64Processor::compose( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::RealType>::value )
			DoubleProcessor::compose( root.getComposer(), b[idx] );
		else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::StringType>::value )
			StringProcessor::compose( root.getComposer(), b[idx] );
		else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ElemTypeT>::value )
		{
			// impl::publishableComposeLeafeStructBegin( root.getComposer() );
			ElemTypeT::compose( root.getComposer(), b[idx] );
			// impl::publishableComposeLeafeStructEnd( root.getComposer() );
		}
		else
			static_assert( std::is_same<ElemTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
	}

protected:
	VectorT& b;
	RootT& root;
	GMQ_COLL vector<uint64_t> address;

public:
	VectorRefWrapper4Set( VectorT& actual, RootT& root_, const GMQ_COLL vector<uint64_t> address_, uint64_t idx ) : b( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}

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
		finalizeInsertOrUpdateAt( idx );
		root.getComposer().changeEnd();
	}
};

template<class VectorT, class ElemTypeT, class RootT, class RefWrapper4SetT>
class VectorOfStructRefWrapper4Set : public VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>
{
public:
	VectorOfStructRefWrapper4Set( VectorT& actual, RootT& root_, const GMQ_COLL vector<uint64_t> address_, uint64_t idx ) : 
		VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>( actual, root_, address_, idx ) {}
	auto get4set_at( size_t idx ) { return RefWrapper4SetT(VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::b[idx], VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::root, VectorRefWrapper4Set<VectorT, ElemTypeT, RootT>::address, idx); }
};

class PublishableVectorProcessor
{
public:
	template<class ParserT, class VectorT, class ProcType>
	static
	void parseSingleValue( ParserT& parser, typename VectorT::value_type& value ) { 
		if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::SignedIntegralType>::value )
			Int64Processor::parse( parser, value );
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::UnsignedIntegralType>::value )
			UInt64Processor::parse( parser, value );
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::RealType>::value )
			DoubleProcessor::parse( parser, value );
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::StringType>::value )
			StringProcessor::parse( parser, value );
		else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ProcType>::value )
		{
			// parser.structBegin();
			ProcType::parse( parser, value );
			// parser.structEnd();
		}
		else
			static_assert( std::is_same<ProcType, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
	}

	template<class ParserT, class VectorT, class ProcType>
	static
	bool parseSingleValueAndCompare( ParserT& parser, typename VectorT::value_type& value, const typename VectorT::value_type& oldValue ) { 
		if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ProcType>::value )
		{
			// parser.structBegin();
			ProcType::parse( parser, value );
			// parser.structEnd();
			return !ProcType::isSame( value, oldValue );
		}
		else 
		{
			if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::SignedIntegralType>::value )
				Int64Processor::parse( parser, value );
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::UnsignedIntegralType>::value )
				UInt64Processor::parse( parser, value );
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::RealType>::value )
				DoubleProcessor::parse( parser, value );
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::StringType>::value )
				StringProcessor::parse( parser, value );
			else
				static_assert( std::is_same<ProcType, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
			return value != oldValue;
		}
	}

	template<class ElemTypeT, class ComposerTT, class VectorT>
	static
	void compose( ComposerTT& composer, const VectorT& what ) { 
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
		size_t collSz = what.size();
		// if constexpr ( ComposerT::proto == Proto::GMQ )
		// {
		// 	impl::composeUnsignedInteger( composer, collSz );
		// 	for ( size_t i=0; i<collSz; ++i )
		// 	{
		// 		if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
		// 			impl::composeSignedInteger( composer, what[i] );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
		// 			impl::composeUnsignedInteger( composer, what[i] );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
		// 			impl::composeReal( composer, what[i] );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
		// 			impl::composeString( composer, what[i] );
		// 		else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
		// 		{
		// 			impl::composeStructBegin( composer );
		// 			ElemTypeT::compose( composer, what[i] );
		// 			impl::composeStructEnd( composer );
		// 		}
		// 		else
		// 			static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
		// 	}
		// }
		// else
		// {
			// static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			composer.vectorBegin( collSz );
			for ( size_t i=0; i<collSz; ++i )
			{

				if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					composer.composeSignedInteger( what[i] );
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					composer.composeUnsignedInteger( what[i] );
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::RealType>::value )
					composer.composeReal( what[i] );
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::StringType>::value )
					composer.composeString( what[i] );
				else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ElemTypeT>::value )
				{
					// composer.structBegin();
					ElemTypeT::compose( composer, what[i] );
					// composer.structEnd( composer );
				}
				else
					static_assert( std::is_same<ElemTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
				// if ( i + 1 < collSz ) 
				// 	composer.buff.append( ", ", 2 );
				composer.nextElement();
			}
			composer.vectorEnd();
		// }
	}

	// template<class ComposerT, class VectorT, class ElemTypeT, typename NameT>
	// static
	// void compose( ComposerT& composer, const VectorT& what, NameT name, bool addListSeparator ) { 
	// 	// if constexpr ( ComposerT::proto == Proto::GMQ )
	// 	// 	compose<ComposerT, VectorT, ElemTypeT>( composer, what );
	// 	// else
	// 	// {
	// 	// 	static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
	// 		composer.paramNameBegin( name );
	// 		// impl::json::addNamePart( composer, name );
	// 		compose<ComposerT, VectorT, ElemTypeT>( composer, what );
	// 		if ( addListSeparator )
	// 			composer.nextElement();
	// 	// }
	// }

	template<class ParserT, class VectorT, class ElemTypeT, bool suppressNotifications = false>
	static
	void parse( ParserT& parser, VectorT& dest ) { 
		dest.clear();
		// if constexpr ( ParserT::proto == Proto::GMQ )
		// {
		// 	size_t collSz;
		// 	parser.parseUnsignedInteger( &collSz );
		// 	dest.reserve( collSz );
		// 	for ( size_t i=0; i<collSz; ++i )
		// 	{
		// 		typename VectorT::value_type what;
		// 		if constexpr ( std::is_same<ElemTypeT, impl::SignedIntegralType>::value )
		// 			parser.parseSignedInteger( &what );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::UnsignedIntegralType>::value )
		// 			parser.parseUnsignedInteger( &what );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::RealType>::value )
		// 			parser.parseReal( &what );
		// 		else if constexpr ( std::is_same<ElemTypeT, impl::StringType>::value )
		// 			parser.parseString( &what );
		// 		else if constexpr ( std::is_base_of<impl::StructType, ElemTypeT>::value )
		// 		{
		// 			impl::parseStructBegin( parser );
		// 			if constexpr( suppressNotifications )
		// 				ElemTypeT::parseForStateSyncOrMessageInDepth( parser, what );
		// 			else
		// 				ElemTypeT::parse( parser, what );
		// 			impl::parseStructEnd( parser );
		// 		}
		// 		else
		// 			static_assert( std::is_same<ElemTypeT, AllowedDataType>::value, "unsupported type" );
		// 		dest.push_back( what );
		// 	}
		// }
		// else
		// {
		// 	static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );

			// parser.skipDelimiter( '[' );
			uint64_t collSz = parser.vectorBegin();
			if(collSz != UINT64_MAX)
				dest.reserve( collSz );

			// if ( parser.isVectorEnd( ']' ) )
			// {
			// 	parser.skipDelimiter( ']' );
			// 	if ( parser.isDelimiter( ',' ) )
			// 		parser.skipDelimiter( ',' );
			// 	return;
			// }
			for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
			{
				typename VectorT::value_type what;
				if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					what = parser.parseSignedInteger();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					what = parser.parseUnsignedInteger();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::RealType>::value )
					what = parser.parseReal();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::StringType>::value )
					what = parser.parseString();
				else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ElemTypeT>::value )
				{
					// parser.structBegin();
					if constexpr( suppressNotifications )
						ElemTypeT::parseForStateSyncOrMessageInDepth( parser, what );
					else
						ElemTypeT::parse( parser, what );
					// parser.structEnd();
				}
				else
					static_assert( std::is_same<ElemTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
				dest.push_back( what );

				if(!parser.isVectorEnd())
					parser.nextElement();
				// if ( parser.isDelimiter( ',' ) )
				// {
				// 	parser.skipDelimiter( ',' );
				// 	continue;
				// }
				// if ( parser.isDelimiter( ']' ) )
				// {
				// 	parser.skipDelimiter( ']' );
				// 	break;
				// }
			}
			parser.vectorEnd();
			// if ( parser.isDelimiter( ',' ) )
			// 	parser.skipDelimiter( ',' );
		// }
	}

	template<class ElemTypeT, class ParserT, class VectorT>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, VectorT& dest ) { 
		dest.clear();
			uint64_t collSz = parser.vectorBegin();
			if(collSz != UINT64_MAX)
				dest.reserve( collSz );

			for( size_t i = 0; i < collSz && !parser.isVectorEnd(); ++i )
			{
				typename VectorT::value_type what;
				if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					what = parser.parseSignedInteger();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					what = parser.parseUnsignedInteger();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::RealType>::value )
					what = parser.parseReal();
				else if constexpr ( std::is_same<ElemTypeT, globalmq::marshalling::impl::StringType>::value )
					what = parser.parseString();
				else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ElemTypeT>::value )
				{
					ElemTypeT::parseForStateSyncOrMessageInDepth( parser, what );
				}
				else
					static_assert( std::is_same<ElemTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
				dest.push_back( what );

				if(!parser.isVectorEnd())
					parser.nextElement();
				// if ( parser.isDelimiter( ',' ) )
				// {
				// 	parser.skipDelimiter( ',' );
				// 	continue;
				// }
				// if ( parser.isDelimiter( ']' ) )
				// {
				// 	parser.skipDelimiter( ']' );
				// 	break;
				// }
			}
			parser.vectorEnd();
			// if ( parser.isDelimiter( ',' ) )
			// 	parser.skipDelimiter( ',' );
		// }
	}

};

template<class ElemProc>
class MessageVectorProcessor
{
public:
	template<class ComposerTT, class VectorT>
	static
	void compose( ComposerTT& composer, const VectorT& what ) { 

		size_t collSz = what.size();
		composer.vectorBegin( collSz );
		for ( size_t i=0; i<collSz; ++i )
		{
			if(i != 0)
				composer.nextElement();

			ElemProc::compose( composer, what[i] );
		}
		composer.vectorEnd();
	}
};


class PublishableDictionaryProcessor
{
public:
	template<class ParserT, class DictionaryT, class ProcType>
	static
	void parseKey( ParserT& parser, typename DictionaryT::key_type& key ) {
		parser.namedParamBegin("key");
		if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::SignedIntegralType>::value )
			key = parser.parseSignedInteger();
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::UnsignedIntegralType>::value )
			key = parser.parseUnsignedInteger();
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::StringType>::value )
			key = parser.parseString();
		else
			static_assert( std::is_same<ProcType, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
	}

	template<class ParserT, class DictionaryT, class ProcType>
	static
	void parseValue( ParserT& parser, typename DictionaryT::mapped_type& value ) { 
		parser.namedParamBegin("value");
		if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::SignedIntegralType>::value )
			value = parser.parseSignedInteger();
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::UnsignedIntegralType>::value )
			value = parser.parseUnsignedInteger();
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::RealType>::value )
			value = parser.parseReal();
		else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::StringType>::value )
			value = parser.parseString();
		else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ProcType>::value )
		{
			// parser.structBegin();
			ProcType::parse( parser, value );
			// parser.structEnd( parser );
		}
		else
			static_assert( std::is_same<ProcType, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
	}

	template<class ParserT, class DictionaryT, class ProcType>
	static
	bool parseValueAndCompare( ParserT& parser, typename DictionaryT::mapped_type& value, const typename DictionaryT::mapped_type& oldValue ) { 
		parser.namedParamBegin("value");
		if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ProcType>::value )
		{
			// parser.structBegin();
			ProcType::parse( parser, value );
			// parser.structEnd();
			return !ProcType::isSame( value, oldValue );
		}
		else 
		{
			if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::SignedIntegralType>::value )
				value = parser.parseSignedInteger();
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::UnsignedIntegralType>::value )
				value = parser.parseUnsignedInteger();
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::RealType>::value )
				value = parser.parseReal();
			else if constexpr ( std::is_same<ProcType, globalmq::marshalling::impl::StringType>::value )
				value = parser.parseString();
			else
				static_assert( std::is_same<ProcType, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );
			return value != oldValue;
		}
	}

	template<class KeyTypeT, class ValueTypeT, class ComposerTT, class DictionaryT>
	static
	void compose( ComposerTT& composer, const DictionaryT& what ) { 
		using ComposerT = typename std::remove_reference<ComposerTT>::type;
		using key_type = typename DictionaryT::key_type;
		using value_type = typename DictionaryT::mapped_type;
		size_t collSz = what.size();
		// if constexpr ( ComposerT::proto == Proto::GMQ )
		// {
		// 	impl::composeUnsignedInteger( composer, collSz );
		// 	for ( const auto& it: what )
		// 	{
		// 		// key
		// 		if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
		// 			impl::composeSignedInteger( composer, it.first );
		// 		else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
		// 			impl::composeUnsignedInteger( composer, it.first );
		// 		else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
		// 			impl::composeString( composer, it.first );
		// 		else
		// 			static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

		// 		// value
		// 		if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
		// 			impl::composeSignedInteger( composer, it.second );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
		// 			impl::composeUnsignedInteger( composer, it.second );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
		// 			impl::composeReal( composer, it.second );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
		// 			impl::composeString( composer, it.second );
		// 		else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
		// 		{
		// 			impl::composeStructBegin( composer );
		// 			ValueTypeT::compose( composer, it.second );
		// 			impl::composeStructEnd( composer );
		// 		}
		// 		else
		// 			static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );
		// 	}
		// }
		// else
		// {
		// 	static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			composer.dictionaryBegin(collSz);
			size_t commaCtr = 0;
			for ( const auto& it: what )
			{
				if(commaCtr != 0)
					composer.nextElement();

				++commaCtr;

				composer.structBegin();

				// key
				if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					composer.composeSignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					composer.composeUnsignedInteger( composer, it.first );
				else if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::StringType>::value )
					composer.composeString( composer, it.first );
				else
					static_assert( std::is_same<KeyTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );

				composer.nextElement();

				// value
				if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					composer.composeSignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					composer.composeUnsignedInteger( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::RealType>::value )
					composer.composeReal( composer, it.second );
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::StringType>::value )
					composer.composeString( composer, it.second );
				else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ValueTypeT>::value )
				{
					// composer.structBegin( composer );
					ValueTypeT::compose( composer, it.second );
					// composer.structEnd( composer );
				}
				else
					static_assert( std::is_same<ValueTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );

				composer.structEnd();
				// if ( commaCtr + 1 < collSz )
				// {
				// 	composer.buff.append( ",", 1 );
				// 	++commaCtr;
				// }
			}
			// assert( collSz == 0 || commaCtr + 1 == collSz );
			composer.dictionaryEnd();
		// }
	}

	// template<class ComposerT, class DictionaryT, class KeyTypeT, class ValueTypeT, typename NameT>
	// static
	// void compose( ComposerT& composer, const DictionaryT& what, NameT name, bool addListSeparator ) { 

	// 	// if constexpr ( ComposerT::proto == Proto::GMQ )
	// 	// 	compose<ComposerT, DictionaryT, KeyTypeT, ValueTypeT>( composer, what );
	// 	// else
	// 	// {
	// 	// 	static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
	// 	// 	impl::json::addNamePart( composer, name );
	// 		composer.namedParamBegin(name);
	// 		compose<ComposerT, DictionaryT, KeyTypeT, ValueTypeT>( composer, what );
	// 		if ( addListSeparator )
	// 			composer.nextElement();
	// 	// }
	// }

	template<class ParserT, class DictionaryT, class KeyTypeT, class ValueTypeT, bool suppressNotifications = false>
	static
	void parse( ParserT& parser, DictionaryT& dest ) { 
		using key_type = typename DictionaryT::key_type;
		using value_type = typename DictionaryT::mapped_type;
		dest.clear();
		// if constexpr ( ParserT::proto == Proto::GMQ )
		// {
		// 	size_t collSz;
		// 	parser.parseUnsignedInteger( &collSz );
		// 	for ( size_t i=0; i<collSz; ++i )
		// 	{
		// 		key_type key;
		// 		value_type value;

		// 		// key
		// 		if constexpr ( std::is_same<KeyTypeT, impl::SignedIntegralType>::value )
		// 			parser.parseSignedInteger( &key );
		// 		else if constexpr ( std::is_same<KeyTypeT, impl::UnsignedIntegralType>::value )
		// 			parser.parseUnsignedInteger( &key );
		// 		else if constexpr ( std::is_same<KeyTypeT, impl::StringType>::value )
		// 			parser.parseString( &key );
		// 		else
		// 			static_assert( std::is_same<KeyTypeT, AllowedDataType>::value, "unsupported type" );

		// 		// value
		// 		if constexpr ( std::is_same<ValueTypeT, impl::SignedIntegralType>::value )
		// 			parser.parseSignedInteger( &value );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::UnsignedIntegralType>::value )
		// 			parser.parseUnsignedInteger( &value );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::RealType>::value )
		// 			parser.parseReal( &value );
		// 		else if constexpr ( std::is_same<ValueTypeT, impl::StringType>::value )
		// 			parser.parseString( &value );
		// 		else if constexpr ( std::is_base_of<impl::StructType, ValueTypeT>::value )
		// 		{
		// 			impl::parseStructBegin( parser );
		// 			if constexpr( suppressNotifications )
		// 				ValueTypeT::parseForStateSyncOrMessageInDepth( parser, value );
		// 			else
		// 				ValueTypeT::parse( parser, value );
		// 			impl::parseStructEnd( parser );
		// 		}
		// 		else
		// 			static_assert( std::is_same<ValueTypeT, AllowedDataType>::value, "unsupported type" );

		// 		dest.insert( std::make_pair( key, value ) );
		// 	}
		// }
		// else
		// {
		// 	static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );

			size_t collSz = parser.dictionaryBegin();

			// parser.skipDelimiter( '{' );
			// if ( parser.isDelimiter( '}' ) )
			// {
			// 	parser.skipDelimiter( '}' );
			// 	if ( parser.isDelimiter( ',' ) )
			// 		parser.skipDelimiter( ',' );
			// 	return;
			// }
			for ( size_t i = 0; i < collSz && !parser.isDictionaryEnd(); ++i )
			{
				key_type key;
				value_type value;

				if(i != 0)
					parser.nextElement();


				parser.structBegin();

				// key
				if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					key = parser.parseSignedInteger();
				else if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					key = parser.parseUnsignedInteger();
				else if constexpr ( std::is_same<KeyTypeT, globalmq::marshalling::impl::StringType>::value )
					key = parser.readString();
				else
					static_assert( std::is_same<KeyTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );

				parser.nextElement();

				// value
				if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::SignedIntegralType>::value )
					value = parser.parseSignedInteger();
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::UnsignedIntegralType>::value )
					value = parser.parseUnsignedInteger();
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::RealType>::value )
					value = parser.parseReal();
				else if constexpr ( std::is_same<ValueTypeT, globalmq::marshalling::impl::StringType>::value )
					value = parser.parseString();
				else if constexpr ( std::is_base_of<globalmq::marshalling::impl::StructType, ValueTypeT>::value )
				{
					// parser.structBegin();
					if constexpr( suppressNotifications )
						ValueTypeT::parseForStateSyncOrMessageInDepth( parser, value );
					else
						ValueTypeT::parse( parser, value );
					// parser.structEnd( parser );
				}
				else
					static_assert( std::is_same<ValueTypeT, globalmq::marshalling::AllowedDataType>::value, "unsupported type" );

				parser.structEnd();
				dest.insert( std::make_pair( key, value ) );

				// if ( parser.isDelimiter( ',' ) )
				// {
				// 	parser.skipDelimiter( ',' );
				// 	continue;
				// }
				// if ( parser.isDelimiter( '}' ) )
				// {
				// 	parser.skipDelimiter( '}' );
				// 	break;
				// }
			}
			parser.dictionaryEnd();
			// if ( parser.isDelimiter( ',' ) )
			// 	parser.skipDelimiter( ',' );
		// }
	}
};


} // namespace globalmq::marshalling2

#endif // GLOBALMQ_MARSHALLING2_MARSHALLING2_H
