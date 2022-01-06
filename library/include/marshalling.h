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

#ifndef NAMED_PARAMS_CORE_H
#define NAMED_PARAMS_CORE_H

#include "marshalling_impl.h"

namespace globalmq::marshalling {

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

template<typename NameT, typename DataT>
struct ExpectedParameter
{
	using NameTag = NameT;
	using DataType = DataT;
};

struct AllowedDataType {};

struct AnyCollectionWrapperBase {
	static constexpr size_t unknown_size = (size_t)(-1);
};

struct CollectionWrapperBase : public AnyCollectionWrapperBase {};

struct SimpleTypeCollectionWrapperBase : public AnyCollectionWrapperBase {};

template<class LambdaSize, class LambdaNext>
class CollectionWrapperForComposing : public CollectionWrapperBase {
	LambdaSize lsize_;
	LambdaNext lnext_;
public:
	CollectionWrapperForComposing(LambdaSize &&lsize, LambdaNext &&lnext) : lsize_(std::forward<LambdaSize>(lsize)), lnext_(std::forward<LambdaNext>(lnext)) {}
	size_t size() { return lsize_(); }
	template<typename ComposerT>
	void compose_next( ComposerT& composer, size_t ordinal ) { 
		return lnext_( composer, ordinal ); 
	}
};

template<class LambdaSize, class LambdaNext>
class CollectionWrapperForParsing : public CollectionWrapperBase {
    LambdaSize lsize_;
	LambdaNext lnext_;
public:
	CollectionWrapperForParsing(LambdaSize &&lsizeHint, LambdaNext &&lnext) : lsize_(std::forward<LambdaSize>(lsizeHint)), lnext_(std::forward<LambdaNext>(lnext)) {
//		static_assert( std::is_same<std::nullptr_t, LambdaSize>::value || std::is_invocable<LambdaSize>::value, "lambda-expression is expected" );
//		static_assert( std::is_invocable<LambdaNext>::value || std::is_invocable<std::remove_reference<LambdaNext>::type>::value, "lambda-expression is expected" );
	}
	void size_hint( size_t sz ) { 
		if constexpr ( !std::is_same<LambdaSize, std::nullptr_t>::value )
			if ( sz != CollectionWrapperBase::unknown_size )
				lsize_( sz );
	}
	template<typename ParserT>
	void parse_next( ParserT& p, size_t ordinal ) { 
		lnext_( p, ordinal ); 
	}
};

template<class T>
class SimpleTypeCollectionWrapper : public SimpleTypeCollectionWrapperBase
{
	T& coll;
	typename T::iterator it;

public:
	using value_type = typename T::value_type;
	static_assert( std::is_same<T, GMQ_COLL vector<value_type>>::value, "vector type is expected only" ); // TODO: add list
	static_assert( std::is_arithmetic<value_type>::value || std::is_same<value_type, GMQ_COLL string>::value || std::is_enum<value_type>::value, "intended for simple idl types only" );

	SimpleTypeCollectionWrapper( T& coll_ ) : coll( coll_ ), it( coll.begin() ) {};
	size_t size() const { return coll.size(); }
	template<class ExpectedType, class ComposerT>
	bool compose_next_to_gmq( ComposerT& composer )
	{ 
		if ( it != coll.end() )
		{
			if constexpr ( std::is_same<typename ExpectedType::value_type, impl::SignedIntegralType>::value && std::is_integral<value_type>::value )
				impl::composeSignedInteger( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::UnsignedIntegralType>::value && std::is_integral<value_type>::value )
				impl::composeUnsignedInteger( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::RealType>::value && std::is_arithmetic<value_type>::value )
				impl::composeReal( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::StringType>::value && std::is_same<value_type, GMQ_COLL string>::value )
				impl::composeString( composer, *it );
			else
				static_assert( std::is_same<value_type, AllowedDataType>::value, "unsupported type" );
			it++;
			return it != coll.end();
		}
		else
			return false;
	}
	template<class ExpectedType, class ComposerT>
	bool compose_next_to_json( ComposerT& composer )
	{ 
		if ( it != coll.end() )
		{
			if constexpr ( std::is_same<typename ExpectedType::value_type, impl::SignedIntegralType>::value && std::is_integral<value_type>::value )
				impl::json::composeSignedInteger( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::UnsignedIntegralType>::value && std::is_integral<value_type>::value )
				impl::json::composeUnsignedInteger( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::RealType>::value && std::is_arithmetic<value_type>::value )
				impl::json::composeReal( composer, *it );
			else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::StringType>::value && std::is_same<value_type, GMQ_COLL string>::value )
				impl::json::composeString( composer, *it );
			else
				static_assert( std::is_same<value_type, AllowedDataType>::value, "unsupported type" );
			it++;
			return it != coll.end();
		}
		else
			return false;
	}

	void size_hint( size_t count )
	{
		// Note: here we can do some preliminary steps based on a number of collection elements declared in the message (if such a number exists for a particular protocol) 
		if constexpr ( std::is_same<T, GMQ_COLL vector<value_type>>::value )
		{
			if ( count != unknown_size )
				coll.reserve( count );
		}
	}
	template<class ExpectedType, class ParserT>
	void parse_next_from_gmq( ParserT& p )
	{
		value_type val;
		if constexpr ( std::is_same<typename ExpectedType::value_type, impl::SignedIntegralType>::value && std::is_integral<value_type>::value )
			p.parseSignedInteger( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::UnsignedIntegralType>::value && std::is_integral<value_type>::value )
			p.parseUnsignedInteger( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::RealType>::value && std::is_arithmetic<value_type>::value )
			p.parseReal( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::StringType>::value && std::is_same<value_type, GMQ_COLL string>::value )
			p.parseString( &val );
		else
			static_assert( std::is_same<value_type, AllowedDataType>::value, "unsupported type" );
		coll.push_back( val );
	}
	template<class ExpectedType, class ParserT>
	void parse_next_from_json( ParserT& p )
	{
		value_type val;
		if constexpr ( std::is_same<typename ExpectedType::value_type, impl::SignedIntegralType>::value && std::is_integral<value_type>::value )
			p.readSignedIntegerFromJson( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::UnsignedIntegralType>::value && std::is_integral<value_type>::value )
			p.readUnsignedIntegerFromJson( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::RealType>::value && std::is_arithmetic<value_type>::value )
			p.readRealFromJson( &val );
		else if constexpr ( std::is_same<typename ExpectedType::value_type, impl::StringType>::value && std::is_same<value_type, GMQ_COLL string>::value )
			p.readStringFromJson( &val );
		else
			static_assert( std::is_same<value_type, AllowedDataType>::value, "unsupported type" );
		coll.push_back( val );
	}
};


struct MessageWrapperBase {
	static constexpr size_t unknown_size = (size_t)(-1);
};

template<class LambdaCompose>
class MessageWrapperForComposing : public MessageWrapperBase {
	LambdaCompose lcompose_;
public:
	MessageWrapperForComposing(LambdaCompose &&lcompose) : lcompose_(std::forward<LambdaCompose>(lcompose)) {}
	template<typename ComposerT>
	void compose( ComposerT& composer ) { 
		return lcompose_( composer ); 
	}
};

template<class LambdaParse>
class MessageWrapperForParsing : public MessageWrapperBase {
	LambdaParse lparse_;
public:
	MessageWrapperForParsing(LambdaParse &&lparse) : lparse_(std::forward<LambdaParse>(lparse)) {
	}
	template<typename ParserT>
	void parse( ParserT& p ) { 
		lparse_( p ); 
	}
};

template<typename BaseT, typename Arg0, typename ... Args>
void findMatch(const Arg0 arg0, const Args ... args)
{
	static_assert( std::is_same<typename BaseT::NameTag, typename Arg0::NameTag>::value == false, "same name used more than once" );
	findMatch<BaseT>(args...);
}

template<typename BaseT, typename Arg0>
void findMatch(const Arg0 arg0)
{
	static_assert( std::is_same<typename BaseT::NameTag, typename Arg0::NameTag>::value == false, "same name used more than once" );
}

template<typename Arg0, typename ... Args>
void ensureUniqueness(const Arg0 arg0, const Args ... args)
{
	findMatch<Arg0>(args...);
	ensureUniqueness(args...);
}

template<typename Arg0>
void ensureUniqueness(const Arg0 arg0)
{
	return;
}


template<typename TypeToMatch, typename Arg0, typename ... Args>
constexpr size_t isMatched(const TypeToMatch matcher, const Arg0 arg0, const Args ... args)
{
	if constexpr ( std::is_same<typename Arg0::NameTag, typename TypeToMatch::NameTag>::value )
		return 1;
	else if constexpr ( std::is_same<typename TypeToMatch::NameTag, impl::SignedIntegralType>::value && std::is_integral<typename Arg0::NameTag>::value )
		 return 1;
	else if constexpr ( std::is_same<typename TypeToMatch::NameTag, impl::UnsignedIntegralType>::value && std::is_integral<typename Arg0::NameTag>::value )
		 return 1;
   else
		return isMatched<TypeToMatch>(matcher, args...);
}

template<typename TypeToMatch>
constexpr size_t isMatched(const TypeToMatch matcher)
{
	return 0;
}



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

namespace impl {

// composing - general



// parsing - general


namespace gmq {

template<typename ParserT, typename TypeToPick, bool required>
void parseGmqParam(ParserT& p, const typename TypeToPick::NameAndTypeID expected)
{
	if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value )
		p.skipSignedInteger();
	else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value )
		p.skipUnsignedInteger();
	else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value )
		p.skipReal();
	else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
		p.skipString();
	else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
	{
		size_t sz = 0;
		p.parseUnsignedInteger( &sz );
		if ( sz != 0 )
			assert( false ); // not implemented
	}
	else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
	{
		assert( false ); // not implemented
	}
	else
		static_assert( std::is_same<typename TypeToPick::Type, AllowedDataType>::value, "unsupported type" );
}

template<typename ParserT, typename TypeToPick, bool required, typename Arg0, typename ... Args>
void parseGmqParam(ParserT& p, const typename TypeToPick::NameAndTypeID expected, Arg0&& arg0, Args&& ... args)
{
	using Agr0Type = special_decay_t<Arg0>;
	using Agr0DataType = typename std::remove_pointer<typename Agr0Type::Type>::type;
	if constexpr ( std::is_same<typename Agr0Type::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value && std::is_integral<Agr0DataType>::value )
			p.parseSignedInteger( arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value && std::is_integral<Agr0DataType>::value )
			p.parseUnsignedInteger( arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value && std::is_arithmetic<Agr0DataType>::value )
			p.parseReal( arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
			p.parseString( arg0.get() );
		else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
		{
			if constexpr ( std::is_base_of<VectorOfSympleTypesBase, typename TypeToPick::Type>::value && std::is_base_of<SimpleTypeCollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				size_t sz = 0;
				p.parseUnsignedInteger( &sz );
				auto& coll = arg0.get();
				coll.size_hint( sz );
				for ( size_t i=0; i<sz; ++i )
					coll.template parse_next_from_gmq<typename TypeToPick::Type>( p );
			}
			else if constexpr ( std::is_base_of<VectorOfNonextMessageTypesBase, typename TypeToPick::Type>::value && std::is_base_of<CollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				size_t sz = 0;
				p.parseUnsignedInteger( &sz );
				auto& coll = arg0.get();
				coll.size_hint( sz );
				for ( size_t i=0; i<sz; ++i )
					coll.parse_next( p, i );
			}
			else if constexpr ( std::is_base_of<VectorOfMessageType, typename TypeToPick::Type>::value && std::is_base_of<CollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				size_t collSz = 0;
				p.parseUnsignedInteger( &collSz );
				auto& coll = arg0.get();
				coll.size_hint( collSz );
				for ( size_t i=0; i<collSz; ++i )
				{
					size_t itemSz = 0;
					p.parseUnsignedInteger( &itemSz );
					// ParserT itemParser( p );
					coll.parse_next( p, i );
					// p.adjustParsingPos( itemSz );
				}
			}
			else
				static_assert( std::is_same<Agr0DataType, AllowedDataType>::value, "unsupported type" );
		}
		else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
		{
			if constexpr ( std::is_base_of<NonextMessageType, typename TypeToPick::Type>::value && std::is_base_of<MessageWrapperBase, typename Agr0Type::Type>::value )
			{
				// TODO: check whether there is any difference between ext and non-ext cases
				auto& msg = arg0.get();
				msg.parse( p );
			}
			else if constexpr ( std::is_base_of<MessageType, typename TypeToPick::Type>::value && std::is_base_of<MessageWrapperBase, typename Agr0Type::Type>::value )
			{
				// TODO: check whether there is any difference between ext and non-ext cases
				auto& msg = arg0.get();
				msg.parse( p );
			}
			else
				static_assert( std::is_same<Agr0DataType, AllowedDataType>::value, "unsupported type" );
		}
		else
			static_assert( std::is_same<Agr0DataType, AllowedDataType>::value, "unsupported type" );
	}
	else
		parseGmqParam<ParserT, TypeToPick, required>(p, expected, args...);
}


///////////////////////////////////////////

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue>
void composeParamToGmq(ComposerT& composer, const typename TypeToPick::NameAndTypeID expected)
{
	static_assert( !required, "required parameter" );
	if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value )
	{
		static_assert ( std::is_integral<AssumedDefaultT>::value );
		composeSignedInteger( composer, defaultValue );
	}
	else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value )
	{
		static_assert ( std::is_integral<AssumedDefaultT>::value );
		composeUnsignedInteger( composer, defaultValue );
	}
	else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value )
	{
		composeReal( composer, AssumedDefaultT::value() );
	}
	else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
	{
		composeString( composer, defaultValue );
	}
	else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
	{
		composeUnsignedInteger( composer, 0 );
	}
	else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
	{
	}
	// TODO: add supported types here
	else
		static_assert( std::is_same<typename TypeToPick::Type, AllowedDataType>::value, "unsupported type" );
}

namespace checkers {
template<typename T>
concept has_set_size = requires { { std::declval<T>().set_size(std::declval<size_t>()) }; };
} // namespace impl::checkers

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue, typename Arg0, typename ... Args>
void composeParamToGmq(ComposerT& composer, const typename TypeToPick::NameAndTypeID expected, Arg0&& arg0, Args&& ... args)
{
	using Agr0Type = special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename special_decay_t<Arg0>::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		if constexpr ( std::is_same<typename TypeToPick::Type, impl::SignedIntegralType>::value && (std::is_integral<typename Agr0Type::Type>::value || std::is_integral<typename std::remove_reference<typename Agr0Type::Type>::type>::value) )
			composeSignedInteger( composer, arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, impl::UnsignedIntegralType>::value && (std::is_integral<typename Agr0Type::Type>::value || std::is_integral<typename std::remove_reference<typename Agr0Type::Type>::type>::value) )
			composeUnsignedInteger( composer, arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, impl::RealType>::value && (std::is_arithmetic<typename Agr0Type::Type>::value || std::is_arithmetic<typename std::remove_reference<typename Agr0Type::Type>::type>::value) )
			composeReal( composer, arg0.get() );
		else if constexpr ( std::is_same<typename TypeToPick::Type, impl::StringType>::value )
			composeString( composer, arg0.get() );
		else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
		{
			if constexpr ( std::is_base_of<VectorOfSympleTypesBase, typename TypeToPick::Type>::value && std::is_base_of<SimpleTypeCollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				auto& coll = arg0.get();
				size_t collSz = coll.size();
				composeUnsignedInteger( composer, collSz );
				for ( size_t i=0; i<collSz; ++i )
					coll.template compose_next_to_gmq<typename TypeToPick::Type>(composer);
			}
			else if constexpr ( std::is_base_of<VectorOfNonextMessageTypesBase, typename TypeToPick::Type>::value && std::is_base_of<CollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				auto& coll = arg0.get();
				size_t collSz = coll.size();
				composeUnsignedInteger( composer, collSz );
				for ( size_t i=0; i<collSz; ++i )
					coll.compose_next(composer, i);
			}
			else if constexpr ( std::is_base_of<VectorOfMessageType, typename TypeToPick::Type>::value && std::is_base_of<CollectionWrapperBase, typename Agr0Type::Type>::value )
			{
				static_assert( checkers::has_set_size<typename ComposerT::BufferType>, "not properly implemented for certain kind of underlying buffers" );
				// TODO: ...
				auto& coll = arg0.get();
				size_t collSz = coll.size();
				composeUnsignedInteger( composer, collSz );
				if ( collSz )
				{
					size_t pos = composer.buff.size();
					composer.buff.set_size( composer.buff.size() + integer_max_size ); // TODO: revise toward lowest estimation of 1 with move if longer
					for ( size_t i=0; i<collSz; ++i )
					{
						coll.compose_next(composer, i);
						int64_t written = composer.buff.size() - pos - integer_max_size;
						memcpy( composer.buff.begin() + pos, &written, integer_max_size );
						pos = composer.buff.size();
						if ( i != collSz - 1 )
							composer.buff.set_size( composer.buff.size() + integer_max_size ); // TODO: revise toward lowest estimation of 1 with move if longer
					}
				}
			}
		}
		else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
		{
			if constexpr ( std::is_base_of<NonextMessageType, typename TypeToPick::Type>::value && std::is_base_of<MessageWrapperBase, typename Agr0Type::Type>::value )
			{
				auto& msg = arg0.get();
				msg.compose(composer);
			}
			else if constexpr ( std::is_base_of<MessageType, typename TypeToPick::Type>::value && std::is_base_of<MessageWrapperBase, typename Agr0Type::Type>::value )
			{
				auto& msg = arg0.get();
				msg.compose( composer );
				// TODO: anything else?
			}
		}
		else
			static_assert( std::is_same<typename Agr0Type::Type, AllowedDataType>::value, "unsupported type" );
	}
	else
		composeParamToGmq<ComposerT, TypeToPick, required, AssumedDefaultT, DefaultT, defaultValue>(composer, expected, args...);
}

} // namespace gmq

namespace json {

template<typename ParserT, typename ParamTypeClassifier, typename ArgT>
void parseJsonEntry( ParserT& p, ArgT& arg)
{
	using AgrType = typename std::remove_reference<typename special_decay_t<ArgT>::Type>::type;

	if constexpr ( std::is_same<ParamTypeClassifier, SignedIntegralType>::value && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_pointer<AgrType>::type>::value) )
		p.readSignedIntegerFromJson( arg.get() );
	else if constexpr ( std::is_same<ParamTypeClassifier, UnsignedIntegralType>::value/* && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_pointer<AgrType>::type>::value)*/ )
		p.readUnsignedIntegerFromJson( arg.get() );
	else if constexpr ( std::is_same<ParamTypeClassifier, RealType>::value/* && (std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_pointer<ArgT>::type>::value)*/ )
		p.readRealFromJson( arg.get() );
	else if constexpr ( std::is_same<ParamTypeClassifier, StringType>::value )
		p.readStringFromJson( arg.get() );
	else if constexpr ( std::is_base_of<impl::VectorType, ParamTypeClassifier>::value )
	{
		auto& coll = arg.get();
		coll.size_hint( CollectionWrapperBase::unknown_size );
		if constexpr ( std::is_base_of<VectorOfSympleTypesBase, ParamTypeClassifier>::value/* && std::is_base_of<SimpleTypeCollectionWrapperBase, ArgT>::value*/ )
		{
			p.skipDelimiter( '[' );
			if ( !p.isDelimiter( ']' ) ) // there are some items there
			{
				for ( ;; )
				{
					coll.template parse_next_from_json<ParamTypeClassifier>( p );
					if ( p.isDelimiter( ',' ) )
					{
						p.skipDelimiter( ',' );
						continue;
					}
					if ( p.isDelimiter( ']' ) )
					{
						p.skipDelimiter( ']' );
						break;
					}
				}
			}
			else
				p.skipDelimiter( ']' );
		}
		else if constexpr ( std::is_base_of<VectorOfNonextMessageTypesBase, ParamTypeClassifier>::value/* && std::is_base_of<CollectionWrapperBase, ArgT>::value*/ )
		{
			p.skipDelimiter( '[' );
			if ( !p.isDelimiter( ']' ) ) // there are some items there
			{
				for ( size_t i=0;;++i )
				{
					coll.parse_next( p, i );
					if ( p.isDelimiter( ',' ) )
					{
						p.skipDelimiter( ',' );
						continue;
					}
					if ( p.isDelimiter( ']' ) )
					{
						p.skipDelimiter( ']' );
						break;
					}
				}
			}
			else
				p.skipDelimiter( ']' );
		}
		else if constexpr ( std::is_base_of<VectorOfMessageType, ParamTypeClassifier>::value/* && std::is_base_of<CollectionWrapperBase, ArgT>::value*/ )
		{
			p.skipDelimiter( '[' );
			if ( !p.isDelimiter( ']' ) ) // there are some items there
			{
				for ( size_t i=0;;++i )
				{
					coll.parse_next( p, i );
					if ( p.isDelimiter( ',' ) )
					{
						p.skipDelimiter( ',' );
						continue;
					}
					if ( p.isDelimiter( ']' ) )
					{
						p.skipDelimiter( ']' );
						break;
					}
				}
			}
			else
				p.skipDelimiter( ']' );
		}
		else
			static_assert( std::is_same<ArgT, AllowedDataType>::value, "unsupported type" );
	}
	else if constexpr ( std::is_base_of<impl::MessageType, ParamTypeClassifier>::value )
	{
		auto& msg = arg.get();
		if constexpr ( std::is_same<NonextMessageType, ParamTypeClassifier>::value/* && std::is_base_of<MessageWrapperBase, ArgT>::value*/ )
		{
			// TODO: check whether there is any difference between ext and non-ext cases
			msg.parse( p );
		}
		else if constexpr ( std::is_same<MessageType, ParamTypeClassifier>::value/* && std::is_base_of<MessageWrapperBase, ArgT>::value*/ )
		{
			// TODO: check whether there is any difference between ext and non-ext cases
			msg.parse( p );
		}
		else
			static_assert( std::is_same<ArgT, AllowedDataType>::value, "unsupported type" );
	}
	else
		static_assert( std::is_same<ArgT, AllowedDataType>::value, "unsupported type" );
}

template<typename ComposerT, typename ParamTypeClassifier, typename ArgT>
void composeEntryToJson(ComposerT& composer, GMQ_COLL string name, ArgT& arg)
{
	using AgrType = typename std::remove_reference<typename special_decay_t<ArgT>::Type>::type;

	{
		if constexpr ( std::is_same<ParamTypeClassifier, impl::SignedIntegralType>::value && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_reference<AgrType>::type>::value) )
			composeNamedSignedInteger( composer, name, arg.get() );
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::UnsignedIntegralType>::value && (std::is_integral<AgrType>::value || std::is_integral<typename std::remove_reference<AgrType>::type>::value) )
			composeNamedUnsignedInteger( composer, name, arg.get() );
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::RealType>::value && (std::is_arithmetic<AgrType>::value || std::is_arithmetic<typename std::remove_reference<AgrType>::type>::value) )
			composeNamedReal( composer, name, arg.get() );
		else if constexpr ( std::is_same<ParamTypeClassifier, impl::StringType>::value )
			composeNamedString( composer, name, arg.get() );
		else if constexpr ( std::is_base_of<impl::VectorType, ParamTypeClassifier>::value )
		{
			if constexpr ( std::is_base_of<VectorOfSympleTypesBase, ParamTypeClassifier>::value && std::is_base_of<SimpleTypeCollectionWrapperBase, AgrType>::value )
			{
				json::addNamePart( composer, name );
				composer.buff.appendUint8( '[' );
				auto& coll = arg.get();
				size_t collSz = coll.size();
				for ( size_t i=0; i<collSz; ++i )
				{
					if ( i )
						composer.buff.append( ", ", 2 );
					bool ok = coll.template compose_next_to_json<ParamTypeClassifier>(composer);
				}
				composer.buff.appendUint8( ']' );
			}
			else if constexpr ( std::is_base_of<VectorOfNonextMessageTypesBase, ParamTypeClassifier>::value && std::is_base_of<CollectionWrapperBase, AgrType>::value )
			{
				json::addNamePart( composer, name );
				composer.buff.appendUint8( '[' );
				auto& coll = arg.get();
				size_t collSz = coll.size();
				for ( size_t i=0; i<collSz; ++i )
				{
					if ( i )
						composer.buff.append( ", ", 2 );
					coll.compose_next(composer, i);
				}
				composer.buff.appendUint8( ']' );
			}
			else if constexpr ( std::is_base_of<VectorOfMessageType, ParamTypeClassifier>::value && std::is_base_of<CollectionWrapperBase, AgrType>::value )
			{
				json::addNamePart( composer, name );
				composer.buff.appendUint8( '[' );
				auto& coll = arg.get();
				size_t collSz = coll.size();
				for ( size_t i=0; i<collSz; ++i )
				{
					if ( i )
						composer.buff.append( ", ", 2 );
					coll.compose_next(composer, i);
				}
				composer.buff.appendUint8( ']' );
			}
		}
		else if constexpr ( std::is_base_of<impl::MessageType, ParamTypeClassifier>::value )
		{
			if constexpr ( std::is_base_of<NonextMessageType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				// TODO: check whether there are any difference between ext and non-ext case
				json::addNamePart( composer, name );
				auto& msg = arg.get();
				msg.compose(composer);
			}
			else if constexpr ( std::is_base_of<MessageType, ParamTypeClassifier>::value && std::is_base_of<MessageWrapperBase, AgrType>::value )
			{
				// TODO: check whether there are any difference between ext and non-ext case
				json::addNamePart( composer, name );
				auto& msg = arg.get();
				msg.compose(composer);
			}
		}
		else
			static_assert( std::is_same<AgrType, AllowedDataType>::value, "unsupported type" );
	}
}

template<typename ParserT, typename TypeToPick, bool required>
void parseJsonParam(const typename TypeToPick::NameAndTypeID expected, ParserT& p)
{
	if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value )
		p.skipSignedIntegerFromJson();
	else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value )
		p.skipUnsignedIntegerFromJson();
	else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value )
		p.skipRealFromJson();
	else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
		p.skipStringFromJson();
	else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
		p.skipVectorFromJson();
	else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
		p.skipMessageFromJson();
	else
		static_assert( std::is_same<typename TypeToPick::Type, AllowedDataType>::value, "unsupported type" );
}

template<typename ParserT, typename TypeToPick, bool required, typename Arg0, typename ... Args>
void parseJsonParam(const typename TypeToPick::NameAndTypeID expected, ParserT& p, Arg0& arg0, Args& ... args)
{
	using Arg0Type = special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename Arg0Type::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		parseJsonEntry<ParserT, typename TypeToPick::Type, Arg0>( p, arg0 );
	}
	else
		parseJsonParam<ParserT, TypeToPick, required>(expected, p, args...);
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue>
void composeParamToJson(ComposerT& composer, GMQ_COLL string name, const typename TypeToPick::NameAndTypeID expected)
{
		static_assert( !required, "required parameter" );
		if constexpr ( std::is_same<typename TypeToPick::Type, SignedIntegralType>::value )
		{
			static_assert ( std::is_integral<AssumedDefaultT>::value );
			composeNamedSignedInteger( composer, name, defaultValue );
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, UnsignedIntegralType>::value )
		{
			static_assert ( std::is_integral<AssumedDefaultT>::value );
			composeNamedUnsignedInteger( composer, name, defaultValue );
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, RealType>::value )
		{
			composeNamedReal( composer, name, AssumedDefaultT::value() );
		}
		else if constexpr ( std::is_same<typename TypeToPick::Type, StringType>::value )
		{
			composeNamedString( composer, name, defaultValue );
		}
		else if constexpr ( std::is_base_of<impl::VectorType, typename TypeToPick::Type>::value )
		{
			json::addNamePart( composer, name );
			composer.buff.appendUint8( '[' );
			composer.buff.appendUint8( ']' );
		}
		else if constexpr ( std::is_base_of<impl::MessageType, typename TypeToPick::Type>::value )
		{
			json::addNamePart( composer, name );
			composer.buff.appendUint8( '{' );
			composer.buff.appendUint8( '}' );
		}
		// TODO: add supported types here
		else
			static_assert( std::is_same<typename TypeToPick::Type, AllowedDataType>::value, "unsupported type" );
}

template<typename ComposerT, typename TypeToPick, bool required, class AssumedDefaultT, class DefaultT, DefaultT defaultValue, typename Arg0, typename ... Args>
void composeParamToJson(ComposerT& composer, GMQ_COLL string name, const typename TypeToPick::NameAndTypeID expected, Arg0&& arg0, Args&& ... args)
{
	using Agr0Type = special_decay_t<Arg0>;
	if constexpr ( std::is_same<typename special_decay_t<Arg0>::Name, typename TypeToPick::Name>::value ) // same parameter name
	{
		composeEntryToJson<ComposerT, typename TypeToPick::Type, Arg0>(composer, name, arg0);
	}
	else
		json::composeParamToJson<ComposerT, TypeToPick, required, AssumedDefaultT, DefaultT, defaultValue>(composer, name, expected, args...);
}

} // namespace json


} // namespace impl


struct MessageHandlerBase {};

template<typename msgID_, class LambdaHandler>
class MessageHandler : public MessageHandlerBase
{
	LambdaHandler lhandler_;
public:
	static constexpr uint64_t msgID = msgID_::id;
	MessageHandler(LambdaHandler &&lhandler) : lhandler_(std::forward<LambdaHandler>(lhandler)) {}
	template<typename ParserT>
	void handle( ParserT& parser ) { 
		return lhandler_( parser ); 
	}
};

struct DefaultMessageHandlerBase : public MessageHandlerBase {};

template<class LambdaHandler>
class DefaultMessageHandler : public DefaultMessageHandlerBase
{
	LambdaHandler lhandler_;
public:
	DefaultMessageHandler(LambdaHandler &&lhandler) : lhandler_(std::forward<LambdaHandler>(lhandler)) {}
	template<typename ParserT>
	void handle( ParserT& parser, uint64_t msgID ) { 
		return lhandler_( parser, msgID ); 
	}
};

template<typename msgID_, class LambdaHandler>
MessageHandler<msgID_, LambdaHandler> makeMessageHandler( LambdaHandler &&lhandler ) {
	return MessageHandler<msgID_, LambdaHandler>(std::forward<LambdaHandler>(lhandler));
}

template<class LambdaHandler>
DefaultMessageHandler<LambdaHandler> makeDefaultMessageHandler( LambdaHandler &&lhandler ) {
	return DefaultMessageHandler<LambdaHandler>(std::forward<LambdaHandler>(lhandler));
}

namespace impl {

struct MessageNameBase {
	static constexpr uint64_t unknownMsgID = 0xFFFFFFFFFFFFFFFFULL;
};
template<uint64_t msgID>
struct MessageName : public MessageNameBase
{
	static constexpr uint64_t id = msgID;
};
using UnknownMessageName = MessageName<MessageNameBase::unknownMsgID>;

template<typename msgID, class ParserT>
bool implHandleMessage( ParserT& parser )
{
	return false;
}

template<typename msgID, class ParserT, class HandlerT, class ... HandlersT >
bool implHandleMessage( ParserT& parser, HandlerT handler, HandlersT ... handlers )
{
	static_assert( std::is_base_of<MessageNameBase, msgID>::value, "(Default) message handler is expected" );
	static_assert( std::is_base_of<MessageNameBase, msgID>::value );
	if constexpr ( std::is_base_of<DefaultMessageHandlerBase, HandlerT>::value )
	{
		constexpr size_t remainingArgCount = sizeof ... (HandlersT);
		static_assert ( remainingArgCount == 0, "Default handler, if present, must be the last argument" );
		handler.handle( parser, msgID::id );
		return false;
	}
	else
	{
		if constexpr ( HandlerT::msgID == msgID::id )
		{
			handler.handle( parser );
			return true;
		}
		else
			return implHandleMessage<msgID, ParserT, HandlersT...>( parser, handlers... );
	}
}

} // namespace impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace impl {


class IntegerProcessor
{
public:
	template<typename ComposerT, typename ArgT>
	static
	void compose(ComposerT& composer, ArgT arg)
	{
		static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
		if constexpr ( ComposerT::proto == Proto::GMQ )
			composeSignedInteger( composer, arg );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			json::composeSignedInteger( composer, arg );
		}
	}

	template<typename ParserT, typename ArgT>
	static
	void parse(ParserT& p, ArgT* arg)
	{
		static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
		if constexpr ( ParserT::proto == Proto::GMQ )
			p.parseSignedInteger( arg );
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			p.readSignedIntegerFromJson( arg );
		}
	}
};

class UnsignedIntegerProcessor
{
public:
	template<typename ComposerT, typename ArgT>
	static
	void compose(ComposerT& composer, ArgT arg)
	{
		static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
		if constexpr ( ComposerT::proto == Proto::GMQ )
			composeUnsignedInteger( composer, arg );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			json::composeUnsignedInteger( composer, arg );
		}
	}

	template<typename ParserT, typename ArgT>
	static
	void parse(ParserT& p, ArgT* arg)
	{
		static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
		if constexpr ( ParserT::proto == Proto::GMQ )
			p.parseUnsignedInteger( arg );
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			p.readUnsignedIntegerFromJson( arg );
		}
	}
};

class RealProcessor
{
public:
	template<typename ComposerT, typename ArgT>
	static
	void compose(ComposerT& composer, ArgT arg)
	{
		static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_reference<ArgT>::type>::value );
		if constexpr ( ComposerT::proto == Proto::GMQ )
			composeReal( composer, arg );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			json::composeReal( composer, arg );
		}
	}

	template<typename ParserT, typename ArgT>
	static
	void parse(ParserT& p, ArgT* arg)
	{
		static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_pointer<ArgT>::type>::value );
		if constexpr ( ParserT::proto == Proto::GMQ )
			p.parseReal( arg );
		else
		{
			p.readRealFromJson( arg );
		}
	}
};

class StringProcessor
{
public:
	template<typename ComposerT, typename ArgT>
	static
	void compose(ComposerT& composer, const ArgT& arg)
	{
		if constexpr ( ComposerT::proto == Proto::GMQ )
			composeString( composer, arg );
		else
		{
			static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
			json::composeString( composer, arg );

		}
	}

	template<typename ParserT, typename ArgT>
	static
	void parse(ParserT& p, ArgT* arg)
	{
		if constexpr ( ParserT::proto == Proto::GMQ )
			p.parseString( arg );
		else
		{
			static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
			p.readStringFromJson( arg );
		}
	}
};


template<typename ComposerT, typename NameT>
void composeKey(ComposerT& composer, NameT name)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::addNamePart( composer, name );
	}
}

template<typename ParserT, typename NameT>
void parseKey(ParserT& p, NameT expectedName )
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
	}
}

template<typename ComposerT, typename ArgT, typename NameT>
void publishableStructComposeInteger(ComposerT& composer, ArgT arg, NameT name, bool addListSeparator)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeSignedInteger( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedSignedInteger( composer, name, arg );
		if ( addListSeparator )
			composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT, typename NameT>
void publishableParseInteger(ParserT& p, ArgT* arg, NameT expectedName)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseSignedInteger( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
		p.readSignedIntegerFromJson( arg );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT, typename NameT>
void publishableStructComposeUnsignedInteger(ComposerT& composer, ArgT arg, NameT name, bool addListSeparator)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeUnsignedInteger( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedUnsignedInteger( composer, name, arg );
		if ( addListSeparator )
			composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT, typename NameT>
void publishableParseUnsignedInteger(ParserT& p, ArgT* arg, NameT expectedName)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseUnsignedInteger( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
		p.readUnsignedIntegerFromJson( arg );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT, typename NameT>
void publishableStructComposeReal(ComposerT& composer, ArgT arg, NameT name, bool addListSeparator)
{
	static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeReal( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedReal( composer, name, arg );
		if ( addListSeparator )
			composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT, typename NameT>
void publishableParseReal(ParserT& p, ArgT* arg, NameT expectedName)
{
	static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseReal( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
		p.readRealFromJson( arg );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT, typename NameT>
void publishableStructComposeString(ComposerT& composer, const ArgT& arg, NameT name, bool addListSeparator)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeString( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedString( composer, name, arg );
		if ( addListSeparator )
			composer.buff.append( ",", 1 );
	}
}

template<typename ComposerT, class NameT>
void composePublishableStructBegin(ComposerT& composer, NameT name )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::addNamePart( composer, name );
		composer.buff.append( "{", 1 );
	}
}

template<typename ComposerT>
void composePublishableStructEnd(ComposerT& composer, bool addListSeparator )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "}", 1 );
		if ( addListSeparator )
			composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename NameT>
void parsePublishableStructBegin(ParserT& p, NameT expectedName )
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
		if ( p.isDelimiter( '{' ) )
			p.skipDelimiter( '{' );
		else
			throw std::exception(); // TODO: bad format
	}
}

template<typename ParserT>
void parsePublishableStructEnd(ParserT& p )
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '}' ) )
			p.skipDelimiter( '}' );
		else
			throw std::exception(); // TODO: bad format
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

template<typename ParserT, typename ArgT, typename NameT>
void publishableParseString(ParserT& p, ArgT* arg, NameT expectedName)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseString( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != expectedName )
			throw std::exception(); // bad format
		p.readStringFromJson( arg );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

template<typename ComposerT>
void composeStructBegin(ComposerT& composer )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "{", 1 );
	}
}

template<typename ComposerT>
void composeStructEnd(ComposerT& composer )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "}", 1 );
	}
}

template<typename ParserT>
void parseStructBegin(ParserT& p )
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		p.skipDelimiter( '{' );
	}
}

template<typename ParserT>
void parseStructEnd(ParserT& p )
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		p.skipDelimiter( '}' );
	}
}


template<typename ComposerT>
void composeStateUpdateMessageBegin(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "{\"changes\":[", sizeof("{\"changes\":[")-1 );
	}
}

template<typename ParserT>
void parseStateUpdateMessageBegin(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '{' ) )
			p.skipDelimiter( '{' );
		else
			throw std::exception(); // bad format
		std::string key;
		p.readKey( &key );
		if ( key != "changes" )
			throw std::exception(); // bad format
		if ( p.isDelimiter( '[' ) )
			p.skipDelimiter( '[' );
		else
			throw std::exception(); // bad format
	}
}

template<typename ComposerT>
void composeStateUpdateMessageEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
	{
		composeSignedInteger( composer, 0 );
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "{}]}", 4 );
	}
}

template<typename ParserT>
void parseStateUpdateMessageEnd(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( ']' ) )
			p.skipDelimiter( ']' );
		else
			throw std::exception(); // bad format
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '}' ) )
			p.skipDelimiter( '}' );
		else
			throw std::exception(); // bad format
	}
}

template<typename ComposerT>
void composeStateUpdateBlockEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "},", 2 );
	}
}

template<typename ParserT>
void parseStateUpdateBlockEnd(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '}' ) )
			p.skipDelimiter( '}' );
		else
			throw std::exception(); // bad format
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
		else
			throw std::exception(); // bad format
	}
}

#if 0
template<typename ComposerT>
void composeStateUpdateBlockBegin(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "{", 1 );
	}
}

template<typename ParserT>
void parseStateUpdateBlockBegin(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '{' ) )
			p.skipDelimiter( '{' );
		else
			throw std::exception(); // bad format
	}
}

template<typename ComposerT>
void composeStateUpdateBlockEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "},", 2 );
	}
}

template<typename ParserT>
void parseStateUpdateBlockEnd(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}
#endif

template<typename ComposerT, typename ArgT>
void publishableComposeLeafeInteger(ComposerT& composer, ArgT arg)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeSignedInteger( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedSignedInteger( composer, "value", arg );
		composer.buff.append( "}", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT>
void publishableParseLeafeInteger(ParserT& p, ArgT* arg)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseSignedInteger( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
		p.readSignedIntegerFromJson( arg );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT>
void publishableComposeLeafeUnsignedInteger(ComposerT& composer, ArgT arg)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeUnsignedInteger( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedUnsignedInteger( composer, "value", arg );
		composer.buff.append( "}", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT>
void publishableParseLeafeUnsignedInteger(ParserT& p, ArgT* arg)
{
	static_assert( std::is_integral<ArgT>::value || std::is_integral<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseUnsignedInteger( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
		p.readUnsignedIntegerFromJson( arg );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT>
void publishableComposeLeafeReal(ComposerT& composer, ArgT arg)
{
	static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_reference<ArgT>::type>::value );
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeReal( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedReal( composer, "value", arg );
		composer.buff.append( "}", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT>
void publishableParseLeafeReal(ParserT& p, ArgT* arg)
{
	static_assert( std::is_arithmetic<ArgT>::value || std::is_arithmetic<typename std::remove_pointer<ArgT>::type>::value );
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseReal( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
		p.readRealFromJson( arg );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}

template<typename ComposerT, typename ArgT>
void publishableComposeLeafeString(ComposerT& composer, const ArgT& arg)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		composeString( composer, arg );
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::composeNamedString( composer, "value", arg );
		composer.buff.append( "}", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT, typename ArgT>
void publishableParseLeafeString(ParserT& p, ArgT* arg)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		p.parseString( arg );
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
		p.readStringFromJson( arg );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}

template<typename ComposerT>
void publishableComposeLeafeStructBegin(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::addNamePart( composer, "value" );
		composer.buff.append( "{", 1 );
	}
}

template<typename ComposerT>
void publishableComposeLeafeValueBegin(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::addNamePart( composer, "value" );
	}
}

template<typename ParserT>
void publishableParseLeafeValueBegin(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
	}
}

template<typename ComposerT>
void publishableComposeLeafeStructEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "}", 1 );
		composer.buff.append( "}", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT>
void publishableParseLeafeStructBegin(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
		p.skipDelimiter( '{' );
	}
}

template<typename ParserT>
void publishableParseLeafeStructEnd(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		p.skipDelimiter( '}' );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}

template<typename ComposerT>
void publishableComposeLeafeVectorBegin(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		json::addNamePart( composer, "value" );
		composer.buff.append( "[", 1 );
	}
}

template<typename ComposerT>
void publishableComposeLeafeVectorEnd(ComposerT& composer)
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ComposerT::proto == Proto::JSON, "unexpected protocol id" );
		composer.buff.append( "]", 1 );
		composer.buff.append( ",", 1 );
	}
}

template<typename ParserT>
void publishableParseLeafeVectorBegin(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		std::string key;
		p.readKey( &key );
		if ( key != "value" )
			throw std::exception(); // bad format
//		p.skipDelimiter( '[' );
	}
}

template<typename ParserT>
void publishableParseLeafeVectorEnd(ParserT& p)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
		; // do nothing
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
//		p.skipDelimiter( ']' );
		p.skipDelimiter( '}' );
		p.skipDelimiter( ',' );
	}
}


template<typename ComposerT>
void composeAddressInPublishable( ComposerT& composer, const GMQ_COLL vector<size_t>& addr, size_t last )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
	{
		size_t collSz = addr.size();
		composeUnsignedInteger( composer, collSz + 1 );
		for ( size_t i=0; i<collSz; ++i )
			composeUnsignedInteger( composer, addr[i] );
		composeUnsignedInteger( composer, last );
	}
	else
	{
		composer.buff.append( "{", 1 );
		json::addNamePart( composer, "addr" );
		composer.buff.appendUint8( '[' );
		size_t collSz = addr.size();
		for ( size_t i=0; i<collSz; ++i )
		{
			json::composeUnsignedInteger( composer, addr[i] );
			composer.buff.append( ", ", 2 );
		}
		json::composeUnsignedInteger( composer, last );
		composer.buff.appendUint8( ']' );
		composer.buff.appendUint8( ',' );
	}
}

template<typename ParserT, typename ArgT>
bool parseAddressInPublishable(ParserT& p, GMQ_COLL vector<size_t>& addr)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
	{
		size_t cnt;
		p.parseUnsignedInteger( &cnt );
		if ( cnt == 0 )
			return false;
		size_t tmp;
		for ( size_t i=0; i<cnt; ++i )
		{
			p.parseUnsignedInteger( &tmp );
			addr.push_back( tmp );
		}
		return true;
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		if ( p.isDelimiter( '{' ) )
		{
			p.skipDelimiter( '{' );
			if ( p.isDelimiter( '}' ) )
			{
				p.skipDelimiter( '}' );
				p.skipDelimiter( ']' );
				p.skipDelimiter( '}' );
				return false;
			}
		}
		else if ( p.isDelimiter( ']' ) )
		{
			p.skipDelimiter( ']' );
			p.skipDelimiter( '}' );
			return false;
		}
		
		std::string key;
		p.readKey( &key );
		if ( key != "addr" )
			throw std::exception(); // bad format
		p.skipDelimiter( '[' );
		if ( !p.isDelimiter( ']' ) ) // there are some items there
		{
			size_t tmp;
			for ( ;; )
			{
				p.readUnsignedIntegerFromJson( &tmp );
				addr.push_back( tmp );
				if ( p.isDelimiter( ',' ) )
				{
					p.skipDelimiter( ',' );
					continue;
				}
				if ( p.isDelimiter( ']' ) )
				{
					p.skipDelimiter( ']' );
					break;
				}
			}
		}
		else
			p.skipDelimiter( ']' );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
		else
			throw std::exception(); // bad format
		return true;
	}
}

template<typename ComposerT>
void composeActionInPublishable( ComposerT& composer, size_t action, bool noData )
{
	if constexpr ( ComposerT::proto == Proto::GMQ )
	{
		composeUnsignedInteger( composer, action );
	}
	else
	{
		json::addNamePart( composer, "action" );
		json::composeUnsignedInteger( composer, action );
		if ( noData )
		{
			composer.buff.append( "}", 1 );
			composer.buff.append( ",", 1 );
		}
		else
			composer.buff.appendUint8( ',' );
	}
}

template<typename ParserT>
void parseActionInPublishable(ParserT& p, size_t& action)
{
	if constexpr ( ParserT::proto == Proto::GMQ )
	{
		p.parseUnsignedInteger( &action );
	}
	else
	{
		static_assert( ParserT::proto == Proto::JSON, "unexpected protocol id" );
		
		std::string key;
		p.readKey( &key );
		if ( key != "action" )
			throw std::exception(); // bad format
		p.readUnsignedIntegerFromJson( &action );
		if ( p.isDelimiter( ',' ) )
			p.skipDelimiter( ',' );
	}
}

} // namespace impl

} // namespace globalmq::marshalling

#endif // NAMED_PARAMS_CORE_H
