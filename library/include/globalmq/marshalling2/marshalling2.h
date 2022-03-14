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

// mb: this is a reference interface for a composer. not really to be used virtually 
class IComposer2
{
	virtual void composeSignedInteger(int64_t val) = 0;
	virtual void composeUnsignedInteger(uint64_t val) = 0;
	virtual void composeReal(double val) = 0;
	virtual void composeString(const GMQ_COLL string& val) = 0;

	virtual void structBegin() = 0;
	virtual void structEnd() = 0;

	virtual void vectorBegin(uint64_t sz) = 0;
	virtual void vectorEnd() = 0;

	virtual void namedParamBegin(const char* name) = 0;
	virtual void nextElement() = 0;

	virtual ~IComposer2() {}
};

template<class BufferT>
class JsonComposer2 : public IComposer2
{
	BufferT& buff;

public:
	JsonComposer2( BufferT& buff_ ) : buff( buff_ ) {}

	void composeSignedInteger(int64_t val) override { impl::json::composeSignedInteger(*this, val); }
	void composeUnsignedInteger(uint64_t val) override { impl::json::composeUnsignedInteger(*this, val); }
	void composeReal(double val) override { impl::json::composeReal(*this, val); }
	void composeString(const GMQ_COLL string& val) override { impl::json::composeString(*this, val); }

	void structBegin() override { buff.appendUint8( '{' ); }
	void structEnd() override { buff.appendUint8( '}' ); }

	void vectorBegin(uint64_t sz) override { buff.appendUint8( '[' ); }
	void vectorEnd() override { buff.appendUint8( ']' ); }

	void namedParamBegin(const char* name) override { impl::json::addNamePart(*this, name); }
	void nextElement() override { buff.appendUint8( ',' ); }

	~JsonComposer2() override {}
};

template<class BufferT>
class GmqComposer2
{
	BufferT& buff;

public:
	GmqComposer2( BufferT& buff_ ) : buff( buff_ ) {}

	void composeSignedInteger(int64_t val) { impl::composeSignedInteger(*this, val); }
	void composeUnsignedInteger(uint64_t val) { impl::composeUnsignedInteger(*this, val); }
	void composeReal(double val) { impl::composeReal(*this, val); }
	void composeString(const GMQ_COLL string& val) { impl::composeString(*this, val); }

	void structBegin() {}
	void structEnd() {}

	void vectorBegin(uint64_t sz) { composeUnsignedInteger(sz); }
	void vectorEnd() {}

	void namedParamBegin(const char* name) {}
	void nextElement() {}
};


template<typename ComposerT>
void composeAddressInPublishable2( ComposerT& composer, const GMQ_COLL vector<size_t>& addr, size_t last )
{
	composer.namedParamBegin("addr");
	composer.vectorBegin(addr.size());

	for (size_t each : addr )
	{
		composer.composeUnsignedInteger(each);
		composer.nextElement();
	}

	composer.composeUnsignedInteger(last);
	composer.vectorEnd();
}

template<typename ComposerT>
void composeActionInPublishable2( ComposerT& composer, size_t action )
{
	composer.namedParamBegin("action");
	composer.composeUnsignedInteger(action);
}


template<typename ComposerT, typename ParamTypeClassifier, typename ArgT>
void composeEntry2(ComposerT& composer, const char* name, ArgT& arg)
{
	using AgrType = typename std::remove_reference<typename special_decay_t<ArgT>::Type>::type;

	{
		if constexpr ( std::is_same<ParamTypeClassifier, impl::SignedIntegralType>::value && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_reference<AgrType>::type>::value) )
		{
			composer.namedParamBegin(name);
			composer.composeSignedInteger(arg.get());
		}
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::UnsignedIntegralType>::value && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_reference<AgrType>::type>::value) )
		{
			composer.namedParamBegin(name);
			composer.composeUnsignedInteger(arg.get());
		}
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::RealType>::value && (std::is_arithmetic<AgrType>::value || std::is_arithmetic<typename std::remove_reference<AgrType>::type>::value) )
		{
			composer.namedParamBegin(name);
			composer.composeReal(arg.get());
		}
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::StringType>::value )
		{
			composer.namedParamBegin(name);
			composer.composeString(arg.get());
		}
		else if constexpr ( std::is_base_of<impl::VectorType, ParamTypeClassifier>::value )
		{
			if constexpr ( std::is_base_of<VectorOfSympleTypesBase, ParamTypeClassifier>::value && std::is_base_of<SimpleTypeCollectionWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& coll = arg.get();
				composer.vectorBegin(0);
				composer.vectorEnd();
			}
			else if constexpr ( (std::is_base_of<VectorOfNonextMessageTypesBase, ParamTypeClassifier>::value || std::is_base_of<VectorOfNonextDiscriminatedUnionTypesBase, ParamTypeClassifier>::value) && std::is_base_of<CollectionWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& coll = arg.get();
				composer.vectorBegin(0);
				composer.vectorEnd();
			}
			else if constexpr ( (std::is_base_of<VectorOfMessageType, ParamTypeClassifier>::value || std::is_base_of<VectorOfDiscriminatedUnionType, ParamTypeClassifier>::value) && std::is_base_of<CollectionWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& coll = arg.get();
				composer.vectorBegin(0);
				composer.vectorEnd();
			}
		}
		else if constexpr ( std::is_base_of<impl::MessageType, ParamTypeClassifier>::value )
		{
			if constexpr ( std::is_base_of<NonextMessageType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& msg = arg.get();
				msg.compose(composer);
			}
			else if constexpr ( std::is_base_of<MessageType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& msg = arg.get();
				msg.compose(composer);
			}
		}
		else if constexpr ( std::is_base_of<impl::DiscriminatedUnionType, ParamTypeClassifier>::value )
		{
			if constexpr ( std::is_base_of<NonextDiscriminatedUnionType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& msg = arg.get();
				msg.compose(composer);
			}
			else if constexpr ( std::is_base_of<DiscriminatedUnionType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				composer.namedParamBegin(name);
				auto& msg = arg.get();
				msg.compose(composer);
			}
		}
		else
			static_assert( std::is_same<AgrType, AllowedDataType>::value, "unsupported type" );
	}
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue>
void composeParam2(ComposerT& composer, GMQ_COLL string name, const typename TypeToPick::NameAndTypeID expected)
{
		static_assert( !required, "required parameter" );
		if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value )
		{
			static_assert ( std::is_integral<AssumedDefaultT>::value );
			composer.namedParamBegin(name);
			composer.composeSignedInteger(defaultValue);
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value )
		{
			static_assert ( std::is_integral<AssumedDefaultT>::value );
			composer.namedParamBegin(name);
			composer.composeUnsignedInteger(defaultValue);
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value )
		{
			composer.namedParamBegin(name);
			composer.composeReal(AssumedDefaultT::value());
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
		{
			composer.namedParamBegin(name);
			composer.composeReal(defaultValue);
		}
		else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
		{
			composer.namedParamBegin(name);
			composer.vectorBegin(0);
			composer.vectorEnd();
		}
		else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
		{
			composer.namedParamBegin(name);
			composer.structBegin();
			composer.structEnd();
		}
		else if constexpr ( std::is_base_of<impl::DiscriminatedUnionType, typename TypeToPick::Type>::value )
		{
			composer.namedParamBegin(name);
			composer.structBegin();
			composer.structEnd();
		}
		// TODO: add supported types here
		else
			static_assert( std::is_same<typename TypeToPick::Type, AllowedDataType>::value, "unsupported type" );
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue, typename Arg0, typename ... Args>
void composeParam2(ComposerT& composer, const char* name, const typename TypeToPick::NameAndTypeID expected, Arg0&& arg0, Args&& ... args)
{
	using Agr0Type = special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename special_decay_t<Arg0>::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		composeEntry2<ComposerT, typename TypeToPick::Type, Arg0>(composer, name, arg0);
	}
	else
		composeParam2<ComposerT, TypeToPick, required, AssumedDefaultT, DefaultT, defaultValue>(composer, name, expected, args...);
}


// mb: this is a reference interface for a composer. not really to be used virtually 
class IParser2
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

	virtual void namedParamBegin(const char* name) = 0;
	virtual bool nextElement() = 0;

	virtual ~IParser2() {}
};

// mb: this is a reference interface for a composer. not really to be used virtually
template<class SubParserT>
class JsonParser2
{
	SubParserT p;

public:
	JsonParser2(SubParserT p) :p(p) {}

	int64_t parseSignedInteger() override { int64_t v; p.readSignedIntegerFromJson(&v); return v; }
	uint64_t parseUnsignedInteger() override { uint64_t v; p.readUnsignedIntegerFromJson(&v); return v; }
	double parseReal() override { double v; p.readRealFromJson(&v); return v; }
	GMQ_COLL string parseString() override { GMQ_COLL string v; p.readStringFromJson(&v); return v; }

	void structBegin() override { p.skipDelimiter('{'); }
	void structEnd() override { p.skipDelimiter('}'); }

	uint64_t vectorBegin() override { p.skipDelimiter('['); return UINT64_MAX; }
	void vectorEnd() override { p.skipDelimiter(']'); }

	void namedParamBegin(const char* name) override { GMQ_COLL string v; p.readKey(&v); if(v != name) throw std::exception(); }
	bool nextElement() override
	{
		if(p.isDelimeter(',')
		{
			p.skipDelimiter(',');
			return true;	
		}
		return false;
	}

	virtual ~JsonParser2() {}
};


template<class SubParserT>
class GmqParser2
{
	SubParserT parser;

public:
	GmqParser2(SubParserT parser) :parser(parser) {}

	int64_t parseSignedInteger() { int64_t v; parser.parseSignedInteger(&v); return v; }
	uint64_t parseUnsignedInteger() { uint64_t v; parser.parseUnsignedInteger(&v); return v; }
	double parseReal() { double v; parser.parseReal(&v); return v; }
	GMQ_COLL string parseString() { GMQ_COLL string v; parser.parseString(&v); return v; }

	void structBegin() { }
	void structEnd() { }

	uint64_t vectorBegin() { return parseUnsignedInteger(); }
	void vectorEnd() { }

	void namedParamBegin(const char* name) { }
	bool nextElement() { return true; }
};

template<typename ParserT>
uint64_t parseMessageId(ParserT& parser)
{
	parser.namedParamBegin("msgid");
	uint64_t v = parser.parseUnsignedInteger();
	parser.nextElement();

	return v;
}

template<typename ParserT>
bool parseAddressInPublishable(ParserT& parser, GMQ_COLL vector<size_t>& addr)
{
	addr.clear();
	parser.namedParamBegin("addr");
	uint64_t sz = parser.vectorBegin();

	if(sz != UINT64_MAX)
		addr.reserve(static_cast<size_t>(sz));

	for (size_t i = 0; i != sz && parser.nextElement(); ++i)
	{
		uint64_t v = parser.parseUnsignedInteger();
		addr.push_back(static_cast<size_t>(v));
	}

	parser.vectorEnd();
	return !add.empty();
}

template<typename ParserT>
uint64_t parseActionInPublishable(ParserT& parser)
{
	parser.namedParamBegin("action");
	uint64_t v = parser.parseUnsignedInteger();
	parser.nextElement();
	return v;
}

} // namespace globalmq::marshalling2

#endif // GLOBALMQ_MARSHALLING2_MARSHALLING2_H
