
#include <stdio.h>

#include "nodecpp_publish_subscribe_support.h"
#include <publishable_impl.h>
#include <gmqueue.h>
#include <platforms/inproc_queue.h>
#include "marshalling/test_marshalling.h"

#include <typeinfo>
#include <typeindex>
#include <unordered_map>

globalmq::marshalling::MetaPool<StatePublisherSubscriberPoolInfo> mp;

// transporting level simulation (for this test, single-threaded)
GMQ_COLL vector<globalmq::marshalling::Buffer> likeQueues[2];
void deliverMessages()
{
	using ParserT = typename StatePublisherSubscriberPoolInfo::ParserT;

	for ( size_t i=0; i<likeQueues[publisherPoolAddress].size(); ++i )
	{
		if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
		{
			fmt::print( "To publisher pool:\n" );
			std::string_view sview( reinterpret_cast<const char*>(likeQueues[publisherPoolAddress][i].begin()), likeQueues[publisherPoolAddress][i].size() );
			fmt::print( "{}\n\n", sview );
		}

		auto riter = likeQueues[publisherPoolAddress][i].getReadIter();
		ParserT parser( riter );
		mp.onMessage( parser/*, subscriberPoolAddress*/ );
	}
	likeQueues[publisherPoolAddress].clear();

	for ( size_t i=0; i<likeQueues[subscriberPoolAddress].size(); ++i )
	{
		if constexpr ( ParserT::proto == globalmq::marshalling::Proto::JSON )
		{
			fmt::print( "To subscriber pool:\n" );
			std::string_view sview( reinterpret_cast<const char*>(likeQueues[subscriberPoolAddress][i].begin()), likeQueues[subscriberPoolAddress][i].size() );
			fmt::print( "{}\n\n", sview );
		}

		auto riter = likeQueues[subscriberPoolAddress][i].getReadIter();
		ParserT parser( riter );
		mp.onMessage( parser/*, subscriberPoolAddress*/ );
	}
	likeQueues[subscriberPoolAddress].clear();
}


struct implCallerValue
{
	std::type_index typeidx = std::type_index(typeid(void));
	void* ptr = nullptr;
	implCallerValue* prev = nullptr;

	implCallerValue( std::type_index idx, void* ptr_, implCallerValue* prev_) : typeidx( idx ), ptr( ptr_ ), prev( prev_ ) {}
	implCallerValue() {}
	implCallerValue( const implCallerValue& other ) = delete;
	implCallerValue& operator = ( const implCallerValue& other ) = delete;
	implCallerValue( implCallerValue&& other ) noexcept
	{
		typeidx = other.typeidx;
		other.typeidx = std::type_index(typeid(void));
		ptr = other.ptr;
		other.ptr = nullptr;
		prev = other.prev;
		other.prev = nullptr;
	}
	implCallerValue& operator = ( implCallerValue&& other ) noexcept
	{
		typeidx = other.typeidx;
		other.typeidx = std::type_index(typeid(void));
		ptr = other.ptr;
		other.ptr = nullptr;
		prev = other.prev;
		other.prev = nullptr;
		return *this;
	}
};

thread_local std::unordered_map<std::type_index, implCallerValue> implCallerValues;

template<class LabelT, class ActualT = LabelT>
void pushHiddenParam( ActualT* param ) {
	auto ins = implCallerValues.insert( std::make_pair( std::type_index(typeid(LabelT)), implCallerValue({std::type_index(typeid(ActualT)), param, nullptr}) ) );
	if ( !ins.second )
	{
		implCallerValue cv( std::type_index(typeid(ActualT)), param, new implCallerValue( std::move( ins.first->second ) ) );
		auto ins1 = implCallerValues.insert( std::make_pair( std::type_index(typeid(LabelT)), std::move(cv) ) );
		assert( ins.second );
	}
}

template<class LabelT, class ActualT = LabelT>
ActualT* getHiddenParam() {
	auto findres = implCallerValues.find( std::type_index(typeid(LabelT)) );
	if ( findres != implCallerValues.end() )
	{
		auto tidx = std::type_index(typeid(ActualT));
		bool ok = findres->second.typeidx == tidx;
		if ( ok )
			return reinterpret_cast<ActualT*>(findres->second.ptr);
		else return nullptr;
	}
	else
		return nullptr;
}

template<class LabelT>
void popHiddenParam() {
	auto findres = implCallerValues.find( std::type_index(typeid(LabelT)) );
	if ( findres != implCallerValues.end() )
	{
		if ( findres->second.prev != nullptr )
			findres->second = std::move( *(findres->second.prev) );
		else
			implCallerValues.erase( findres );
	}
}

template<class LabelT, class UserT = LabelT>
struct CallerValue
{
	CallerValue() { pushHiddenParam<LabelT, UserT>(nullptr); }
	CallerValue( UserT* ptr ) { pushHiddenParam<LabelT, UserT>(ptr); }
	CallerValue( const CallerValue<UserT>& other ) = delete;
	CallerValue& operator = ( const CallerValue<UserT>& other ) = delete;
	CallerValue( CallerValue<UserT>&& other ) = delete;
	CallerValue<UserT>& operator = ( CallerValue<UserT>&& other ) = delete;
	~CallerValue() { popHiddenParam<LabelT>(); }
};


struct CurrentNodeLabel {};

template<class NodeT>
void setCurrentNode( NodeT* nodeptr ) { 
	popHiddenParam<CurrentNodeLabel>();
	pushHiddenParam<CurrentNodeLabel, NodeT>( nodeptr );
}
template<class NodeT>
NodeT* getCurrentNode() { 
	auto ret = getHiddenParam<CurrentNodeLabel, NodeT>();
	if ( ret != nullptr )
		return ret;
	throw std:: exception();
	return nullptr;
}

void testCurrentNodeDataManipulation()
{
	double x = 1.234;
	setCurrentNode( &x );
	try { auto cn = getCurrentNode<int>(); fmt::print( "Cannot be here; value = {}\n", *cn ); }
	catch (...) { printf( "Bad attempt to get cn failed\n" ); }
	auto cn = getCurrentNode<double>();
	assert( &x == cn );
	assert( x == *cn );
	int n = 17;
	setCurrentNode( &n );
	auto cn2 = getCurrentNode<int>();
	assert( &n == cn2 );
	assert( n == *cn2 );
}

void testCallerValueInCall_1()
{
	auto cn = getHiddenParam<double>();
	assert( *cn == 1.234 );
}

void testCallerValueInCall_2()
{
	auto cn = getHiddenParam<double>();
	assert( cn == nullptr );
}

void testCallerValue()
{
	double x = 1.234;
	{
		CallerValue<double> any( &x );
		testCallerValueInCall_1();
	}
	testCallerValueInCall_2();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SIZE{
	double X = 1.0;
	double Y = 2.0;
	double Z = 3.0;
};

struct POINT3DREAL{
  double X = 4.0;
  double Y = 5.0;
  double Z = 6.0;
};

struct StructWithVectorOfInt{
	int ID;
	std::vector<uint64_t> signedInts;
};

struct StructWithVectorOfSize{
	std::vector<SIZE> sizes;
	int NN;
};

struct CharacterParam{
	uint64_t ID = 600;
	SIZE Size = {11., 12., 13};
};

struct PublishableSample {
	int ID = 333;
	GMQ_COLL string name = "abc";
	SIZE size;
	CharacterParam chp;
	std::vector<POINT3DREAL> vector_struct_point3dreal = {{310, 311, 312}, {320, 321, 322}};
	std::vector<int> vector_of_int = {111, 112, 113};
	StructWithVectorOfInt structWithVectorOfInt;
	StructWithVectorOfSize structWithVectorOfSize;
	mtest::du_one du_one_instance;
};


class SampleNode
{
public:
	size_t preCtr = 0;
	size_t postCtr = 0;
	SampleNode() {}
	SampleNode(const SampleNode&) = delete;
	SampleNode& operator = (const SampleNode&) = delete;
	void addPreAccess() {++preCtr;}
	void addPostAccess() {++postCtr;}
};
thread_local SampleNode* currentNode = nullptr;
SampleNode* setCurrentNode( SampleNode* node ) { auto ret = currentNode; currentNode = node; return ret; }
SampleNode* getCurrentNode() { return currentNode; }

// simple notifiers

struct SIZE_UPD{
	double X = 1.0;
	double Y = 2.0;
	double Z = 3.0;
	void notifyUpdated_X() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD::notifyUpdated_X()\n" ); }
	void notifyUpdated_Y() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD::notifyUpdated_Y()\n" ); }
	void notifyUpdated_Z() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD::notifyUpdated_Z()\n" ); }
};

struct POINT3DREAL_UPD{
  double X = 4.0;
  double Y = 5.0;
  double Z = 6.0;
	void notifyUpdated_X() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD::notifyUpdated_X()\n" ); }
	void notifyUpdated_Y() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD::notifyUpdated_Y()\n" ); }
	void notifyUpdated_Z() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD::notifyUpdated_Z()\n" ); }
};

struct StructWithVectorOfInt_UPD{
	int ID;
	std::vector<uint64_t> signedInts;
	void notifyUpdated_signedInts() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "StructWithVectorOfInt_UPD::notifyUpdated_signedInts()\n" ); }
};

struct StructWithVectorOfSize_UPD{
	std::vector<SIZE_UPD> sizes;
	int NN;
	void notifyUpdated_sizes() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "StructWithVectorOfSize_UPD::notifyUpdated_sizes()\n" ); }
};

struct CharacterParam_UPD{
	uint64_t ID = 600;
	SIZE_UPD Size = {11., 12., 13};

	void notifyUpdated_ID() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "CharacterParam_UPD::notifyUpdated_ID()\n" ); }
	void notifyUpdated_Size() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "CharacterParam_UPD::notifyUpdated_Size()\n" ); }
};

struct PublishableSample_UPD {
//	SampleNode* node = nullptr;
	SampleNode& node;
	int ID = 333;
	GMQ_COLL string name = "abc";
	SIZE_UPD size;
	CharacterParam_UPD chp;
	std::vector<POINT3DREAL_UPD> vector_struct_point3dreal = {{310, 311, 312}, {320, 321, 322}};
	std::vector<int> vector_of_int = {111, 112, 113};
	StructWithVectorOfInt_UPD structWithVectorOfInt;
	StructWithVectorOfSize_UPD structWithVectorOfSize;
	mtest::du_one du_one_instance;

	PublishableSample_UPD( SampleNode& node_ ) : node( node_ ) {}

	void notifyUpdated_ID() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "PublishableSample_UPD::notifyUpdated_ID()\n" ); }
};

// notifiers with data

struct SIZE_UPD_D{
	double X = 1.0;
	double Y = 2.0;
	double Z = 3.0;
	void notifyUpdated_X( double x ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD_D::notifyUpdated_X({})\n", x ); }
	void notifyUpdated_Y( double y ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD_D::notifyUpdated_Y({})\n", y ); }
	void notifyUpdated_Z( double z ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "SIZE_UPD_D::notifyUpdated_Z({})\n", z ); }
};

struct POINT3DREAL_UPD_D{
  double X = 4.0;
  double Y = 5.0;
  double Z = 6.0;
	void notifyUpdated_X( double x ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD_D::notifyUpdated_X({})\n", x ); }
	void notifyUpdated_Y( double y ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD_D::notifyUpdated_Y({})\n", y ); }
	void notifyUpdated_Z( double z ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "POINT3DREAL_UPD_D::notifyUpdated_Z({})\n", z ); }
};

struct StructWithVectorOfInt_UPD_D{
	int ID;
	std::vector<uint64_t> signedInts;
	void notifyUpdated_signedInts() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "StructWithVectorOfInt_UPD_D::notifyUpdated_signedInts()\n" ); }
};

struct StructWithVectorOfSize_UPD_D{
	std::vector<SIZE_UPD_D> sizes;
	int NN;
	void notifyUpdated_sizes() const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "StructWithVectorOfSize_UPD_D::notifyUpdated_sizes(...)\n" ); }
};

struct CharacterParam_UPD_D{
	uint64_t ID = 600;
	SIZE_UPD_D Size = {11., 12., 13};

	void notifyUpdated_ID( uint64_t id ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "CharacterParam_UPD_D::notifyUpdated_ID({})\n", id ); }
	void notifyUpdated_Size( const SIZE_UPD_D& size ) const { 
//	void notifyUpdated_Size( SIZE_UPD_D size ) const { 
		assert( getCurrentNode() != nullptr ); 
		getCurrentNode()->addPreAccess(); 
		fmt::print( "CharacterParam_UPD_D::notifyUpdatedr_Size({},{},{})\n", size.X, size.Y, size.Z );
	}
};

struct PublishableSample_UPD_D {
//	SampleNode* node = nullptr;
	SampleNode& node;
	int ID = 333;
	GMQ_COLL string name = "abc";
	SIZE_UPD_D size;
	CharacterParam_UPD_D chp;
	std::vector<POINT3DREAL_UPD_D> vector_struct_point3dreal = {{310, 311, 312}, {320, 321, 322}};
	std::vector<int> vector_of_int = {111, 112, 113};
	StructWithVectorOfInt_UPD_D structWithVectorOfInt;
	StructWithVectorOfSize_UPD_D structWithVectorOfSize;
	mtest::du_one du_one_instance;

	PublishableSample_UPD_D( SampleNode& node_ ) : node( node_ ) {}

	void notifyUpdated_ID( int id ) const { assert( getCurrentNode() != nullptr ); getCurrentNode()->addPreAccess(); fmt::print( "PublishableSample_UPD_D::notifyUpdated_ID({})\n", id ); }
};

struct PublishableShortSample {
	int ID = 777;
	GMQ_COLL string name = "xyz";

	PublishableShortSample() {}
};

void quickTestForGmqParts()
{
	using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;
//	using BufferT = globalmq::marshalling::Buffer;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
/*	using ComposerT = globalmq::marshalling::JsonComposer<BufferT>;
	using ParserT = globalmq::marshalling::JsonParser<BufferT>;

	using BufferT = StatePublisherSubscriberPoolInfo::BufferT;
//	using ComposerT = globalmq::marshalling::GmqComposer<BufferT>;
//	using ParserT = globalmq::marshalling::GmqParser<BufferT>;
	using ComposerT = StatePublisherSubscriberPoolInfo::ParserT;
	using ParserT = StatePublisherSubscriberPoolInfo::ComposerT;*/

	mtest::StateConcentratorFactory<BufferT, ComposerT> cf;
	auto p1 = cf.createConcentrator( 1 );
	auto p2 = cf.createConcentrator( 2 );
	auto p3 = cf.createConcentrator( 3 );
	assert( p1 != nullptr );
	assert( p2 != nullptr );
	assert( p3 == nullptr );

	globalmq::marshalling::GMQueue<StatePublisherSubscriberPoolInfo> gmq;

	globalmq::marshalling::GmqPathHelper::PathComponents pc1, pc2;

	pc1.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
	pc1.authority = "abc.com";
	pc1.nodeName = "myNode";
	pc1.statePublisherOrConnectionType = "state";

	pc2.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;

	GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose( pc1 );
	bool ok = globalmq::marshalling::GmqPathHelper::parse( path, pc2 );
	assert( ok );
	assert( pc2.authority == "abc.com" );
	assert( pc2.nodeName == "myNode" );
	assert( pc2.statePublisherOrConnectionType == "state" );
	assert( !pc2.furtherResolution );
	assert( !pc2.hasPort );
	
	pc1.furtherResolution = true;
	pc1.hasPort = true;
	pc1.port = 12345;
	path = globalmq::marshalling::GmqPathHelper::compose( pc1 );
	ok = globalmq::marshalling::GmqPathHelper::parse( path, pc2 );
	assert( ok );
	assert( pc2.authority == "abc.com" );
	assert( pc2.nodeName == "myNode" );
	assert( pc2.statePublisherOrConnectionType == "state" );
	assert( pc2.furtherResolution );
	assert( pc2.hasPort );

	// testing message header
	globalmq::marshalling::PublishableStateMessageHeader mh1;
	mh1.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::subscriptionRequest;
	mh1.state_type_id_or_direction = 333;
	mh1.priority = 444;
	mh1.path = globalmq::marshalling::GmqPathHelper::compose( pc1 );
	mh1.ref_id_at_subscriber = 112;
	mh1.ref_id_at_publisher = 113;

	BufferT buff;
	ComposerT composer( buff );
	globalmq::marshalling::helperComposePublishableStateMessageBegin( composer, mh1 );
	globalmq::marshalling::impl::composeString( composer, "fake" );
	globalmq::marshalling::helperComposePublishableStateMessageEnd( composer );
	if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON )
	{
		std::string_view sview( reinterpret_cast<const char*>(buff.begin()), buff.size() );
		fmt::print( "{}\n\n", sview );
	}
	auto riter = buff.getReadIter();
	ParserT parser( riter );
	globalmq::marshalling::PublishableStateMessageHeader mh2;
	globalmq::marshalling::helperParsePublishableStateMessageBegin( parser, mh2 );
	assert( mh1.type == mh2.type );
	assert( mh1.state_type_id_or_direction == mh2.state_type_id_or_direction );
	assert( mh1.priority == mh2.priority );
	assert( mh1.path == mh2.path );
	assert( mh1.ref_id_at_subscriber == mh2.ref_id_at_subscriber );

	buff.set_size(0);
	ComposerT composer1( buff );
	mh1.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::subscriptionResponse;
	globalmq::marshalling::helperComposePublishableStateMessageBegin( composer1, mh1 );
	globalmq::marshalling::impl::composeString( composer, "fake" );
	globalmq::marshalling::helperComposePublishableStateMessageEnd( composer1 );
	if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON )
	{
		std::string_view sview( reinterpret_cast<const char*>(buff.begin()), buff.size() );
		fmt::print( "{}\n\n", sview );
	}
	auto riter1 = buff.getReadIter();
	ParserT parser1( riter1 );
	globalmq::marshalling::PublishableStateMessageHeader mh3;
	globalmq::marshalling::helperParsePublishableStateMessageBegin( parser1, mh3 );
	assert( mh1.type == mh3.type );
	assert( mh1.state_type_id_or_direction == mh3.state_type_id_or_direction );
	assert( mh1.priority == mh3.priority );
	assert( mh1.ref_id_at_subscriber == mh3.ref_id_at_subscriber );
	assert( mh1.ref_id_at_publisher == mh3.ref_id_at_publisher );

	buff.set_size(0);
	ComposerT composer2( buff );
	mh1.type = globalmq::marshalling::PublishableStateMessageHeader::MsgType::stateUpdate;
	globalmq::marshalling::helperComposePublishableStateMessageBegin( composer2, mh1 );
	globalmq::marshalling::impl::composeString( composer, "fake" );
	globalmq::marshalling::helperComposePublishableStateMessageEnd( composer2 );
	if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON )
	{
		std::string_view sview( reinterpret_cast<const char*>(buff.begin()), buff.size() );
		fmt::print( "{}\n\n", sview );
	}
	auto riter2 = buff.getReadIter();
	ParserT parser2( riter2 );
	globalmq::marshalling::PublishableStateMessageHeader mh4;
	globalmq::marshalling::helperParsePublishableStateMessageBegin( parser2, mh4 );
	assert( mh1.type == mh4.type );
	assert( mh1.state_type_id_or_direction == mh4.state_type_id_or_direction );
	assert( mh1.priority == mh4.priority );
	assert( mh1.ref_id_at_subscriber == mh4.ref_id_at_subscriber );
	assert( mh1.ref_id_at_publisher == mh4.ref_id_at_publisher );

	globalmq::marshalling::PublishableStateMessageHeader::UpdatedData ud;
	ud.ref_id_at_subscriber = 6661;
	ud.ref_id_at_publisher = 6662;
	ud.update_ref_id_at_subscriber = true;
	ud.update_ref_id_at_publisher = true;
//	ParserT parser3( buff );
	BufferT buff2;
	globalmq::marshalling::helperParseAndUpdatePublishableStateMessage<ParserT, ComposerT>( buff, buff2, ud );
	if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON )
	{
		std::string_view sview( reinterpret_cast<const char*>(buff2.begin()), buff2.size() );
		fmt::print( "{}\n\n", sview );
	}
	auto riter4 = buff2.getReadIter();
	ParserT parser4( riter4 );
	globalmq::marshalling::PublishableStateMessageHeader mh5;
	globalmq::marshalling::helperParsePublishableStateMessageBegin( parser4, mh5 );
	assert( mh1.type == mh5.type );
	assert( mh1.state_type_id_or_direction == mh5.state_type_id_or_direction );
	assert( mh1.priority == mh5.priority );
	assert( ud.ref_id_at_subscriber == mh5.ref_id_at_subscriber );
	assert( ud.ref_id_at_publisher == mh5.ref_id_at_publisher );


	// gmqueue
	GMQueue<GMQueueStatePublisherSubscriberTypeInfo> gmqueue;
	gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
	gmqueue.setAuthority( "" );

	using PostmanT = globalmq::marshalling::ThreadQueuePostman<GMQueueStatePublisherSubscriberTypeInfo::BufferT>;
	typename PostmanT::MsgQueue queue;

	BufferT msgBuff;
	ComposerT composer3( msgBuff );
	globalmq::marshalling::helperComposePublishableStateMessageBegin( composer3, mh1 );
//	globalmq::marshalling::impl::composeString( composer3, "fake" );
	globalmq::marshalling::helperComposePublishableStateMessageEnd( composer3 );
	if constexpr ( ComposerT::proto == globalmq::marshalling::Proto::JSON )
	{
		std::string_view sview( reinterpret_cast<const char*>(msgBuff.begin()), msgBuff.size() );
		fmt::print( "{}\n\n", sview );
	}

	globalmq::marshalling::SlotIdx idx;

	gmqueue.postMessage( std::move( msgBuff ), 0, idx );
}


template<typename StateT, typename NodeT>
concept has_count_call_member4 = requires { { std::declval<StateT>().count(std::declval<NodeT>()) }; };

using index_t = size_t&; 
template<typename StateT>
concept has_count_call_member5 = requires { { std::declval<StateT>().count2(std::declval<index_t>()) }; };

void testSubscriptionTest()
{
    struct bar
    {
		int * intptr;
		bool x;
        int count(int*) const { return 6; }
        int count2(int&) const { return 6; }
    };
    struct bar1
    {
		bool value;
        int count_() const { return 6; }
//        static int count() { return 6; }
        static void count(int*) { return; }
        void count2(size_t&) { return; }
    };

	constexpr bool has_4count = has_count_call_member4<bar, int*>;
	constexpr bool has_4count1 = has_count_call_member4<bar1, int*>;
	printf( "%s, %s\n", has_4count ? "Y" : "N", has_4count1 ? "Y" : "N" );

	constexpr bool has_5count = has_count_call_member5<bar>;
	constexpr bool has_5count1 = has_count_call_member5<bar1>;
	printf( "%s, %s\n", has_5count ? "Y" : "N", has_5count1 ? "Y" : "N" );


 //   printf( "foo(bar{}): : %d\n", foo(bar{}) );
}

void publishableTestOne()
{
	testCurrentNodeDataManipulation();
	testCallerValue();
//	return;
	SampleNode node;
	setCurrentNode( &node );
//	mtest::Buffer b;
//	typename StatePublisherSubscriberPoolInfo::ComposerT composer( b );
	mtest::publishable_sample_NodecppWrapperForPublisher<PublishableSample, MetaPoolT> publishableSampleWrapper( mp );

	fmt::print( "OK so far...\n" );

	globalmq::marshalling::GmqPathHelper::PathComponents pc;
	pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
	pc.authority = "";
	pc.nodeName = "myNode";
	pc.statePublisherOrConnectionType = mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample, MetaPoolT>::stringTypeID;
	GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose( pc );

	mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample, MetaPoolT> publishableSampleWrapperSlave( mp );
	publishableSampleWrapperSlave.subscribe( path );
	mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample_UPD, MetaPoolT> publishableSampleWrapperSlave2( mp, node );
	publishableSampleWrapperSlave2.subscribe( path );
	mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample_UPD_D, MetaPoolT> publishableSampleWrapperSlave3( mp, node );
	publishableSampleWrapperSlave3.subscribe( path );

	deliverMessages(); // simulate transport layer
	deliverMessages(); // simulate transport layer

//	assert( publishers.publishers.size() == 1 );
//	b.append( "\"msg_type\":3, \"subscriber_id\":0, \"update\":", 42 );
//	publishableSampleWrapper.resetComposer( &composer );

	// quick test for getting right after ctoring
	/**/int id = publishableSampleWrapper.get_ID();
	assert( id == 333 );
	fmt::print( "ID = {}\n", id );

	auto& size = publishableSampleWrapper.get_size();
	assert( size.X == 1.0 );
	assert( size.Y == 2.0 );
	assert( size.Z == 3.0 );
	fmt::print( "size = {}, {}, {}\n", size.X, size.Y, size.Z );
	publishableSampleWrapper.set_size( {901., 902., 903} );
	auto& size1 = publishableSampleWrapper.get_size();
	assert( size1.X == 901.0 );
	assert( size1.Y == 902.0 );
	assert( size1.Z == 903.0 );

	publishableSampleWrapper.get4set_chp().set_ID( 13 );

	CharacterParam chp( { 186, {55., 56., 57.}} );
	publishableSampleWrapper.set_chp( chp );

	auto voi = publishableSampleWrapper.get_vector_of_int();
	assert( voi.size() == 3 );
	fmt::print( "vector_of_int[0] = {}\n", voi.get_at(0) );
	auto vosp3d = publishableSampleWrapper.get_vector_struct_point3dreal();
	assert( vosp3d.size() == 2 );
	auto vosp3d_0 = vosp3d.get_at( 0 );
	assert( vosp3d_0.get_X() == 310 );
	assert( vosp3d_0.get_Y() == 311 );
	assert( vosp3d_0.get_Z() == 312 );
	fmt::print( "vector_of_int.size() = {}\n", vosp3d.size() );
	fmt::print( "vector_of_int[0] = {}, {}, {}\n", vosp3d_0.get_X(), vosp3d_0.get_Y(), vosp3d_0.get_Z() );

	// updating (some) values
	publishableSampleWrapper.set_ID( 38 );
	assert( publishableSampleWrapper.get_ID() == 38 );
	publishableSampleWrapper.set_vector_of_int({44,45,46});

	int ins1 = 17;
	publishableSampleWrapper.get4set_vector_of_int().insert_before( 0, ins1 );
	int ins1_back = publishableSampleWrapper.get_vector_of_int().get_at(0);
	assert( ins1 == ins1_back );
	POINT3DREAL point3dreal = {410, 411, 412};
	publishableSampleWrapper.get4set_vector_struct_point3dreal().insert_before( 1, point3dreal );
	auto point3dreal_back = publishableSampleWrapper.get_vector_struct_point3dreal().get_at( 1 );
	assert( point3dreal.X == point3dreal_back.get_X() );
	assert( point3dreal.Y == point3dreal_back.get_Y() );
	assert( point3dreal.Z == point3dreal_back.get_Z() );
	publishableSampleWrapper.get4set_vector_struct_point3dreal().get4set_at( 1 ).set_Y( 555 );
	auto point3dreal_Y_back = publishableSampleWrapper.get_vector_struct_point3dreal().get_at( 1 ).get_Y();
	assert( point3dreal_Y_back == 555 );

	std::vector<SIZE> sizes({{4,5,6},{7,8,9}});
	publishableSampleWrapper.get4set_structWithVectorOfSize().set_sizes( sizes );

	auto voint = publishableSampleWrapper.get_vector_of_int();
	assert( voint.size() == 4 );
	assert( voint.get_at( 0 ) == 17 );
	assert( voint.get_at( 1 ) == 44 );
	assert( voint.get_at( 2 ) == 45 );
	assert( voint.get_at( 3 ) == 46 );

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

// test incremental updating
//	typename StatePublisherSubscriberPoolInfo::ParserT parser( b );
//	subscribers.onMessage( parser );

	assert( publishableSampleWrapperSlave.get_ID() == 38 );
	auto& size1Slave = publishableSampleWrapperSlave.get_size();
	assert( size1Slave.X == 901.0 );
	assert( size1Slave.Y == 902.0 );
	assert( size1Slave.Z == 903.0 );

	auto& chpSlave = publishableSampleWrapper.get_chp();
	assert( memcmp( &chp, &chpSlave, sizeof(chp ) ) == 0 );

	// test whole state initializing
	/*mtest::Buffer b2;
	b2.append( "\"msg_type\":2, \"subscriber_id\":1, \"state\":", 41 );
	typename StatePublisherSubscriberPoolInfo::ComposerT composer2( b2 );
	publishableSampleWrapper.generateStateSyncMessage(composer2);
	std::string_view sview2( reinterpret_cast<const char*>(b2.begin()), b2.size() );
	fmt::print( "\n\n{}\n", sview2 );*/

//	typename StatePublisherSubscriberPoolInfo::ParserT parser2( b2 );
//	subscribers.onMessage( parser2 );

	assert( publishableSampleWrapperSlave2.get_ID() == 38 );
	auto& size1Slave2 = publishableSampleWrapperSlave2.get_size();
	assert( size1Slave2.X == 901.0 );
	assert( size1Slave2.Y == 902.0 );
	assert( size1Slave2.Z == 903.0 );

	auto& chpSlave2 = publishableSampleWrapper.get_chp();
	assert( memcmp( &chp, &chpSlave2, sizeof(chp ) ) == 0 );

	const auto& refSizes = publishableSampleWrapperSlave2.get_structWithVectorOfSize();
	assert( refSizes.sizes.size() == 2 );
	assert( refSizes.sizes[1].X == 7 );
	assert( refSizes.sizes[1].Y == 8 );
	assert( refSizes.sizes[1].Z == 9 );	

	// Part 2
	std::vector<SIZE> sizes2({{10,11,12},{13,14,15}});
	publishableSampleWrapper.get4set_structWithVectorOfSize().set_sizes( sizes2 );

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

	assert( refSizes.sizes.size() == 2 );
	assert( refSizes.sizes[1].X == 13 );
	assert( refSizes.sizes[1].Y == 14 );
	assert( refSizes.sizes[1].Z == 15 );	

	// Part 3
	std::vector<SIZE> sizes3;
	publishableSampleWrapper.get4set_structWithVectorOfSize().get4set_sizes().remove(0);

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

	assert( refSizes.sizes.size() == 1 );
	assert( refSizes.sizes[0].X == 13 );
	assert( refSizes.sizes[0].Y == 14 );
	assert( refSizes.sizes[0].Z == 15 );	

	// Part 4
	std::vector<SIZE> sizes4;
	publishableSampleWrapper.get4set_structWithVectorOfSize().set_sizes( sizes4 );

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

	assert( refSizes.sizes.size() == 0 );

	fmt::print( "=== Part 5 =======================\n" );

	// Part 5
	publishableSampleWrapper.get4set_chp().set_Size({1155., 1156., 1157.});

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

	auto& sizeSlave = publishableSampleWrapperSlave.get_chp().Size;
	assert( sizeSlave.X == 1155 );
	assert( sizeSlave.Y == 1156 );
	assert( sizeSlave.Z == 1157 );

	auto& sizeSlave2 = publishableSampleWrapperSlave2.get_chp().Size;
	assert( sizeSlave2.X == 1155 );
	assert( sizeSlave2.Y == 1156 );
	assert( sizeSlave2.Z == 1157 );

	auto& sizeSlave3 = publishableSampleWrapperSlave3.get_chp().Size;
	assert( sizeSlave3.X == 1155 );
	assert( sizeSlave3.Y == 1156 );
	assert( sizeSlave3.Z == 1157 );

	// Part 6
	publishableSampleWrapper.set_name( "def" );

	deliverMessages(); // simulate transport layer
	mp.postAllUpdates(); // simulate infrastructural call
	deliverMessages(); // simulate transport layer

	auto& nameSlave = publishableSampleWrapperSlave.get_name();
	assert( nameSlave == "def" );
	auto& nameSlave2 = publishableSampleWrapperSlave2.get_name();
	assert( nameSlave2 == "def" );
	auto& nameSlave3 = publishableSampleWrapperSlave3.get_name();
	assert( nameSlave3 == "def" );
	
	setCurrentNode( nullptr );
}

// no notifiers

struct HtmlNodeSample
{
	mtest::HtmlNode node;
};

// version with notifiers: we have to add them

struct Property
{
	GMQ_COLL string name = "ini_name";
	GMQ_COLL string value = "ini_value";

	void notifyUpdated_value() const { assert( getCurrentNode() != nullptr ); /*getCurrentNode()->addPreAccess();*/ fmt::print( "Property::notifyUpdated_value()\n" ); }
};

class HtmlTextOrNodes : public mtest::HtmlTextOrNodes
{
public:
	SampleNode& node;
	void notifyUpdated_currentVariant() const { assert( getCurrentNode() != nullptr ); /*getCurrentNode()->addPreAccess();*/ fmt::print( "HtmlTextOrNodes::notifyUpdated_currentVariant()\n" ); }
	void notifyUpdated_text_() const { assert( getCurrentNode() != nullptr ); /*getCurrentNode()->addPreAccess();*/ fmt::print( "HtmlTextOrNodes::notifyUpdated_text_()\n" ); }
	void notifyUpdated_nodes_() const { assert( getCurrentNode() != nullptr ); /*getCurrentNode()->addPreAccess();*/ fmt::print( "HtmlTextOrNodes::notifyUpdated_nodes_()\n" ); }
};

struct HtmlNode
{
	GMQ_COLL vector<Property> properties;
	HtmlTextOrNodes nodes;
};

struct HtmlNodeSampleWithNotifiers
{
	mtest::HtmlNode node;

	SampleNode& processingNode;
	HtmlNodeSampleWithNotifiers( SampleNode& node_ ) : processingNode( node_ ) {}
};

void publishableTestTwo()
{
	testCurrentNodeDataManipulation();
	testCallerValue();

	SampleNode node;
	setCurrentNode( &node );

	// gmqueue
	using BufferT = GMQueueStatePublisherSubscriberTypeInfo::BufferT;
	using ComposerT = GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	using ParserT = GMQueueStatePublisherSubscriberTypeInfo::ParserT;

	GMQueue<StatePublisherSubscriberPoolInfo> gmqueue;
	gmqueue.template initStateConcentratorFactory<mtest::StateConcentratorFactory<BufferT, ComposerT>>();
	gmqueue.setAuthority( "" );

	using PostmanT = globalmq::marshalling::ThreadQueuePostman<StatePublisherSubscriberPoolInfo::BufferT>;
	typename PostmanT::MsgQueue queue;

	globalmq::marshalling::GMQThreadQueueTransport<StatePublisherSubscriberPoolInfo> transport( gmqueue, "myNode", queue, 1 );
	mp.setTransport( &transport );


		

	fmt::print( "Testing publishing/subscribing...\n" );

	mtest::publishable_html_node_NodecppWrapperForPublisher<HtmlNodeSample, MetaPoolT> htmlNodeWrapper( mp );

	globalmq::marshalling::GmqPathHelper::PathComponents pc;
	pc.type = PublishableStateMessageHeader::MsgType::subscriptionRequest;
	pc.authority = "";
	pc.nodeName = "myNode";
	pc.statePublisherOrConnectionType = mtest::publishable_html_node_NodecppWrapperForSubscriber<HtmlNodeSample, MetaPoolT>::stringTypeID;
	GMQ_COLL string path = globalmq::marshalling::GmqPathHelper::compose( pc );

	mtest::publishable_html_node_NodecppWrapperForSubscriber<HtmlNodeSample, MetaPoolT> htmlNodeWrapperSlave( mp );
	htmlNodeWrapperSlave.subscribe( path );
	mtest::publishable_html_node_NodecppWrapperForSubscriber<HtmlNodeSampleWithNotifiers, MetaPoolT> htmlNodeWrapperSlave2( mp, node );
	htmlNodeWrapperSlave2.subscribe( path );
	mtest::publishable_html_node_NodecppWrapperForSubscriber<HtmlNodeSampleWithNotifiers, MetaPoolT> htmlNodeWrapperSlave3( mp, node );
	htmlNodeWrapperSlave3.subscribe( path );

	mp.postAllUpdates();
	constexpr size_t maxMsg = 1000;
	ThreadQueueItem<StatePublisherSubscriberPoolInfo::BufferT> messages[maxMsg];
	size_t msgCnt = queue.pop_front( messages, maxMsg, 0 );
	for ( size_t i=0; i<msgCnt; ++i )
	{
		fmt::print( "msg = \"{}\"\n", messages[i].msg.begin() );
		mp.onMessage( messages[i].msg );
	}

//	deliverMessages(); // simulate transport layer
//	deliverMessages(); // simulate transport layer

	// quick test for getting right after ctoring
	auto& hn = htmlNodeWrapper.get_node();
	assert( hn.nodes.currentVariant() == mtest::HtmlTextOrNodes::Variants::unknown );

	auto g4s_hn = htmlNodeWrapper.get4set_node();
	auto g4s_hn_nodes = g4s_hn.get4set_nodes();
	g4s_hn_nodes.set_currentVariant( mtest::HtmlTextOrNodes::Variants::text );
	g4s_hn_nodes.set_str( "abc" );

	mp.postAllUpdates();
	msgCnt = queue.pop_front( messages, maxMsg, 0 );
	for ( size_t i=0; i<msgCnt; ++i )
	{
		fmt::print( "msg = \"{}\"\n", messages[i].msg.begin() );
		mp.onMessage( messages[i].msg );
	}


}
