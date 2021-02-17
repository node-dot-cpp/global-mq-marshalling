
#include <stdio.h>

#include "nodecpp_publish_subscribe_support.h"
#include <publishable_impl.h>
#include "marshalling/test_marshalling.h"

#include <typeinfo>
#include <typeindex>
#include <unordered_map>

thread_local globalmq::marshalling::PublisherPool<PublisherSubscriberRegistrar> publishers;
thread_local globalmq::marshalling::SubscriberPool<PublisherSubscriberRegistrar> subscribers;

// transporting level simulation (for this test, single-threaded)
GMQ_COLL vector<globalmq::marshalling::Buffer> likeQueues[2];
void deliverMessages()
{
	using ParserT = typename PublisherSubscriberRegistrar::ParserT;

	for ( size_t i=0; i<likeQueues[publisherPoolAddress].size(); ++i )
	{
		ParserT parser( likeQueues[publisherPoolAddress][i] );
		publishers.onMessage( parser, subscriberPoolAddress );
	}
	likeQueues[publisherPoolAddress].clear();

	for ( size_t i=0; i<likeQueues[subscriberPoolAddress].size(); ++i )
	{
		ParserT parser( likeQueues[subscriberPoolAddress][i] );
		subscribers.onMessage( parser );
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


class SampleNode
{
public:
	size_t preCtr = 0;
	size_t postCtr = 0;
	SampleNode() {}
	void addPreAccess() {++preCtr;}
	void addPostAccess() {++postCtr;}
};

struct SIZE{
	double X = 1.0;
	double Y = 2.0;
	double Z = 3.0;
	void prenotifyBefore_X( SampleNode* node_ ) { node_->addPreAccess(); printf( "SIZE::notifyBefore_X()\n" ); }
	void notifyAfter_X() { /*node_->addPreAccess();*/ printf( "SIZE::notifyAfter_X()\n" ); }
	void prenotifyBefore_Y( SampleNode* node_ ) { node_->addPreAccess(); printf( "SIZE::notifyBefore_Y()\n" ); }
	void notifyAfter_Y() { /*node_->addPreAccess();*/ printf( "SIZE::notifyAfter_Y()\n" ); }
	void prenotifyBefore_Z( SampleNode* node_ ) { node_->addPreAccess(); printf( "SIZE::notifyBefore_Z()\n" ); }
	void notifyAfter_Z() { /*node_->addPreAccess();*/ printf( "SIZE::notifyAfter_Z()\n" ); }
};

struct POINT3DREAL{
  double X = 4.0;
  double Y = 5.0;
  double Z = 6.0;
	void prenotifyBefore_X( SampleNode* node_ ) { node_->addPreAccess(); printf( "POINT3DREAL::notifyBefore_X()\n" ); }
	void notifyAfter_X() { /*node_->addPreAccess();*/ printf( "POINT3DREAL::notifyAfter_X()\n" ); }
	void prenotifyBefore_Y( SampleNode* node_ ) { node_->addPreAccess(); printf( "POINT3DREAL::notifyBefore_Y()\n" ); }
	void notifyAfter_Y() { /*node_->addPreAccess();*/ printf( "POINT3DREAL::notifyAfter_Y()\n" ); }
	void prenotifyBefore_Z( SampleNode* node_ ) { node_->addPreAccess(); printf( "POINT3DREAL::notifyBefore_Z()\n" ); }
	void notifyAfter_Z() { /*node_->addPreAccess();*/ printf( "POINT3DREAL::notifyAfter_Z()\n" ); }
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

	void prenotifyBefore_ID( SampleNode* node_ ) { node_->addPreAccess(); printf( "CharacterParam::notifyBefore_ID()\n" ); }
	void notifyAfter_ID() { /*node_->addPreAccess();*/ printf( "CharacterParam::notifyAfter_ID()\n" ); }
	void prenotifyBefore_Size( SampleNode* node_ ) { node_->addPreAccess(); printf( "CharacterParam::notifyBeforer_ID()\n" ); }
	void notifyAfter_Size() { /*node_->addPreAccess();*/ printf( "CharacterParam::notifyAfter_Size()\n" ); }
};

struct PublishableSample {
	SampleNode* node = nullptr;
	int ID = 333;
	SIZE size;
	CharacterParam chp;
	std::vector<POINT3DREAL> vector_struct_point3dreal = {{310, 311, 312}, {320, 321, 322}};
	std::vector<int> vector_of_int = {111, 112, 113};
	StructWithVectorOfInt structWithVectorOfInt;
	StructWithVectorOfSize structWithVectorOfSize;

	PublishableSample( SampleNode* node_ ) { node = node_; }

	void notifyBefore_ID() { /*node_->addPreAccess();*/ printf( "PublishableSample::notifyBefore_ID()\n" ); }
	void notifyAfter_ID() { /*node_->addPreAccess();*/ printf( "PublishableSample::notifyAfter_ID()\n" ); }
};

void publishableTestOne()
{
	testCurrentNodeDataManipulation();
	testCallerValue();
//	return;
	SampleNode node;
	mtest::Buffer b;
	typename PublisherSubscriberRegistrar::ComposerT composer( b );
	mtest::publishable_sample_NodecppWrapperForPublisher<PublishableSample> publishableSampleWrapper( &node );

	assert( publishers.publishers.size() == 1 );
	b.append( "\"msg_type\":3, \"subscriber_id\":0, \"update\":", 42 );
	publishableSampleWrapper.resetComposer( &composer );

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

//	publishableSampleWrapper.finalizeComposing();
	publishableSampleWrapper.finalizeComposing();
	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	auto voint = publishableSampleWrapper.get_vector_of_int();
	assert( voint.size() == 4 );
	assert( voint.get_at( 0 ) == 17 );
	assert( voint.get_at( 1 ) == 44 );
	assert( voint.get_at( 2 ) == 45 );
	assert( voint.get_at( 3 ) == 46 );

	mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample> publishableSampleWrapperSlave( &node );
	mtest::publishable_sample_NodecppWrapperForSubscriber<PublishableSample> publishableSampleWrapperSlave2( &node );

	// test incremental updating
	typename PublisherSubscriberRegistrar::ParserT parser( b );
	subscribers.onMessage( parser );

	assert( publishableSampleWrapperSlave.get_ID() == 38 );
	auto& size1Slave = publishableSampleWrapperSlave.get_size();
	assert( size1Slave.X == 901.0 );
	assert( size1Slave.Y == 902.0 );
	assert( size1Slave.Z == 903.0 );

	auto& chpSlave = publishableSampleWrapper.get_chp();
	assert( memcmp( &chp, &chpSlave, sizeof(chp ) ) == 0 );

	// test whole state initializing
	mtest::Buffer b2;
	b2.append( "\"msg_type\":2, \"subscriber_id\":1, \"state\":", 41 );
	typename PublisherSubscriberRegistrar::ComposerT composer2( b2 );
	publishableSampleWrapper.generateStateSyncMessage(composer2);
	std::string_view sview2( reinterpret_cast<const char*>(b2.begin()), b2.size() );
	fmt::print( "\n\n{}\n", sview2 );

	typename PublisherSubscriberRegistrar::ParserT parser2( b2 );
	subscribers.onMessage( parser2 );

	assert( publishableSampleWrapperSlave2.get_ID() == 38 );
	auto& size1Slave2 = publishableSampleWrapperSlave2.get_size();
	assert( size1Slave2.X == 901.0 );
	assert( size1Slave2.Y == 902.0 );
	assert( size1Slave2.Z == 903.0 );

	auto& chpSlave2 = publishableSampleWrapper.get_chp();
	assert( memcmp( &chp, &chpSlave2, sizeof(chp ) ) == 0 );
}
