#ifndef test_marshalling_h_71a7abdf_guard
#define test_marshalling_h_71a7abdf_guard

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
//    message_two
//    message_four
//    message_du
//  }
//
//  test_json
//  {
//    message_three
//    message_five
//  }
//
//////////////////////////////////////////////////////////////

using D1_Type = NamedParameter<struct D1_Struct>;
using D2_Type = NamedParameter<struct D2_Struct>;
using D3_Type = NamedParameter<struct D3_Struct>;
using Data_Type = NamedParameter<struct Data_Struct>;
using disc_union_Type = NamedParameter<struct disc_union_Struct>;
using eighthParam_Type = NamedParameter<struct eighthParam_Struct>;
using fifthParam_Type = NamedParameter<struct fifthParam_Struct>;
using firstParam_Type = NamedParameter<struct firstParam_Struct>;
using forthParam_Type = NamedParameter<struct forthParam_Struct>;
using ninethParam_Type = NamedParameter<struct ninethParam_Struct>;
using pt_Type = NamedParameter<struct pt_Struct>;
using pts3d_Type = NamedParameter<struct pts3d_Struct>;
using secondParam_Type = NamedParameter<struct secondParam_Struct>;
using seventhParam_Type = NamedParameter<struct seventhParam_Struct>;
using sixthParam_Type = NamedParameter<struct sixthParam_Struct>;
using tenthParam_Type = NamedParameter<struct tenthParam_Struct>;
using thirdParam_Type = NamedParameter<struct thirdParam_Struct>;
using x_Type = NamedParameter<struct x_Struct>;
using y_Type = NamedParameter<struct y_Struct>;
using z_Type = NamedParameter<struct z_Struct>;

constexpr D1_Type::TypeConverter D1;
constexpr D2_Type::TypeConverter D2;
constexpr D3_Type::TypeConverter D3;
constexpr Data_Type::TypeConverter Data;
constexpr disc_union_Type::TypeConverter disc_union;
constexpr eighthParam_Type::TypeConverter eighthParam;
constexpr fifthParam_Type::TypeConverter fifthParam;
constexpr firstParam_Type::TypeConverter firstParam;
constexpr forthParam_Type::TypeConverter forthParam;
constexpr ninethParam_Type::TypeConverter ninethParam;
constexpr pt_Type::TypeConverter pt;
constexpr pts3d_Type::TypeConverter pts3d;
constexpr secondParam_Type::TypeConverter secondParam;
constexpr seventhParam_Type::TypeConverter seventhParam;
constexpr sixthParam_Type::TypeConverter sixthParam;
constexpr tenthParam_Type::TypeConverter tenthParam;
constexpr thirdParam_Type::TypeConverter thirdParam;
constexpr x_Type::TypeConverter x;
constexpr y_Type::TypeConverter y;
constexpr z_Type::TypeConverter z;


// member name presence checkers
template<typename T> concept has_aReal_member = requires { { T::aReal }; };
template<typename T> concept has_aString_member = requires { { T::aString }; };
template<typename T> concept has_aggregate_member = requires { { T::aggregate }; };
template<typename T> concept has_anInt_member = requires { { T::anInt }; };
template<typename T> concept has_anUInt_member = requires { { T::anUInt }; };
template<typename T> concept has_basic_member = requires { { T::basic }; };
template<typename T> concept has_lastValue_member = requires { { T::lastValue }; };
template<typename T> concept has_name_member = requires { { T::name }; };
template<typename T> concept has_theAggregate_member = requires { { T::theAggregate }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_aReal = requires(T t) { { t.notifyUpdated_aReal() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_aReal = requires { { std::declval<StateT>().notifyUpdated_aReal(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_aString = requires(T t) { { t.notifyUpdated_aString() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_aString = requires { { std::declval<StateT>().notifyUpdated_aString(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_aggregate = requires(T t) { { t.notifyUpdated_aggregate() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_aggregate = requires { { std::declval<StateT>().notifyUpdated_aggregate(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_anInt = requires(T t) { { t.notifyUpdated_anInt() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_anInt = requires { { std::declval<StateT>().notifyUpdated_anInt(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_anUInt = requires(T t) { { t.notifyUpdated_anUInt() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_anUInt = requires { { std::declval<StateT>().notifyUpdated_anUInt(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_basic = requires(T t) { { t.notifyUpdated_basic() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_basic = requires { { std::declval<StateT>().notifyUpdated_basic(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_lastValue = requires(T t) { { t.notifyUpdated_lastValue() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_lastValue = requires { { std::declval<StateT>().notifyUpdated_lastValue(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_name = requires(T t) { { t.notifyUpdated_name() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_name = requires { { std::declval<StateT>().notifyUpdated_name(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_theAggregate = requires(T t) { { t.notifyUpdated_theAggregate() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_theAggregate = requires { { std::declval<StateT>().notifyUpdated_theAggregate(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct point;
struct point3D;
struct struct_one;
class du_one;
struct struct_du;
struct BasicTypes;
struct AggregateType;

struct BasicTypes
{
	int64_t anInt;
	uint64_t anUInt;
	double aReal;
	GMQ_COLL string aString;
};

class du_one : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { one=1, two=2, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_one
	{
		double D1;
		double D2;
		double D3;
	};

	struct Case_two
	{
		GMQ_COLL vector<double> Data;
	};

public:
	using Case_one_D1_T = decltype( Case_one::D1 );
	using Case_one_D2_T = decltype( Case_one::D2 );
	using Case_one_D3_T = decltype( Case_one::D3 );
	using Case_two_Data_T = decltype( Case_two::Data );

private:
	static constexpr size_t one_two_memsz = sizeof( Case_two ) > ( sizeof( Case_one ) ) ? sizeof( Case_two ) : ( sizeof( Case_one ) );
	uint8_t one_two_mem[one_two_memsz];
	void implDeinit() {
		if ( v != Variants::unknown ) // then destruct existing value
		{
			switch ( v )
			{
				case Variants::one: reinterpret_cast<Case_one*>( one_two_mem ) -> ~Case_one(); break;
				case Variants::two: reinterpret_cast<Case_two*>( one_two_mem ) -> ~Case_two(); break;
			}
			v = Variants::unknown;
		}
	}

	void implCopyFrom( const du_one& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::one:
				new ( one_two_mem ) Case_one( *reinterpret_cast<const Case_one*>( other.one_two_mem ) );
				break;
			case Variants::two:
				new ( one_two_mem ) Case_two( *reinterpret_cast<const Case_two*>( other.one_two_mem ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
	}

	void implMoveFrom( du_one&& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::one:
				new ( one_two_mem ) Case_one( std::move( *reinterpret_cast<Case_one*>( other.one_two_mem ) ) );
				break;
			case Variants::two:
				new ( one_two_mem ) Case_two( std::move( *reinterpret_cast<Case_two*>( other.one_two_mem ) ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
		other.v = Variants::unknown;
	}

public:
	du_one() {}
	du_one( const du_one &other ) {
		implCopyFrom( other );
	}
	du_one& operator = ( const du_one &other) {
		implCopyFrom( other );
		return *this;
	}
	du_one( du_one&& other) noexcept {
		implMoveFrom( std::move( other ) );
	}
	du_one& operator = ( du_one&& other) noexcept {
		implMoveFrom( std::move( other ) );
		return *this;
	}
	virtual ~du_one() {
		implDeinit();
	}
	Variants currentVariant() const { return v; }
	void initAs( Variants v_ ) {
		implDeinit();
		switch ( v_ ) // init for a new type
		{
			case Variants::one: new ( one_two_mem ) Case_one; break;
			case Variants::two: new ( one_two_mem ) Case_two; break;
		}
		v = v_;
	}

	// IDL CASE one:
	Case_one_D1_T& D1() {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<Case_one*>( one_two_mem )->D1;
	}
	const Case_one_D1_T& D1() const {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<const Case_one*>( one_two_mem )->D1;
	}
	
	Case_one_D2_T& D2() {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<Case_one*>( one_two_mem )->D2;
	}
	const Case_one_D2_T& D2() const {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<const Case_one*>( one_two_mem )->D2;
	}
	
	Case_one_D3_T& D3() {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<Case_one*>( one_two_mem )->D3;
	}
	const Case_one_D3_T& D3() const {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<const Case_one*>( one_two_mem )->D3;
	}
	

	// IDL CASE two:
	Case_two_Data_T& Data() {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<Case_two*>( one_two_mem )->Data;
	}
	const Case_two_Data_T& Data() const {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<const Case_two*>( one_two_mem )->Data;
	}
	
};

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

struct AggregateType
{
	GMQ_COLL string name;
	BasicTypes theAggregate;
	int64_t lastValue;
};

struct struct_du
{
	point3D pt;
	du_one disc_union;
};

struct struct_one
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
struct MESSAGE_message_two : public struct_one {};
} // namespace test_gmq

namespace test_json {
struct MESSAGE_message_three : public struct_one {};
} // namespace test_json

namespace test_gmq {
struct MESSAGE_message_four
{
	point pt;
	GMQ_COLL vector<point3D> pts3d;
};
} // namespace test_gmq

namespace test_json {
struct MESSAGE_message_five
{
	point pt;
	GMQ_COLL vector<point3D> pts3d;
};
} // namespace test_json

namespace test_gmq {
struct MESSAGE_message_du : public struct_du {};
} // namespace test_gmq

struct StructSix
{
	GMQ_COLL string name;
	BasicTypes basic;
	AggregateType aggregate;
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_BasicTypes;
template<class T> class BasicTypes_RefWrapper;
template<class T, class RootT> class BasicTypes_RefWrapper4Set;

struct publishable_STRUCT_AggregateType;
template<class T> class AggregateType_RefWrapper;
template<class T, class RootT> class AggregateType_RefWrapper4Set;


struct publishable_STRUCT_BasicTypes : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::anInt)>( parser, &(t.anInt), "anInt" );

		::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::anUInt)>( parser, &(t.anUInt), "anUInt" );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::aReal)>( parser, &(t.aReal), "aReal" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::aString)>( parser, &(t.aString), "aString" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.anInt, "anInt", true );

		::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, t.anUInt, "anUInt", true );

		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.aReal, "aReal", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.aString, "aString", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_anInt = has_void_update_notifier_call_for_anInt<T>;
		static constexpr bool has_update_notifier_for_anInt = has_update_notifier_call_for_anInt<T, decltype(T::anInt)>;
		static constexpr bool has_any_notifier_for_anInt = has_void_update_notifier_for_anInt || has_update_notifier_for_anInt;
		static constexpr bool has_void_update_notifier_for_anUInt = has_void_update_notifier_call_for_anUInt<T>;
		static constexpr bool has_update_notifier_for_anUInt = has_update_notifier_call_for_anUInt<T, decltype(T::anUInt)>;
		static constexpr bool has_any_notifier_for_anUInt = has_void_update_notifier_for_anUInt || has_update_notifier_for_anUInt;
		static constexpr bool has_void_update_notifier_for_aReal = has_void_update_notifier_call_for_aReal<T>;
		static constexpr bool has_update_notifier_for_aReal = has_update_notifier_call_for_aReal<T, decltype(T::aReal)>;
		static constexpr bool has_any_notifier_for_aReal = has_void_update_notifier_for_aReal || has_update_notifier_for_aReal;
		static constexpr bool has_void_update_notifier_for_aString = has_void_update_notifier_call_for_aString<T>;
		static constexpr bool has_update_notifier_for_aString = has_update_notifier_call_for_aString<T, decltype(T::aString)>;
		static constexpr bool has_any_notifier_for_aString = has_void_update_notifier_for_aString || has_update_notifier_for_aString;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_anInt || reportChanges )
		{
			decltype(T::anInt) oldVal = t.anInt;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::anInt)>( parser, &(t.anInt), "anInt" );
			bool currentChanged = oldVal != t.anInt;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_anInt )
					t.notifyUpdated_anInt();
				if constexpr ( has_update_notifier_for_anInt )
					t.notifyUpdated_anInt( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::anInt)>( parser, &(t.anInt), "anInt" );

		if constexpr( has_any_notifier_for_anUInt || reportChanges )
		{
			decltype(T::anUInt) oldVal = t.anUInt;
			::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::anUInt)>( parser, &(t.anUInt), "anUInt" );
			bool currentChanged = oldVal != t.anUInt;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_anUInt )
					t.notifyUpdated_anUInt();
				if constexpr ( has_update_notifier_for_anUInt )
					t.notifyUpdated_anUInt( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseUnsignedInteger<ParserT, decltype(T::anUInt)>( parser, &(t.anUInt), "anUInt" );

		if constexpr( has_any_notifier_for_aReal || reportChanges )
		{
			decltype(T::aReal) oldVal = t.aReal;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::aReal)>( parser, &(t.aReal), "aReal" );
			bool currentChanged = oldVal != t.aReal;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_aReal )
					t.notifyUpdated_aReal();
				if constexpr ( has_update_notifier_for_aReal )
					t.notifyUpdated_aReal( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::aReal)>( parser, &(t.aReal), "aReal" );

		if constexpr( has_any_notifier_for_aString || reportChanges )
		{
			decltype(T::aString) oldVal = t.aString;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::aString)>( parser, &(t.aString), "aString" );
			bool currentChanged = oldVal != t.aString;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_aString )
					t.notifyUpdated_aString();
				if constexpr ( has_update_notifier_for_aString )
					t.notifyUpdated_aString( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::aString)>( parser, &(t.aString), "aString" );


		if constexpr ( reportChanges )
			return changed;
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_anInt = has_void_update_notifier_call_for_anInt<T>;
		static constexpr bool has_update_notifier_for_anInt = has_update_notifier_call_for_anInt<T, decltype(T::anInt)>;
		static constexpr bool has_any_notifier_for_anInt = has_void_update_notifier_for_anInt || has_update_notifier_for_anInt;
		static constexpr bool has_void_update_notifier_for_anUInt = has_void_update_notifier_call_for_anUInt<T>;
		static constexpr bool has_update_notifier_for_anUInt = has_update_notifier_call_for_anUInt<T, decltype(T::anUInt)>;
		static constexpr bool has_any_notifier_for_anUInt = has_void_update_notifier_for_anUInt || has_update_notifier_for_anUInt;
		static constexpr bool has_void_update_notifier_for_aReal = has_void_update_notifier_call_for_aReal<T>;
		static constexpr bool has_update_notifier_for_aReal = has_update_notifier_call_for_aReal<T, decltype(T::aReal)>;
		static constexpr bool has_any_notifier_for_aReal = has_void_update_notifier_for_aReal || has_update_notifier_for_aReal;
		static constexpr bool has_void_update_notifier_for_aString = has_void_update_notifier_call_for_aString<T>;
		static constexpr bool has_update_notifier_for_aString = has_update_notifier_call_for_aString<T, decltype(T::aString)>;
		static constexpr bool has_any_notifier_for_aString = has_void_update_notifier_for_aString || has_update_notifier_for_aString;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_anInt || reportChanges )
				{
					decltype(T::anInt) oldVal = t.anInt;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::anInt)>( parser, &(t.anInt) );
					bool currentChanged = oldVal != t.anInt;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_anInt )
							t.notifyUpdated_anInt();
						if constexpr ( has_update_notifier_for_anInt )
							t.notifyUpdated_anInt( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::anInt)>( parser, &(t.anInt) );
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_anUInt || reportChanges )
				{
					decltype(T::anUInt) oldVal = t.anUInt;
					::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, decltype(T::anUInt)>( parser, &(t.anUInt) );
					bool currentChanged = oldVal != t.anUInt;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_anUInt )
							t.notifyUpdated_anUInt();
						if constexpr ( has_update_notifier_for_anUInt )
							t.notifyUpdated_anUInt( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, decltype(T::anUInt)>( parser, &(t.anUInt) );
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_aReal || reportChanges )
				{
					decltype(T::aReal) oldVal = t.aReal;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::aReal)>( parser, &(t.aReal) );
					bool currentChanged = oldVal != t.aReal;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_aReal )
							t.notifyUpdated_aReal();
						if constexpr ( has_update_notifier_for_aReal )
							t.notifyUpdated_aReal( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::aReal)>( parser, &(t.aReal) );
				break;
			}
			case 3:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_aString || reportChanges )
				{
					decltype(T::aString) oldVal = t.aString;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::aString)>( parser, &(t.aString) );
					bool currentChanged = oldVal != t.aString;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_aString )
							t.notifyUpdated_aString();
						if constexpr ( has_update_notifier_for_aString )
							t.notifyUpdated_aString( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::aString)>( parser, &(t.aString) );
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		dst.anInt = src.anInt;
		dst.anUInt = src.anUInt;
		dst.aReal = src.aReal;
		dst.aString = src.aString;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.anInt != s2.anInt ) return false;
		if ( s1.anUInt != s2.anUInt ) return false;
		if ( s1.aReal != s2.aReal ) return false;
		if ( s1.aString != s2.aString ) return false;
		return true;
	}
};

struct publishable_DISCRIMINATED_UNION_du_one : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		uint64_t caseId;
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, uint64_t>( parser, &(caseId), "caseid" );
		t.initAs( (typename T::Variants)(caseId) );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "caseData" );
			switch ( caseId )
			{
				case 1: // IDL CASE (null)
				{
					::globalmq::marshalling::impl::publishableParseReal<ParserT, typename T::Case_one_D1_T>( parser, &(t.D1()), "D1" );

					::globalmq::marshalling::impl::publishableParseReal<ParserT, typename T::Case_one_D2_T>( parser, &(t.D2()), "D2" );

					::globalmq::marshalling::impl::publishableParseReal<ParserT, typename T::Case_one_D3_T>( parser, &(t.D3()), "D3" );

				}
				case 2: // IDL CASE (null)
				{
					::globalmq::marshalling::impl::parseKey( parser, "Data" );
					PublishableVectorProcessor::parse<ParserT, typename T::Case_two_Data_T, ::globalmq::marshalling::impl::RealType, true>( parser, t.Data() );

				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}
	}

};

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

struct publishable_STRUCT_AggregateType : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "theAggregate" );
		publishable_STRUCT_BasicTypes::parseForStateSyncOrMessageInDepth( parser, t.theAggregate );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::lastValue)>( parser, &(t.lastValue), "lastValue" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "theAggregate" );
		publishable_STRUCT_BasicTypes::compose( composer, t.theAggregate );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.lastValue, "lastValue", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_theAggregate = has_void_update_notifier_call_for_theAggregate<T>;
		static constexpr bool has_update_notifier_for_theAggregate = has_update_notifier_call_for_theAggregate<T, decltype(T::theAggregate)>;
		static constexpr bool has_any_notifier_for_theAggregate = has_void_update_notifier_for_theAggregate || has_update_notifier_for_theAggregate;
		static constexpr bool has_void_update_notifier_for_lastValue = has_void_update_notifier_call_for_lastValue<T>;
		static constexpr bool has_update_notifier_for_lastValue = has_update_notifier_call_for_lastValue<T, decltype(T::lastValue)>;
		static constexpr bool has_any_notifier_for_lastValue = has_void_update_notifier_for_lastValue || has_update_notifier_for_lastValue;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_name || reportChanges )
		{
			decltype(T::name) oldVal = t.name;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );
			bool currentChanged = oldVal != t.name;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_name )
					t.notifyUpdated_name();
				if constexpr ( has_update_notifier_for_name )
					t.notifyUpdated_name( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "theAggregate" );
		if constexpr( has_update_notifier_for_theAggregate )
		{
			decltype(T::theAggregate) temp_theAggregate;
			publishable_STRUCT_BasicTypes::copy<decltype(T::theAggregate)>( t.theAggregate, temp_theAggregate );
			bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr( has_void_update_notifier_for_theAggregate )
					t.notifyUpdated_theAggregate();
				t.notifyUpdated_theAggregate( temp_theAggregate );
			}
		}
		else if constexpr( has_void_update_notifier_for_theAggregate )
		{
			bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				t.notifyUpdated_theAggregate();
			}
		}
		else if constexpr ( reportChanges )
			changed = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
		else
			publishable_STRUCT_BasicTypes::parse( parser, t.theAggregate );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		if constexpr( has_any_notifier_for_lastValue || reportChanges )
		{
			decltype(T::lastValue) oldVal = t.lastValue;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::lastValue)>( parser, &(t.lastValue), "lastValue" );
			bool currentChanged = oldVal != t.lastValue;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_lastValue )
					t.notifyUpdated_lastValue();
				if constexpr ( has_update_notifier_for_lastValue )
					t.notifyUpdated_lastValue( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::lastValue)>( parser, &(t.lastValue), "lastValue" );


		if constexpr ( reportChanges )
			return changed;
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t, GMQ_COLL vector<size_t>& addr, size_t offset )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
		static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
		static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
		static constexpr bool has_void_update_notifier_for_theAggregate = has_void_update_notifier_call_for_theAggregate<T>;
		static constexpr bool has_update_notifier_for_theAggregate = has_update_notifier_call_for_theAggregate<T, decltype(T::theAggregate)>;
		static constexpr bool has_any_notifier_for_theAggregate = has_void_update_notifier_for_theAggregate || has_update_notifier_for_theAggregate;
		static constexpr bool has_void_update_notifier_for_lastValue = has_void_update_notifier_call_for_lastValue<T>;
		static constexpr bool has_update_notifier_for_lastValue = has_update_notifier_call_for_lastValue<T, decltype(T::lastValue)>;
		static constexpr bool has_any_notifier_for_lastValue = has_void_update_notifier_for_lastValue || has_update_notifier_for_lastValue;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_name || reportChanges )
				{
					decltype(T::name) oldVal = t.name;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					bool currentChanged = oldVal != t.name;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_name )
							t.notifyUpdated_name();
						if constexpr ( has_update_notifier_for_name )
							t.notifyUpdated_name( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_theAggregate )
					{
						decltype(T::theAggregate) temp_theAggregate;
						publishable_STRUCT_BasicTypes::copy<decltype(T::theAggregate)>( t.theAggregate, temp_theAggregate );
						bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_theAggregate )
								t.notifyUpdated_theAggregate();
							t.notifyUpdated_theAggregate( temp_theAggregate );
						}
					}
					else if constexpr( has_void_update_notifier_for_theAggregate )
					{
						bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_theAggregate();
						}
					}

					else
					{
							if constexpr ( reportChanges )
								changed = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate );
							else
								publishable_STRUCT_BasicTypes::parse( parser, t.theAggregate );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_theAggregate )
					{
						decltype(T::theAggregate) temp_theAggregate;
						publishable_STRUCT_BasicTypes::copy<decltype(T::theAggregate)>( t.theAggregate, temp_theAggregate );
						bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_theAggregate )
								t.notifyUpdated_theAggregate();
							t.notifyUpdated_theAggregate( temp_theAggregate );
						}
					}
					else if constexpr( has_void_update_notifier_for_theAggregate )
					{
						bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_theAggregate();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::theAggregate), bool>( parser, t.theAggregate, addr, offset + 1 );
					else
						publishable_STRUCT_BasicTypes::parse( parser, t.theAggregate, addr, offset + 1 );
				}
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_lastValue || reportChanges )
				{
					decltype(T::lastValue) oldVal = t.lastValue;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::lastValue)>( parser, &(t.lastValue) );
					bool currentChanged = oldVal != t.lastValue;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_lastValue )
							t.notifyUpdated_lastValue();
						if constexpr ( has_update_notifier_for_lastValue )
							t.notifyUpdated_lastValue( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::lastValue)>( parser, &(t.lastValue) );
				break;
			}
			default:
				throw std::exception(); // unexpected
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		dst.name = src.name;
		publishable_STRUCT_BasicTypes::copy( src.theAggregate, dst.theAggregate );
		dst.lastValue = src.lastValue;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.name != s2.name ) return false;
		if( ! publishable_STRUCT_BasicTypes::isSame( s1.theAggregate, s2.theAggregate ) ) return false;
		if ( s1.lastValue != s2.lastValue ) return false;
		return true;
	}
};

struct publishable_STRUCT_struct_du : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.pt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "disc_union" );
		publishable_DISCRIMINATED_UNION_du_one::parseForStateSyncOrMessageInDepth( parser, t.disc_union );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

};

struct publishable_STRUCT_struct_one : public ::globalmq::marshalling::impl::StructType
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

using message_two = ::globalmq::marshalling::impl::MessageName<2>;
using message_four = ::globalmq::marshalling::impl::MessageName<4>;
using message_du = ::globalmq::marshalling::impl::MessageName<5>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::GMQ, "According to IDL GMQ parser is expected" );
	parser.parseUnsignedInteger( &msgID );
	bool ok = false;

	switch ( msgID )
	{
		case message_two::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_two>( parser, handlers... ); break;
		case message_four::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_four>( parser, handlers... ); break;
		case message_du::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_du>( parser, handlers... ); break;
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
// MESSAGE "message_two" Targets: GMQ (Alias of struct_one)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_two_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_struct_one_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_message_two_parse(ParserT& p, Args&& ... args)
{
	STRUCT_struct_one_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::struct_one STRUCT_struct_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::struct_one;
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
structures::test_gmq::MESSAGE_message_two MESSAGE_message_two_parse(ParserT& p)
{
	return static_cast<structures::test_gmq::MESSAGE_message_two>(STRUCT_struct_one_parse(p));
}

//**********************************************************************
// MESSAGE "message_four" NONEXTENDABLE Targets: GMQ (2 parameters)
//  1. STRUCT point pt (REQUIRED)
//  2. VECTOR< STRUCT point3D> pts3d (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_four_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, pt_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, pts3d_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::test_gmq::MESSAGE_message_four MESSAGE_message_four_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::test_gmq::MESSAGE_message_four;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.pt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "pts3d" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::pts3d), publishable_STRUCT_point3D, true>( parser, t.pts3d );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

//**********************************************************************
// MESSAGE "message_du" Targets: GMQ (Alias of struct_du)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_du_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_struct_du_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_message_du_parse(ParserT& p, Args&& ... args)
{
	STRUCT_struct_du_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::struct_du STRUCT_struct_du_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::struct_du;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.pt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "disc_union" );
		publishable_DISCRIMINATED_UNION_du_one::parseForStateSyncOrMessageInDepth( parser, t.disc_union );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<class ParserT>
structures::test_gmq::MESSAGE_message_du MESSAGE_message_du_parse(ParserT& p)
{
	return static_cast<structures::test_gmq::MESSAGE_message_du>(STRUCT_struct_du_parse(p));
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::GmqComposer composer( buffer );
	::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );
	if constexpr ( msgID::id == message_two::id )
		MESSAGE_message_two_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == message_four::id )
		MESSAGE_message_four_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == message_du::id )
		MESSAGE_message_du_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
}

} // namespace test_gmq 

namespace test_json {

using message_three = ::globalmq::marshalling::impl::MessageName<3>;
using message_five = ::globalmq::marshalling::impl::MessageName<5>;

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
		case message_three::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_three>( parser, handlers... ); break;
		case message_five::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_five>( parser, handlers... ); break;
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
// MESSAGE "message_three" Targets: JSON (Alias of struct_one)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_three_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_struct_one_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_message_three_parse(ParserT& p, Args&& ... args)
{
	STRUCT_struct_one_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::struct_one STRUCT_struct_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::struct_one;
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
structures::test_json::MESSAGE_message_three MESSAGE_message_three_parse(ParserT& p)
{
	return static_cast<structures::test_json::MESSAGE_message_three>(STRUCT_struct_one_parse(p));
}

//**********************************************************************
// MESSAGE "message_five" NONEXTENDABLE Targets: JSON (2 parameters)
//  1. STRUCT point pt (REQUIRED)
//  2. VECTOR< STRUCT point3D> pts3d (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_message_five_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, pt_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, pts3d_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this MESSAGE assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "pt", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "pts3d", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

template<class ParserT>
structures::test_json::MESSAGE_message_five MESSAGE_message_five_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::test_json::MESSAGE_message_five;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.pt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "pts3d" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::pts3d), publishable_STRUCT_point3D, true>( parser, t.pts3d );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
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
	if constexpr ( msgID::id == message_three::id )
		MESSAGE_message_three_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == message_five::id )
		MESSAGE_message_five_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace test_json 

//**********************************************************************
// PUBLISHABLE StructSix (3 parameters)
// 1. CHARACTER_STRING name
// 2. STRUCT BasicTypes basic
// 3. STRUCT AggregateType aggregate
//**********************************************************************

template<class T, class ComposerT>
class StructSix_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_basic = has_basic_member<T>;
	static_assert( has_basic, "type T must have member T::basic of a type corresponding to IDL type STRUCT BasicTypes" );
	static constexpr bool has_aggregate = has_aggregate_member<T>;
	static_assert( has_aggregate, "type T must have member T::aggregate of a type corresponding to IDL type STRUCT AggregateType" );


public:
	static constexpr uint64_t numTypeID = 6;
	static constexpr const char* stringTypeID = "StructSix";

	template<class ... ArgsT>
	StructSix_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( composer, t.name );
	}
	const auto& get_basic() { return t.basic; }
	void set_basic( decltype(T::basic) val) { 
		t.basic = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_BasicTypes::compose( composer, t.basic );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_basic() { return BasicTypes_RefWrapper4Set</*aaa*/decltype(T::basic), StructSix_WrapperForPublisher>(t.basic, *this, GMQ_COLL vector<size_t>(), 1); }
	const auto& get_aggregate() { return t.aggregate; }
	void set_aggregate( decltype(T::aggregate) val) { 
		t.aggregate = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_AggregateType::compose( composer, t.aggregate );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_aggregate() { return AggregateType_RefWrapper4Set</*aaa*/decltype(T::aggregate), StructSix_WrapperForPublisher>(t.aggregate, *this, GMQ_COLL vector<size_t>(), 2); }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "basic" );
		publishable_STRUCT_BasicTypes::compose( composer, t.basic );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "aggregate" );
		publishable_STRUCT_AggregateType::compose( composer, t.aggregate );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class StructSix_NodecppWrapperForPublisher : public StructSix_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	StructSix_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : StructSix_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~StructSix_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { StructSix_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  StructSix_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { StructSix_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return StructSix_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class StructSix_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_basic = has_basic_member<T>;
	static_assert( has_basic, "type T must have member T::basic of a type corresponding to IDL type STRUCT BasicTypes" );
	static constexpr bool has_aggregate = has_aggregate_member<T>;
	static_assert( has_aggregate, "type T must have member T::aggregate of a type corresponding to IDL type STRUCT AggregateType" );

	static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
	static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
	static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
	static constexpr bool has_void_update_notifier_for_basic = has_void_update_notifier_call_for_basic<T>;
	static constexpr bool has_update_notifier_for_basic = has_update_notifier_call_for_basic<T, decltype(T::basic)>;
	static constexpr bool has_any_notifier_for_basic = has_void_update_notifier_for_basic || has_update_notifier_for_basic;
	static constexpr bool has_void_update_notifier_for_aggregate = has_void_update_notifier_call_for_aggregate<T>;
	static constexpr bool has_update_notifier_for_aggregate = has_update_notifier_call_for_aggregate<T, decltype(T::aggregate)>;
	static constexpr bool has_any_notifier_for_aggregate = has_void_update_notifier_for_aggregate || has_update_notifier_for_aggregate;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 6;
	static constexpr const char* stringTypeID = "StructSix";

	template<class ... ArgsT>
	StructSix_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
	const T& getState() { return t; }
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			GMQ_ASSERT( addr.size() );
			switch ( addr[0] )
			{
				case 0:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					if constexpr( has_any_notifier_for_name )
					{
						decltype(T::name) oldVal = t.name;
						::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
						bool currentChanged = oldVal != t.name;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_name )
								t.notifyUpdated_name();
							if constexpr ( has_update_notifier_for_name )
								t.notifyUpdated_name( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					break;
				}
				case 1:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_basic )
						{
							decltype(T::basic) temp_basic;
							publishable_STRUCT_BasicTypes::copy<decltype(T::basic)>( t.basic, temp_basic );
							bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::basic), bool>( parser, t.basic );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_basic )
									t.notifyUpdated_basic();
								t.notifyUpdated_basic( temp_basic );
							}
						}
						else if constexpr( has_void_update_notifier_for_basic )
						{
							bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::basic), bool>( parser, t.basic );
							if ( changedCurrent )
							{
								t.notifyUpdated_basic();
							}
						}

						else
						{
							publishable_STRUCT_BasicTypes::parse( parser, t.basic );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_basic )
						{
							decltype(T::basic) temp_basic;
							publishable_STRUCT_BasicTypes::copy<decltype(T::basic)>( t.basic, temp_basic );
							bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::basic), bool>( parser, t.basic, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_basic )
									t.notifyUpdated_basic();
								t.notifyUpdated_basic( temp_basic );
							}
						}
						else if constexpr( has_void_update_notifier_for_basic )
						{
							bool changedCurrent = publishable_STRUCT_BasicTypes::parse<ParserT, decltype(T::basic), bool>( parser, t.basic, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_basic();
							}
						}
						else
							publishable_STRUCT_BasicTypes::parse( parser, t.basic, addr, 1 );
					}
					break;
				}
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_aggregate )
						{
							decltype(T::aggregate) temp_aggregate;
							publishable_STRUCT_AggregateType::copy<decltype(T::aggregate)>( t.aggregate, temp_aggregate );
							bool changedCurrent = publishable_STRUCT_AggregateType::parse<ParserT, decltype(T::aggregate), bool>( parser, t.aggregate );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_aggregate )
									t.notifyUpdated_aggregate();
								t.notifyUpdated_aggregate( temp_aggregate );
							}
						}
						else if constexpr( has_void_update_notifier_for_aggregate )
						{
							bool changedCurrent = publishable_STRUCT_AggregateType::parse<ParserT, decltype(T::aggregate), bool>( parser, t.aggregate );
							if ( changedCurrent )
							{
								t.notifyUpdated_aggregate();
							}
						}

						else
						{
							publishable_STRUCT_AggregateType::parse( parser, t.aggregate );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_aggregate )
						{
							decltype(T::aggregate) temp_aggregate;
							publishable_STRUCT_AggregateType::copy<decltype(T::aggregate)>( t.aggregate, temp_aggregate );
							bool changedCurrent = publishable_STRUCT_AggregateType::parse<ParserT, decltype(T::aggregate), bool>( parser, t.aggregate, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_aggregate )
									t.notifyUpdated_aggregate();
								t.notifyUpdated_aggregate( temp_aggregate );
							}
						}
						else if constexpr( has_void_update_notifier_for_aggregate )
						{
							bool changedCurrent = publishable_STRUCT_AggregateType::parse<ParserT, decltype(T::aggregate), bool>( parser, t.aggregate, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_aggregate();
							}
						}
						else
							publishable_STRUCT_AggregateType::parse( parser, t.aggregate, addr, 1 );
					}
					break;
				}
				default:
					throw std::exception(); // bad format, TODO: ...
			}
			addr.clear();
		}
	}


	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "basic" );
		publishable_STRUCT_BasicTypes::parse( parser, t.basic );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "aggregate" );
		publishable_STRUCT_AggregateType::parse( parser, t.aggregate );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	const auto& get_name() { return t.name; }
	const auto& get_basic() { return t.basic; }
	const auto& get_aggregate() { return t.aggregate; }
};

template<class T, class RegistrarT>
class StructSix_NodecppWrapperForSubscriber : public StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	StructSix_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~StructSix_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return StructSix_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class StructSix_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_basic = has_basic_member<T>;
	static_assert( has_basic, "type T must have member T::basic of a type corresponding to IDL type STRUCT BasicTypes" );
	static constexpr bool has_aggregate = has_aggregate_member<T>;
	static_assert( has_aggregate, "type T must have member T::aggregate of a type corresponding to IDL type STRUCT AggregateType" );


public:
	static constexpr uint64_t numTypeID = 6;

	StructSix_WrapperForConcentrator() {}
	const char* name() {return "StructSix";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "basic" );
		publishable_STRUCT_BasicTypes::compose( composer, t.basic );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "aggregate" );
		publishable_STRUCT_AggregateType::compose( composer, t.aggregate );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}

	// Acting as subscriber
	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<BufferT>& parser ) { applyMessageWithUpdates(parser); }
	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser ) { parseStateSyncMessage(parser); }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser ) { parseStateSyncMessage(parser); }

	template<typename ParserT>
	void applyMessageWithUpdates(ParserT& parser)
	{
		::globalmq::marshalling::impl::parseStateUpdateMessageBegin( parser );
		GMQ_COLL vector<size_t> addr;
		while( ::globalmq::marshalling::impl::parseAddressInPublishable<ParserT, GMQ_COLL vector<size_t>>( parser, addr ) )
		{
			GMQ_ASSERT( addr.size() );
			switch ( addr[0] )
			{
				case 0:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					break;
				}
				case 1:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_BasicTypes::parse( parser, t.basic );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_BasicTypes::parse( parser, t.basic, addr, 1 );
					}
					break;
				}
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_AggregateType::parse( parser, t.aggregate );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_AggregateType::parse( parser, t.aggregate, addr, 1 );
					}
					break;
				}
				default:
					throw std::exception(); // bad format, TODO: ...
			}
			addr.clear();
		}
	}

	template<class ParserT>
	void parseStateSyncMessage( ParserT& parser )
	{
		::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "basic" );
		publishable_STRUCT_BasicTypes::parse( parser, t.basic );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "aggregate" );
		publishable_STRUCT_AggregateType::parse( parser, t.aggregate );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};

template<class InputBufferT, class ComposerT>
class StateConcentratorFactory : public ::globalmq::marshalling::StateConcentratorFactoryBase<InputBufferT, ComposerT>
{
public:
	virtual StateConcentratorBase<InputBufferT, ComposerT>* createConcentrator( uint64_t typeID )
	{
		switch( typeID )
		{
			case 6:
				return new StructSix_WrapperForConcentrator<structures::StructSix, InputBufferT, ComposerT>;
			default:
				return nullptr;
		}
	}
};

//===============================================================================

template<class T>
class BasicTypes_RefWrapper
{
	T& t;
	static constexpr bool has_anInt = has_anInt_member<T>;
	static_assert( has_anInt, "type T must have member T::anInt of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_anUInt = has_anUInt_member<T>;
	static_assert( has_anUInt, "type T must have member T::anUInt of a type corresponding to IDL type UINTEGER" );
	static constexpr bool has_aReal = has_aReal_member<T>;
	static_assert( has_aReal, "type T must have member T::aReal of a type corresponding to IDL type REAL" );
	static constexpr bool has_aString = has_aString_member<T>;
	static_assert( has_aString, "type T must have member T::aString of a type corresponding to IDL type CHARACTER_STRING" );


public:
	BasicTypes_RefWrapper( T& actual ) : t( actual ) {}
	auto get_anInt() { return t.anInt; }
	auto get_anUInt() { return t.anUInt; }
	auto get_aReal() { return t.aReal; }
	const auto& get_aString() { return t.aString; }
};

template<class T, class RootT>
class BasicTypes_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_anInt = has_anInt_member<T>;
	static_assert( has_anInt, "type T must have member T::anInt of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_anUInt = has_anUInt_member<T>;
	static_assert( has_anUInt, "type T must have member T::anUInt of a type corresponding to IDL type UINTEGER" );
	static constexpr bool has_aReal = has_aReal_member<T>;
	static_assert( has_aReal, "type T must have member T::aReal of a type corresponding to IDL type REAL" );
	static constexpr bool has_aString = has_aString_member<T>;
	static_assert( has_aString, "type T must have member T::aString of a type corresponding to IDL type CHARACTER_STRING" );


public:
	BasicTypes_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_anInt() { return t.anInt; }
	void set_anInt( decltype(T::anInt) val) { 
		t.anInt = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.anInt );
	}
	auto get_anUInt() { return t.anUInt; }
	void set_anUInt( decltype(T::anUInt) val) { 
		t.anUInt = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( root.getComposer(), t.anUInt );
	}
	auto get_aReal() { return t.aReal; }
	void set_aReal( decltype(T::aReal) val) { 
		t.aReal = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.aReal );
	}
	const auto& get_aString() { return t.aString; }
	void set_aString( decltype(T::aString) val) { 
		t.aString = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 3 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.aString );
	}
};

template<class T>
class AggregateType_RefWrapper
{
	T& t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_theAggregate = has_theAggregate_member<T>;
	static_assert( has_theAggregate, "type T must have member T::theAggregate of a type corresponding to IDL type STRUCT BasicTypes" );
	static constexpr bool has_lastValue = has_lastValue_member<T>;
	static_assert( has_lastValue, "type T must have member T::lastValue of a type corresponding to IDL type INTEGER" );


public:
	AggregateType_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() { return t.name; }
	const auto& get_theAggregate() { return t.theAggregate; }
	auto get_lastValue() { return t.lastValue; }
};

template<class T, class RootT>
class AggregateType_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_theAggregate = has_theAggregate_member<T>;
	static_assert( has_theAggregate, "type T must have member T::theAggregate of a type corresponding to IDL type STRUCT BasicTypes" );
	static constexpr bool has_lastValue = has_lastValue_member<T>;
	static_assert( has_lastValue, "type T must have member T::lastValue of a type corresponding to IDL type INTEGER" );


public:
	AggregateType_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.name );
	}
	const auto& get_theAggregate() { return t.theAggregate; }
	void set_theAggregate( decltype(T::theAggregate) val) { 
		t.theAggregate = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_STRUCT_BasicTypes::compose( root.getComposer(), t.theAggregate );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_theAggregate() { return BasicTypes_RefWrapper4Set</*aaa*/decltype(T::theAggregate), RootT>(t.theAggregate, root, address, 1); }
	auto get_lastValue() { return t.lastValue; }
	void set_lastValue( decltype(T::lastValue) val) { 
		t.lastValue = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.lastValue );
	}
};

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
// STRUCT "struct_one" Targets: JSON GMQ (10 parameters)
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
void STRUCT_struct_one_compose(ComposerT& composer, Args&& ... args)
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

//**********************************************************************
// DISCRIMINATED_UNION "du_one" Targets: GMQ (2 cases)
//  CASE one (3 parameters)(3 parameters)
//    1. REAL D1 (REQUIRED)
//    2. REAL D2 (REQUIRED)
//    3. REAL D3 (REQUIRED)
//  CASE two (1 parameters)(1 parameters)
//    1. VECTOR<REAL> Data (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void DISCRIMINATED_UNION_du_one_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, D1_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, D2_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, D3_Type::Name>;
	using arg_4_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::RealType>, Data_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this DISCRIMINATED_UNION assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_2_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_3_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
}

//**********************************************************************
// STRUCT "struct_du" Targets: GMQ (2 parameters)
//  1. STRUCT point3D pt (REQUIRED)
//  2. DISCRIMINATED_UNION du_one disc_union (REQUIRED)
//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_struct_du_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, pt_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::DiscriminatedUnionType, disc_union_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this STRUCT assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
}


} // namespace mtest

#endif // test_marshalling_h_71a7abdf_guard
