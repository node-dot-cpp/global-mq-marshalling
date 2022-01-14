#ifndef test_marshalling_h_d3979a03_guard
#define test_marshalling_h_d3979a03_guard

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
template<typename T> concept has_i1_member = requires { { T::i1 }; };
template<typename T> concept has_name_member = requires { { T::name }; };
template<typename T> concept has_p1_member = requires { { T::p1 }; };
template<typename T> concept has_p2_member = requires { { T::p2 }; };
template<typename T> concept has_tag_member = requires { { T::tag }; };
template<typename T> concept has_value_member = requires { { T::value }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_i1 = requires(T t) { { t.notifyUpdated_i1() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_i1 = requires { { std::declval<StateT>().notifyUpdated_i1(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_name = requires(T t) { { t.notifyUpdated_name() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_name = requires { { std::declval<StateT>().notifyUpdated_name(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_p1 = requires(T t) { { t.notifyUpdated_p1() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_p1 = requires { { std::declval<StateT>().notifyUpdated_p1(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_p2 = requires(T t) { { t.notifyUpdated_p2() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_p2 = requires { { std::declval<StateT>().notifyUpdated_p2(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_tag = requires(T t) { { t.notifyUpdated_tag() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_tag = requires { { std::declval<StateT>().notifyUpdated_tag(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_value = requires(T t) { { t.notifyUpdated_value() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_value = requires { { std::declval<StateT>().notifyUpdated_value(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct point;
struct point3D;
struct struct_one;
class du_one;
struct struct_du;
struct Property;
struct HtmlTag;
struct HtmlSubData;

struct Property
{
	GMQ_COLL string name;
	GMQ_COLL string value;
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

struct HtmlSubData
{
	GMQ_COLL string name;
	GMQ_COLL string value;
};

struct HtmlTag
{
	Property p1;
	Property p2;
	int64_t i1;
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

struct html_data
{
	HtmlTag tag;
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_Property;
template<class T> class Property_RefWrapper;
template<class T, class RootT> class Property_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
template<class T> class HtmlTag_RefWrapper;
template<class T, class RootT> class HtmlTag_RefWrapper4Set;


struct publishable_STRUCT_Property : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.value, "value", false );

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
		static constexpr bool has_void_update_notifier_for_value = has_void_update_notifier_call_for_value<T>;
		static constexpr bool has_update_notifier_for_value = has_update_notifier_call_for_value<T, decltype(T::value)>;
		static constexpr bool has_any_notifier_for_value = has_void_update_notifier_for_value || has_update_notifier_for_value;
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

		if constexpr( has_any_notifier_for_value || reportChanges )
		{
			decltype(T::value) oldVal = t.value;
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );
			bool currentChanged = oldVal != t.value;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_value )
					t.notifyUpdated_value();
				if constexpr ( has_update_notifier_for_value )
					t.notifyUpdated_value( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );


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
		static constexpr bool has_void_update_notifier_for_value = has_void_update_notifier_call_for_value<T>;
		static constexpr bool has_update_notifier_for_value = has_update_notifier_call_for_value<T, decltype(T::value)>;
		static constexpr bool has_any_notifier_for_value = has_void_update_notifier_for_value || has_update_notifier_for_value;
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
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_value || reportChanges )
				{
					decltype(T::value) oldVal = t.value;
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::value)>( parser, &(t.value) );
					bool currentChanged = oldVal != t.value;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_value )
							t.notifyUpdated_value();
						if constexpr ( has_update_notifier_for_value )
							t.notifyUpdated_value( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::value)>( parser, &(t.value) );
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
		dst.value = src.value;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.name != s2.name ) return false;
		if ( s1.value != s2.value ) return false;
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

struct publishable_STRUCT_HtmlSubData : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::value)>( parser, &(t.value), "value" );

	}

};

struct publishable_STRUCT_HtmlTag : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "p1" );
		publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( parser, t.p1 );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "p2" );
		publishable_STRUCT_Property::parseForStateSyncOrMessageInDepth( parser, t.p2 );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::i1)>( parser, &(t.i1), "i1" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "p1" );
		publishable_STRUCT_Property::compose( composer, t.p1 );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "p2" );
		publishable_STRUCT_Property::compose( composer, t.p2 );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.i1, "i1", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_p1 = has_void_update_notifier_call_for_p1<T>;
		static constexpr bool has_update_notifier_for_p1 = has_update_notifier_call_for_p1<T, decltype(T::p1)>;
		static constexpr bool has_any_notifier_for_p1 = has_void_update_notifier_for_p1 || has_update_notifier_for_p1;
		static constexpr bool has_void_update_notifier_for_p2 = has_void_update_notifier_call_for_p2<T>;
		static constexpr bool has_update_notifier_for_p2 = has_update_notifier_call_for_p2<T, decltype(T::p2)>;
		static constexpr bool has_any_notifier_for_p2 = has_void_update_notifier_for_p2 || has_update_notifier_for_p2;
		static constexpr bool has_void_update_notifier_for_i1 = has_void_update_notifier_call_for_i1<T>;
		static constexpr bool has_update_notifier_for_i1 = has_update_notifier_call_for_i1<T, decltype(T::i1)>;
		static constexpr bool has_any_notifier_for_i1 = has_void_update_notifier_for_i1 || has_update_notifier_for_i1;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "p1" );
		if constexpr( has_update_notifier_for_p1 )
		{
			decltype(T::p1) temp_p1;
			publishable_STRUCT_Property::copy<decltype(T::p1)>( t.p1, temp_p1 );
			bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr( has_void_update_notifier_for_p1 )
					t.notifyUpdated_p1();
				t.notifyUpdated_p1( temp_p1 );
			}
		}
		else if constexpr( has_void_update_notifier_for_p1 )
		{
			bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				t.notifyUpdated_p1();
			}
		}
		else if constexpr ( reportChanges )
			changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
		else
			publishable_STRUCT_Property::parse( parser, t.p1 );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "p2" );
		if constexpr( has_update_notifier_for_p2 )
		{
			decltype(T::p2) temp_p2;
			publishable_STRUCT_Property::copy<decltype(T::p2)>( t.p2, temp_p2 );
			bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr( has_void_update_notifier_for_p2 )
					t.notifyUpdated_p2();
				t.notifyUpdated_p2( temp_p2 );
			}
		}
		else if constexpr( has_void_update_notifier_for_p2 )
		{
			bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				t.notifyUpdated_p2();
			}
		}
		else if constexpr ( reportChanges )
			changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
		else
			publishable_STRUCT_Property::parse( parser, t.p2 );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		if constexpr( has_any_notifier_for_i1 || reportChanges )
		{
			decltype(T::i1) oldVal = t.i1;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::i1)>( parser, &(t.i1), "i1" );
			bool currentChanged = oldVal != t.i1;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_i1 )
					t.notifyUpdated_i1();
				if constexpr ( has_update_notifier_for_i1 )
					t.notifyUpdated_i1( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::i1)>( parser, &(t.i1), "i1" );


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
		static constexpr bool has_void_update_notifier_for_p1 = has_void_update_notifier_call_for_p1<T>;
		static constexpr bool has_update_notifier_for_p1 = has_update_notifier_call_for_p1<T, decltype(T::p1)>;
		static constexpr bool has_any_notifier_for_p1 = has_void_update_notifier_for_p1 || has_update_notifier_for_p1;
		static constexpr bool has_void_update_notifier_for_p2 = has_void_update_notifier_call_for_p2<T>;
		static constexpr bool has_update_notifier_for_p2 = has_update_notifier_call_for_p2<T, decltype(T::p2)>;
		static constexpr bool has_any_notifier_for_p2 = has_void_update_notifier_for_p2 || has_update_notifier_for_p2;
		static constexpr bool has_void_update_notifier_for_i1 = has_void_update_notifier_call_for_i1<T>;
		static constexpr bool has_update_notifier_for_i1 = has_update_notifier_call_for_i1<T, decltype(T::i1)>;
		static constexpr bool has_any_notifier_for_i1 = has_void_update_notifier_for_i1 || has_update_notifier_for_i1;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_p1 )
					{
						decltype(T::p1) temp_p1;
						publishable_STRUCT_Property::copy<decltype(T::p1)>( t.p1, temp_p1 );
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_p1 )
								t.notifyUpdated_p1();
							t.notifyUpdated_p1( temp_p1 );
						}
					}
					else if constexpr( has_void_update_notifier_for_p1 )
					{
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_p1();
						}
					}

					else
					{
							if constexpr ( reportChanges )
								changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1 );
							else
								publishable_STRUCT_Property::parse( parser, t.p1 );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_p1 )
					{
						decltype(T::p1) temp_p1;
						publishable_STRUCT_Property::copy<decltype(T::p1)>( t.p1, temp_p1 );
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_p1 )
								t.notifyUpdated_p1();
							t.notifyUpdated_p1( temp_p1 );
						}
					}
					else if constexpr( has_void_update_notifier_for_p1 )
					{
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_p1();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p1), bool>( parser, t.p1, addr, offset + 1 );
					else
						publishable_STRUCT_Property::parse( parser, t.p1, addr, offset + 1 );
				}
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_p2 )
					{
						decltype(T::p2) temp_p2;
						publishable_STRUCT_Property::copy<decltype(T::p2)>( t.p2, temp_p2 );
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_p2 )
								t.notifyUpdated_p2();
							t.notifyUpdated_p2( temp_p2 );
						}
					}
					else if constexpr( has_void_update_notifier_for_p2 )
					{
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_p2();
						}
					}

					else
					{
							if constexpr ( reportChanges )
								changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2 );
							else
								publishable_STRUCT_Property::parse( parser, t.p2 );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_p2 )
					{
						decltype(T::p2) temp_p2;
						publishable_STRUCT_Property::copy<decltype(T::p2)>( t.p2, temp_p2 );
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_p2 )
								t.notifyUpdated_p2();
							t.notifyUpdated_p2( temp_p2 );
						}
					}
					else if constexpr( has_void_update_notifier_for_p2 )
					{
						bool changedCurrent = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_p2();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_STRUCT_Property::parse<ParserT, decltype(T::p2), bool>( parser, t.p2, addr, offset + 1 );
					else
						publishable_STRUCT_Property::parse( parser, t.p2, addr, offset + 1 );
				}
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_i1 || reportChanges )
				{
					decltype(T::i1) oldVal = t.i1;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::i1)>( parser, &(t.i1) );
					bool currentChanged = oldVal != t.i1;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_i1 )
							t.notifyUpdated_i1();
						if constexpr ( has_update_notifier_for_i1 )
							t.notifyUpdated_i1( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::i1)>( parser, &(t.i1) );
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
		publishable_STRUCT_Property::copy( src.p1, dst.p1 );
		publishable_STRUCT_Property::copy( src.p2, dst.p2 );
		dst.i1 = src.i1;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if( ! publishable_STRUCT_Property::isSame( s1.p1, s2.p1 ) ) return false;
		if( ! publishable_STRUCT_Property::isSame( s1.p2, s2.p2 ) ) return false;
		if ( s1.i1 != s2.i1 ) return false;
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
// PUBLISHABLE html_data (1 parameters)
// 1. STRUCT HtmlTag tag
//**********************************************************************

template<class T, class ComposerT>
class html_data_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_tag = has_tag_member<T>;
	static_assert( has_tag, "type T must have member T::tag of a type corresponding to IDL type STRUCT HtmlTag" );


public:
	static constexpr uint64_t numTypeID = 3;
	static constexpr const char* stringTypeID = "html_data";

	template<class ... ArgsT>
	html_data_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	const auto& get_tag() { return t.tag; }
	void set_tag( decltype(T::tag) val) { 
		t.tag = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_HtmlTag::compose( composer, t.tag );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_tag() { return HtmlTag_RefWrapper4Set</*aaa*/decltype(T::tag), html_data_WrapperForPublisher>(t.tag, *this, GMQ_COLL vector<size_t>(), 0); }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "tag" );
		publishable_STRUCT_HtmlTag::compose( composer, t.tag );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class html_data_NodecppWrapperForPublisher : public html_data_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	html_data_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : html_data_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~html_data_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { html_data_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  html_data_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { html_data_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return html_data_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class html_data_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_tag = has_tag_member<T>;
	static_assert( has_tag, "type T must have member T::tag of a type corresponding to IDL type STRUCT HtmlTag" );

	static constexpr bool has_void_update_notifier_for_tag = has_void_update_notifier_call_for_tag<T>;
	static constexpr bool has_update_notifier_for_tag = has_update_notifier_call_for_tag<T, decltype(T::tag)>;
	static constexpr bool has_any_notifier_for_tag = has_void_update_notifier_for_tag || has_update_notifier_for_tag;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 3;
	static constexpr const char* stringTypeID = "html_data";

	template<class ... ArgsT>
	html_data_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
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
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_tag )
						{
							decltype(T::tag) temp_tag;
							publishable_STRUCT_HtmlTag::copy<decltype(T::tag)>( t.tag, temp_tag );
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::tag), bool>( parser, t.tag );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_tag )
									t.notifyUpdated_tag();
								t.notifyUpdated_tag( temp_tag );
							}
						}
						else if constexpr( has_void_update_notifier_for_tag )
						{
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::tag), bool>( parser, t.tag );
							if ( changedCurrent )
							{
								t.notifyUpdated_tag();
							}
						}

						else
						{
							publishable_STRUCT_HtmlTag::parse( parser, t.tag );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_tag )
						{
							decltype(T::tag) temp_tag;
							publishable_STRUCT_HtmlTag::copy<decltype(T::tag)>( t.tag, temp_tag );
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::tag), bool>( parser, t.tag, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_tag )
									t.notifyUpdated_tag();
								t.notifyUpdated_tag( temp_tag );
							}
						}
						else if constexpr( has_void_update_notifier_for_tag )
						{
							bool changedCurrent = publishable_STRUCT_HtmlTag::parse<ParserT, decltype(T::tag), bool>( parser, t.tag, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_tag();
							}
						}
						else
							publishable_STRUCT_HtmlTag::parse( parser, t.tag, addr, 1 );
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

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tag" );
		publishable_STRUCT_HtmlTag::parse( parser, t.tag );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	const auto& get_tag() { return t.tag; }
};

template<class T, class RegistrarT>
class html_data_NodecppWrapperForSubscriber : public html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	html_data_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~html_data_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return html_data_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class html_data_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_tag = has_tag_member<T>;
	static_assert( has_tag, "type T must have member T::tag of a type corresponding to IDL type STRUCT HtmlTag" );


public:
	static constexpr uint64_t numTypeID = 3;

	html_data_WrapperForConcentrator() {}
	const char* name() {return "html_data";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "tag" );
		publishable_STRUCT_HtmlTag::compose( composer, t.tag );
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
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_HtmlTag::parse( parser, t.tag );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_HtmlTag::parse( parser, t.tag, addr, 1 );
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

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tag" );
		publishable_STRUCT_HtmlTag::parse( parser, t.tag );
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
			case 3:
				return new html_data_WrapperForConcentrator<structures::html_data, InputBufferT, ComposerT>;
			default:
				return nullptr;
		}
	}
};

//===============================================================================

template<class T>
class Property_RefWrapper
{
	T& t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_value = has_value_member<T>;
	static_assert( has_value, "type T must have member T::value of a type corresponding to IDL type CHARACTER_STRING" );


public:
	Property_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() { return t.name; }
	const auto& get_value() { return t.value; }
};

template<class T, class RootT>
class Property_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_value = has_value_member<T>;
	static_assert( has_value, "type T must have member T::value of a type corresponding to IDL type CHARACTER_STRING" );


public:
	Property_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.name );
	}
	const auto& get_value() { return t.value; }
	void set_value( decltype(T::value) val) { 
		t.value = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.value );
	}
};

template<class T>
class HtmlTag_RefWrapper
{
	T& t;
	static constexpr bool has_p1 = has_p1_member<T>;
	static_assert( has_p1, "type T must have member T::p1 of a type corresponding to IDL type STRUCT Property" );
	static constexpr bool has_p2 = has_p2_member<T>;
	static_assert( has_p2, "type T must have member T::p2 of a type corresponding to IDL type STRUCT Property" );
	static constexpr bool has_i1 = has_i1_member<T>;
	static_assert( has_i1, "type T must have member T::i1 of a type corresponding to IDL type INTEGER" );


public:
	HtmlTag_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_p1() { return t.p1; }
	const auto& get_p2() { return t.p2; }
	auto get_i1() { return t.i1; }
};

template<class T, class RootT>
class HtmlTag_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_p1 = has_p1_member<T>;
	static_assert( has_p1, "type T must have member T::p1 of a type corresponding to IDL type STRUCT Property" );
	static constexpr bool has_p2 = has_p2_member<T>;
	static_assert( has_p2, "type T must have member T::p2 of a type corresponding to IDL type STRUCT Property" );
	static constexpr bool has_i1 = has_i1_member<T>;
	static_assert( has_i1, "type T must have member T::i1 of a type corresponding to IDL type INTEGER" );


public:
	HtmlTag_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_p1() { return t.p1; }
	void set_p1( decltype(T::p1) val) { 
		t.p1 = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_STRUCT_Property::compose( root.getComposer(), t.p1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_p1() { return Property_RefWrapper4Set</*aaa*/decltype(T::p1), RootT>(t.p1, root, address, 0); }
	const auto& get_p2() { return t.p2; }
	void set_p2( decltype(T::p2) val) { 
		t.p2 = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_STRUCT_Property::compose( root.getComposer(), t.p2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_p2() { return Property_RefWrapper4Set</*aaa*/decltype(T::p2), RootT>(t.p2, root, address, 1); }
	auto get_i1() { return t.i1; }
	void set_i1( decltype(T::i1) val) { 
		t.i1 = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.i1 );
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

#endif // test_marshalling_h_d3979a03_guard
