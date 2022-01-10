#ifndef _gen_message_one_h_6c18e345_guard
#define _gen_message_one_h_6c18e345_guard

#include <marshalling.h>
#include <publishable_impl.h>
using namespace globalmq::marshalling;
namespace mtest {

#ifdef METASCOPE_mtest_ALREADY_DEFINED
#error metascope must reside in a single idl file
#endif
#define METASCOPE_mtest_ALREADY_DEFINED

// Useful aliases:
//     (note: since clang apparently too often requires providing template arguments for aliased type ctors we use wrappers instead of type aliasing)
using Buffer = globalmq::marshalling::Buffer;
using FileReadBuffer = globalmq::marshalling::FileReadBuffer;
template<class BufferT>
class GmqComposer : public globalmq::marshalling::GmqComposer<BufferT> { public: GmqComposer( BufferT& buff_ ) : globalmq::marshalling::GmqComposer<BufferT>( buff_ ) {} };
template<class BufferT>
class GmqParser : public globalmq::marshalling::GmqParser<BufferT> { public: /*GmqParser( BufferT& buff_ ) : globalmq::marshalling::GmqParser<BufferT>( buff_ ) {}*/ GmqParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::GmqParser<BufferT>( iter ) {} GmqParser( const GmqParser<BufferT>& other ) : globalmq::marshalling::GmqParser<BufferT>( other ) {} GmqParser& operator = ( const GmqParser<BufferT>& other ) { globalmq::marshalling::GmqParser<BufferT>::operator = ( other ); return *this; }};
template<class BufferT>
class JsonComposer : public globalmq::marshalling::JsonComposer<BufferT> { public: JsonComposer( BufferT& buff_ ) : globalmq::marshalling::JsonComposer<BufferT>( buff_ ) {} };
template<class BufferT>
class JsonParser : public globalmq::marshalling::JsonParser<BufferT> { public: /*JsonParser( BufferT& buff_ ) : globalmq::marshalling::JsonParser<BufferT>( buff_ ) {}*/ JsonParser( typename BufferT::ReadIteratorT& iter ) : globalmq::marshalling::JsonParser<BufferT>( iter ) {} JsonParser( const JsonParser<BufferT>& other ) : globalmq::marshalling::JsonParser<BufferT>( other ) {} JsonParser& operator = ( const JsonParser<BufferT>& other ) { globalmq::marshalling::JsonParser<BufferT>::operator = ( other ); return *this; } };
template<class T>
class SimpleTypeCollectionWrapper : public globalmq::marshalling::SimpleTypeCollectionWrapper<T> { public: SimpleTypeCollectionWrapper( T& coll ) : globalmq::marshalling::SimpleTypeCollectionWrapper<T>( coll ) {} };
template<class LambdaSize, class LambdaNext>
class CollectionWrapperForComposing : public globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext> { public: CollectionWrapperForComposing(LambdaSize &&lsize, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForComposing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsize), std::forward<LambdaNext>(lnext)) {} };
template<class LambdaCompose>
class MessageWrapperForComposing : public globalmq::marshalling::MessageWrapperForComposing<LambdaCompose> { public: MessageWrapperForComposing(LambdaCompose &&lcompose) : globalmq::marshalling::MessageWrapperForComposing<LambdaCompose>( std::forward<LambdaCompose>(lcompose) ) {} };
template<class LambdaSize, class LambdaNext>
class CollectionWrapperForParsing : public globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext> { public: CollectionWrapperForParsing(LambdaSize &&lsizeHint, LambdaNext &&lnext) : globalmq::marshalling::CollectionWrapperForParsing<LambdaSize, LambdaNext>(std::forward<LambdaSize>(lsizeHint), std::forward<LambdaNext>(lnext)) {} };
template<typename msgID_, class LambdaHandler>
MessageHandler<msgID_, LambdaHandler> makeMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeMessageHandler<msgID_, LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }
template<class LambdaHandler>
DefaultMessageHandler<LambdaHandler> makeDefaultMessageHandler( LambdaHandler &&lhandler ) { return globalmq::marshalling::makeDefaultMessageHandler<LambdaHandler>(std::forward<LambdaHandler>(lhandler)); }

//////////////////////////////////////////////////////////////
//
//  Scopes:
//
//  test_gmq
//  {
//    point3D_alias
//    message_one_gmq
//  }
//
//  test_json
//  {
//    point_alias
//    message_one_json
//  }
//
//////////////////////////////////////////////////////////////

using eighthParam_Type = NamedParameter<struct eighthParam_Struct>;
using fifthParam_Type = NamedParameter<struct fifthParam_Struct>;
using firstParam_Type = NamedParameter<struct firstParam_Struct>;
using forthParam_Type = NamedParameter<struct forthParam_Struct>;
using ninethParam_Type = NamedParameter<struct ninethParam_Struct>;
using secondParam_Type = NamedParameter<struct secondParam_Struct>;
using seventhParam_Type = NamedParameter<struct seventhParam_Struct>;
using sixthParam_Type = NamedParameter<struct sixthParam_Struct>;
using tenthParam_Type = NamedParameter<struct tenthParam_Struct>;
using thirdParam_Type = NamedParameter<struct thirdParam_Struct>;
using x_Type = NamedParameter<struct x_Struct>;
using y_Type = NamedParameter<struct y_Struct>;
using z_Type = NamedParameter<struct z_Struct>;

constexpr eighthParam_Type::TypeConverter eighthParam;
constexpr fifthParam_Type::TypeConverter fifthParam;
constexpr firstParam_Type::TypeConverter firstParam;
constexpr forthParam_Type::TypeConverter forthParam;
constexpr ninethParam_Type::TypeConverter ninethParam;
constexpr secondParam_Type::TypeConverter secondParam;
constexpr seventhParam_Type::TypeConverter seventhParam;
constexpr sixthParam_Type::TypeConverter sixthParam;
constexpr tenthParam_Type::TypeConverter tenthParam;
constexpr thirdParam_Type::TypeConverter thirdParam;
constexpr x_Type::TypeConverter x;
constexpr y_Type::TypeConverter y;
constexpr z_Type::TypeConverter z;


// member name presence checkers


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct point;
struct point3D;
struct message_one;

struct point3D
{
	int64_t x;
	int64_t y;
	int64_t z;
};

struct point
{
	int64_t x;
	int64_t y;
};

struct message_one
{
	int64_t firstParam;
	GMQ_COLL vector<int64_t> secondParam;
	GMQ_COLL vector<point3D> thirdParam;
	uint64_t forthParam;
	GMQ_COLL string fifthParam;
	GMQ_COLL vector<point> sixthParam;
	double seventhParam;
	point eighthParam;
	point3D ninethParam;
	GMQ_COLL vector<double> tenthParam;
};

namespace test_gmq {
struct MESSAGE_point3D_alias : public point3D {};
} // namespace test_gmq

namespace test_json {
struct MESSAGE_point_alias : public point {};
} // namespace test_json

namespace test_gmq {
struct MESSAGE_message_one_gmq : public message_one {};
} // namespace test_gmq

namespace test_json {
struct MESSAGE_message_one_json : public message_one {};
} // namespace test_json


} // namespace structures

//===============================================================================


struct publishable_STRUCT_point3D : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::z)>( parser, &(t.z), "z" );

	}

};

struct publishable_STRUCT_point : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );

	}

};

struct publishable_STRUCT_message_one : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::firstParam)>( parser, &(t.firstParam), "firstParam" );

		::globalmq::marshalling::impl::parseKey( parser, "secondParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::secondParam), ::globalmq::marshalling::impl::SignedIntegralType, true>( parser, t.secondParam );

		::globalmq::marshalling::impl::parseKey( parser, "thirdParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::thirdParam), publishable_STRUCT_point3D, true>( parser, t.thirdParam );

		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::forthParam)>( parser, &(t.forthParam), "forthParam" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::fifthParam)>( parser, &(t.fifthParam), "fifthParam" );

		::globalmq::marshalling::impl::parseKey( parser, "sixthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::sixthParam), publishable_STRUCT_point, true>( parser, t.sixthParam );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::seventhParam)>( parser, &(t.seventhParam), "seventhParam" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "eighthParam" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.eighthParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "ninethParam" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.ninethParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "tenthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::tenthParam), ::globalmq::marshalling::impl::RealType, true>( parser, t.tenthParam );

	}

};

namespace test_gmq {

using point3D_alias = ::globalmq::marshalling::impl::MessageName<1>;
using message_one_gmq = ::globalmq::marshalling::impl::MessageName<1>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::GMQ, "According to IDL GMQ parser is expected" );
	parser.parseUnsignedInteger( &msgID );
	bool ok = false;

	switch ( msgID )
	{
		case point3D_alias::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point3D_alias>( parser, handlers... ); break;
		case message_one_gmq::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_one_gmq>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

}

template<class BufferT, class ... HandlersT >
void handleMessage( BufferT& buffer, HandlersT ... handlers )
{
	auto riter = buffer.getReadIter();
	GmqParser<BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<class ReadIteratorT, class ... HandlersT >
void handleMessage2( ReadIteratorT& riter, HandlersT ... handlers )
{
	GmqParser<typename ReadIteratorT::BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args );

//**********************************************************************
// MESSAGE "point3D_alias" Targets: GMQ (Alias of point3D)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_point3D_alias_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_point3D_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_point3D_alias_parse(ParserT& p, Args&& ... args)
{
	STRUCT_point3D_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::point3D STRUCT_point3D_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::point3D;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::z)>( parser, &(t.z), "z" );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<class ParserT>
structures::test_gmq::MESSAGE_point3D_alias MESSAGE_point3D_alias_parse(ParserT& p)
{
	return static_cast<structures::test_gmq::MESSAGE_point3D_alias>(STRUCT_point3D_parse(p));
}

//**********************************************************************
// MESSAGE "message_one_gmq" Targets: GMQ (Alias of message_one)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_one_gmq_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_message_one_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_message_one_gmq_parse(ParserT& p, Args&& ... args)
{
	STRUCT_message_one_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::message_one STRUCT_message_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::message_one;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::firstParam)>( parser, &(t.firstParam), "firstParam" );

		::globalmq::marshalling::impl::parseKey( parser, "secondParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::secondParam), ::globalmq::marshalling::impl::SignedIntegralType, true>( parser, t.secondParam );

		::globalmq::marshalling::impl::parseKey( parser, "thirdParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::thirdParam), publishable_STRUCT_point3D, true>( parser, t.thirdParam );

		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::forthParam)>( parser, &(t.forthParam), "forthParam" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::fifthParam)>( parser, &(t.fifthParam), "fifthParam" );

		::globalmq::marshalling::impl::parseKey( parser, "sixthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::sixthParam), publishable_STRUCT_point, true>( parser, t.sixthParam );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::seventhParam)>( parser, &(t.seventhParam), "seventhParam" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "eighthParam" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.eighthParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "ninethParam" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.ninethParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "tenthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::tenthParam), ::globalmq::marshalling::impl::RealType, true>( parser, t.tenthParam );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<class ParserT>
structures::test_gmq::MESSAGE_message_one_gmq MESSAGE_message_one_gmq_parse(ParserT& p)
{
	return static_cast<structures::test_gmq::MESSAGE_message_one_gmq>(STRUCT_message_one_parse(p));
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::GmqComposer composer( buffer );
	::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );
	if constexpr ( msgID::id == point3D_alias::id )
		MESSAGE_point3D_alias_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == message_one_gmq::id )
		MESSAGE_message_one_gmq_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
}

} // namespace test_gmq 

namespace test_json {

using point_alias = ::globalmq::marshalling::impl::MessageName<2>;
using message_one_json = ::globalmq::marshalling::impl::MessageName<2>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::JSON, "According to IDL JSON parser is expected" );
	parser.skipDelimiter('{');
	std::string key;
	parser.readKey(&key);
	if (key != "msgid")
		throw std::exception(); // bad format
	parser.readUnsignedIntegerFromJson(&msgID);
	parser.skipSpacesEtc();
	if (!parser.isDelimiter(','))
		throw std::exception(); // bad format
	parser.skipDelimiter(',');
	parser.readKey(&key);
	if (key != "msgbody")
		throw std::exception(); // bad format
	JsonParser p( parser );

	bool ok = false;

	switch ( msgID )
	{
		case point_alias::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point_alias>( parser, handlers... ); break;
		case message_one_json::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_one_json>( parser, handlers... ); break;
		default: ::globalmq::marshalling::impl::implHandleMessage<::globalmq::marshalling::impl::UnknownMessageName>( parser, handlers... ); break;
	}

	/*if (!ok) return;
	if (!parser.isDelimiter('}'))
		throw std::exception(); // bad format
	parser.skipDelimiter('}');*/
}

template<class BufferT, class ... HandlersT >
void handleMessage( BufferT& buffer, HandlersT ... handlers )
{
	auto riter = buffer.getReadIter();
	JsonParser<BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<class ReadIteratorT, class ... HandlersT >
void handleMessage2( ReadIteratorT& riter, HandlersT ... handlers )
{
	JsonParser<typename ReadIteratorT::BufferT> parser( riter );
	implHandleMessage( parser, handlers... );
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args );

//**********************************************************************
// MESSAGE "point_alias" Targets: JSON (Alias of point)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_point_alias_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_point_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_point_alias_parse(ParserT& p, Args&& ... args)
{
	STRUCT_point_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::point STRUCT_point_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::point;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<class ParserT>
structures::test_json::MESSAGE_point_alias MESSAGE_point_alias_parse(ParserT& p)
{
	return static_cast<structures::test_json::MESSAGE_point_alias>(STRUCT_point_parse(p));
}

//**********************************************************************
// MESSAGE "message_one_json" Targets: JSON (Alias of message_one)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_one_json_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_message_one_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_message_one_json_parse(ParserT& p, Args&& ... args)
{
	STRUCT_message_one_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::message_one STRUCT_message_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::message_one;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::firstParam)>( parser, &(t.firstParam), "firstParam" );

		::globalmq::marshalling::impl::parseKey( parser, "secondParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::secondParam), ::globalmq::marshalling::impl::SignedIntegralType, true>( parser, t.secondParam );

		::globalmq::marshalling::impl::parseKey( parser, "thirdParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::thirdParam), publishable_STRUCT_point3D, true>( parser, t.thirdParam );

		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::forthParam)>( parser, &(t.forthParam), "forthParam" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::fifthParam)>( parser, &(t.fifthParam), "fifthParam" );

		::globalmq::marshalling::impl::parseKey( parser, "sixthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::sixthParam), publishable_STRUCT_point, true>( parser, t.sixthParam );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::seventhParam)>( parser, &(t.seventhParam), "seventhParam" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "eighthParam" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.eighthParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "ninethParam" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.ninethParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "tenthParam" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::tenthParam), ::globalmq::marshalling::impl::RealType, true>( parser, t.tenthParam );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<class ParserT>
structures::test_json::MESSAGE_message_one_json MESSAGE_message_one_json_parse(ParserT& p)
{
	return static_cast<structures::test_json::MESSAGE_message_one_json>(STRUCT_message_one_parse(p));
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::JsonComposer composer( buffer );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeNamedSignedInteger( composer, "msgid", msgID::id);
	composer.buff.append( ",\n  ", sizeof(",\n  ") - 1 );
	::globalmq::marshalling::impl::json::addNamePart( composer, "msgbody" );
	if constexpr ( msgID::id == point_alias::id )
		MESSAGE_point_alias_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == message_one_json::id )
		MESSAGE_message_one_json_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace test_json 

//**********************************************************************
// STRUCT "point" NONEXTENDABLE Targets: JSON GMQ (2 parameters)
//  1. INTEGER x (REQUIRED)
//  2. INTEGER y (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_point_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, y_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "x", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "y", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );

	}
}

//**********************************************************************
// STRUCT "point3D" Targets: JSON GMQ (3 parameters)
//  1. INTEGER x (REQUIRED)
//  2. INTEGER y (REQUIRED)
//  3. INTEGER z (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_point3D_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, x_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, y_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, z_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "x", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "y", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "z", arg_3_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );

	}
}

//**********************************************************************
// STRUCT "message_one" Targets: JSON GMQ (10 parameters)
//  1. INTEGER firstParam (REQUIRED)
//  2. VECTOR<INTEGER> secondParam (REQUIRED)
//  3. VECTOR< STRUCT point3D> thirdParam (REQUIRED)
//  4. UINTEGER forthParam (REQUIRED)
//  5. CHARACTER_STRING fifthParam (REQUIRED)
//  6. VECTOR<NONEXTENDABLE STRUCT point> sixthParam (REQUIRED)
//  7. REAL seventhParam (REQUIRED)
//  8. STRUCT NONEXTENDABLE point eighthParam (REQUIRED)
//  9. STRUCT point3D ninethParam (REQUIRED)
//  10. VECTOR<REAL> tenthParam (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_message_one_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, firstParam_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::SignedIntegralType>, secondParam_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, thirdParam_Type::Name>;
	using arg_4_type = NamedParameterWithType<::globalmq::marshalling::impl::UnsignedIntegralType, forthParam_Type::Name>;
	using arg_5_type = NamedParameterWithType<::globalmq::marshalling::impl::StringType, fifthParam_Type::Name>;
	using arg_6_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfNonextMessageTypes, sixthParam_Type::Name>;
	using arg_7_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, seventhParam_Type::Name>;
	using arg_8_type = NamedParameterWithType<::globalmq::marshalling::impl::NonextMessageType, eighthParam_Type::Name>;
	using arg_9_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, ninethParam_Type::Name>;
	using arg_10_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::RealType>, tenthParam_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_7_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_8_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_9_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_10_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	if constexpr( ComposerT::proto == Proto::GMQ )
	{
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)0>(composer, arg_5_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_6_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_6_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_7_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_7_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_8_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_8_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_9_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_9_type::nameAndTypeID, args...);
		::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_10_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_10_type::nameAndTypeID, args...);
	}
	else
	{
		static_assert( ComposerT::proto == Proto::JSON );
		composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "firstParam", arg_1_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "secondParam", arg_2_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "thirdParam", arg_3_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "forthParam", arg_4_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, "fifthParam", arg_5_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_6_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "sixthParam", arg_6_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_7_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "seventhParam", arg_7_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_8_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "eighthParam", arg_8_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_9_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ninethParam", arg_9_type::nameAndTypeID, args...);
		composer.buff.append( ",\n  ", 4 );
		::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_10_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "tenthParam", arg_10_type::nameAndTypeID, args...);
		composer.buff.append( "\n}", 2 );

	}
}


} // namespace mtest

#endif // _gen_message_one_h_6c18e345_guard
