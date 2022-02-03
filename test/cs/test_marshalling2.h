#ifndef test_marshalling2_h_c8113083_guard
#define test_marshalling2_h_c8113083_guard

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
//////////////////////////////////////////////////////////////




// member name presence checkers
template<typename T> concept has_number_member = requires { { T::number }; };
template<typename T> concept has_testData_member = requires { { T::testData }; };
template<typename T> concept has_vectorOfTestData_member = requires { { T::vectorOfTestData }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_number = requires(T t) { { t.notifyUpdated_number() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_number = requires { { std::declval<StateT>().notifyUpdated_number(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_testData = requires(T t) { { t.notifyUpdated_testData() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_testData = requires { { std::declval<StateT>().notifyUpdated_testData(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_vectorOfTestData = requires(T t) { { t.notifyUpdated_vectorOfTestData() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_vectorOfTestData = requires { { std::declval<StateT>().notifyUpdated_vectorOfTestData(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_vectorOfTestData = requires(T t) { { t.notifyElementUpdated_vectorOfTestData() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_vectorOfTestData = requires { { std::declval<StateT>().notifyElementUpdated_vectorOfTestData(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_vectorOfTestData = requires { { std::declval<StateT>().notifyElementUpdated_vectorOfTestData(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_vectorOfTestData = requires(T t) { { t.notifyInserted_vectorOfTestData() }; };
template<typename StateT> concept has_insert_notifier_call2_for_vectorOfTestData = requires { { std::declval<StateT>().notifyInserted_vectorOfTestData(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_vectorOfTestData = requires { { std::declval<StateT>().notifyInserted_vectorOfTestData(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_vectorOfTestData = requires(T t) { { t.notifyErased_vectorOfTestData() }; };
template<typename StateT> concept has_erased_notifier_call2_for_vectorOfTestData = requires { { std::declval<StateT>().notifyErased_vectorOfTestData(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_vectorOfTestData = requires { { std::declval<StateT>().notifyErased_vectorOfTestData(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct TestData;

struct TestData
{
	int64_t number;
};

struct TestPub
{
	int64_t number;
	TestData testData;
	GMQ_COLL vector<TestData> vectorOfTestData;
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_TestData;
template<class T> class TestData_RefWrapper;
template<class T, class RootT> class TestData_RefWrapper4Set;


struct publishable_STRUCT_TestData : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::number)>( parser, &(t.number), "number" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.number, "number", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_number = has_void_update_notifier_call_for_number<T>;
		static constexpr bool has_update_notifier_for_number = has_update_notifier_call_for_number<T, decltype(T::number)>;
		static constexpr bool has_any_notifier_for_number = has_void_update_notifier_for_number || has_update_notifier_for_number;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_number || reportChanges )
		{
			decltype(T::number) oldVal = t.number;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::number)>( parser, &(t.number), "number" );
			bool currentChanged = oldVal != t.number;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_number )
					t.notifyUpdated_number();
				if constexpr ( has_update_notifier_for_number )
					t.notifyUpdated_number( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::number)>( parser, &(t.number), "number" );


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
		static constexpr bool has_void_update_notifier_for_number = has_void_update_notifier_call_for_number<T>;
		static constexpr bool has_update_notifier_for_number = has_update_notifier_call_for_number<T, decltype(T::number)>;
		static constexpr bool has_any_notifier_for_number = has_void_update_notifier_for_number || has_update_notifier_for_number;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_number || reportChanges )
				{
					decltype(T::number) oldVal = t.number;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::number)>( parser, &(t.number) );
					bool currentChanged = oldVal != t.number;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_number )
							t.notifyUpdated_number();
						if constexpr ( has_update_notifier_for_number )
							t.notifyUpdated_number( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::number)>( parser, &(t.number) );
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
		dst.number = src.number;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.number != s2.number ) return false;
		return true;
	}
};

//**********************************************************************
// PUBLISHABLE TestPub (3 parameters)
// 1. INTEGER number
// 2. STRUCT TestData testData
// 3. VECTOR< STRUCT TestData> vectorOfTestData
//**********************************************************************

template<class T, class ComposerT>
class TestPub_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_number = has_number_member<T>;
	static_assert( has_number, "type T must have member T::number of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_testData = has_testData_member<T>;
	static_assert( has_testData, "type T must have member T::testData of a type corresponding to IDL type STRUCT TestData" );
	static constexpr bool has_vectorOfTestData = has_vectorOfTestData_member<T>;
	static_assert( has_vectorOfTestData, "type T must have member T::vectorOfTestData of a type corresponding to IDL type VECTOR<STRUCT TestData>" );


public:
	static constexpr uint64_t numTypeID = 7;
	static constexpr const char* stringTypeID = "TestPub";

	template<class ... ArgsT>
	TestPub_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	auto get_number() { return t.number; }
	void set_number( decltype(T::number) val) { 
		t.number = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( composer, t.number );
	}
	const auto& get_testData() { return t.testData; }
	void set_testData( decltype(T::testData) val) { 
		t.testData = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_TestData::compose( composer, t.testData );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_testData() { return TestData_RefWrapper4Set<decltype(T::testData), TestPub_WrapperForPublisher>(t.testData, *this, GMQ_COLL vector<size_t>(), 1); }
	auto get_vectorOfTestData() { return globalmq::marshalling::VectorOfStructRefWrapper<TestData_RefWrapper<typename decltype(T::vectorOfTestData)::value_type>, decltype(T::vectorOfTestData)>(t.vectorOfTestData); }
	void set_vectorOfTestData( decltype(T::vectorOfTestData) val) { 
		t.vectorOfTestData = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( composer );
		PublishableVectorProcessor::compose<ComposerT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( composer, t.vectorOfTestData );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( composer );
	}
	auto get4set_vectorOfTestData() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<decltype(T::vectorOfTestData), publishable_STRUCT_TestData, TestPub_WrapperForPublisher, TestData_RefWrapper4Set<typename decltype(T::vectorOfTestData)::value_type, TestPub_WrapperForPublisher>>(t.vectorOfTestData, *this, GMQ_COLL vector<size_t>(), 2); }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.number, "number", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "testData" );
		publishable_STRUCT_TestData::compose( composer, t.testData );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( composer, t.vectorOfTestData, "vectorOfTestData", false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class TestPub_NodecppWrapperForPublisher : public TestPub_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	TestPub_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : TestPub_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~TestPub_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { TestPub_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  TestPub_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { TestPub_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return TestPub_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class TestPub_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_number = has_number_member<T>;
	static_assert( has_number, "type T must have member T::number of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_testData = has_testData_member<T>;
	static_assert( has_testData, "type T must have member T::testData of a type corresponding to IDL type STRUCT TestData" );
	static constexpr bool has_vectorOfTestData = has_vectorOfTestData_member<T>;
	static_assert( has_vectorOfTestData, "type T must have member T::vectorOfTestData of a type corresponding to IDL type VECTOR<STRUCT TestData>" );

	static constexpr bool has_void_update_notifier_for_number = has_void_update_notifier_call_for_number<T>;
	static constexpr bool has_update_notifier_for_number = has_update_notifier_call_for_number<T, decltype(T::number)>;
	static constexpr bool has_any_notifier_for_number = has_void_update_notifier_for_number || has_update_notifier_for_number;
	static constexpr bool has_void_update_notifier_for_testData = has_void_update_notifier_call_for_testData<T>;
	static constexpr bool has_update_notifier_for_testData = has_update_notifier_call_for_testData<T, decltype(T::testData)>;
	static constexpr bool has_any_notifier_for_testData = has_void_update_notifier_for_testData || has_update_notifier_for_testData;
	static constexpr bool has_void_update_notifier_for_vectorOfTestData = has_void_update_notifier_call_for_vectorOfTestData<T>;
	static constexpr bool has_update_notifier_for_vectorOfTestData = has_update_notifier_call_for_vectorOfTestData<T, decltype(T::vectorOfTestData)>;
	static constexpr bool has_any_notifier_for_vectorOfTestData = has_void_update_notifier_for_vectorOfTestData || has_update_notifier_for_vectorOfTestData;
	using vectorOfTestDataT = decltype(T::vectorOfTestData);
	static constexpr bool has_void_insert_notifier_for_vectorOfTestData = has_void_insert_notifier_call_for_vectorOfTestData<T>;
	static constexpr bool has_insert_notifier2_for_vectorOfTestData = has_insert_notifier_call2_for_vectorOfTestData<T>;
	static constexpr bool has_insert_notifier3_for_vectorOfTestData = has_insert_notifier_call3_for_vectorOfTestData<T, GMQ_COLL vector<vectorOfTestDataT>&>;
	static constexpr bool has_void_erased_notifier_for_vectorOfTestData = has_void_erased_notifier_call_for_vectorOfTestData<T>;
	static constexpr bool has_erased_notifier2_for_vectorOfTestData = has_erased_notifier_call2_for_vectorOfTestData<T>;
	static constexpr bool has_erased_notifier3_for_vectorOfTestData = has_erased_notifier_call3_for_vectorOfTestData<T, GMQ_COLL vector<vectorOfTestDataT>&>;
	static constexpr bool has_void_element_updated_notifier_for_vectorOfTestData = has_element_updated_void_notifier_call_for_vectorOfTestData<T>;
	static constexpr bool has_element_updated_notifier_for_vectorOfTestData = has_element_updated_notifier_call_for_vectorOfTestData<T>;
	static constexpr bool has_full_element_updated_notifier_for_vectorOfTestData = has_full_element_updated_notifier_call_for_vectorOfTestData<T, vectorOfTestDataT&>;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 7;
	static constexpr const char* stringTypeID = "TestPub";

	template<class ... ArgsT>
	TestPub_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
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
					if constexpr( has_any_notifier_for_number )
					{
						decltype(T::number) oldVal = t.number;
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::number)>( parser, &(t.number) );
						bool currentChanged = oldVal != t.number;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_number )
								t.notifyUpdated_number();
							if constexpr ( has_update_notifier_for_number )
								t.notifyUpdated_number( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::number)>( parser, &(t.number) );
					break;
				}
				case 1:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_testData )
						{
							decltype(T::testData) temp_testData;
							publishable_STRUCT_TestData::copy<decltype(T::testData)>( t.testData, temp_testData );
							bool changedCurrent = publishable_STRUCT_TestData::parse<ParserT, decltype(T::testData), bool>( parser, t.testData );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_testData )
									t.notifyUpdated_testData();
								t.notifyUpdated_testData( temp_testData );
							}
						}
						else if constexpr( has_void_update_notifier_for_testData )
						{
							bool changedCurrent = publishable_STRUCT_TestData::parse<ParserT, decltype(T::testData), bool>( parser, t.testData );
							if ( changedCurrent )
							{
								t.notifyUpdated_testData();
							}
						}

						else
						{
							publishable_STRUCT_TestData::parse( parser, t.testData );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_testData )
						{
							decltype(T::testData) temp_testData;
							publishable_STRUCT_TestData::copy<decltype(T::testData)>( t.testData, temp_testData );
							bool changedCurrent = publishable_STRUCT_TestData::parse<ParserT, decltype(T::testData), bool>( parser, t.testData, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_testData )
									t.notifyUpdated_testData();
								t.notifyUpdated_testData( temp_testData );
							}
						}
						else if constexpr( has_void_update_notifier_for_testData )
						{
							bool changedCurrent = publishable_STRUCT_TestData::parse<ParserT, decltype(T::testData), bool>( parser, t.testData, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_testData();
							}
						}
						else
							publishable_STRUCT_TestData::parse( parser, t.testData, addr, 1 );
					}
					break;
				}
				case 2:
				{
					decltype(T::vectorOfTestData) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_vectorOfTestData;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( t.vectorOfTestData, oldVectorVal );
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vectorOfTestData.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::vectorOfTestData)::value_type& value = t.vectorOfTestData[pos];
							if constexpr ( has_full_element_updated_notifier_for_vectorOfTestData )
							{
								typename decltype(T::vectorOfTestData)::value_type oldValue;
								publishable_STRUCT_TestData::copy( value, oldValue );
								currentChanged = publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_vectorOfTestData( pos, oldValue );
									if constexpr ( has_element_updated_notifier_for_vectorOfTestData )
										t.notifyElementUpdated_vectorOfTestData();
									if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
										t.notifyElementUpdated_vectorOfTestData();
								}
							}
							else if constexpr ( has_element_updated_notifier_for_vectorOfTestData )
							{
								currentChanged = publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_vectorOfTestData( pos );
									if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
										t.notifyElementUpdated_vectorOfTestData();
								}
							}
							else if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
							{
								currentChanged = publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
									t.notifyElementUpdated_vectorOfTestData();
							}
							else
							{
								if constexpr ( alwaysCollectChanges )
									currentChanged = publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type, bool>( parser, value, addr, 2 );
								else
									publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type>( parser, value, addr, 2 );
							}
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									decltype(T::vectorOfTestData) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( t.vectorOfTestData, oldVal );
									t.vectorOfTestData.erase( t.vectorOfTestData.begin() + pos );
									if constexpr ( has_erased_notifier3_for_vectorOfTestData )
										t.notifyErased_vectorOfTestData( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_vectorOfTestData )
										t.notifyErased_vectorOfTestData( pos );
									if constexpr ( has_void_erased_notifier_for_vectorOfTestData )
										t.notifyErased_vectorOfTestData();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vectorOfTestData)::value_type& value = t.vectorOfTestData[pos];
									typename decltype(T::vectorOfTestData)::value_type oldValue;
									publishable_STRUCT_TestData::copy( value, oldValue );
									if constexpr ( has_full_element_updated_notifier_for_vectorOfTestData )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vectorOfTestData( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_vectorOfTestData )
												t.notifyElementUpdated_vectorOfTestData( pos );
											if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
												t.notifyElementUpdated_vectorOfTestData();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_vectorOfTestData )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vectorOfTestData( pos );
											if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
												t.notifyElementUpdated_vectorOfTestData();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_vectorOfTestData )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value, oldValue );
										if ( currentChanged )
											t.notifyElementUpdated_vectorOfTestData();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value, oldValue );
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vectorOfTestData)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value );
									if constexpr ( has_insert_notifier3_for_vectorOfTestData )
									{
										decltype(T::vectorOfTestData) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( t.vectorOfTestData, oldVal );
										t.notifyInserted_vectorOfTestData( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_vectorOfTestData )
										t.notifyInserted_vectorOfTestData( pos );
									if constexpr ( has_void_insert_notifier_for_vectorOfTestData )
										t.notifyInserted_vectorOfTestData();
									t.vectorOfTestData.insert( t.vectorOfTestData.begin() + pos, value );
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								default:
									throw std::exception();
							}
							::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );
						}
					}
					else // replacement of the whole vector
					{
						::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );

						if constexpr( alwaysCollectChanges )
						{
							PublishableVectorProcessor::parse<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, t.vectorOfTestData );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( oldVectorVal, t.vectorOfTestData );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, t.vectorOfTestData );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_vectorOfTestData )
							t.notifyUpdated_vectorOfTestData();
						if constexpr( has_update_notifier_for_vectorOfTestData )
							t.notifyUpdated_vectorOfTestData( oldVectorVal );
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::number)>( parser, &(t.number), "number" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "testData" );
		publishable_STRUCT_TestData::parse( parser, t.testData );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "vectorOfTestData" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, t.vectorOfTestData );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	auto get_number() { return t.number; }
	const auto& get_testData() { return t.testData; }
	auto get_vectorOfTestData() { return globalmq::marshalling::VectorOfStructRefWrapper<TestData_RefWrapper<typename decltype(T::vectorOfTestData)::value_type>, decltype(T::vectorOfTestData)>(t.vectorOfTestData); }
};

template<class T, class RegistrarT>
class TestPub_NodecppWrapperForSubscriber : public TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	TestPub_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~TestPub_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return TestPub_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class TestPub_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_number = has_number_member<T>;
	static_assert( has_number, "type T must have member T::number of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_testData = has_testData_member<T>;
	static_assert( has_testData, "type T must have member T::testData of a type corresponding to IDL type STRUCT TestData" );
	static constexpr bool has_vectorOfTestData = has_vectorOfTestData_member<T>;
	static_assert( has_vectorOfTestData, "type T must have member T::vectorOfTestData of a type corresponding to IDL type VECTOR<STRUCT TestData>" );


public:
	static constexpr uint64_t numTypeID = 7;

	TestPub_WrapperForConcentrator() {}
	const char* name() {return "TestPub";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.number, "number", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "testData" );
		publishable_STRUCT_TestData::compose( composer, t.testData );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( composer, t.vectorOfTestData, "vectorOfTestData", false );


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
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::number)>( parser, &(t.number) );
					break;
				}
				case 1:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_TestData::parse( parser, t.testData );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_TestData::parse( parser, t.testData, addr, 1 );
					}
					break;
				}
				case 2:
				{
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vectorOfTestData.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::vectorOfTestData)::value_type& value = t.vectorOfTestData[pos];
							publishable_STRUCT_TestData::parse<ParserT, typename decltype(T::vectorOfTestData)::value_type>( parser, value, addr, 2 );
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									t.vectorOfTestData.erase( t.vectorOfTestData.begin() + pos );
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vectorOfTestData)::value_type& value = t.vectorOfTestData[pos];
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value );
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vectorOfTestData)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, value );
									t.vectorOfTestData.insert( t.vectorOfTestData.begin() + pos, value );
									break;
								}
								default:
									throw std::exception();
							}
							::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );
						}
					}
					else // replacement of the whole vector
					{
						::globalmq::marshalling::impl::publishableParseLeafeVectorBegin( parser );
						PublishableVectorProcessor::parse<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, t.vectorOfTestData );
						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::number)>( parser, &(t.number), "number" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "testData" );
		publishable_STRUCT_TestData::parse( parser, t.testData );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "vectorOfTestData" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vectorOfTestData), publishable_STRUCT_TestData>( parser, t.vectorOfTestData );

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
			case 7:
				return new TestPub_WrapperForConcentrator<structures::TestPub, InputBufferT, ComposerT>;
			default:
				return nullptr;
		}
	}
};

//===============================================================================

template<class T>
class TestData_RefWrapper
{
	T& t;
	static constexpr bool has_number = has_number_member<T>;
	static_assert( has_number, "type T must have member T::number of a type corresponding to IDL type INTEGER" );


public:
	TestData_RefWrapper( T& actual ) : t( actual ) {}
	auto get_number() { return t.number; }
};

template<class T, class RootT>
class TestData_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_number = has_number_member<T>;
	static_assert( has_number, "type T must have member T::number of a type corresponding to IDL type INTEGER" );


public:
	TestData_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_number() { return t.number; }
	void set_number( decltype(T::number) val) { 
		t.number = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.number );
	}
};


} // namespace mtest

#endif // test_marshalling2_h_c8113083_guard
