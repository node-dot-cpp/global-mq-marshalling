#ifndef _test_marshalling_h_f36bc8bd_guard
#define _test_marshalling_h_f36bc8bd_guard

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
//  scope_one
//  {
//    point3D
//    point3D_alias
//    point_alias
//  }
//
//  level_trace
//  {
//    LevelTraceData
//  }
//
//  infrastructural
//  {
//    PolygonSt
//    point
//    point3D
//  }
//
//  test_gmq
//  {
//    message_one
//  }
//
//  test_json
//  {
//    message_one
//  }
//
//  level_data
//  {
//    Level
//  }
//
//////////////////////////////////////////////////////////////

using CharacterParam_Type = NamedParameter<struct CharacterParam_Struct>;
using ID_Type = NamedParameter<struct ID_Struct>;
using Level_Type = NamedParameter<struct Level_Struct>;
using LineMap_Type = NamedParameter<struct LineMap_Struct>;
using Points_Type = NamedParameter<struct Points_Struct>;
using Size_Type = NamedParameter<struct Size_Struct>;
using X_Type = NamedParameter<struct X_Struct>;
using Y_Type = NamedParameter<struct Y_Struct>;
using Z_Type = NamedParameter<struct Z_Struct>;
using _ObstacleMap_Type = NamedParameter<struct _ObstacleMap_Struct>;
using _PolygonMap_Type = NamedParameter<struct _PolygonMap_Struct>;
using a_Type = NamedParameter<struct a_Struct>;
using animSpeed_Type = NamedParameter<struct animSpeed_Struct>;
using animVector_Type = NamedParameter<struct animVector_Struct>;
using animZones_Type = NamedParameter<struct animZones_Struct>;
using b_Type = NamedParameter<struct b_Struct>;
using concaveMap_Type = NamedParameter<struct concaveMap_Struct>;
using du_one_instance_Type = NamedParameter<struct du_one_instance_Struct>;
using eighthParam_Type = NamedParameter<struct eighthParam_Struct>;
using fifthParam_Type = NamedParameter<struct fifthParam_Struct>;
using firstParam_Type = NamedParameter<struct firstParam_Struct>;
using forthParam_Type = NamedParameter<struct forthParam_Struct>;
using i_1_Type = NamedParameter<struct i_1_Struct>;
using i_2_Type = NamedParameter<struct i_2_Struct>;
using jumpMap_Type = NamedParameter<struct jumpMap_Struct>;
using ninethParam_Type = NamedParameter<struct ninethParam_Struct>;
using num2stringDictionary_Type = NamedParameter<struct num2stringDictionary_Struct>;
using obstacleMap_Type = NamedParameter<struct obstacleMap_Struct>;
using point_Type = NamedParameter<struct point_Struct>;
using polygonMap_Type = NamedParameter<struct polygonMap_Struct>;
using polygonSpeed_Type = NamedParameter<struct polygonSpeed_Struct>;
using portalMap_Type = NamedParameter<struct portalMap_Struct>;
using pt_Type = NamedParameter<struct pt_Struct>;
using pt3d_1_Type = NamedParameter<struct pt3d_1_Struct>;
using secondParam_Type = NamedParameter<struct secondParam_Struct>;
using seventhParam_Type = NamedParameter<struct seventhParam_Struct>;
using sixthParam_Type = NamedParameter<struct sixthParam_Struct>;
using tenthParam_Type = NamedParameter<struct tenthParam_Struct>;
using thirdParam_Type = NamedParameter<struct thirdParam_Struct>;
using vp_2_Type = NamedParameter<struct vp_2_Struct>;
using x_Type = NamedParameter<struct x_Struct>;
using y_Type = NamedParameter<struct y_Struct>;
using z_Type = NamedParameter<struct z_Struct>;

constexpr CharacterParam_Type::TypeConverter CharacterParam;
constexpr ID_Type::TypeConverter ID;
constexpr Level_Type::TypeConverter Level;
constexpr LineMap_Type::TypeConverter LineMap;
constexpr Points_Type::TypeConverter Points;
constexpr Size_Type::TypeConverter Size;
constexpr X_Type::TypeConverter X;
constexpr Y_Type::TypeConverter Y;
constexpr Z_Type::TypeConverter Z;
constexpr _ObstacleMap_Type::TypeConverter _ObstacleMap;
constexpr _PolygonMap_Type::TypeConverter _PolygonMap;
constexpr a_Type::TypeConverter a;
constexpr animSpeed_Type::TypeConverter animSpeed;
constexpr animVector_Type::TypeConverter animVector;
constexpr animZones_Type::TypeConverter animZones;
constexpr b_Type::TypeConverter b;
constexpr concaveMap_Type::TypeConverter concaveMap;
constexpr du_one_instance_Type::TypeConverter du_one_instance;
constexpr eighthParam_Type::TypeConverter eighthParam;
constexpr fifthParam_Type::TypeConverter fifthParam;
constexpr firstParam_Type::TypeConverter firstParam;
constexpr forthParam_Type::TypeConverter forthParam;
constexpr i_1_Type::TypeConverter i_1;
constexpr i_2_Type::TypeConverter i_2;
constexpr jumpMap_Type::TypeConverter jumpMap;
constexpr ninethParam_Type::TypeConverter ninethParam;
constexpr num2stringDictionary_Type::TypeConverter num2stringDictionary;
constexpr obstacleMap_Type::TypeConverter obstacleMap;
constexpr point_Type::TypeConverter point;
constexpr polygonMap_Type::TypeConverter polygonMap;
constexpr polygonSpeed_Type::TypeConverter polygonSpeed;
constexpr portalMap_Type::TypeConverter portalMap;
constexpr pt_Type::TypeConverter pt;
constexpr pt3d_1_Type::TypeConverter pt3d_1;
constexpr secondParam_Type::TypeConverter secondParam;
constexpr seventhParam_Type::TypeConverter seventhParam;
constexpr sixthParam_Type::TypeConverter sixthParam;
constexpr tenthParam_Type::TypeConverter tenthParam;
constexpr thirdParam_Type::TypeConverter thirdParam;
constexpr vp_2_Type::TypeConverter vp_2;
constexpr x_Type::TypeConverter x;
constexpr y_Type::TypeConverter y;
constexpr z_Type::TypeConverter z;


// member name presence checkers
template<typename T> concept has_ID_member = requires { { T::ID }; };
template<typename T> concept has_NN_member = requires { { T::NN }; };
template<typename T> concept has_Size_member = requires { { T::Size }; };
template<typename T> concept has_X_member = requires { { T::X }; };
template<typename T> concept has_Y_member = requires { { T::Y }; };
template<typename T> concept has_Z_member = requires { { T::Z }; };
template<typename T> concept has_chp_member = requires { { T::chp }; };
template<typename T> concept has_du_one_instance_member = requires { { T::du_one_instance }; };
template<typename T> concept has_i_1_member = requires { { T::i_1 }; };
template<typename T> concept has_i_2_member = requires { { T::i_2 }; };
template<typename T> concept has_name_member = requires { { T::name }; };
template<typename T> concept has_properties_member = requires { { T::properties }; };
template<typename T> concept has_pt3d_1_member = requires { { T::pt3d_1 }; };
template<typename T> concept has_signedInts_member = requires { { T::signedInts }; };
template<typename T> concept has_size_member = requires { { T::size }; };
template<typename T> concept has_sizes_member = requires { { T::sizes }; };
template<typename T> concept has_str_member = requires { { T::str }; };
template<typename T> concept has_structWithVectorOfInt_member = requires { { T::structWithVectorOfInt }; };
template<typename T> concept has_structWithVectorOfSize_member = requires { { T::structWithVectorOfSize }; };
template<typename T> concept has_tag_member = requires { { T::tag }; };
template<typename T> concept has_tags_member = requires { { T::tags }; };
template<typename T> concept has_vector_of_int_member = requires { { T::vector_of_int }; };
template<typename T> concept has_vector_struct_point3dreal_member = requires { { T::vector_struct_point3dreal }; };
template<typename T> concept has_vp_2_member = requires { { T::vp_2 }; };
template<typename T> concept has_x_member = requires { { T::x }; };
template<typename T> concept has_y_member = requires { { T::y }; };
template<typename T> concept has_z_member = requires { { T::z }; };


// member update notifier presence checks
using index_type_for_array_notifiers = size_t&;
template<typename T> concept has_full_update_notifier_call = requires(T t) { { t.notifyFullyUpdated() }; };
template<typename T> concept has_void_update_notifier_call_for_ID = requires(T t) { { t.notifyUpdated_ID() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_ID = requires { { std::declval<StateT>().notifyUpdated_ID(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_NN = requires(T t) { { t.notifyUpdated_NN() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_NN = requires { { std::declval<StateT>().notifyUpdated_NN(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_Size = requires(T t) { { t.notifyUpdated_Size() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_Size = requires { { std::declval<StateT>().notifyUpdated_Size(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_X = requires(T t) { { t.notifyUpdated_X() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_X = requires { { std::declval<StateT>().notifyUpdated_X(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_Y = requires(T t) { { t.notifyUpdated_Y() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_Y = requires { { std::declval<StateT>().notifyUpdated_Y(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_Z = requires(T t) { { t.notifyUpdated_Z() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_Z = requires { { std::declval<StateT>().notifyUpdated_Z(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_chp = requires(T t) { { t.notifyUpdated_chp() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_chp = requires { { std::declval<StateT>().notifyUpdated_chp(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_currentVariant = requires(T t) { { t.notifyUpdated_currentVariant() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_currentVariant = requires { { std::declval<StateT>().notifyUpdated_currentVariant(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_du_one_instance = requires(T t) { { t.notifyUpdated_du_one_instance() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_du_one_instance = requires { { std::declval<StateT>().notifyUpdated_du_one_instance(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_i_1 = requires(T t) { { t.notifyUpdated_i_1() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_i_1 = requires { { std::declval<StateT>().notifyUpdated_i_1(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_i_2 = requires(T t) { { t.notifyUpdated_i_2() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_i_2 = requires { { std::declval<StateT>().notifyUpdated_i_2(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_name = requires(T t) { { t.notifyUpdated_name() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_name = requires { { std::declval<StateT>().notifyUpdated_name(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_properties = requires(T t) { { t.notifyUpdated_properties() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_properties = requires { { std::declval<StateT>().notifyUpdated_properties(std::declval<MemberT>()) }; };
	//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!
template<typename T> concept has_void_update_notifier_call_for_pt3d_1 = requires(T t) { { t.notifyUpdated_pt3d_1() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_pt3d_1 = requires { { std::declval<StateT>().notifyUpdated_pt3d_1(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_signedInts = requires(T t) { { t.notifyUpdated_signedInts() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_signedInts = requires { { std::declval<StateT>().notifyUpdated_signedInts(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_signedInts = requires(T t) { { t.notifyElementUpdated_signedInts() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_signedInts = requires { { std::declval<StateT>().notifyElementUpdated_signedInts(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_signedInts = requires { { std::declval<StateT>().notifyElementUpdated_signedInts(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_signedInts = requires(T t) { { t.notifyInserted_signedInts() }; };
template<typename StateT> concept has_insert_notifier_call2_for_signedInts = requires { { std::declval<StateT>().notifyInserted_signedInts(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_signedInts = requires { { std::declval<StateT>().notifyInserted_signedInts(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_signedInts = requires(T t) { { t.notifyErased_signedInts() }; };
template<typename StateT> concept has_erased_notifier_call2_for_signedInts = requires { { std::declval<StateT>().notifyErased_signedInts(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_signedInts = requires { { std::declval<StateT>().notifyErased_signedInts(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_size = requires(T t) { { t.notifyUpdated_size() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_size = requires { { std::declval<StateT>().notifyUpdated_size(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_sizes = requires(T t) { { t.notifyUpdated_sizes() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_sizes = requires { { std::declval<StateT>().notifyUpdated_sizes(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_sizes = requires(T t) { { t.notifyElementUpdated_sizes() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_sizes = requires { { std::declval<StateT>().notifyElementUpdated_sizes(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_sizes = requires { { std::declval<StateT>().notifyElementUpdated_sizes(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_sizes = requires(T t) { { t.notifyInserted_sizes() }; };
template<typename StateT> concept has_insert_notifier_call2_for_sizes = requires { { std::declval<StateT>().notifyInserted_sizes(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_sizes = requires { { std::declval<StateT>().notifyInserted_sizes(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_sizes = requires(T t) { { t.notifyErased_sizes() }; };
template<typename StateT> concept has_erased_notifier_call2_for_sizes = requires { { std::declval<StateT>().notifyErased_sizes(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_sizes = requires { { std::declval<StateT>().notifyErased_sizes(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_str = requires(T t) { { t.notifyUpdated_str() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_str = requires { { std::declval<StateT>().notifyUpdated_str(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_structWithVectorOfInt = requires(T t) { { t.notifyUpdated_structWithVectorOfInt() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_structWithVectorOfInt = requires { { std::declval<StateT>().notifyUpdated_structWithVectorOfInt(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_structWithVectorOfSize = requires(T t) { { t.notifyUpdated_structWithVectorOfSize() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_structWithVectorOfSize = requires { { std::declval<StateT>().notifyUpdated_structWithVectorOfSize(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_tag = requires(T t) { { t.notifyUpdated_tag() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_tag = requires { { std::declval<StateT>().notifyUpdated_tag(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_tags = requires(T t) { { t.notifyUpdated_tags() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_tags = requires { { std::declval<StateT>().notifyUpdated_tags(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_tags = requires(T t) { { t.notifyElementUpdated_tags() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_tags = requires { { std::declval<StateT>().notifyElementUpdated_tags(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_tags = requires { { std::declval<StateT>().notifyElementUpdated_tags(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_tags = requires(T t) { { t.notifyInserted_tags() }; };
template<typename StateT> concept has_insert_notifier_call2_for_tags = requires { { std::declval<StateT>().notifyInserted_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_tags = requires { { std::declval<StateT>().notifyInserted_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_tags = requires(T t) { { t.notifyErased_tags() }; };
template<typename StateT> concept has_erased_notifier_call2_for_tags = requires { { std::declval<StateT>().notifyErased_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_tags = requires { { std::declval<StateT>().notifyErased_tags(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_vector_of_int = requires(T t) { { t.notifyUpdated_vector_of_int() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_vector_of_int = requires { { std::declval<StateT>().notifyUpdated_vector_of_int(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_vector_of_int = requires(T t) { { t.notifyElementUpdated_vector_of_int() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_vector_of_int = requires { { std::declval<StateT>().notifyElementUpdated_vector_of_int(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_vector_of_int = requires { { std::declval<StateT>().notifyElementUpdated_vector_of_int(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_vector_of_int = requires(T t) { { t.notifyInserted_vector_of_int() }; };
template<typename StateT> concept has_insert_notifier_call2_for_vector_of_int = requires { { std::declval<StateT>().notifyInserted_vector_of_int(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_vector_of_int = requires { { std::declval<StateT>().notifyInserted_vector_of_int(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_vector_of_int = requires(T t) { { t.notifyErased_vector_of_int() }; };
template<typename StateT> concept has_erased_notifier_call2_for_vector_of_int = requires { { std::declval<StateT>().notifyErased_vector_of_int(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_vector_of_int = requires { { std::declval<StateT>().notifyErased_vector_of_int(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_vector_struct_point3dreal = requires(T t) { { t.notifyUpdated_vector_struct_point3dreal() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyUpdated_vector_struct_point3dreal(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_vector_struct_point3dreal = requires(T t) { { t.notifyElementUpdated_vector_struct_point3dreal() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyElementUpdated_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyElementUpdated_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_vector_struct_point3dreal = requires(T t) { { t.notifyInserted_vector_struct_point3dreal() }; };
template<typename StateT> concept has_insert_notifier_call2_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyInserted_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyInserted_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_vector_struct_point3dreal = requires(T t) { { t.notifyErased_vector_struct_point3dreal() }; };
template<typename StateT> concept has_erased_notifier_call2_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyErased_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_vector_struct_point3dreal = requires { { std::declval<StateT>().notifyErased_vector_struct_point3dreal(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_vp_2 = requires(T t) { { t.notifyUpdated_vp_2() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_vp_2 = requires { { std::declval<StateT>().notifyUpdated_vp_2(std::declval<MemberT>()) }; };
template<typename T> concept has_element_updated_void_notifier_call_for_vp_2 = requires(T t) { { t.notifyElementUpdated_vp_2() }; };
template<typename StateT> concept has_element_updated_notifier_call_for_vp_2 = requires { { std::declval<StateT>().notifyElementUpdated_vp_2(std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_full_element_updated_notifier_call_for_vp_2 = requires { { std::declval<StateT>().notifyElementUpdated_vp_2(std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_insert_notifier_call_for_vp_2 = requires(T t) { { t.notifyInserted_vp_2() }; };
template<typename StateT> concept has_insert_notifier_call2_for_vp_2 = requires { { std::declval<StateT>().notifyInserted_vp_2(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_insert_notifier_call3_for_vp_2 = requires { { std::declval<StateT>().notifyInserted_vp_2(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()), std::declval<MemberT>() }; };
template<typename T> concept has_void_erased_notifier_call_for_vp_2 = requires(T t) { { t.notifyErased_vp_2() }; };
template<typename StateT> concept has_erased_notifier_call2_for_vp_2 = requires { { std::declval<StateT>().notifyErased_vp_2(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>()) }; };
template<typename StateT, typename MemberT> concept has_erased_notifier_call3_for_vp_2 = requires { { std::declval<StateT>().notifyErased_vp_2(std::declval<index_type_for_array_notifiers>(), std::declval<index_type_for_array_notifiers>(), std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_x = requires(T t) { { t.notifyUpdated_x() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_x = requires { { std::declval<StateT>().notifyUpdated_x(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_y = requires(T t) { { t.notifyUpdated_y() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_y = requires { { std::declval<StateT>().notifyUpdated_y(std::declval<MemberT>()) }; };
template<typename T> concept has_void_update_notifier_call_for_z = requires(T t) { { t.notifyUpdated_z() }; };
template<typename StateT, typename MemberT> concept has_update_notifier_call_for_z = requires { { std::declval<StateT>().notifyUpdated_z(std::declval<MemberT>()) }; };

//===============================================================================
// C-structures for idl STRUCTs, DISCRIMINATED_UNIONs, MESSAGEs and PUBLISHABLEs

namespace structures {

struct StructWithVectorOfInt;
struct StructWithVectorOfSize;
struct CharacterParamStruct;
struct SIZE;
struct POINT3DREAL;
struct LineMap;
struct Line;
struct ObstacleMap;
struct PolygonMap;
struct Vertex;
struct point;
struct point3D;
class du_one;
struct HtmlTag;
class HtmlTextOrTags;
struct PolygonSt;
struct AnimZone;
struct Line_;

struct AnimZone
{
	GMQ_COLL vector<Line_> animVector;
	GMQ_COLL vector<double> animSpeed;
};

class HtmlTextOrTags : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { text=21, taglists=22, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_text
	{
		GMQ_COLL string str;
	};

	struct Case_taglists
	{
		GMQ_COLL vector<HtmlTag> tags;
	};

public:
	using Case_text_str_T = decltype( Case_text::str );
	using Case_taglists_tags_T = decltype( Case_taglists::tags );

private:
	static constexpr size_t text_taglists_memsz = sizeof( Case_taglists ) > ( sizeof( Case_text ) ) ? sizeof( Case_taglists ) : ( sizeof( Case_text ) );
	uint8_t text_taglists_mem[text_taglists_memsz];
	void implDeinit() {
		if ( v != Variants::unknown ) // then destruct existing value
		{
			switch ( v )
			{
				case Variants::text: reinterpret_cast<Case_text*>( text_taglists_mem ) -> ~Case_text(); break;
				case Variants::taglists: reinterpret_cast<Case_taglists*>( text_taglists_mem ) -> ~Case_taglists(); break;
			}
			v = Variants::unknown;
		}
	}

	void implCopyFrom( const HtmlTextOrTags& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::text:
				new ( text_taglists_mem ) Case_text( *reinterpret_cast<const Case_text*>( other.text_taglists_mem ) );
				break;
			case Variants::taglists:
				new ( text_taglists_mem ) Case_taglists( *reinterpret_cast<const Case_taglists*>( other.text_taglists_mem ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
	}

	void implMoveFrom( HtmlTextOrTags&& other ) {
		if ( v != other.v )
			implDeinit();
		switch ( other.v )
		{
			case Variants::text:
				new ( text_taglists_mem ) Case_text( std::move( *reinterpret_cast<Case_text*>( other.text_taglists_mem ) ) );
				break;
			case Variants::taglists:
				new ( text_taglists_mem ) Case_taglists( std::move( *reinterpret_cast<Case_taglists*>( other.text_taglists_mem ) ) );
				break;
			case Variants::unknown: break;
		}
		v = other.v;
		other.v = Variants::unknown;
	}

public:
	HtmlTextOrTags() {}
	HtmlTextOrTags( const HtmlTextOrTags &other ) {
		implCopyFrom( other );
	}
	HtmlTextOrTags& operator = ( const HtmlTextOrTags &other) {
		implCopyFrom( other );
		return *this;
	}
	HtmlTextOrTags( HtmlTextOrTags&& other) noexcept {
		implMoveFrom( std::move( other ) );
	}
	HtmlTextOrTags& operator = ( HtmlTextOrTags&& other) noexcept {
		implMoveFrom( std::move( other ) );
		return *this;
	}
	virtual ~HtmlTextOrTags() {
		implDeinit();
	}
	Variants currentVariant() const { return v; }
	void initAs( Variants v_ ) {
		implDeinit();
		switch ( v_ ) // init for a new type
		{
			case Variants::text: new ( text_taglists_mem ) Case_text; break;
			case Variants::taglists: new ( text_taglists_mem ) Case_taglists; break;
		}
		v = v_;
	}

	// IDL CASE text:
	Case_text_str_T& str() {
		if ( v != Variants::text )
			throw std::exception();
		return reinterpret_cast<Case_text*>( text_taglists_mem )->str;
	}
	const Case_text_str_T& str() const {
		if ( v != Variants::text )
			throw std::exception();
		return reinterpret_cast<const Case_text*>( text_taglists_mem )->str;
	}
	

	// IDL CASE taglists:
	Case_taglists_tags_T& tags() {
		if ( v != Variants::taglists )
			throw std::exception();
		return reinterpret_cast<Case_taglists*>( text_taglists_mem )->tags;
	}
	const Case_taglists_tags_T& tags() const {
		if ( v != Variants::taglists )
			throw std::exception();
		return reinterpret_cast<const Case_taglists*>( text_taglists_mem )->tags;
	}
	
};

struct point3D
{
	int64_t x;
	int64_t y;
	int64_t z;
};

struct Vertex
{
	int64_t x;
	int64_t y;
	int64_t z;
};

struct SIZE
{
	double X;
	double Y;
	double Z;
};

struct Line_
{
	Vertex a;
	Vertex b;
};

struct PolygonSt
{
	GMQ_COLL vector<Line_> portalMap;
	AnimZone animZones;
};

struct HtmlTag
{
	GMQ_COLL string name;
	GMQ_COLL unordered_map<GMQ_COLL string,GMQ_COLL string> properties;
	HtmlTextOrTags tags;
};

class du_one : public ::globalmq::marshalling::impl::DiscriminatedUnionType
{
public:
	enum Variants { one=1, two=2, unknown };
private:
	Variants v = Variants::unknown;
	struct Case_one
	{
		point3D pt3d_1;
		int64_t i_1;
	};

	struct Case_two
	{
		int64_t i_2;
		GMQ_COLL vector<double> vp_2;
	};

public:
	using Case_one_pt3d_1_T = decltype( Case_one::pt3d_1 );
	using Case_one_i_1_T = decltype( Case_one::i_1 );
	using Case_two_i_2_T = decltype( Case_two::i_2 );
	using Case_two_vp_2_T = decltype( Case_two::vp_2 );

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
	Case_one_pt3d_1_T& pt3d_1() {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<Case_one*>( one_two_mem )->pt3d_1;
	}
	const Case_one_pt3d_1_T& pt3d_1() const {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<const Case_one*>( one_two_mem )->pt3d_1;
	}
	
	Case_one_i_1_T& i_1() {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<Case_one*>( one_two_mem )->i_1;
	}
	const Case_one_i_1_T& i_1() const {
		if ( v != Variants::one )
			throw std::exception();
		return reinterpret_cast<const Case_one*>( one_two_mem )->i_1;
	}
	

	// IDL CASE two:
	Case_two_i_2_T& i_2() {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<Case_two*>( one_two_mem )->i_2;
	}
	const Case_two_i_2_T& i_2() const {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<const Case_two*>( one_two_mem )->i_2;
	}
	
	Case_two_vp_2_T& vp_2() {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<Case_two*>( one_two_mem )->vp_2;
	}
	const Case_two_vp_2_T& vp_2() const {
		if ( v != Variants::two )
			throw std::exception();
		return reinterpret_cast<const Case_two*>( one_two_mem )->vp_2;
	}
	
};

struct point
{
	int64_t x;
	int64_t y;
};

struct PolygonMap
{
	GMQ_COLL vector<Vertex> _PolygonMap;
};

struct ObstacleMap
{
	GMQ_COLL vector<PolygonMap> _ObstacleMap;
};

struct Line
{
	GMQ_COLL vector<Vertex> a;
	GMQ_COLL vector<Vertex> b;
};

struct LineMap
{
	GMQ_COLL vector<Line> LineMap;
};

struct POINT3DREAL
{
	double X;
	double Y;
	double Z;
};

struct CharacterParamStruct
{
	int64_t ID;
	SIZE Size;
};

struct StructWithVectorOfSize
{
	GMQ_COLL vector<SIZE> sizes;
	int64_t NN;
};

struct StructWithVectorOfInt
{
	int64_t ID;
	GMQ_COLL vector<int64_t> signedInts;
};

namespace scope_one {
struct MESSAGE_point3D : public point3D {};
} // namespace scope_one

namespace scope_one {
struct MESSAGE_point3D_alias : public point3D {};
} // namespace scope_one

namespace scope_one {
struct MESSAGE_point_alias : public point {};
} // namespace scope_one

namespace level_trace {
struct MESSAGE_LevelTraceData
{
	CharacterParamStruct CharacterParam;
	GMQ_COLL vector<POINT3DREAL> Points;
};
} // namespace level_trace

namespace infrastructural {
struct MESSAGE_PolygonSt
{
	GMQ_COLL vector<PolygonMap> polygonMap;
	GMQ_COLL vector<PolygonMap> concaveMap;
	GMQ_COLL vector<ObstacleMap> obstacleMap;
	GMQ_COLL vector<LineMap> portalMap;
	GMQ_COLL vector<LineMap> jumpMap;
	double polygonSpeed;
};
} // namespace infrastructural

namespace test_gmq {
struct MESSAGE_message_one
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
} // namespace test_gmq

namespace test_json {
struct MESSAGE_message_one
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
} // namespace test_json

namespace infrastructural {
struct MESSAGE_point
{
	point point;
};
} // namespace infrastructural

namespace infrastructural {
struct MESSAGE_point3D
{
	point3D pt;
	du_one du_one_instance;
	GMQ_COLL unordered_map<uint64_t,GMQ_COLL string> num2stringDictionary;
};
} // namespace infrastructural

namespace level_data {
struct MESSAGE_Level
{
	GMQ_COLL vector<PolygonSt> Level;
};
} // namespace level_data

struct publishable_short_sample
{
	int64_t ID;
	GMQ_COLL string name;
};

struct publishable_sample
{
	int64_t ID;
	GMQ_COLL string name;
	SIZE size;
	CharacterParamStruct chp;
	GMQ_COLL vector<int64_t> vector_of_int;
	GMQ_COLL vector<POINT3DREAL> vector_struct_point3dreal;
	StructWithVectorOfInt structWithVectorOfInt;
	StructWithVectorOfSize structWithVectorOfSize;
	du_one du_one_instance;
};

struct publishable_html_tag
{
	HtmlTag tag;
};


} // namespace structures

//===============================================================================

struct publishable_STRUCT_SIZE;
template<class T> class SIZE_RefWrapper;
template<class T, class RootT> class SIZE_RefWrapper4Set;

struct publishable_STRUCT_Line;
template<class T> class Line_RefWrapper;
template<class T, class RootT> class Line_RefWrapper4Set;

struct publishable_STRUCT_Line_;
template<class T> class Line__RefWrapper;
template<class T, class RootT> class Line__RefWrapper4Set;

struct publishable_STRUCT_Vertex;
template<class T> class Vertex_RefWrapper;
template<class T, class RootT> class Vertex_RefWrapper4Set;

struct publishable_STRUCT_PolygonMap;
template<class T> class PolygonMap_RefWrapper;
template<class T, class RootT> class PolygonMap_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
template<class T> class HtmlTag_RefWrapper;
template<class T, class RootT> class HtmlTag_RefWrapper4Set;

struct publishable_STRUCT_StructWithVectorOfInt;
template<class T> class StructWithVectorOfInt_RefWrapper;
template<class T, class RootT> class StructWithVectorOfInt_RefWrapper4Set;

struct publishable_STRUCT_StructWithVectorOfSize;
template<class T> class StructWithVectorOfSize_RefWrapper;
template<class T, class RootT> class StructWithVectorOfSize_RefWrapper4Set;

struct publishable_STRUCT_CharacterParamStruct;
template<class T> class CharacterParamStruct_RefWrapper;
template<class T, class RootT> class CharacterParamStruct_RefWrapper4Set;

struct publishable_STRUCT_SIZE;
template<class T> class SIZE_RefWrapper;
template<class T, class RootT> class SIZE_RefWrapper4Set;

struct publishable_STRUCT_POINT3DREAL;
template<class T> class POINT3DREAL_RefWrapper;
template<class T, class RootT> class POINT3DREAL_RefWrapper4Set;

struct publishable_STRUCT_point3D;
template<class T> class point3D_RefWrapper;
template<class T, class RootT> class point3D_RefWrapper4Set;

struct publishable_STRUCT_HtmlTag;
template<class T> class HtmlTag_RefWrapper;
template<class T, class RootT> class HtmlTag_RefWrapper4Set;


struct publishable_STRUCT_AnimZone : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "animVector" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::animVector), publishable_STRUCT_Line_, true>( parser, t.animVector );

		::globalmq::marshalling::impl::parseKey( parser, "animSpeed" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::animSpeed), ::globalmq::marshalling::impl::RealType, true>( parser, t.animSpeed );

	}

};

struct publishable_DISCRIMINATED_UNION_HtmlTextOrTags : public ::globalmq::marshalling::impl::StructType
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
				case 21: // IDL CASE text
				{
					::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );

				}
				case 22: // IDL CASE taglists
				{
					::globalmq::marshalling::impl::parseKey( parser, "tags" );
					PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag, true>( parser, t.tags() );

				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}
	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		uint64_t caseId = t.currentVariant();
		::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, caseId, "caseId", true );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::composePublishableStructBegin( composer, "caseData" );
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					::globalmq::marshalling::impl::publishableStructComposeString( composer, t.str(), "str", false );

					break;
				}
				case 22: // IDL CASE taglists
				{
					PublishableVectorProcessor::compose<ComposerT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( composer, t.tags(), "tags", false );

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );
		}
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_str = has_void_update_notifier_call_for_str<T>;
		static constexpr bool has_update_notifier_for_str = has_update_notifier_call_for_str<T, typename T::Case_text_str_T>;
		static constexpr bool has_any_notifier_for_str = has_void_update_notifier_for_str || has_update_notifier_for_str;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, typename T::Case_taglists_tags_T>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		using tagsT = typename T::Case_taglists_tags_T;
		static constexpr bool has_void_insert_notifier_for_tags = has_void_insert_notifier_call_for_tags<T>;
		static constexpr bool has_insert_notifier2_for_tags = has_insert_notifier_call2_for_tags<T>;
		static constexpr bool has_insert_notifier3_for_tags = has_insert_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_erased_notifier_for_tags = has_void_erased_notifier_call_for_tags<T>;
		static constexpr bool has_erased_notifier2_for_tags = has_erased_notifier_call2_for_tags<T>;
		static constexpr bool has_erased_notifier3_for_tags = has_erased_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_tags = has_element_updated_void_notifier_call_for_tags<T>;
		static constexpr bool has_element_updated_notifier_for_tags = has_element_updated_notifier_call_for_tags<T>;
		static constexpr bool has_full_element_updated_notifier_for_tags = has_full_element_updated_notifier_call_for_tags<T, tagsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		uint64_t caseId;
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, uint64_t>( parser, &(caseId), "caseId" );
		t.initAs( (typename T::Variants)(caseId) );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "caseData" );
			switch ( caseId )
			{
				case 21: // IDL CASE text
				{
					if constexpr( has_any_notifier_for_str || reportChanges )
					{
						typename T::Case_text_str_T oldVal = t.str();
						::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );
						bool currentChanged = oldVal != t.str();
						if ( currentChanged )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr ( has_void_update_notifier_for_str )
								t.notifyUpdated_str();
							if constexpr ( has_update_notifier_for_str )
								t.notifyUpdated_str( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()), "str" );

					break;
				}
				case 22: // IDL CASE taglists
				{
					if constexpr( reportChanges )
					{
						typename T::Case_taglists_tags_T oldVectorVal;
						::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVectorVal );
						::globalmq::marshalling::impl::parseKey( parser, "tags" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
						bool currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( oldVectorVal, t.tags() );
						changed = changed || currentChanged;
					}
					else
					{
						::globalmq::marshalling::impl::parseKey( parser, "tags" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
					}

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}

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
		static constexpr bool has_void_update_notifier_for_str = has_void_update_notifier_call_for_str<T>;
		static constexpr bool has_update_notifier_for_str = has_update_notifier_call_for_str<T, typename T::Case_text_str_T>;
		static constexpr bool has_any_notifier_for_str = has_void_update_notifier_for_str || has_update_notifier_for_str;
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, typename T::Case_taglists_tags_T>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
		using tagsT = typename T::Case_taglists_tags_T;
		static constexpr bool has_void_insert_notifier_for_tags = has_void_insert_notifier_call_for_tags<T>;
		static constexpr bool has_insert_notifier2_for_tags = has_insert_notifier_call2_for_tags<T>;
		static constexpr bool has_insert_notifier3_for_tags = has_insert_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_erased_notifier_for_tags = has_void_erased_notifier_call_for_tags<T>;
		static constexpr bool has_erased_notifier2_for_tags = has_erased_notifier_call2_for_tags<T>;
		static constexpr bool has_erased_notifier3_for_tags = has_erased_notifier_call3_for_tags<T, GMQ_COLL vector<tagsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_tags = has_element_updated_void_notifier_call_for_tags<T>;
		static constexpr bool has_element_updated_notifier_for_tags = has_element_updated_notifier_call_for_tags<T>;
		static constexpr bool has_full_element_updated_notifier_for_tags = has_full_element_updated_notifier_call_for_tags<T, tagsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_void_update_notifier_for_currentVariant = has_void_update_notifier_call_for_currentVariant<T>;
		static constexpr bool has_update_notifier_for_currentVariant = has_update_notifier_call_for_currentVariant<T, typename T::Variants>;
		static constexpr bool has_any_notifier_for_currentVariant = has_void_update_notifier_for_currentVariant || has_update_notifier_for_currentVariant;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			uint64_t newVar = T::Variants::unknown;
			::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, uint64_t>( parser, &newVar );
			if constexpr( has_any_notifier_for_currentVariant || reportChanges )
			{
				auto oldVal = t.currentVariant();
				t.initAs( (typename T::Variants)( newVar ) );
				bool currentChanged = oldVal != t.currentVariant();
				if ( currentChanged )
				{
					if constexpr ( reportChanges )
						changed = true;
					if constexpr ( has_void_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant();
					if constexpr ( has_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant( oldVal );
				}
			}
			else
				t.initAs( (typename T::Variants)( newVar ) );
		}
		else // updating particular members in particular cases
		{
			switch ( t.currentVariant() )
			{
				case 21: // IDL CASE text
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() > offset + 1 )
								throw std::exception(); // bad format, TODO: ...
							if constexpr( has_any_notifier_for_str || reportChanges )
							{
								typename T::Case_text_str_T oldVal = t.str();
								::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()) );
								bool currentChanged = oldVal != t.str();
								if ( currentChanged )
								{
									if constexpr ( reportChanges )
										changed = true;
									if constexpr ( has_void_update_notifier_for_str )
										t.notifyUpdated_str();
									if constexpr ( has_update_notifier_for_str )
										t.notifyUpdated_str( oldVal );
								}
							}
							else
								::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, typename T::Case_text_str_T>( parser, &(t.str()) );
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case 22: // IDL CASE taglists
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							{
								typename T::Case_taglists_tags_T oldVectorVal;
								bool currentChanged = false;
								constexpr bool alwaysCollectChanges = has_any_notifier_for_tags;
								if constexpr( alwaysCollectChanges )
									::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVectorVal );
								if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
								{
									size_t pos = addr[offset + 1];
									if ( pos > t.tags().size() )
										throw std::exception();
									if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
									{
										typename T::Case_taglists_tags_T::value_type& value = t.tags()[pos];
										if constexpr ( has_full_element_updated_notifier_for_tags )
										{
											typename T::Case_taglists_tags_T::value_type oldValue;
											publishable_STRUCT_HtmlTag::copy( value, oldValue );
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
											{
												t.notifyElementUpdated_tags( pos, oldValue );
												if constexpr ( has_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
												if constexpr ( has_void_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
											}
										}
										else if constexpr ( has_element_updated_notifier_for_tags )
										{
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
											{
												t.notifyElementUpdated_tags( pos );
												if constexpr ( has_void_element_updated_notifier_for_tags )
													t.notifyElementUpdated_tags();
											}
										}
										else if constexpr ( has_void_element_updated_notifier_for_tags )
										{
											currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											if ( currentChanged )
												t.notifyElementUpdated_tags();
										}
										else
										{
											if constexpr ( alwaysCollectChanges )
												currentChanged = publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type, bool>( parser, value, addr, offset + 2 );
											else
												publishable_STRUCT_HtmlTag::parse<ParserT, typename T::Case_taglists_tags_T::value_type>( parser, value, addr, offset + 2 );
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
												typename T::Case_taglists_tags_T oldVal;
												::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVal );
												t.tags().erase( t.tags().begin() + pos );
												if constexpr ( has_erased_notifier3_for_tags )
													t.notifyErased_tags( pos, oldVal );
												if constexpr ( has_erased_notifier2_for_tags )
													t.notifyErased_tags( pos );
												if constexpr ( has_void_erased_notifier_for_tags )
													t.notifyErased_tags();
												if constexpr ( alwaysCollectChanges )
													currentChanged = true;
												break;
											}
											case ActionOnVector::update_at:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_taglists_tags_T::value_type& value = t.tags()[pos];
												typename T::Case_taglists_tags_T::value_type oldValue;
												publishable_STRUCT_HtmlTag::copy( value, oldValue );
												if constexpr ( has_full_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
													{
														t.notifyElementUpdated_tags( pos, oldValue );
														if constexpr ( has_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags();
														if constexpr ( has_void_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags();
													}
												}
												else if constexpr ( has_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
													{
														t.notifyElementUpdated_tags( pos );
														if constexpr ( has_void_element_updated_notifier_for_tags )
															t.notifyElementUpdated_tags();
													}
												}
												else if constexpr ( has_void_element_updated_notifier_for_tags )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													if ( currentChanged )
														t.notifyElementUpdated_tags();
												}
												else
												{
													if constexpr ( alwaysCollectChanges )
														currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value, oldValue );
													else
														PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value );
												}
												break;
											}
											case ActionOnVector::insert_single_before:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_taglists_tags_T::value_type value;
												PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, value );
												if constexpr ( has_insert_notifier3_for_tags )
												{
													typename T::Case_taglists_tags_T oldVal;
													::globalmq::marshalling::impl::copyVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( t.tags(), oldVal );
													t.notifyInserted_tags( pos, oldVal );
												}
												if constexpr ( has_insert_notifier2_for_tags )
													t.notifyInserted_tags( pos );
												if constexpr ( has_void_insert_notifier_for_tags )
													t.notifyInserted_tags();
												t.tags().insert( t.tags().begin() + pos, value );
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
										PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );
										currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( oldVectorVal, t.tags() );
									}
									else
										PublishableVectorProcessor::parse<ParserT, typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( parser, t.tags() );

									::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
								}

								if ( currentChanged )
								{
									if constexpr( has_void_update_notifier_for_tags )
										t.notifyUpdated_tags();
									if constexpr( has_update_notifier_for_tags )
										t.notifyUpdated_tags( oldVectorVal );
								}
							}
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		auto srcCaseId = src.currentVariant();
		dst.initAs( srcCaseId );
		if ( srcCaseId != UserT::Variants::unknown )
			switch ( srcCaseId )
			{
				case 21: // IDL CASE text
				{
					dst.str() = src.str();
				}
				case 22: // IDL CASE taglists
				{
					::globalmq::marshalling::impl::copyVector<typename UserT::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( src.tags(), dst.tags() );
				}
				default:
					throw std::exception(); // unexpected
			}
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.currentVariant() != s2.currentVariant() )
			return false;
		if ( s1.currentVariant() != UserT::Variants::unknown )
			switch ( s1.currentVariant() )
			{
				case 21: // IDL CASE text
				{
					if ( s1.str() != s2.str() ) return false;
				}
				case 22: // IDL CASE taglists
				{
					if ( !::globalmq::marshalling::impl::isSameVector<typename UserT::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( s1.tags(), s2.tags() ) ) return false;
				}
				default:
					throw std::exception(); // unexpected
			}
		return true;
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

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.x, "x", true );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.y, "y", true );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.z, "z", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_x = has_void_update_notifier_call_for_x<T>;
		static constexpr bool has_update_notifier_for_x = has_update_notifier_call_for_x<T, decltype(T::x)>;
		static constexpr bool has_any_notifier_for_x = has_void_update_notifier_for_x || has_update_notifier_for_x;
		static constexpr bool has_void_update_notifier_for_y = has_void_update_notifier_call_for_y<T>;
		static constexpr bool has_update_notifier_for_y = has_update_notifier_call_for_y<T, decltype(T::y)>;
		static constexpr bool has_any_notifier_for_y = has_void_update_notifier_for_y || has_update_notifier_for_y;
		static constexpr bool has_void_update_notifier_for_z = has_void_update_notifier_call_for_z<T>;
		static constexpr bool has_update_notifier_for_z = has_update_notifier_call_for_z<T, decltype(T::z)>;
		static constexpr bool has_any_notifier_for_z = has_void_update_notifier_for_z || has_update_notifier_for_z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_x || reportChanges )
		{
			decltype(T::x) oldVal = t.x;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );
			bool currentChanged = oldVal != t.x;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_x )
					t.notifyUpdated_x();
				if constexpr ( has_update_notifier_for_x )
					t.notifyUpdated_x( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::x)>( parser, &(t.x), "x" );

		if constexpr( has_any_notifier_for_y || reportChanges )
		{
			decltype(T::y) oldVal = t.y;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );
			bool currentChanged = oldVal != t.y;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_y )
					t.notifyUpdated_y();
				if constexpr ( has_update_notifier_for_y )
					t.notifyUpdated_y( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::y)>( parser, &(t.y), "y" );

		if constexpr( has_any_notifier_for_z || reportChanges )
		{
			decltype(T::z) oldVal = t.z;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::z)>( parser, &(t.z), "z" );
			bool currentChanged = oldVal != t.z;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_z )
					t.notifyUpdated_z();
				if constexpr ( has_update_notifier_for_z )
					t.notifyUpdated_z( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::z)>( parser, &(t.z), "z" );


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
		static constexpr bool has_void_update_notifier_for_x = has_void_update_notifier_call_for_x<T>;
		static constexpr bool has_update_notifier_for_x = has_update_notifier_call_for_x<T, decltype(T::x)>;
		static constexpr bool has_any_notifier_for_x = has_void_update_notifier_for_x || has_update_notifier_for_x;
		static constexpr bool has_void_update_notifier_for_y = has_void_update_notifier_call_for_y<T>;
		static constexpr bool has_update_notifier_for_y = has_update_notifier_call_for_y<T, decltype(T::y)>;
		static constexpr bool has_any_notifier_for_y = has_void_update_notifier_for_y || has_update_notifier_for_y;
		static constexpr bool has_void_update_notifier_for_z = has_void_update_notifier_call_for_z<T>;
		static constexpr bool has_update_notifier_for_z = has_update_notifier_call_for_z<T, decltype(T::z)>;
		static constexpr bool has_any_notifier_for_z = has_void_update_notifier_for_z || has_update_notifier_for_z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_x || reportChanges )
				{
					decltype(T::x) oldVal = t.x;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::x)>( parser, &(t.x) );
					bool currentChanged = oldVal != t.x;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_x )
							t.notifyUpdated_x();
						if constexpr ( has_update_notifier_for_x )
							t.notifyUpdated_x( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::x)>( parser, &(t.x) );
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_y || reportChanges )
				{
					decltype(T::y) oldVal = t.y;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::y)>( parser, &(t.y) );
					bool currentChanged = oldVal != t.y;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_y )
							t.notifyUpdated_y();
						if constexpr ( has_update_notifier_for_y )
							t.notifyUpdated_y( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::y)>( parser, &(t.y) );
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_z || reportChanges )
				{
					decltype(T::z) oldVal = t.z;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::z)>( parser, &(t.z) );
					bool currentChanged = oldVal != t.z;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_z )
							t.notifyUpdated_z();
						if constexpr ( has_update_notifier_for_z )
							t.notifyUpdated_z( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::z)>( parser, &(t.z) );
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
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.x != s2.x ) return false;
		if ( s1.y != s2.y ) return false;
		if ( s1.z != s2.z ) return false;
		return true;
	}
};

struct publishable_STRUCT_Vertex : public ::globalmq::marshalling::impl::StructType
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

struct publishable_STRUCT_SIZE : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.X, "X", true );

		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.Y, "Y", true );

		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.Z, "Z", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_X = has_void_update_notifier_call_for_X<T>;
		static constexpr bool has_update_notifier_for_X = has_update_notifier_call_for_X<T, decltype(T::X)>;
		static constexpr bool has_any_notifier_for_X = has_void_update_notifier_for_X || has_update_notifier_for_X;
		static constexpr bool has_void_update_notifier_for_Y = has_void_update_notifier_call_for_Y<T>;
		static constexpr bool has_update_notifier_for_Y = has_update_notifier_call_for_Y<T, decltype(T::Y)>;
		static constexpr bool has_any_notifier_for_Y = has_void_update_notifier_for_Y || has_update_notifier_for_Y;
		static constexpr bool has_void_update_notifier_for_Z = has_void_update_notifier_call_for_Z<T>;
		static constexpr bool has_update_notifier_for_Z = has_update_notifier_call_for_Z<T, decltype(T::Z)>;
		static constexpr bool has_any_notifier_for_Z = has_void_update_notifier_for_Z || has_update_notifier_for_Z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_X || reportChanges )
		{
			decltype(T::X) oldVal = t.X;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );
			bool currentChanged = oldVal != t.X;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_X )
					t.notifyUpdated_X();
				if constexpr ( has_update_notifier_for_X )
					t.notifyUpdated_X( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );

		if constexpr( has_any_notifier_for_Y || reportChanges )
		{
			decltype(T::Y) oldVal = t.Y;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );
			bool currentChanged = oldVal != t.Y;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_Y )
					t.notifyUpdated_Y();
				if constexpr ( has_update_notifier_for_Y )
					t.notifyUpdated_Y( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );

		if constexpr( has_any_notifier_for_Z || reportChanges )
		{
			decltype(T::Z) oldVal = t.Z;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );
			bool currentChanged = oldVal != t.Z;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_Z )
					t.notifyUpdated_Z();
				if constexpr ( has_update_notifier_for_Z )
					t.notifyUpdated_Z( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );


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
		static constexpr bool has_void_update_notifier_for_X = has_void_update_notifier_call_for_X<T>;
		static constexpr bool has_update_notifier_for_X = has_update_notifier_call_for_X<T, decltype(T::X)>;
		static constexpr bool has_any_notifier_for_X = has_void_update_notifier_for_X || has_update_notifier_for_X;
		static constexpr bool has_void_update_notifier_for_Y = has_void_update_notifier_call_for_Y<T>;
		static constexpr bool has_update_notifier_for_Y = has_update_notifier_call_for_Y<T, decltype(T::Y)>;
		static constexpr bool has_any_notifier_for_Y = has_void_update_notifier_for_Y || has_update_notifier_for_Y;
		static constexpr bool has_void_update_notifier_for_Z = has_void_update_notifier_call_for_Z<T>;
		static constexpr bool has_update_notifier_for_Z = has_update_notifier_call_for_Z<T, decltype(T::Z)>;
		static constexpr bool has_any_notifier_for_Z = has_void_update_notifier_for_Z || has_update_notifier_for_Z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_X || reportChanges )
				{
					decltype(T::X) oldVal = t.X;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::X)>( parser, &(t.X) );
					bool currentChanged = oldVal != t.X;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_X )
							t.notifyUpdated_X();
						if constexpr ( has_update_notifier_for_X )
							t.notifyUpdated_X( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::X)>( parser, &(t.X) );
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_Y || reportChanges )
				{
					decltype(T::Y) oldVal = t.Y;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Y)>( parser, &(t.Y) );
					bool currentChanged = oldVal != t.Y;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_Y )
							t.notifyUpdated_Y();
						if constexpr ( has_update_notifier_for_Y )
							t.notifyUpdated_Y( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Y)>( parser, &(t.Y) );
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_Z || reportChanges )
				{
					decltype(T::Z) oldVal = t.Z;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Z)>( parser, &(t.Z) );
					bool currentChanged = oldVal != t.Z;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_Z )
							t.notifyUpdated_Z();
						if constexpr ( has_update_notifier_for_Z )
							t.notifyUpdated_Z( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Z)>( parser, &(t.Z) );
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
		dst.X = src.X;
		dst.Y = src.Y;
		dst.Z = src.Z;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.X != s2.X ) return false;
		if ( s1.Y != s2.Y ) return false;
		if ( s1.Z != s2.Z ) return false;
		return true;
	}
};

struct publishable_STRUCT_Line_ : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "a" );
		publishable_STRUCT_Vertex::parseForStateSyncOrMessageInDepth( parser, t.a );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "b" );
		publishable_STRUCT_Vertex::parseForStateSyncOrMessageInDepth( parser, t.b );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

};

struct publishable_STRUCT_PolygonSt : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "portalMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::portalMap), publishable_STRUCT_Line_, true>( parser, t.portalMap );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "animZones" );
		publishable_STRUCT_AnimZone::parseForStateSyncOrMessageInDepth( parser, t.animZones );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

};

struct publishable_STRUCT_HtmlTag : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parseKey( parser, "properties" );
		PublishableDictionaryProcessor::parse<ParserT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType, true>( parser, t.properties );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parseForStateSyncOrMessageInDepth( parser, t.tags );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		PublishableDictionaryProcessor::compose<ComposerT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( composer, t.properties, "properties", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "tags" );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( composer, t.tags );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );

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
		static constexpr bool has_void_update_notifier_for_properties = has_void_update_notifier_call_for_properties<T>;
		static constexpr bool has_update_notifier_for_properties = has_update_notifier_call_for_properties<T, decltype(T::properties)>;
		static constexpr bool has_any_notifier_for_properties = has_void_update_notifier_for_properties || has_update_notifier_for_properties;
		using propertiesT = decltype(T::properties);
		//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, decltype(T::tags)>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
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

//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!
		if constexpr( reportChanges )
		{
			decltype(T::properties) oldDictionaryVal;
			::globalmq::marshalling::impl::copyDictionary<decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( t.properties, oldDictionaryVal );
			::globalmq::marshalling::impl::parseKey( parser, "properties" );
			PublishableDictionaryProcessor::parse<ParserT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( parser, t.properties );
			bool currentChanged = !::globalmq::marshalling::impl::isSameDictionary<decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( oldDictionaryVal, t.properties );
			changed = changed || currentChanged;
		}
		else
		{
			::globalmq::marshalling::impl::parseKey( parser, "properties" );
			PublishableDictionaryProcessor::parse<ParserT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( parser, t.properties );
		}

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "tags" );
		if constexpr( has_update_notifier_for_tags )
		{
			decltype(T::tags) temp_tags;
			publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
			bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr( has_void_update_notifier_for_tags )
					t.notifyUpdated_tags();
				t.notifyUpdated_tags( temp_tags );
			}
		}
		else if constexpr( has_void_update_notifier_for_tags )
		{
			bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				t.notifyUpdated_tags();
			}
		}
		else if constexpr ( reportChanges )
			changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
		else
			publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

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
		static constexpr bool has_void_update_notifier_for_properties = has_void_update_notifier_call_for_properties<T>;
		static constexpr bool has_update_notifier_for_properties = has_update_notifier_call_for_properties<T, decltype(T::properties)>;
		static constexpr bool has_any_notifier_for_properties = has_void_update_notifier_for_properties || has_update_notifier_for_properties;
		using propertiesT = decltype(T::properties);
		//// TODO: revise notifier list !!!!!!!!!!!!!!!!!!!!!!!!!
		static constexpr bool has_void_update_notifier_for_tags = has_void_update_notifier_call_for_tags<T>;
		static constexpr bool has_update_notifier_for_tags = has_update_notifier_call_for_tags<T, decltype(T::tags)>;
		static constexpr bool has_any_notifier_for_tags = has_void_update_notifier_for_tags || has_update_notifier_for_tags;
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
				{
					decltype(T::properties) oldDictionaryVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_properties;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyDictionary<decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( t.properties, oldDictionaryVal );
					if ( addr.size() > offset + 1 ) // one of actions over elements of the dictionary
					{
						size_t pos = addr[offset + 1];
						if ( pos > t.properties.size() )
							throw std::exception();
						if ( addr.size() > offset + 2 ) // update for a value of a particular dictionary element
						{
							throw std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.properties elements is UNDEFINED)
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							::globalmq::marshalling::impl::parseStateUpdateBlockEnd( parser );
						}
					}
					else // replacement of the whole dictionary
					{
						::globalmq::marshalling::impl::publishableParseLeafeDictionaryBegin( parser );

						if constexpr( alwaysCollectChanges )
						{
							PublishableDictionaryProcessor::parse<ParserT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( parser, t.properties );
							currentChanged = !::globalmq::marshalling::impl::isSameDictionary<decltype(T::properties), ::globalmq::marshalling::impl::StringType>( oldDictionaryVal, t.properties );
						}
						else
							PublishableDictionaryProcessor::parse<ParserT, decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( parser, t.properties );

						::globalmq::marshalling::impl::publishableParseLeafeDictionaryEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_properties )
							t.notifyUpdated_properties();
						if constexpr( has_update_notifier_for_properties )
							t.notifyUpdated_properties( oldDictionaryVal );
					}
				}
				break;
			}
			case 2:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_tags )
					{
						decltype(T::tags) temp_tags;
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_tags )
								t.notifyUpdated_tags();
							t.notifyUpdated_tags( temp_tags );
						}
					}
					else if constexpr( has_void_update_notifier_for_tags )
					{
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_tags();
						}
					}

					else
					{
							if constexpr ( reportChanges )
								changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags );
							else
								publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_tags )
					{
						decltype(T::tags) temp_tags;
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy<decltype(T::tags)>( t.tags, temp_tags );
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_tags )
								t.notifyUpdated_tags();
							t.notifyUpdated_tags( temp_tags );
						}
					}
					else if constexpr( has_void_update_notifier_for_tags )
					{
						bool changedCurrent = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_tags();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse<ParserT, decltype(T::tags), bool>( parser, t.tags, addr, offset + 1 );
					else
						publishable_DISCRIMINATED_UNION_HtmlTextOrTags::parse( parser, t.tags, addr, offset + 1 );
				}
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
		::globalmq::marshalling::impl::copyDictionary<decltype(UserT::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( src.properties, dst.properties );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::copy( src.tags, dst.tags );
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.name != s2.name ) return false;
		if ( !::globalmq::marshalling::impl::isSameDictionary<decltype(UserT::properties), ::globalmq::marshalling::impl::StringType>( s1.properties, s2.properties ) ) return false;
		if( ! publishable_DISCRIMINATED_UNION_HtmlTextOrTags::isSame( s1.tags, s2.tags ) ) return false;
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
				case 1: // IDL CASE one
				{
					::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt3d_1" );
					publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.pt3d_1() );
					::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

					::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_one_i_1_T>( parser, &(t.i_1()), "i_1" );

				}
				case 2: // IDL CASE two
				{
					::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_two_i_2_T>( parser, &(t.i_2()), "i_2" );

					::globalmq::marshalling::impl::parseKey( parser, "vp_2" );
					PublishableVectorProcessor::parse<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType, true>( parser, t.vp_2() );

				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}
	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		uint64_t caseId = t.currentVariant();
		::globalmq::marshalling::impl::publishableStructComposeUnsignedInteger( composer, caseId, "caseId", true );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::composePublishableStructBegin( composer, "caseData" );
			switch ( caseId )
			{
				case 1: // IDL CASE one
				{
					::globalmq::marshalling::impl::composePublishableStructBegin( composer, "pt3d_1" );
					publishable_STRUCT_point3D::compose( composer, t.pt3d_1() );
					::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

					::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.i_1(), "i_1", false );

					break;
				}
				case 2: // IDL CASE two
				{
					::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.i_2(), "i_2", true );

					PublishableVectorProcessor::compose<ComposerT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( composer, t.vp_2(), "vp_2", false );

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );
		}
	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_pt3d_1 = has_void_update_notifier_call_for_pt3d_1<T>;
		static constexpr bool has_update_notifier_for_pt3d_1 = has_update_notifier_call_for_pt3d_1<T, typename T::Case_one_pt3d_1_T>;
		static constexpr bool has_any_notifier_for_pt3d_1 = has_void_update_notifier_for_pt3d_1 || has_update_notifier_for_pt3d_1;
		static constexpr bool has_void_update_notifier_for_i_1 = has_void_update_notifier_call_for_i_1<T>;
		static constexpr bool has_update_notifier_for_i_1 = has_update_notifier_call_for_i_1<T, typename T::Case_one_i_1_T>;
		static constexpr bool has_any_notifier_for_i_1 = has_void_update_notifier_for_i_1 || has_update_notifier_for_i_1;
		static constexpr bool has_void_update_notifier_for_i_2 = has_void_update_notifier_call_for_i_2<T>;
		static constexpr bool has_update_notifier_for_i_2 = has_update_notifier_call_for_i_2<T, typename T::Case_two_i_2_T>;
		static constexpr bool has_any_notifier_for_i_2 = has_void_update_notifier_for_i_2 || has_update_notifier_for_i_2;
		static constexpr bool has_void_update_notifier_for_vp_2 = has_void_update_notifier_call_for_vp_2<T>;
		static constexpr bool has_update_notifier_for_vp_2 = has_update_notifier_call_for_vp_2<T, typename T::Case_two_vp_2_T>;
		static constexpr bool has_any_notifier_for_vp_2 = has_void_update_notifier_for_vp_2 || has_update_notifier_for_vp_2;
		using vp_2T = typename T::Case_two_vp_2_T;
		static constexpr bool has_void_insert_notifier_for_vp_2 = has_void_insert_notifier_call_for_vp_2<T>;
		static constexpr bool has_insert_notifier2_for_vp_2 = has_insert_notifier_call2_for_vp_2<T>;
		static constexpr bool has_insert_notifier3_for_vp_2 = has_insert_notifier_call3_for_vp_2<T, GMQ_COLL vector<vp_2T>&>;
		static constexpr bool has_void_erased_notifier_for_vp_2 = has_void_erased_notifier_call_for_vp_2<T>;
		static constexpr bool has_erased_notifier2_for_vp_2 = has_erased_notifier_call2_for_vp_2<T>;
		static constexpr bool has_erased_notifier3_for_vp_2 = has_erased_notifier_call3_for_vp_2<T, GMQ_COLL vector<vp_2T>&>;
		static constexpr bool has_void_element_updated_notifier_for_vp_2 = has_element_updated_void_notifier_call_for_vp_2<T>;
		static constexpr bool has_element_updated_notifier_for_vp_2 = has_element_updated_notifier_call_for_vp_2<T>;
		static constexpr bool has_full_element_updated_notifier_for_vp_2 = has_full_element_updated_notifier_call_for_vp_2<T, vp_2T&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		uint64_t caseId;
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, uint64_t>( parser, &(caseId), "caseId" );
		t.initAs( (typename T::Variants)(caseId) );
		if ( caseId != T::Variants::unknown )
		{
			::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "caseData" );
			switch ( caseId )
			{
				case 1: // IDL CASE one
				{
					::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt3d_1" );
					if constexpr( has_update_notifier_for_pt3d_1 )
					{
						typename T::Case_one_pt3d_1_T temp_pt3d_1;
						publishable_STRUCT_point3D::copy<typename T::Case_one_pt3d_1_T>( t.pt3d_1(), temp_pt3d_1 );
						bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_pt3d_1 )
								t.notifyUpdated_pt3d_1();
							t.notifyUpdated_pt3d_1( temp_pt3d_1 );
						}
					}
					else if constexpr( has_void_update_notifier_for_pt3d_1 )
					{
						bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_pt3d_1();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
					else
						publishable_STRUCT_point3D::parse( parser, t.pt3d_1() );
					::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
					if constexpr( has_any_notifier_for_i_1 || reportChanges )
					{
						typename T::Case_one_i_1_T oldVal = t.i_1();
						::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_one_i_1_T>( parser, &(t.i_1()), "i_1" );
						bool currentChanged = oldVal != t.i_1();
						if ( currentChanged )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr ( has_void_update_notifier_for_i_1 )
								t.notifyUpdated_i_1();
							if constexpr ( has_update_notifier_for_i_1 )
								t.notifyUpdated_i_1( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_one_i_1_T>( parser, &(t.i_1()), "i_1" );

					break;
				}
				case 2: // IDL CASE two
				{
					if constexpr( has_any_notifier_for_i_2 || reportChanges )
					{
						typename T::Case_two_i_2_T oldVal = t.i_2();
						::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_two_i_2_T>( parser, &(t.i_2()), "i_2" );
						bool currentChanged = oldVal != t.i_2();
						if ( currentChanged )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr ( has_void_update_notifier_for_i_2 )
								t.notifyUpdated_i_2();
							if constexpr ( has_update_notifier_for_i_2 )
								t.notifyUpdated_i_2( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseInteger<ParserT, typename T::Case_two_i_2_T>( parser, &(t.i_2()), "i_2" );

					if constexpr( reportChanges )
					{
						typename T::Case_two_vp_2_T oldVectorVal;
						::globalmq::marshalling::impl::copyVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( t.vp_2(), oldVectorVal );
						::globalmq::marshalling::impl::parseKey( parser, "vp_2" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, t.vp_2() );
						bool currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( oldVectorVal, t.vp_2() );
						changed = changed || currentChanged;
					}
					else
					{
						::globalmq::marshalling::impl::parseKey( parser, "vp_2" );
						PublishableVectorProcessor::parse<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, t.vp_2() );
					}

					break;
				}
				default:
					throw std::exception(); // unexpected
			}
			::globalmq::marshalling::impl::parsePublishableStructEnd( parser );
		}

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
		static constexpr bool has_void_update_notifier_for_pt3d_1 = has_void_update_notifier_call_for_pt3d_1<T>;
		static constexpr bool has_update_notifier_for_pt3d_1 = has_update_notifier_call_for_pt3d_1<T, typename T::Case_one_pt3d_1_T>;
		static constexpr bool has_any_notifier_for_pt3d_1 = has_void_update_notifier_for_pt3d_1 || has_update_notifier_for_pt3d_1;
		static constexpr bool has_void_update_notifier_for_i_1 = has_void_update_notifier_call_for_i_1<T>;
		static constexpr bool has_update_notifier_for_i_1 = has_update_notifier_call_for_i_1<T, typename T::Case_one_i_1_T>;
		static constexpr bool has_any_notifier_for_i_1 = has_void_update_notifier_for_i_1 || has_update_notifier_for_i_1;
		static constexpr bool has_void_update_notifier_for_i_2 = has_void_update_notifier_call_for_i_2<T>;
		static constexpr bool has_update_notifier_for_i_2 = has_update_notifier_call_for_i_2<T, typename T::Case_two_i_2_T>;
		static constexpr bool has_any_notifier_for_i_2 = has_void_update_notifier_for_i_2 || has_update_notifier_for_i_2;
		static constexpr bool has_void_update_notifier_for_vp_2 = has_void_update_notifier_call_for_vp_2<T>;
		static constexpr bool has_update_notifier_for_vp_2 = has_update_notifier_call_for_vp_2<T, typename T::Case_two_vp_2_T>;
		static constexpr bool has_any_notifier_for_vp_2 = has_void_update_notifier_for_vp_2 || has_update_notifier_for_vp_2;
		using vp_2T = typename T::Case_two_vp_2_T;
		static constexpr bool has_void_insert_notifier_for_vp_2 = has_void_insert_notifier_call_for_vp_2<T>;
		static constexpr bool has_insert_notifier2_for_vp_2 = has_insert_notifier_call2_for_vp_2<T>;
		static constexpr bool has_insert_notifier3_for_vp_2 = has_insert_notifier_call3_for_vp_2<T, GMQ_COLL vector<vp_2T>&>;
		static constexpr bool has_void_erased_notifier_for_vp_2 = has_void_erased_notifier_call_for_vp_2<T>;
		static constexpr bool has_erased_notifier2_for_vp_2 = has_erased_notifier_call2_for_vp_2<T>;
		static constexpr bool has_erased_notifier3_for_vp_2 = has_erased_notifier_call3_for_vp_2<T, GMQ_COLL vector<vp_2T>&>;
		static constexpr bool has_void_element_updated_notifier_for_vp_2 = has_element_updated_void_notifier_call_for_vp_2<T>;
		static constexpr bool has_element_updated_notifier_for_vp_2 = has_element_updated_notifier_call_for_vp_2<T>;
		static constexpr bool has_full_element_updated_notifier_for_vp_2 = has_full_element_updated_notifier_call_for_vp_2<T, vp_2T&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		static constexpr bool has_void_update_notifier_for_currentVariant = has_void_update_notifier_call_for_currentVariant<T>;
		static constexpr bool has_update_notifier_for_currentVariant = has_update_notifier_call_for_currentVariant<T, typename T::Variants>;
		static constexpr bool has_any_notifier_for_currentVariant = has_void_update_notifier_for_currentVariant || has_update_notifier_for_currentVariant;
		GMQ_ASSERT( addr.size() );
		if ( addr[offset] == 0 ) // changing current variant
		{
			if ( addr.size() > offset + 1 )
				throw std::exception(); // bad format, TODO: ...
			uint64_t newVar = T::Variants::unknown;
			::globalmq::marshalling::impl::publishableParseLeafeUnsignedInteger<ParserT, uint64_t>( parser, &newVar );
			if constexpr( has_any_notifier_for_currentVariant || reportChanges )
			{
				auto oldVal = t.currentVariant();
				t.initAs( (typename T::Variants)( newVar ) );
				bool currentChanged = oldVal != t.currentVariant();
				if ( currentChanged )
				{
					if constexpr ( reportChanges )
						changed = true;
					if constexpr ( has_void_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant();
					if constexpr ( has_update_notifier_for_currentVariant )
						t.notifyUpdated_currentVariant( oldVal );
				}
			}
			else
				t.initAs( (typename T::Variants)( newVar ) );
		}
		else // updating particular members in particular cases
		{
			switch ( t.currentVariant() )
			{
				case 1: // IDL CASE one
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
							{
								::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
								if constexpr( has_update_notifier_for_pt3d_1 )
								{
									typename T::Case_one_pt3d_1_T temp_pt3d_1;
									publishable_STRUCT_point3D::copy<typename T::Case_one_pt3d_1_T>( t.pt3d_1(), temp_pt3d_1 );
									bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
									if ( changedCurrent )
									{
										if constexpr ( reportChanges )
											changed = true;
										if constexpr( has_void_update_notifier_for_pt3d_1 )
											t.notifyUpdated_pt3d_1();
										t.notifyUpdated_pt3d_1( temp_pt3d_1 );
									}
								}
								else if constexpr( has_void_update_notifier_for_pt3d_1 )
								{
									bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
									if ( changedCurrent )
									{
										if constexpr ( reportChanges )
											changed = true;
										t.notifyUpdated_pt3d_1();
									}
								}

								else
								{
										if constexpr ( reportChanges )
											changed = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1() );
										else
											publishable_STRUCT_point3D::parse( parser, t.pt3d_1() );
								}
								::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
							}
							else // let child continue parsing
							{
								if constexpr( has_update_notifier_for_pt3d_1 )
								{
									typename T::Case_one_pt3d_1_T temp_pt3d_1;
									publishable_STRUCT_point3D::copy<typename T::Case_one_pt3d_1_T>( t.pt3d_1(), temp_pt3d_1 );
									bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1(), addr, offset + 1 );
									if ( changedCurrent )
									{
										if constexpr ( reportChanges )
											changed = true;
										if constexpr( has_void_update_notifier_for_pt3d_1 )
											t.notifyUpdated_pt3d_1();
										t.notifyUpdated_pt3d_1( temp_pt3d_1 );
									}
								}
								else if constexpr( has_void_update_notifier_for_pt3d_1 )
								{
									bool changedCurrent = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1(), addr, offset + 1 );
									if ( changedCurrent )
									{
										if constexpr ( reportChanges )
											changed = true;
										t.notifyUpdated_pt3d_1();
									}
								}
								else if constexpr ( reportChanges )
									changed = publishable_STRUCT_point3D::parse<ParserT, typename T::Case_one_pt3d_1_T, bool>( parser, t.pt3d_1(), addr, offset + 1 );
								else
									publishable_STRUCT_point3D::parse( parser, t.pt3d_1(), addr, offset + 1 );
							}
							break;
						}
						case 2:
						{
							if ( addr.size() > offset + 1 )
								throw std::exception(); // bad format, TODO: ...
							if constexpr( has_any_notifier_for_i_1 || reportChanges )
							{
								typename T::Case_one_i_1_T oldVal = t.i_1();
								::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, typename T::Case_one_i_1_T>( parser, &(t.i_1()) );
								bool currentChanged = oldVal != t.i_1();
								if ( currentChanged )
								{
									if constexpr ( reportChanges )
										changed = true;
									if constexpr ( has_void_update_notifier_for_i_1 )
										t.notifyUpdated_i_1();
									if constexpr ( has_update_notifier_for_i_1 )
										t.notifyUpdated_i_1( oldVal );
								}
							}
							else
								::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, typename T::Case_one_i_1_T>( parser, &(t.i_1()) );
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				case 2: // IDL CASE two
				{
					switch ( addr[offset] )
					{
						case 1:
						{
							if ( addr.size() > offset + 1 )
								throw std::exception(); // bad format, TODO: ...
							if constexpr( has_any_notifier_for_i_2 || reportChanges )
							{
								typename T::Case_two_i_2_T oldVal = t.i_2();
								::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, typename T::Case_two_i_2_T>( parser, &(t.i_2()) );
								bool currentChanged = oldVal != t.i_2();
								if ( currentChanged )
								{
									if constexpr ( reportChanges )
										changed = true;
									if constexpr ( has_void_update_notifier_for_i_2 )
										t.notifyUpdated_i_2();
									if constexpr ( has_update_notifier_for_i_2 )
										t.notifyUpdated_i_2( oldVal );
								}
							}
							else
								::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, typename T::Case_two_i_2_T>( parser, &(t.i_2()) );
							break;
						}
						case 2:
						{
							{
								typename T::Case_two_vp_2_T oldVectorVal;
								bool currentChanged = false;
								constexpr bool alwaysCollectChanges = has_any_notifier_for_vp_2;
								if constexpr( alwaysCollectChanges )
									::globalmq::marshalling::impl::copyVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( t.vp_2(), oldVectorVal );
								if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
								{
									size_t pos = addr[offset + 1];
									if ( pos > t.vp_2().size() )
										throw std::exception();
									if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
									{
										throw std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.vp_2 elements is REAL)
									}
									else // update of one or more elelments as a whole
									{
										size_t action;
										::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
										switch ( action )
										{
											case ActionOnVector::remove_at:
											{
												typename T::Case_two_vp_2_T oldVal;
												::globalmq::marshalling::impl::copyVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( t.vp_2(), oldVal );
												t.vp_2().erase( t.vp_2().begin() + pos );
												if constexpr ( has_erased_notifier3_for_vp_2 )
													t.notifyErased_vp_2( pos, oldVal );
												if constexpr ( has_erased_notifier2_for_vp_2 )
													t.notifyErased_vp_2( pos );
												if constexpr ( has_void_erased_notifier_for_vp_2 )
													t.notifyErased_vp_2();
												if constexpr ( alwaysCollectChanges )
													currentChanged = true;
												break;
											}
											case ActionOnVector::update_at:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_two_vp_2_T::value_type& value = t.vp_2()[pos];
												typename T::Case_two_vp_2_T::value_type oldValue;
												oldValue = value;
												if constexpr ( has_full_element_updated_notifier_for_vp_2 )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value, oldValue );
													if ( currentChanged )
													{
														t.notifyElementUpdated_vp_2( pos, oldValue );
														if constexpr ( has_element_updated_notifier_for_vp_2 )
															t.notifyElementUpdated_vp_2();
														if constexpr ( has_void_element_updated_notifier_for_vp_2 )
															t.notifyElementUpdated_vp_2();
													}
												}
												else if constexpr ( has_element_updated_notifier_for_vp_2 )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value, oldValue );
													if ( currentChanged )
													{
														t.notifyElementUpdated_vp_2( pos );
														if constexpr ( has_void_element_updated_notifier_for_vp_2 )
															t.notifyElementUpdated_vp_2();
													}
												}
												else if constexpr ( has_void_element_updated_notifier_for_vp_2 )
												{
													currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value, oldValue );
													if ( currentChanged )
														t.notifyElementUpdated_vp_2();
												}
												else
												{
													if constexpr ( alwaysCollectChanges )
														currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value, oldValue );
													else
														PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value );
												}
												break;
											}
											case ActionOnVector::insert_single_before:
											{
												::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
												typename T::Case_two_vp_2_T::value_type value;
												PublishableVectorProcessor::parseSingleValue<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, value );
												if constexpr ( has_insert_notifier3_for_vp_2 )
												{
													typename T::Case_two_vp_2_T oldVal;
													::globalmq::marshalling::impl::copyVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( t.vp_2(), oldVal );
													t.notifyInserted_vp_2( pos, oldVal );
												}
												if constexpr ( has_insert_notifier2_for_vp_2 )
													t.notifyInserted_vp_2( pos );
												if constexpr ( has_void_insert_notifier_for_vp_2 )
													t.notifyInserted_vp_2();
												t.vp_2().insert( t.vp_2().begin() + pos, value );
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
										PublishableVectorProcessor::parse<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, t.vp_2() );
										currentChanged = !::globalmq::marshalling::impl::isSameVector<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( oldVectorVal, t.vp_2() );
									}
									else
										PublishableVectorProcessor::parse<ParserT, typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( parser, t.vp_2() );

									::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
								}

								if ( currentChanged )
								{
									if constexpr( has_void_update_notifier_for_vp_2 )
										t.notifyUpdated_vp_2();
									if constexpr( has_update_notifier_for_vp_2 )
										t.notifyUpdated_vp_2( oldVectorVal );
								}
							}
							break;
						}
						default:
							throw std::exception(); // unexpected
					}
					break;
				}
				default:
					throw std::exception(); // unexpected
			}
		}
		if constexpr ( reportChanges )
			return changed;
	}

	template<typename UserT>
	static void copy(const UserT& src, UserT& dst) {
		auto srcCaseId = src.currentVariant();
		dst.initAs( srcCaseId );
		if ( srcCaseId != UserT::Variants::unknown )
			switch ( srcCaseId )
			{
				case 1: // IDL CASE one
				{
					publishable_STRUCT_point3D::copy( src.pt3d_1(), dst.pt3d_1() );
					dst.i_1() = src.i_1();
				}
				case 2: // IDL CASE two
				{
					dst.i_2() = src.i_2();
					::globalmq::marshalling::impl::copyVector<typename UserT::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( src.vp_2(), dst.vp_2() );
				}
				default:
					throw std::exception(); // unexpected
			}
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.currentVariant() != s2.currentVariant() )
			return false;
		if ( s1.currentVariant() != UserT::Variants::unknown )
			switch ( s1.currentVariant() )
			{
				case 1: // IDL CASE one
				{
					if( ! publishable_STRUCT_point3D::isSame( s1.pt3d_1(), s2.pt3d_1() ) ) return false;
					if ( s1.i_1() != s2.i_1() ) return false;
				}
				case 2: // IDL CASE two
				{
					if ( s1.i_2() != s2.i_2() ) return false;
					if ( !::globalmq::marshalling::impl::isSameVector<typename UserT::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( s1.vp_2(), s2.vp_2() ) ) return false;
				}
				default:
					throw std::exception(); // unexpected
			}
		return true;
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

struct publishable_STRUCT_PolygonMap : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "_PolygonMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::_PolygonMap), publishable_STRUCT_Vertex, true>( parser, t._PolygonMap );

	}

};

struct publishable_STRUCT_ObstacleMap : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "_ObstacleMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::_ObstacleMap), publishable_STRUCT_PolygonMap, true>( parser, t._ObstacleMap );

	}

};

struct publishable_STRUCT_Line : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "a" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::a), publishable_STRUCT_Vertex, true>( parser, t.a );

		::globalmq::marshalling::impl::parseKey( parser, "b" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::b), publishable_STRUCT_Vertex, true>( parser, t.b );

	}

};

struct publishable_STRUCT_LineMap : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "LineMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::LineMap), publishable_STRUCT_Line, true>( parser, t.LineMap );

	}

};

struct publishable_STRUCT_POINT3DREAL : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.X, "X", true );

		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.Y, "Y", true );

		::globalmq::marshalling::impl::publishableStructComposeReal( composer, t.Z, "Z", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_X = has_void_update_notifier_call_for_X<T>;
		static constexpr bool has_update_notifier_for_X = has_update_notifier_call_for_X<T, decltype(T::X)>;
		static constexpr bool has_any_notifier_for_X = has_void_update_notifier_for_X || has_update_notifier_for_X;
		static constexpr bool has_void_update_notifier_for_Y = has_void_update_notifier_call_for_Y<T>;
		static constexpr bool has_update_notifier_for_Y = has_update_notifier_call_for_Y<T, decltype(T::Y)>;
		static constexpr bool has_any_notifier_for_Y = has_void_update_notifier_for_Y || has_update_notifier_for_Y;
		static constexpr bool has_void_update_notifier_for_Z = has_void_update_notifier_call_for_Z<T>;
		static constexpr bool has_update_notifier_for_Z = has_update_notifier_call_for_Z<T, decltype(T::Z)>;
		static constexpr bool has_any_notifier_for_Z = has_void_update_notifier_for_Z || has_update_notifier_for_Z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_X || reportChanges )
		{
			decltype(T::X) oldVal = t.X;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );
			bool currentChanged = oldVal != t.X;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_X )
					t.notifyUpdated_X();
				if constexpr ( has_update_notifier_for_X )
					t.notifyUpdated_X( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::X)>( parser, &(t.X), "X" );

		if constexpr( has_any_notifier_for_Y || reportChanges )
		{
			decltype(T::Y) oldVal = t.Y;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );
			bool currentChanged = oldVal != t.Y;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_Y )
					t.notifyUpdated_Y();
				if constexpr ( has_update_notifier_for_Y )
					t.notifyUpdated_Y( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Y)>( parser, &(t.Y), "Y" );

		if constexpr( has_any_notifier_for_Z || reportChanges )
		{
			decltype(T::Z) oldVal = t.Z;
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );
			bool currentChanged = oldVal != t.Z;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_Z )
					t.notifyUpdated_Z();
				if constexpr ( has_update_notifier_for_Z )
					t.notifyUpdated_Z( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::Z)>( parser, &(t.Z), "Z" );


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
		static constexpr bool has_void_update_notifier_for_X = has_void_update_notifier_call_for_X<T>;
		static constexpr bool has_update_notifier_for_X = has_update_notifier_call_for_X<T, decltype(T::X)>;
		static constexpr bool has_any_notifier_for_X = has_void_update_notifier_for_X || has_update_notifier_for_X;
		static constexpr bool has_void_update_notifier_for_Y = has_void_update_notifier_call_for_Y<T>;
		static constexpr bool has_update_notifier_for_Y = has_update_notifier_call_for_Y<T, decltype(T::Y)>;
		static constexpr bool has_any_notifier_for_Y = has_void_update_notifier_for_Y || has_update_notifier_for_Y;
		static constexpr bool has_void_update_notifier_for_Z = has_void_update_notifier_call_for_Z<T>;
		static constexpr bool has_update_notifier_for_Z = has_update_notifier_call_for_Z<T, decltype(T::Z)>;
		static constexpr bool has_any_notifier_for_Z = has_void_update_notifier_for_Z || has_update_notifier_for_Z;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_X || reportChanges )
				{
					decltype(T::X) oldVal = t.X;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::X)>( parser, &(t.X) );
					bool currentChanged = oldVal != t.X;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_X )
							t.notifyUpdated_X();
						if constexpr ( has_update_notifier_for_X )
							t.notifyUpdated_X( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::X)>( parser, &(t.X) );
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_Y || reportChanges )
				{
					decltype(T::Y) oldVal = t.Y;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Y)>( parser, &(t.Y) );
					bool currentChanged = oldVal != t.Y;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_Y )
							t.notifyUpdated_Y();
						if constexpr ( has_update_notifier_for_Y )
							t.notifyUpdated_Y( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Y)>( parser, &(t.Y) );
				break;
			}
			case 2:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_Z || reportChanges )
				{
					decltype(T::Z) oldVal = t.Z;
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Z)>( parser, &(t.Z) );
					bool currentChanged = oldVal != t.Z;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_Z )
							t.notifyUpdated_Z();
						if constexpr ( has_update_notifier_for_Z )
							t.notifyUpdated_Z( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeReal<ParserT, decltype(T::Z)>( parser, &(t.Z) );
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
		dst.X = src.X;
		dst.Y = src.Y;
		dst.Z = src.Z;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.X != s2.X ) return false;
		if ( s1.Y != s2.Y ) return false;
		if ( s1.Z != s2.Z ) return false;
		return true;
	}
};

struct publishable_STRUCT_CharacterParamStruct : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "Size" );
		publishable_STRUCT_SIZE::parseForStateSyncOrMessageInDepth( parser, t.Size );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "Size" );
		publishable_STRUCT_SIZE::compose( composer, t.Size );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
		static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
		static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
		static constexpr bool has_void_update_notifier_for_Size = has_void_update_notifier_call_for_Size<T>;
		static constexpr bool has_update_notifier_for_Size = has_update_notifier_call_for_Size<T, decltype(T::Size)>;
		static constexpr bool has_any_notifier_for_Size = has_void_update_notifier_for_Size || has_update_notifier_for_Size;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_ID || reportChanges )
		{
			decltype(T::ID) oldVal = t.ID;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );
			bool currentChanged = oldVal != t.ID;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_ID )
					t.notifyUpdated_ID();
				if constexpr ( has_update_notifier_for_ID )
					t.notifyUpdated_ID( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "Size" );
		if constexpr( has_update_notifier_for_Size )
		{
			decltype(T::Size) temp_Size;
			publishable_STRUCT_SIZE::copy<decltype(T::Size)>( t.Size, temp_Size );
			bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr( has_void_update_notifier_for_Size )
					t.notifyUpdated_Size();
				t.notifyUpdated_Size( temp_Size );
			}
		}
		else if constexpr( has_void_update_notifier_for_Size )
		{
			bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
			if ( changedCurrent )
			{
				if constexpr ( reportChanges )
					changed = true;
				t.notifyUpdated_Size();
			}
		}
		else if constexpr ( reportChanges )
			changed = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
		else
			publishable_STRUCT_SIZE::parse( parser, t.Size );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

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
		static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
		static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
		static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
		static constexpr bool has_void_update_notifier_for_Size = has_void_update_notifier_call_for_Size<T>;
		static constexpr bool has_update_notifier_for_Size = has_update_notifier_call_for_Size<T, decltype(T::Size)>;
		static constexpr bool has_any_notifier_for_Size = has_void_update_notifier_for_Size || has_update_notifier_for_Size;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_ID || reportChanges )
				{
					decltype(T::ID) oldVal = t.ID;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					bool currentChanged = oldVal != t.ID;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_ID )
							t.notifyUpdated_ID();
						if constexpr ( has_update_notifier_for_ID )
							t.notifyUpdated_ID( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
				break;
			}
			case 1:
			{
				if ( addr.size() == offset + 1 ) // we have to parse and apply changes of this child
				{
					::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );
					if constexpr( has_update_notifier_for_Size )
					{
						decltype(T::Size) temp_Size;
						publishable_STRUCT_SIZE::copy<decltype(T::Size)>( t.Size, temp_Size );
						bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_Size )
								t.notifyUpdated_Size();
							t.notifyUpdated_Size( temp_Size );
						}
					}
					else if constexpr( has_void_update_notifier_for_Size )
					{
						bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_Size();
						}
					}

					else
					{
							if constexpr ( reportChanges )
								changed = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size );
							else
								publishable_STRUCT_SIZE::parse( parser, t.Size );
					}
					::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
				}
				else // let child continue parsing
				{
					if constexpr( has_update_notifier_for_Size )
					{
						decltype(T::Size) temp_Size;
						publishable_STRUCT_SIZE::copy<decltype(T::Size)>( t.Size, temp_Size );
						bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							if constexpr( has_void_update_notifier_for_Size )
								t.notifyUpdated_Size();
							t.notifyUpdated_Size( temp_Size );
						}
					}
					else if constexpr( has_void_update_notifier_for_Size )
					{
						bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size, addr, offset + 1 );
						if ( changedCurrent )
						{
							if constexpr ( reportChanges )
								changed = true;
							t.notifyUpdated_Size();
						}
					}
					else if constexpr ( reportChanges )
						changed = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::Size), bool>( parser, t.Size, addr, offset + 1 );
					else
						publishable_STRUCT_SIZE::parse( parser, t.Size, addr, offset + 1 );
				}
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
		dst.ID = src.ID;
		publishable_STRUCT_SIZE::copy( src.Size, dst.Size );
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.ID != s2.ID ) return false;
		if( ! publishable_STRUCT_SIZE::isSame( s1.Size, s2.Size ) ) return false;
		return true;
	}
};

struct publishable_STRUCT_StructWithVectorOfSize : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::parseKey( parser, "sizes" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE, true>( parser, t.sizes );

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::NN)>( parser, &(t.NN), "NN" );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		PublishableVectorProcessor::compose<ComposerT, decltype(T::sizes), publishable_STRUCT_SIZE>( composer, t.sizes, "sizes", true );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.NN, "NN", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_sizes = has_void_update_notifier_call_for_sizes<T>;
		static constexpr bool has_update_notifier_for_sizes = has_update_notifier_call_for_sizes<T, decltype(T::sizes)>;
		static constexpr bool has_any_notifier_for_sizes = has_void_update_notifier_for_sizes || has_update_notifier_for_sizes;
		using sizesT = decltype(T::sizes);
		static constexpr bool has_void_insert_notifier_for_sizes = has_void_insert_notifier_call_for_sizes<T>;
		static constexpr bool has_insert_notifier2_for_sizes = has_insert_notifier_call2_for_sizes<T>;
		static constexpr bool has_insert_notifier3_for_sizes = has_insert_notifier_call3_for_sizes<T, GMQ_COLL vector<sizesT>&>;
		static constexpr bool has_void_erased_notifier_for_sizes = has_void_erased_notifier_call_for_sizes<T>;
		static constexpr bool has_erased_notifier2_for_sizes = has_erased_notifier_call2_for_sizes<T>;
		static constexpr bool has_erased_notifier3_for_sizes = has_erased_notifier_call3_for_sizes<T, GMQ_COLL vector<sizesT>&>;
		static constexpr bool has_void_element_updated_notifier_for_sizes = has_element_updated_void_notifier_call_for_sizes<T>;
		static constexpr bool has_element_updated_notifier_for_sizes = has_element_updated_notifier_call_for_sizes<T>;
		static constexpr bool has_full_element_updated_notifier_for_sizes = has_full_element_updated_notifier_call_for_sizes<T, sizesT&>;
		static constexpr bool has_void_update_notifier_for_NN = has_void_update_notifier_call_for_NN<T>;
		static constexpr bool has_update_notifier_for_NN = has_update_notifier_call_for_NN<T, decltype(T::NN)>;
		static constexpr bool has_any_notifier_for_NN = has_void_update_notifier_for_NN || has_update_notifier_for_NN;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( reportChanges )
		{
			decltype(T::sizes) oldVectorVal;
			::globalmq::marshalling::impl::copyVector<decltype(T::sizes), publishable_STRUCT_SIZE>( t.sizes, oldVectorVal );
			::globalmq::marshalling::impl::parseKey( parser, "sizes" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, t.sizes );
			bool currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::sizes), publishable_STRUCT_SIZE>( oldVectorVal, t.sizes );
			changed = changed || currentChanged;
		}
		else
		{
			::globalmq::marshalling::impl::parseKey( parser, "sizes" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, t.sizes );
		}

		if constexpr( has_any_notifier_for_NN || reportChanges )
		{
			decltype(T::NN) oldVal = t.NN;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::NN)>( parser, &(t.NN), "NN" );
			bool currentChanged = oldVal != t.NN;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_NN )
					t.notifyUpdated_NN();
				if constexpr ( has_update_notifier_for_NN )
					t.notifyUpdated_NN( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::NN)>( parser, &(t.NN), "NN" );


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
		static constexpr bool has_void_update_notifier_for_sizes = has_void_update_notifier_call_for_sizes<T>;
		static constexpr bool has_update_notifier_for_sizes = has_update_notifier_call_for_sizes<T, decltype(T::sizes)>;
		static constexpr bool has_any_notifier_for_sizes = has_void_update_notifier_for_sizes || has_update_notifier_for_sizes;
		using sizesT = decltype(T::sizes);
		static constexpr bool has_void_insert_notifier_for_sizes = has_void_insert_notifier_call_for_sizes<T>;
		static constexpr bool has_insert_notifier2_for_sizes = has_insert_notifier_call2_for_sizes<T>;
		static constexpr bool has_insert_notifier3_for_sizes = has_insert_notifier_call3_for_sizes<T, GMQ_COLL vector<sizesT>&>;
		static constexpr bool has_void_erased_notifier_for_sizes = has_void_erased_notifier_call_for_sizes<T>;
		static constexpr bool has_erased_notifier2_for_sizes = has_erased_notifier_call2_for_sizes<T>;
		static constexpr bool has_erased_notifier3_for_sizes = has_erased_notifier_call3_for_sizes<T, GMQ_COLL vector<sizesT>&>;
		static constexpr bool has_void_element_updated_notifier_for_sizes = has_element_updated_void_notifier_call_for_sizes<T>;
		static constexpr bool has_element_updated_notifier_for_sizes = has_element_updated_notifier_call_for_sizes<T>;
		static constexpr bool has_full_element_updated_notifier_for_sizes = has_full_element_updated_notifier_call_for_sizes<T, sizesT&>;
		static constexpr bool has_void_update_notifier_for_NN = has_void_update_notifier_call_for_NN<T>;
		static constexpr bool has_update_notifier_for_NN = has_update_notifier_call_for_NN<T, decltype(T::NN)>;
		static constexpr bool has_any_notifier_for_NN = has_void_update_notifier_for_NN || has_update_notifier_for_NN;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				{
					decltype(T::sizes) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_sizes;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::sizes), publishable_STRUCT_SIZE>( t.sizes, oldVectorVal );
					if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[offset + 1];
						if ( pos > t.sizes.size() )
							throw std::exception();
						if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::sizes)::value_type& value = t.sizes[pos];
							if constexpr ( has_full_element_updated_notifier_for_sizes )
							{
								typename decltype(T::sizes)::value_type oldValue;
								publishable_STRUCT_SIZE::copy( value, oldValue );
								currentChanged = publishable_STRUCT_SIZE::parse<ParserT, typename decltype(T::sizes)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_sizes( pos, oldValue );
									if constexpr ( has_element_updated_notifier_for_sizes )
										t.notifyElementUpdated_sizes();
									if constexpr ( has_void_element_updated_notifier_for_sizes )
										t.notifyElementUpdated_sizes();
								}
							}
							else if constexpr ( has_element_updated_notifier_for_sizes )
							{
								currentChanged = publishable_STRUCT_SIZE::parse<ParserT, typename decltype(T::sizes)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_sizes( pos );
									if constexpr ( has_void_element_updated_notifier_for_sizes )
										t.notifyElementUpdated_sizes();
								}
							}
							else if constexpr ( has_void_element_updated_notifier_for_sizes )
							{
								currentChanged = publishable_STRUCT_SIZE::parse<ParserT, typename decltype(T::sizes)::value_type, bool>( parser, value, addr, offset + 2 );
								if ( currentChanged )
									t.notifyElementUpdated_sizes();
							}
							else
							{
								if constexpr ( alwaysCollectChanges )
									currentChanged = publishable_STRUCT_SIZE::parse<ParserT, typename decltype(T::sizes)::value_type, bool>( parser, value, addr, offset + 2 );
								else
									publishable_STRUCT_SIZE::parse<ParserT, typename decltype(T::sizes)::value_type>( parser, value, addr, offset + 2 );
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
									decltype(T::sizes) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::sizes), publishable_STRUCT_SIZE>( t.sizes, oldVal );
									t.sizes.erase( t.sizes.begin() + pos );
									if constexpr ( has_erased_notifier3_for_sizes )
										t.notifyErased_sizes( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_sizes )
										t.notifyErased_sizes( pos );
									if constexpr ( has_void_erased_notifier_for_sizes )
										t.notifyErased_sizes();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::sizes)::value_type& value = t.sizes[pos];
									typename decltype(T::sizes)::value_type oldValue;
									publishable_STRUCT_SIZE::copy( value, oldValue );
									if constexpr ( has_full_element_updated_notifier_for_sizes )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_sizes( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_sizes )
												t.notifyElementUpdated_sizes();
											if constexpr ( has_void_element_updated_notifier_for_sizes )
												t.notifyElementUpdated_sizes();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_sizes )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_sizes( pos );
											if constexpr ( has_void_element_updated_notifier_for_sizes )
												t.notifyElementUpdated_sizes();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_sizes )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value, oldValue );
										if ( currentChanged )
											t.notifyElementUpdated_sizes();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value, oldValue );
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::sizes)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, value );
									if constexpr ( has_insert_notifier3_for_sizes )
									{
										decltype(T::sizes) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::sizes), publishable_STRUCT_SIZE>( t.sizes, oldVal );
										t.notifyInserted_sizes( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_sizes )
										t.notifyInserted_sizes( pos );
									if constexpr ( has_void_insert_notifier_for_sizes )
										t.notifyInserted_sizes();
									t.sizes.insert( t.sizes.begin() + pos, value );
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
							PublishableVectorProcessor::parse<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, t.sizes );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::sizes), publishable_STRUCT_SIZE>( oldVectorVal, t.sizes );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::sizes), publishable_STRUCT_SIZE>( parser, t.sizes );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_sizes )
							t.notifyUpdated_sizes();
						if constexpr( has_update_notifier_for_sizes )
							t.notifyUpdated_sizes( oldVectorVal );
					}
				}
				break;
			}
			case 1:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_NN || reportChanges )
				{
					decltype(T::NN) oldVal = t.NN;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::NN)>( parser, &(t.NN) );
					bool currentChanged = oldVal != t.NN;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_NN )
							t.notifyUpdated_NN();
						if constexpr ( has_update_notifier_for_NN )
							t.notifyUpdated_NN( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::NN)>( parser, &(t.NN) );
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
		::globalmq::marshalling::impl::copyVector<decltype(UserT::sizes), publishable_STRUCT_SIZE>( src.sizes, dst.sizes );
		dst.NN = src.NN;
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( !::globalmq::marshalling::impl::isSameVector<decltype(UserT::sizes), publishable_STRUCT_SIZE>( s1.sizes, s2.sizes ) ) return false;
		if ( s1.NN != s2.NN ) return false;
		return true;
	}
};

struct publishable_STRUCT_StructWithVectorOfInt : public ::globalmq::marshalling::impl::StructType
{
	template<class ParserT, class T>
	static
	void parseForStateSyncOrMessageInDepth( ParserT& parser, T& t )
	{
		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::parseKey( parser, "signedInts" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType, true>( parser, t.signedInts );

	}

	template<class ComposerT, class T>
	static
	void compose( ComposerT& composer, const T& t )
	{
		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( composer, t.signedInts, "signedInts", false );

	}

	template<class ParserT, class T, class RetT = void>
	static
	RetT parse( ParserT& parser, T& t )
	{
		static_assert( std::is_same<RetT, bool>::value || std::is_same<RetT, void>::value );
		constexpr bool reportChanges = std::is_same<RetT, bool>::value;
		bool changed = false;
		static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
		static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
		static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
		static constexpr bool has_void_update_notifier_for_signedInts = has_void_update_notifier_call_for_signedInts<T>;
		static constexpr bool has_update_notifier_for_signedInts = has_update_notifier_call_for_signedInts<T, decltype(T::signedInts)>;
		static constexpr bool has_any_notifier_for_signedInts = has_void_update_notifier_for_signedInts || has_update_notifier_for_signedInts;
		using signedIntsT = decltype(T::signedInts);
		static constexpr bool has_void_insert_notifier_for_signedInts = has_void_insert_notifier_call_for_signedInts<T>;
		static constexpr bool has_insert_notifier2_for_signedInts = has_insert_notifier_call2_for_signedInts<T>;
		static constexpr bool has_insert_notifier3_for_signedInts = has_insert_notifier_call3_for_signedInts<T, GMQ_COLL vector<signedIntsT>&>;
		static constexpr bool has_void_erased_notifier_for_signedInts = has_void_erased_notifier_call_for_signedInts<T>;
		static constexpr bool has_erased_notifier2_for_signedInts = has_erased_notifier_call2_for_signedInts<T>;
		static constexpr bool has_erased_notifier3_for_signedInts = has_erased_notifier_call3_for_signedInts<T, GMQ_COLL vector<signedIntsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_signedInts = has_element_updated_void_notifier_call_for_signedInts<T>;
		static constexpr bool has_element_updated_notifier_for_signedInts = has_element_updated_notifier_call_for_signedInts<T>;
		static constexpr bool has_full_element_updated_notifier_for_signedInts = has_full_element_updated_notifier_call_for_signedInts<T, signedIntsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		if constexpr( has_any_notifier_for_ID || reportChanges )
		{
			decltype(T::ID) oldVal = t.ID;
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );
			bool currentChanged = oldVal != t.ID;
			if ( currentChanged )
			{
				if constexpr ( reportChanges )
					changed = true;
				if constexpr ( has_void_update_notifier_for_ID )
					t.notifyUpdated_ID();
				if constexpr ( has_update_notifier_for_ID )
					t.notifyUpdated_ID( oldVal );
			}
		}
		else
			::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		if constexpr( reportChanges )
		{
			decltype(T::signedInts) oldVectorVal;
			::globalmq::marshalling::impl::copyVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( t.signedInts, oldVectorVal );
			::globalmq::marshalling::impl::parseKey( parser, "signedInts" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.signedInts );
			bool currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( oldVectorVal, t.signedInts );
			changed = changed || currentChanged;
		}
		else
		{
			::globalmq::marshalling::impl::parseKey( parser, "signedInts" );
			PublishableVectorProcessor::parse<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.signedInts );
		}


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
		static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
		static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
		static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
		static constexpr bool has_void_update_notifier_for_signedInts = has_void_update_notifier_call_for_signedInts<T>;
		static constexpr bool has_update_notifier_for_signedInts = has_update_notifier_call_for_signedInts<T, decltype(T::signedInts)>;
		static constexpr bool has_any_notifier_for_signedInts = has_void_update_notifier_for_signedInts || has_update_notifier_for_signedInts;
		using signedIntsT = decltype(T::signedInts);
		static constexpr bool has_void_insert_notifier_for_signedInts = has_void_insert_notifier_call_for_signedInts<T>;
		static constexpr bool has_insert_notifier2_for_signedInts = has_insert_notifier_call2_for_signedInts<T>;
		static constexpr bool has_insert_notifier3_for_signedInts = has_insert_notifier_call3_for_signedInts<T, GMQ_COLL vector<signedIntsT>&>;
		static constexpr bool has_void_erased_notifier_for_signedInts = has_void_erased_notifier_call_for_signedInts<T>;
		static constexpr bool has_erased_notifier2_for_signedInts = has_erased_notifier_call2_for_signedInts<T>;
		static constexpr bool has_erased_notifier3_for_signedInts = has_erased_notifier_call3_for_signedInts<T, GMQ_COLL vector<signedIntsT>&>;
		static constexpr bool has_void_element_updated_notifier_for_signedInts = has_element_updated_void_notifier_call_for_signedInts<T>;
		static constexpr bool has_element_updated_notifier_for_signedInts = has_element_updated_notifier_call_for_signedInts<T>;
		static constexpr bool has_full_element_updated_notifier_for_signedInts = has_full_element_updated_notifier_call_for_signedInts<T, signedIntsT&>;
		static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;
		GMQ_ASSERT( addr.size() );
		switch ( addr[offset] )
		{
			case 0:
			{
				if ( addr.size() > offset + 1 )
					throw std::exception(); // bad format, TODO: ...
				if constexpr( has_any_notifier_for_ID || reportChanges )
				{
					decltype(T::ID) oldVal = t.ID;
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					bool currentChanged = oldVal != t.ID;
					if ( currentChanged )
					{
						if constexpr ( reportChanges )
							changed = true;
						if constexpr ( has_void_update_notifier_for_ID )
							t.notifyUpdated_ID();
						if constexpr ( has_update_notifier_for_ID )
							t.notifyUpdated_ID( oldVal );
					}
				}
				else
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
				break;
			}
			case 1:
			{
				{
					decltype(T::signedInts) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_signedInts;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( t.signedInts, oldVectorVal );
					if ( addr.size() > offset + 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[offset + 1];
						if ( pos > t.signedInts.size() )
							throw std::exception();
						if ( addr.size() > offset + 2 ) // update for a member of a particular vector element
						{
							throw std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.signedInts elements is INTEGER)
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									decltype(T::signedInts) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( t.signedInts, oldVal );
									t.signedInts.erase( t.signedInts.begin() + pos );
									if constexpr ( has_erased_notifier3_for_signedInts )
										t.notifyErased_signedInts( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_signedInts )
										t.notifyErased_signedInts( pos );
									if constexpr ( has_void_erased_notifier_for_signedInts )
										t.notifyErased_signedInts();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::signedInts)::value_type& value = t.signedInts[pos];
									typename decltype(T::signedInts)::value_type oldValue;
									oldValue = value;
									if constexpr ( has_full_element_updated_notifier_for_signedInts )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_signedInts( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_signedInts )
												t.notifyElementUpdated_signedInts();
											if constexpr ( has_void_element_updated_notifier_for_signedInts )
												t.notifyElementUpdated_signedInts();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_signedInts )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_signedInts( pos );
											if constexpr ( has_void_element_updated_notifier_for_signedInts )
												t.notifyElementUpdated_signedInts();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_signedInts )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
											t.notifyElementUpdated_signedInts();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::signedInts)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									if constexpr ( has_insert_notifier3_for_signedInts )
									{
										decltype(T::signedInts) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( t.signedInts, oldVal );
										t.notifyInserted_signedInts( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_signedInts )
										t.notifyInserted_signedInts( pos );
									if constexpr ( has_void_insert_notifier_for_signedInts )
										t.notifyInserted_signedInts();
									t.signedInts.insert( t.signedInts.begin() + pos, value );
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
							PublishableVectorProcessor::parse<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.signedInts );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( oldVectorVal, t.signedInts );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.signedInts );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_signedInts )
							t.notifyUpdated_signedInts();
						if constexpr( has_update_notifier_for_signedInts )
							t.notifyUpdated_signedInts( oldVectorVal );
					}
				}
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
		dst.ID = src.ID;
		::globalmq::marshalling::impl::copyVector<decltype(UserT::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( src.signedInts, dst.signedInts );
	}

	template<typename UserT>
	static bool isSame(const UserT& s1, const UserT& s2) {
		if ( s1.ID != s2.ID ) return false;
		if ( !::globalmq::marshalling::impl::isSameVector<decltype(UserT::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( s1.signedInts, s2.signedInts ) ) return false;
		return true;
	}
};

namespace scope_one {

using point3D = ::globalmq::marshalling::impl::MessageName<11>;
using point3D_alias = ::globalmq::marshalling::impl::MessageName<1>;
using point_alias = ::globalmq::marshalling::impl::MessageName<2>;

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
		case point3D::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point3D>( parser, handlers... ); break;
		case point3D_alias::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point3D_alias>( parser, handlers... ); break;
		case point_alias::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point_alias>( parser, handlers... ); break;
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

//**********************************************************************
// MESSAGE "point3D" Targets: JSON (Alias of point3D)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_point3D_compose(ComposerT& composer, Args&& ... args)
{
	STRUCT_point3D_compose(composer, std::forward<Args>( args )...);
}

template<class ParserT, typename ... Args>
void MESSAGE_point3D_parse(ParserT& p, Args&& ... args)
{
	STRUCT_point3D_parse(p, std::forward<Args>( args )...);
}

template<class ParserT>
structures::scope_one::MESSAGE_point3D MESSAGE_point3D_parse(ParserT& p)
{
	return static_cast<structures::scope_one::MESSAGE_point3D>(STRUCT_point3D_parse(p));
}

//**********************************************************************
// MESSAGE "point3D_alias" Targets: JSON (Alias of point3D)

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
structures::scope_one::MESSAGE_point3D_alias MESSAGE_point3D_alias_parse(ParserT& p)
{
	return static_cast<structures::scope_one::MESSAGE_point3D_alias>(STRUCT_point3D_parse(p));
}

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
structures::scope_one::MESSAGE_point_alias MESSAGE_point_alias_parse(ParserT& p)
{
	return static_cast<structures::scope_one::MESSAGE_point_alias>(STRUCT_point_parse(p));
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
	if constexpr ( msgID::id == point3D::id )
		MESSAGE_point3D_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == point3D_alias::id )
		MESSAGE_point3D_alias_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == point_alias::id )
		MESSAGE_point_alias_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace scope_one 

namespace level_trace {

using LevelTraceData = ::globalmq::marshalling::impl::MessageName<1>;

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
		case LevelTraceData::id: ok = ::globalmq::marshalling::impl::implHandleMessage<LevelTraceData>( parser, handlers... ); break;
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
// MESSAGE "LevelTraceData" Targets: JSON (2 parameters)
//  1. STRUCT CharacterParamStruct CharacterParam (REQUIRED)
//  2. VECTOR< STRUCT POINT3DREAL> Points (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_LevelTraceData_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, CharacterParam_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, Points_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this MESSAGE assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "CharacterParam", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "Points", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

template<class ParserT>
structures::level_trace::MESSAGE_LevelTraceData MESSAGE_LevelTraceData_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::level_trace::MESSAGE_LevelTraceData;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "CharacterParam" );
		publishable_STRUCT_CharacterParamStruct::parseForStateSyncOrMessageInDepth( parser, t.CharacterParam );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "Points" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::Points), publishable_STRUCT_POINT3DREAL, true>( parser, t.Points );

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
	if constexpr ( msgID::id == LevelTraceData::id )
		MESSAGE_LevelTraceData_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace level_trace 

namespace infrastructural {

using PolygonSt = ::globalmq::marshalling::impl::MessageName<2>;
using point = ::globalmq::marshalling::impl::MessageName<4>;
using point3D = ::globalmq::marshalling::impl::MessageName<5>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::GMQ, "According to IDL GMQ parser is expected" );
	parser.parseUnsignedInteger( &msgID );
	bool ok = false;

	switch ( msgID )
	{
		case PolygonSt::id: ok = ::globalmq::marshalling::impl::implHandleMessage<PolygonSt>( parser, handlers... ); break;
		case point::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point>( parser, handlers... ); break;
		case point3D::id: ok = ::globalmq::marshalling::impl::implHandleMessage<point3D>( parser, handlers... ); break;
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
// MESSAGE "PolygonSt" Targets: GMQ (6 parameters)
//  1. VECTOR< STRUCT PolygonMap> polygonMap (REQUIRED)
//  2. VECTOR< STRUCT PolygonMap> concaveMap (REQUIRED)
//  3. VECTOR< STRUCT ObstacleMap> obstacleMap (REQUIRED)
//  4. VECTOR< STRUCT LineMap> portalMap (REQUIRED)
//  5. VECTOR< STRUCT LineMap> jumpMap (REQUIRED)
//  6. REAL polygonSpeed (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_PolygonSt_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, polygonMap_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, concaveMap_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, obstacleMap_Type::Name>;
	using arg_4_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, portalMap_Type::Name>;
	using arg_5_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, jumpMap_Type::Name>;
	using arg_6_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, polygonSpeed_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_5_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_6_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_5_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_5_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_6_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, arg_6_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::infrastructural::MESSAGE_PolygonSt MESSAGE_PolygonSt_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::infrastructural::MESSAGE_PolygonSt;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parseKey( parser, "polygonMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::polygonMap), publishable_STRUCT_PolygonMap, true>( parser, t.polygonMap );

		::globalmq::marshalling::impl::parseKey( parser, "concaveMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::concaveMap), publishable_STRUCT_PolygonMap, true>( parser, t.concaveMap );

		::globalmq::marshalling::impl::parseKey( parser, "obstacleMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::obstacleMap), publishable_STRUCT_ObstacleMap, true>( parser, t.obstacleMap );

		::globalmq::marshalling::impl::parseKey( parser, "portalMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::portalMap), publishable_STRUCT_LineMap, true>( parser, t.portalMap );

		::globalmq::marshalling::impl::parseKey( parser, "jumpMap" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::jumpMap), publishable_STRUCT_LineMap, true>( parser, t.jumpMap );

		::globalmq::marshalling::impl::publishableParseReal<ParserT, decltype(T::polygonSpeed)>( parser, &(t.polygonSpeed), "polygonSpeed" );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

//**********************************************************************
// MESSAGE "point" NONEXTENDABLE Targets: GMQ (1 parameters)
//  1. STRUCT point point (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_point_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, point_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::infrastructural::MESSAGE_point MESSAGE_point_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::infrastructural::MESSAGE_point;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "point" );
		publishable_STRUCT_point::parseForStateSyncOrMessageInDepth( parser, t.point );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

//**********************************************************************
// MESSAGE "point3D" NONEXTENDABLE Targets: GMQ (3 parameters)
//  1. STRUCT point3D pt (REQUIRED)
//  2. DISCRIMINATED_UNION du_one du_one_instance (REQUIRED)
//  3. DICTIONARY num2stringDictionary (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_point3D_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, pt_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::DiscriminatedUnionType, du_one_instance_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::DictionaryOfSympleTypes<::globalmq::marshalling::impl::UnsignedIntegralType, ::globalmq::marshalling::impl::StringType>, num2stringDictionary_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
}

template<class ParserT>
structures::infrastructural::MESSAGE_point3D MESSAGE_point3D_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::infrastructural::MESSAGE_point3D;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "pt" );
		publishable_STRUCT_point3D::parseForStateSyncOrMessageInDepth( parser, t.pt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "du_one_instance" );
		publishable_DISCRIMINATED_UNION_du_one::parseForStateSyncOrMessageInDepth( parser, t.du_one_instance );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "num2stringDictionary" );
		PublishableDictionaryProcessor::parse<ParserT, decltype(T::num2stringDictionary), ::globalmq::marshalling::impl::UnsignedIntegralType, ::globalmq::marshalling::impl::StringType, true>( parser, t.num2stringDictionary );

	::globalmq::marshalling::impl::parseStructEnd( parser );
	return t;
}

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::GmqComposer composer( buffer );
	::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );
	if constexpr ( msgID::id == PolygonSt::id )
		MESSAGE_PolygonSt_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == point::id )
		MESSAGE_point_compose( composer, std::forward<Args>( args )... );
	else if constexpr ( msgID::id == point3D::id )
		MESSAGE_point3D_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
}

} // namespace infrastructural 

namespace test_gmq {

using message_one = ::globalmq::marshalling::impl::MessageName<3>;

template<class ParserT, class ... HandlersT >
void implHandleMessage( ParserT& parser, HandlersT ... handlers )
{
	uint64_t msgID;

	static_assert( ParserT::proto == Proto::GMQ, "According to IDL GMQ parser is expected" );
	parser.parseUnsignedInteger( &msgID );
	bool ok = false;

	switch ( msgID )
	{
		case message_one::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_one>( parser, handlers... ); break;
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
// MESSAGE "message_one" Targets: GMQ (10 parameters)
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
void MESSAGE_message_one_compose(ComposerT& composer, Args&& ... args)
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

	static_assert( ComposerT::proto == Proto::GMQ, "this MESSAGE assumes only GMQ protocol" );
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

template<class ParserT>
structures::test_gmq::MESSAGE_message_one MESSAGE_message_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::test_gmq::MESSAGE_message_one;
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

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::GmqComposer composer( buffer );
	::globalmq::marshalling::impl::composeUnsignedInteger( composer, msgID::id );
	if constexpr ( msgID::id == message_one::id )
		MESSAGE_message_one_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
}

} // namespace test_gmq 

namespace test_json {

using message_one = ::globalmq::marshalling::impl::MessageName<3>;

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
		case message_one::id: ok = ::globalmq::marshalling::impl::implHandleMessage<message_one>( parser, handlers... ); break;
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
// MESSAGE "message_one" Targets: JSON (10 parameters)
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
void MESSAGE_message_one_compose(ComposerT& composer, Args&& ... args)
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

	static_assert( ComposerT::proto == Proto::JSON, "this MESSAGE assumes only JSON protocol" );
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

template<class ParserT>
structures::test_json::MESSAGE_message_one MESSAGE_message_one_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::test_json::MESSAGE_message_one;
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

template<typename msgID, class BufferT, typename ... Args>
void composeMessage( BufferT& buffer, Args&& ... args )
{
	static_assert( std::is_base_of<::globalmq::marshalling::impl::MessageNameBase, msgID>::value );
	globalmq::marshalling::JsonComposer composer( buffer );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeNamedSignedInteger( composer, "msgid", msgID::id);
	composer.buff.append( ",\n  ", sizeof(",\n  ") - 1 );
	::globalmq::marshalling::impl::json::addNamePart( composer, "msgbody" );
	if constexpr ( msgID::id == message_one::id )
		MESSAGE_message_one_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace test_json 

namespace level_data {

using Level = ::globalmq::marshalling::impl::MessageName<1>;

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
		case Level::id: ok = ::globalmq::marshalling::impl::implHandleMessage<Level>( parser, handlers... ); break;
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
// MESSAGE "Level" Targets: JSON (1 parameters)
//  1. VECTOR< STRUCT PolygonSt> Level (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void MESSAGE_Level_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, Level_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this MESSAGE assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "Level", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

template<class ParserT>
structures::level_data::MESSAGE_Level MESSAGE_Level_parse(ParserT& parser)
{
	static_assert( std::is_base_of<ParserBase, ParserT>::value, "Parser must be one of GmqParser<> or JsonParser<>" );

	using T = structures::level_data::MESSAGE_Level;
	T t;
	::globalmq::marshalling::impl::parseStructBegin( parser );

		::globalmq::marshalling::impl::parseKey( parser, "Level" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::Level), publishable_STRUCT_PolygonSt, true>( parser, t.Level );

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
	if constexpr ( msgID::id == Level::id )
		MESSAGE_Level_compose( composer, std::forward<Args>( args )... );
	else
		static_assert( std::is_same<::globalmq::marshalling::impl::MessageNameBase, msgID>::value, "unexpected value of msgID" ); // note: should be just static_assert(false,"..."); but it seems that in this case clang asserts yet before looking at constexpr conditions
	composer.buff.append( "\n}", 2 );
}

} // namespace level_data 

//**********************************************************************
// PUBLISHABLE publishable_short_sample (2 parameters)
// 1. INTEGER ID
// 2. CHARACTER_STRING name
//**********************************************************************

template<class T, class ComposerT>
class publishable_short_sample_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );


public:
	static constexpr uint64_t numTypeID = 1;
	static constexpr const char* stringTypeID = "publishable_short_sample";

	template<class ... ArgsT>
	publishable_short_sample_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	auto get_ID() { return t.ID; }
	void set_ID( decltype(T::ID) val) { 
		t.ID = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( composer, t.ID );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( composer, t.name );
	}

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class publishable_short_sample_NodecppWrapperForPublisher : public publishable_short_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	publishable_short_sample_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_short_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_short_sample_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { publishable_short_sample_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  publishable_short_sample_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { publishable_short_sample_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return publishable_short_sample_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class publishable_short_sample_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );

	static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
	static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
	static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
	static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
	static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
	static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 1;
	static constexpr const char* stringTypeID = "publishable_short_sample";

	template<class ... ArgsT>
	publishable_short_sample_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
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
					if constexpr( has_any_notifier_for_ID )
					{
						decltype(T::ID) oldVal = t.ID;
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
						bool currentChanged = oldVal != t.ID;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_ID )
								t.notifyUpdated_ID();
							if constexpr ( has_update_notifier_for_ID )
								t.notifyUpdated_ID( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					break;
				}
				case 1:
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	auto get_ID() { return t.ID; }
	const auto& get_name() { return t.name; }
};

template<class T, class RegistrarT>
class publishable_short_sample_NodecppWrapperForSubscriber : public publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	publishable_short_sample_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_short_sample_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return publishable_short_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class publishable_short_sample_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );


public:
	static constexpr uint64_t numTypeID = 1;

	publishable_short_sample_WrapperForConcentrator() {}
	const char* name() {return "publishable_short_sample";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", false );


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
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					break;
				}
				case 1:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};

//**********************************************************************
// PUBLISHABLE publishable_sample (9 parameters)
// 1. INTEGER ID
// 2. CHARACTER_STRING name
// 3. STRUCT SIZE size
// 4. STRUCT CharacterParamStruct chp
// 5. VECTOR<INTEGER> vector_of_int
// 6. VECTOR< STRUCT POINT3DREAL> vector_struct_point3dreal
// 7. STRUCT StructWithVectorOfInt structWithVectorOfInt
// 8. STRUCT StructWithVectorOfSize structWithVectorOfSize
// 9. DISCRIMINATED_UNION du_one_instance
//**********************************************************************

template<class T, class ComposerT>
class publishable_sample_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_size = has_size_member<T>;
	static_assert( has_size, "type T must have member T::size of a type corresponding to IDL type STRUCT SIZE" );
	static constexpr bool has_chp = has_chp_member<T>;
	static_assert( has_chp, "type T must have member T::chp of a type corresponding to IDL type STRUCT CharacterParamStruct" );
	static constexpr bool has_vector_of_int = has_vector_of_int_member<T>;
	static_assert( has_vector_of_int, "type T must have member T::vector_of_int of a type corresponding to IDL type VECTOR<INTEGER>" );
	static constexpr bool has_vector_struct_point3dreal = has_vector_struct_point3dreal_member<T>;
	static_assert( has_vector_struct_point3dreal, "type T must have member T::vector_struct_point3dreal of a type corresponding to IDL type VECTOR<STRUCT POINT3DREAL>" );
	static constexpr bool has_structWithVectorOfInt = has_structWithVectorOfInt_member<T>;
	static_assert( has_structWithVectorOfInt, "type T must have member T::structWithVectorOfInt of a type corresponding to IDL type STRUCT StructWithVectorOfInt" );
	static constexpr bool has_structWithVectorOfSize = has_structWithVectorOfSize_member<T>;
	static_assert( has_structWithVectorOfSize, "type T must have member T::structWithVectorOfSize of a type corresponding to IDL type STRUCT StructWithVectorOfSize" );
	static constexpr bool has_du_one_instance = has_du_one_instance_member<T>;
	static_assert( has_du_one_instance, "type T must have member T::du_one_instance of a type corresponding to IDL type DISCRIMINATED_UNION du_one" );


public:
	static constexpr uint64_t numTypeID = 2;
	static constexpr const char* stringTypeID = "publishable_sample";

	template<class ... ArgsT>
	publishable_sample_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
	const T& getState() { return t; }
	ComposerT& getComposer() { return composer; }
	void startTick( BufferT&& buff ) { buffer = std::move( buff ); composer.reset(); ::globalmq::marshalling::impl::composeStateUpdateMessageBegin<ComposerT>( composer );}
	BufferT&& endTick() { ::globalmq::marshalling::impl::composeStateUpdateMessageEnd( composer ); return std::move( buffer ); }
	const char* name() { return stringTypeID; }
	virtual uint64_t stateTypeID() { return numTypeID; }
	auto get_ID() { return t.ID; }
	void set_ID( decltype(T::ID) val) { 
		t.ID = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( composer, t.ID );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( composer, t.name );
	}
	const auto& get_size() { return t.size; }
	void set_size( decltype(T::size) val) { 
		t.size = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_SIZE::compose( composer, t.size );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_size() { return SIZE_RefWrapper4Set<decltype(T::size), publishable_sample_WrapperForPublisher>(t.size, *this, GMQ_COLL vector<size_t>(), 2); }
	const auto& get_chp() { return t.chp; }
	void set_chp( decltype(T::chp) val) { 
		t.chp = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 3 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_CharacterParamStruct::compose( composer, t.chp );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_chp() { return CharacterParamStruct_RefWrapper4Set<decltype(T::chp), publishable_sample_WrapperForPublisher>(t.chp, *this, GMQ_COLL vector<size_t>(), 3); }
	auto get_vector_of_int() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.vector_of_int); }
	void set_vector_of_int( decltype(T::vector_of_int) val) { 
		t.vector_of_int = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 4 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( composer );
		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( composer, t.vector_of_int );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( composer );
	}
	auto get4set_vector_of_int() { return globalmq::marshalling::VectorRefWrapper4Set<decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType, publishable_sample_WrapperForPublisher>(t.vector_of_int, *this, GMQ_COLL vector<size_t>(), 4); }
	auto get_vector_struct_point3dreal() { return globalmq::marshalling::VectorOfStructRefWrapper<POINT3DREAL_RefWrapper<typename decltype(T::vector_struct_point3dreal)::value_type>, decltype(T::vector_struct_point3dreal)>(t.vector_struct_point3dreal); }
	void set_vector_struct_point3dreal( decltype(T::vector_struct_point3dreal) val) { 
		t.vector_struct_point3dreal = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 5 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( composer );
		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( composer, t.vector_struct_point3dreal );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( composer );
	}
	auto get4set_vector_struct_point3dreal() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL, publishable_sample_WrapperForPublisher, POINT3DREAL_RefWrapper4Set<typename decltype(T::vector_struct_point3dreal)::value_type, publishable_sample_WrapperForPublisher>>(t.vector_struct_point3dreal, *this, GMQ_COLL vector<size_t>(), 5); }
	const auto& get_structWithVectorOfInt() { return t.structWithVectorOfInt; }
	void set_structWithVectorOfInt( decltype(T::structWithVectorOfInt) val) { 
		t.structWithVectorOfInt = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 6 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_StructWithVectorOfInt::compose( composer, t.structWithVectorOfInt );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_structWithVectorOfInt() { return StructWithVectorOfInt_RefWrapper4Set<decltype(T::structWithVectorOfInt), publishable_sample_WrapperForPublisher>(t.structWithVectorOfInt, *this, GMQ_COLL vector<size_t>(), 6); }
	const auto& get_structWithVectorOfSize() { return t.structWithVectorOfSize; }
	void set_structWithVectorOfSize( decltype(T::structWithVectorOfSize) val) { 
		t.structWithVectorOfSize = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 7 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_STRUCT_StructWithVectorOfSize::compose( composer, t.structWithVectorOfSize );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_structWithVectorOfSize() { return StructWithVectorOfSize_RefWrapper4Set<decltype(T::structWithVectorOfSize), publishable_sample_WrapperForPublisher>(t.structWithVectorOfSize, *this, GMQ_COLL vector<size_t>(), 7); }
	const auto& get_du_one_instance() { return t.du_one_instance; }
	void set_du_one_instance( decltype(T::du_one_instance) val) { 
		t.du_one_instance = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( composer, GMQ_COLL vector<size_t>(), 8 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( composer );
		publishable_DISCRIMINATED_UNION_du_one::compose( composer, t.du_one_instance );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( composer );
	}
	auto get4set_du_one_instance() { return du_one_RefWrapper4Set<decltype(T::du_one_instance), publishable_sample_WrapperForPublisher>(t.du_one_instance, *this, GMQ_COLL vector<size_t>(), 8); }

	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "size" );
		publishable_STRUCT_SIZE::compose( composer, t.size );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "chp" );
		publishable_STRUCT_CharacterParamStruct::compose( composer, t.chp );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( composer, t.vector_of_int, "vector_of_int", true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( composer, t.vector_struct_point3dreal, "vector_struct_point3dreal", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "structWithVectorOfInt" );
		publishable_STRUCT_StructWithVectorOfInt::compose( composer, t.structWithVectorOfInt );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "structWithVectorOfSize" );
		publishable_STRUCT_StructWithVectorOfSize::compose( composer, t.structWithVectorOfSize );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "du_one_instance" );
		publishable_DISCRIMINATED_UNION_du_one::compose( composer, t.du_one_instance );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, false );


		::globalmq::marshalling::impl::composeStructEnd( composer );
	}
};

template<class T, class RegistrarT>
class publishable_sample_NodecppWrapperForPublisher : public publishable_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	publishable_sample_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_sample_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_sample_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { publishable_sample_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  publishable_sample_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { publishable_sample_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return publishable_sample_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class publishable_sample_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
{
	T t;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_size = has_size_member<T>;
	static_assert( has_size, "type T must have member T::size of a type corresponding to IDL type STRUCT SIZE" );
	static constexpr bool has_chp = has_chp_member<T>;
	static_assert( has_chp, "type T must have member T::chp of a type corresponding to IDL type STRUCT CharacterParamStruct" );
	static constexpr bool has_vector_of_int = has_vector_of_int_member<T>;
	static_assert( has_vector_of_int, "type T must have member T::vector_of_int of a type corresponding to IDL type VECTOR<INTEGER>" );
	static constexpr bool has_vector_struct_point3dreal = has_vector_struct_point3dreal_member<T>;
	static_assert( has_vector_struct_point3dreal, "type T must have member T::vector_struct_point3dreal of a type corresponding to IDL type VECTOR<STRUCT POINT3DREAL>" );
	static constexpr bool has_structWithVectorOfInt = has_structWithVectorOfInt_member<T>;
	static_assert( has_structWithVectorOfInt, "type T must have member T::structWithVectorOfInt of a type corresponding to IDL type STRUCT StructWithVectorOfInt" );
	static constexpr bool has_structWithVectorOfSize = has_structWithVectorOfSize_member<T>;
	static_assert( has_structWithVectorOfSize, "type T must have member T::structWithVectorOfSize of a type corresponding to IDL type STRUCT StructWithVectorOfSize" );
	static constexpr bool has_du_one_instance = has_du_one_instance_member<T>;
	static_assert( has_du_one_instance, "type T must have member T::du_one_instance of a type corresponding to IDL type DISCRIMINATED_UNION du_one" );

	static constexpr bool has_void_update_notifier_for_ID = has_void_update_notifier_call_for_ID<T>;
	static constexpr bool has_update_notifier_for_ID = has_update_notifier_call_for_ID<T, decltype(T::ID)>;
	static constexpr bool has_any_notifier_for_ID = has_void_update_notifier_for_ID || has_update_notifier_for_ID;
	static constexpr bool has_void_update_notifier_for_name = has_void_update_notifier_call_for_name<T>;
	static constexpr bool has_update_notifier_for_name = has_update_notifier_call_for_name<T, decltype(T::name)>;
	static constexpr bool has_any_notifier_for_name = has_void_update_notifier_for_name || has_update_notifier_for_name;
	static constexpr bool has_void_update_notifier_for_size = has_void_update_notifier_call_for_size<T>;
	static constexpr bool has_update_notifier_for_size = has_update_notifier_call_for_size<T, decltype(T::size)>;
	static constexpr bool has_any_notifier_for_size = has_void_update_notifier_for_size || has_update_notifier_for_size;
	static constexpr bool has_void_update_notifier_for_chp = has_void_update_notifier_call_for_chp<T>;
	static constexpr bool has_update_notifier_for_chp = has_update_notifier_call_for_chp<T, decltype(T::chp)>;
	static constexpr bool has_any_notifier_for_chp = has_void_update_notifier_for_chp || has_update_notifier_for_chp;
	static constexpr bool has_void_update_notifier_for_vector_of_int = has_void_update_notifier_call_for_vector_of_int<T>;
	static constexpr bool has_update_notifier_for_vector_of_int = has_update_notifier_call_for_vector_of_int<T, decltype(T::vector_of_int)>;
	static constexpr bool has_any_notifier_for_vector_of_int = has_void_update_notifier_for_vector_of_int || has_update_notifier_for_vector_of_int;
	using vector_of_intT = decltype(T::vector_of_int);
	static constexpr bool has_void_insert_notifier_for_vector_of_int = has_void_insert_notifier_call_for_vector_of_int<T>;
	static constexpr bool has_insert_notifier2_for_vector_of_int = has_insert_notifier_call2_for_vector_of_int<T>;
	static constexpr bool has_insert_notifier3_for_vector_of_int = has_insert_notifier_call3_for_vector_of_int<T, GMQ_COLL vector<vector_of_intT>&>;
	static constexpr bool has_void_erased_notifier_for_vector_of_int = has_void_erased_notifier_call_for_vector_of_int<T>;
	static constexpr bool has_erased_notifier2_for_vector_of_int = has_erased_notifier_call2_for_vector_of_int<T>;
	static constexpr bool has_erased_notifier3_for_vector_of_int = has_erased_notifier_call3_for_vector_of_int<T, GMQ_COLL vector<vector_of_intT>&>;
	static constexpr bool has_void_element_updated_notifier_for_vector_of_int = has_element_updated_void_notifier_call_for_vector_of_int<T>;
	static constexpr bool has_element_updated_notifier_for_vector_of_int = has_element_updated_notifier_call_for_vector_of_int<T>;
	static constexpr bool has_full_element_updated_notifier_for_vector_of_int = has_full_element_updated_notifier_call_for_vector_of_int<T, vector_of_intT&>;
	static constexpr bool has_void_update_notifier_for_vector_struct_point3dreal = has_void_update_notifier_call_for_vector_struct_point3dreal<T>;
	static constexpr bool has_update_notifier_for_vector_struct_point3dreal = has_update_notifier_call_for_vector_struct_point3dreal<T, decltype(T::vector_struct_point3dreal)>;
	static constexpr bool has_any_notifier_for_vector_struct_point3dreal = has_void_update_notifier_for_vector_struct_point3dreal || has_update_notifier_for_vector_struct_point3dreal;
	using vector_struct_point3drealT = decltype(T::vector_struct_point3dreal);
	static constexpr bool has_void_insert_notifier_for_vector_struct_point3dreal = has_void_insert_notifier_call_for_vector_struct_point3dreal<T>;
	static constexpr bool has_insert_notifier2_for_vector_struct_point3dreal = has_insert_notifier_call2_for_vector_struct_point3dreal<T>;
	static constexpr bool has_insert_notifier3_for_vector_struct_point3dreal = has_insert_notifier_call3_for_vector_struct_point3dreal<T, GMQ_COLL vector<vector_struct_point3drealT>&>;
	static constexpr bool has_void_erased_notifier_for_vector_struct_point3dreal = has_void_erased_notifier_call_for_vector_struct_point3dreal<T>;
	static constexpr bool has_erased_notifier2_for_vector_struct_point3dreal = has_erased_notifier_call2_for_vector_struct_point3dreal<T>;
	static constexpr bool has_erased_notifier3_for_vector_struct_point3dreal = has_erased_notifier_call3_for_vector_struct_point3dreal<T, GMQ_COLL vector<vector_struct_point3drealT>&>;
	static constexpr bool has_void_element_updated_notifier_for_vector_struct_point3dreal = has_element_updated_void_notifier_call_for_vector_struct_point3dreal<T>;
	static constexpr bool has_element_updated_notifier_for_vector_struct_point3dreal = has_element_updated_notifier_call_for_vector_struct_point3dreal<T>;
	static constexpr bool has_full_element_updated_notifier_for_vector_struct_point3dreal = has_full_element_updated_notifier_call_for_vector_struct_point3dreal<T, vector_struct_point3drealT&>;
	static constexpr bool has_void_update_notifier_for_structWithVectorOfInt = has_void_update_notifier_call_for_structWithVectorOfInt<T>;
	static constexpr bool has_update_notifier_for_structWithVectorOfInt = has_update_notifier_call_for_structWithVectorOfInt<T, decltype(T::structWithVectorOfInt)>;
	static constexpr bool has_any_notifier_for_structWithVectorOfInt = has_void_update_notifier_for_structWithVectorOfInt || has_update_notifier_for_structWithVectorOfInt;
	static constexpr bool has_void_update_notifier_for_structWithVectorOfSize = has_void_update_notifier_call_for_structWithVectorOfSize<T>;
	static constexpr bool has_update_notifier_for_structWithVectorOfSize = has_update_notifier_call_for_structWithVectorOfSize<T, decltype(T::structWithVectorOfSize)>;
	static constexpr bool has_any_notifier_for_structWithVectorOfSize = has_void_update_notifier_for_structWithVectorOfSize || has_update_notifier_for_structWithVectorOfSize;
	static constexpr bool has_void_update_notifier_for_du_one_instance = has_void_update_notifier_call_for_du_one_instance<T>;
	static constexpr bool has_update_notifier_for_du_one_instance = has_update_notifier_call_for_du_one_instance<T, decltype(T::du_one_instance)>;
	static constexpr bool has_any_notifier_for_du_one_instance = has_void_update_notifier_for_du_one_instance || has_update_notifier_for_du_one_instance;
	static constexpr bool has_full_update_notifier = has_full_update_notifier_call<T>;

public:
	static constexpr uint64_t numTypeID = 2;
	static constexpr const char* stringTypeID = "publishable_sample";

	template<class ... ArgsT>
	publishable_sample_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
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
					if constexpr( has_any_notifier_for_ID )
					{
						decltype(T::ID) oldVal = t.ID;
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
						bool currentChanged = oldVal != t.ID;
						if ( currentChanged )
						{
							if constexpr ( has_void_update_notifier_for_ID )
								t.notifyUpdated_ID();
							if constexpr ( has_update_notifier_for_ID )
								t.notifyUpdated_ID( oldVal );
						}
					}
					else
						::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					break;
				}
				case 1:
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
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_size )
						{
							decltype(T::size) temp_size;
							publishable_STRUCT_SIZE::copy<decltype(T::size)>( t.size, temp_size );
							bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::size), bool>( parser, t.size );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_size )
									t.notifyUpdated_size();
								t.notifyUpdated_size( temp_size );
							}
						}
						else if constexpr( has_void_update_notifier_for_size )
						{
							bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::size), bool>( parser, t.size );
							if ( changedCurrent )
							{
								t.notifyUpdated_size();
							}
						}

						else
						{
							publishable_STRUCT_SIZE::parse( parser, t.size );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_size )
						{
							decltype(T::size) temp_size;
							publishable_STRUCT_SIZE::copy<decltype(T::size)>( t.size, temp_size );
							bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::size), bool>( parser, t.size, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_size )
									t.notifyUpdated_size();
								t.notifyUpdated_size( temp_size );
							}
						}
						else if constexpr( has_void_update_notifier_for_size )
						{
							bool changedCurrent = publishable_STRUCT_SIZE::parse<ParserT, decltype(T::size), bool>( parser, t.size, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_size();
							}
						}
						else
							publishable_STRUCT_SIZE::parse( parser, t.size, addr, 1 );
					}
					break;
				}
				case 3:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_chp )
						{
							decltype(T::chp) temp_chp;
							publishable_STRUCT_CharacterParamStruct::copy<decltype(T::chp)>( t.chp, temp_chp );
							bool changedCurrent = publishable_STRUCT_CharacterParamStruct::parse<ParserT, decltype(T::chp), bool>( parser, t.chp );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_chp )
									t.notifyUpdated_chp();
								t.notifyUpdated_chp( temp_chp );
							}
						}
						else if constexpr( has_void_update_notifier_for_chp )
						{
							bool changedCurrent = publishable_STRUCT_CharacterParamStruct::parse<ParserT, decltype(T::chp), bool>( parser, t.chp );
							if ( changedCurrent )
							{
								t.notifyUpdated_chp();
							}
						}

						else
						{
							publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_chp )
						{
							decltype(T::chp) temp_chp;
							publishable_STRUCT_CharacterParamStruct::copy<decltype(T::chp)>( t.chp, temp_chp );
							bool changedCurrent = publishable_STRUCT_CharacterParamStruct::parse<ParserT, decltype(T::chp), bool>( parser, t.chp, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_chp )
									t.notifyUpdated_chp();
								t.notifyUpdated_chp( temp_chp );
							}
						}
						else if constexpr( has_void_update_notifier_for_chp )
						{
							bool changedCurrent = publishable_STRUCT_CharacterParamStruct::parse<ParserT, decltype(T::chp), bool>( parser, t.chp, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_chp();
							}
						}
						else
							publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp, addr, 1 );
					}
					break;
				}
				case 4:
				{
					decltype(T::vector_of_int) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_vector_of_int;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( t.vector_of_int, oldVectorVal );
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vector_of_int.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							throw std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.vector_of_int elements is INTEGER)
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									decltype(T::vector_of_int) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( t.vector_of_int, oldVal );
									t.vector_of_int.erase( t.vector_of_int.begin() + pos );
									if constexpr ( has_erased_notifier3_for_vector_of_int )
										t.notifyErased_vector_of_int( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_vector_of_int )
										t.notifyErased_vector_of_int( pos );
									if constexpr ( has_void_erased_notifier_for_vector_of_int )
										t.notifyErased_vector_of_int();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_of_int)::value_type& value = t.vector_of_int[pos];
									typename decltype(T::vector_of_int)::value_type oldValue;
									oldValue = value;
									if constexpr ( has_full_element_updated_notifier_for_vector_of_int )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vector_of_int( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_vector_of_int )
												t.notifyElementUpdated_vector_of_int();
											if constexpr ( has_void_element_updated_notifier_for_vector_of_int )
												t.notifyElementUpdated_vector_of_int();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_vector_of_int )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vector_of_int( pos );
											if constexpr ( has_void_element_updated_notifier_for_vector_of_int )
												t.notifyElementUpdated_vector_of_int();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_vector_of_int )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										if ( currentChanged )
											t.notifyElementUpdated_vector_of_int();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value, oldValue );
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_of_int)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									if constexpr ( has_insert_notifier3_for_vector_of_int )
									{
										decltype(T::vector_of_int) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( t.vector_of_int, oldVal );
										t.notifyInserted_vector_of_int( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_vector_of_int )
										t.notifyInserted_vector_of_int( pos );
									if constexpr ( has_void_insert_notifier_for_vector_of_int )
										t.notifyInserted_vector_of_int();
									t.vector_of_int.insert( t.vector_of_int.begin() + pos, value );
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
							PublishableVectorProcessor::parse<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.vector_of_int );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( oldVectorVal, t.vector_of_int );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.vector_of_int );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_vector_of_int )
							t.notifyUpdated_vector_of_int();
						if constexpr( has_update_notifier_for_vector_of_int )
							t.notifyUpdated_vector_of_int( oldVectorVal );
					}
					break;
				}
				case 5:
				{
					decltype(T::vector_struct_point3dreal) oldVectorVal;
					bool currentChanged = false;
					constexpr bool alwaysCollectChanges = has_any_notifier_for_vector_struct_point3dreal;
					if constexpr( alwaysCollectChanges )
						::globalmq::marshalling::impl::copyVector<decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( t.vector_struct_point3dreal, oldVectorVal );
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vector_struct_point3dreal.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::vector_struct_point3dreal)::value_type& value = t.vector_struct_point3dreal[pos];
							if constexpr ( has_full_element_updated_notifier_for_vector_struct_point3dreal )
							{
								typename decltype(T::vector_struct_point3dreal)::value_type oldValue;
								publishable_STRUCT_POINT3DREAL::copy( value, oldValue );
								currentChanged = publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_vector_struct_point3dreal( pos, oldValue );
									if constexpr ( has_element_updated_notifier_for_vector_struct_point3dreal )
										t.notifyElementUpdated_vector_struct_point3dreal();
									if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
										t.notifyElementUpdated_vector_struct_point3dreal();
								}
							}
							else if constexpr ( has_element_updated_notifier_for_vector_struct_point3dreal )
							{
								currentChanged = publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
								{
									t.notifyElementUpdated_vector_struct_point3dreal( pos );
									if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
										t.notifyElementUpdated_vector_struct_point3dreal();
								}
							}
							else if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
							{
								currentChanged = publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type, bool>( parser, value, addr, 2 );
								if ( currentChanged )
									t.notifyElementUpdated_vector_struct_point3dreal();
							}
							else
							{
								if constexpr ( alwaysCollectChanges )
									currentChanged = publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type, bool>( parser, value, addr, 2 );
								else
									publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type>( parser, value, addr, 2 );
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
									decltype(T::vector_struct_point3dreal) oldVal;
									::globalmq::marshalling::impl::copyVector<decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( t.vector_struct_point3dreal, oldVal );
									t.vector_struct_point3dreal.erase( t.vector_struct_point3dreal.begin() + pos );
									if constexpr ( has_erased_notifier3_for_vector_struct_point3dreal )
										t.notifyErased_vector_struct_point3dreal( pos, oldVal );
									if constexpr ( has_erased_notifier2_for_vector_struct_point3dreal )
										t.notifyErased_vector_struct_point3dreal( pos );
									if constexpr ( has_void_erased_notifier_for_vector_struct_point3dreal )
										t.notifyErased_vector_struct_point3dreal();
									if constexpr ( alwaysCollectChanges )
										currentChanged = true;
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_struct_point3dreal)::value_type& value = t.vector_struct_point3dreal[pos];
									typename decltype(T::vector_struct_point3dreal)::value_type oldValue;
									publishable_STRUCT_POINT3DREAL::copy( value, oldValue );
									if constexpr ( has_full_element_updated_notifier_for_vector_struct_point3dreal )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vector_struct_point3dreal( pos, oldValue );
											if constexpr ( has_element_updated_notifier_for_vector_struct_point3dreal )
												t.notifyElementUpdated_vector_struct_point3dreal();
											if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
												t.notifyElementUpdated_vector_struct_point3dreal();
										}
									}
									else if constexpr ( has_element_updated_notifier_for_vector_struct_point3dreal )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value, oldValue );
										if ( currentChanged )
										{
											t.notifyElementUpdated_vector_struct_point3dreal( pos );
											if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
												t.notifyElementUpdated_vector_struct_point3dreal();
										}
									}
									else if constexpr ( has_void_element_updated_notifier_for_vector_struct_point3dreal )
									{
										currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value, oldValue );
										if ( currentChanged )
											t.notifyElementUpdated_vector_struct_point3dreal();
									}
									else
									{
										if constexpr ( alwaysCollectChanges )
											currentChanged = PublishableVectorProcessor::parseSingleValueAndCompare<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value, oldValue );
										else
											PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value );
									}
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_struct_point3dreal)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value );
									if constexpr ( has_insert_notifier3_for_vector_struct_point3dreal )
									{
										decltype(T::vector_struct_point3dreal) oldVal;
										::globalmq::marshalling::impl::copyVector<decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( t.vector_struct_point3dreal, oldVal );
										t.notifyInserted_vector_struct_point3dreal( pos, oldVal );
									}
									if constexpr ( has_insert_notifier2_for_vector_struct_point3dreal )
										t.notifyInserted_vector_struct_point3dreal( pos );
									if constexpr ( has_void_insert_notifier_for_vector_struct_point3dreal )
										t.notifyInserted_vector_struct_point3dreal();
									t.vector_struct_point3dreal.insert( t.vector_struct_point3dreal.begin() + pos, value );
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
							PublishableVectorProcessor::parse<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, t.vector_struct_point3dreal );
							currentChanged = !::globalmq::marshalling::impl::isSameVector<decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( oldVectorVal, t.vector_struct_point3dreal );
						}
						else
							PublishableVectorProcessor::parse<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, t.vector_struct_point3dreal );

						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					if ( currentChanged )
					{
						if constexpr( has_void_update_notifier_for_vector_struct_point3dreal )
							t.notifyUpdated_vector_struct_point3dreal();
						if constexpr( has_update_notifier_for_vector_struct_point3dreal )
							t.notifyUpdated_vector_struct_point3dreal( oldVectorVal );
					}
					break;
				}
				case 6:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_structWithVectorOfInt )
						{
							decltype(T::structWithVectorOfInt) temp_structWithVectorOfInt;
							publishable_STRUCT_StructWithVectorOfInt::copy<decltype(T::structWithVectorOfInt)>( t.structWithVectorOfInt, temp_structWithVectorOfInt );
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfInt::parse<ParserT, decltype(T::structWithVectorOfInt), bool>( parser, t.structWithVectorOfInt );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_structWithVectorOfInt )
									t.notifyUpdated_structWithVectorOfInt();
								t.notifyUpdated_structWithVectorOfInt( temp_structWithVectorOfInt );
							}
						}
						else if constexpr( has_void_update_notifier_for_structWithVectorOfInt )
						{
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfInt::parse<ParserT, decltype(T::structWithVectorOfInt), bool>( parser, t.structWithVectorOfInt );
							if ( changedCurrent )
							{
								t.notifyUpdated_structWithVectorOfInt();
							}
						}

						else
						{
							publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_structWithVectorOfInt )
						{
							decltype(T::structWithVectorOfInt) temp_structWithVectorOfInt;
							publishable_STRUCT_StructWithVectorOfInt::copy<decltype(T::structWithVectorOfInt)>( t.structWithVectorOfInt, temp_structWithVectorOfInt );
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfInt::parse<ParserT, decltype(T::structWithVectorOfInt), bool>( parser, t.structWithVectorOfInt, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_structWithVectorOfInt )
									t.notifyUpdated_structWithVectorOfInt();
								t.notifyUpdated_structWithVectorOfInt( temp_structWithVectorOfInt );
							}
						}
						else if constexpr( has_void_update_notifier_for_structWithVectorOfInt )
						{
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfInt::parse<ParserT, decltype(T::structWithVectorOfInt), bool>( parser, t.structWithVectorOfInt, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_structWithVectorOfInt();
							}
						}
						else
							publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt, addr, 1 );
					}
					break;
				}
				case 7:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_structWithVectorOfSize )
						{
							decltype(T::structWithVectorOfSize) temp_structWithVectorOfSize;
							publishable_STRUCT_StructWithVectorOfSize::copy<decltype(T::structWithVectorOfSize)>( t.structWithVectorOfSize, temp_structWithVectorOfSize );
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfSize::parse<ParserT, decltype(T::structWithVectorOfSize), bool>( parser, t.structWithVectorOfSize );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_structWithVectorOfSize )
									t.notifyUpdated_structWithVectorOfSize();
								t.notifyUpdated_structWithVectorOfSize( temp_structWithVectorOfSize );
							}
						}
						else if constexpr( has_void_update_notifier_for_structWithVectorOfSize )
						{
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfSize::parse<ParserT, decltype(T::structWithVectorOfSize), bool>( parser, t.structWithVectorOfSize );
							if ( changedCurrent )
							{
								t.notifyUpdated_structWithVectorOfSize();
							}
						}

						else
						{
							publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_structWithVectorOfSize )
						{
							decltype(T::structWithVectorOfSize) temp_structWithVectorOfSize;
							publishable_STRUCT_StructWithVectorOfSize::copy<decltype(T::structWithVectorOfSize)>( t.structWithVectorOfSize, temp_structWithVectorOfSize );
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfSize::parse<ParserT, decltype(T::structWithVectorOfSize), bool>( parser, t.structWithVectorOfSize, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_structWithVectorOfSize )
									t.notifyUpdated_structWithVectorOfSize();
								t.notifyUpdated_structWithVectorOfSize( temp_structWithVectorOfSize );
							}
						}
						else if constexpr( has_void_update_notifier_for_structWithVectorOfSize )
						{
							bool changedCurrent = publishable_STRUCT_StructWithVectorOfSize::parse<ParserT, decltype(T::structWithVectorOfSize), bool>( parser, t.structWithVectorOfSize, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_structWithVectorOfSize();
							}
						}
						else
							publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize, addr, 1 );
					}
					break;
				}
				case 8:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						if constexpr( has_update_notifier_for_du_one_instance )
						{
							decltype(T::du_one_instance) temp_du_one_instance;
							publishable_DISCRIMINATED_UNION_du_one::copy<decltype(T::du_one_instance)>( t.du_one_instance, temp_du_one_instance );
							bool changedCurrent = publishable_DISCRIMINATED_UNION_du_one::parse<ParserT, decltype(T::du_one_instance), bool>( parser, t.du_one_instance );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_du_one_instance )
									t.notifyUpdated_du_one_instance();
								t.notifyUpdated_du_one_instance( temp_du_one_instance );
							}
						}
						else if constexpr( has_void_update_notifier_for_du_one_instance )
						{
							bool changedCurrent = publishable_DISCRIMINATED_UNION_du_one::parse<ParserT, decltype(T::du_one_instance), bool>( parser, t.du_one_instance );
							if ( changedCurrent )
							{
								t.notifyUpdated_du_one_instance();
							}
						}

						else
						{
							publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance );
						}

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						if constexpr( has_update_notifier_for_du_one_instance )
						{
							decltype(T::du_one_instance) temp_du_one_instance;
							publishable_DISCRIMINATED_UNION_du_one::copy<decltype(T::du_one_instance)>( t.du_one_instance, temp_du_one_instance );
							bool changedCurrent = publishable_DISCRIMINATED_UNION_du_one::parse<ParserT, decltype(T::du_one_instance), bool>( parser, t.du_one_instance, addr, 1 );
							if ( changedCurrent )
							{
								if constexpr( has_void_update_notifier_for_du_one_instance )
									t.notifyUpdated_du_one_instance();
								t.notifyUpdated_du_one_instance( temp_du_one_instance );
							}
						}
						else if constexpr( has_void_update_notifier_for_du_one_instance )
						{
							bool changedCurrent = publishable_DISCRIMINATED_UNION_du_one::parse<ParserT, decltype(T::du_one_instance), bool>( parser, t.du_one_instance, addr, 1 );
							if ( changedCurrent )
							{
								t.notifyUpdated_du_one_instance();
							}
						}
						else
							publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance, addr, 1 );
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "size" );
		publishable_STRUCT_SIZE::parse( parser, t.size );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "chp" );
		publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "vector_of_int" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.vector_of_int );

		::globalmq::marshalling::impl::parseKey( parser, "vector_struct_point3dreal" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, t.vector_struct_point3dreal );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "structWithVectorOfInt" );
		publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "structWithVectorOfSize" );
		publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "du_one_instance" );
		publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );

		if constexpr ( has_full_update_notifier )
			t.notifyFullyUpdated();
	}
	auto get_ID() { return t.ID; }
	const auto& get_name() { return t.name; }
	const auto& get_size() { return t.size; }
	const auto& get_chp() { return t.chp; }
	auto get_vector_of_int() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.vector_of_int); }
	auto get_vector_struct_point3dreal() { return globalmq::marshalling::VectorOfStructRefWrapper<POINT3DREAL_RefWrapper<typename decltype(T::vector_struct_point3dreal)::value_type>, decltype(T::vector_struct_point3dreal)>(t.vector_struct_point3dreal); }
	const auto& get_structWithVectorOfInt() { return t.structWithVectorOfInt; }
	const auto& get_structWithVectorOfSize() { return t.structWithVectorOfSize; }
	const auto& get_du_one_instance() { return t.du_one_instance; }
};

template<class T, class RegistrarT>
class publishable_sample_NodecppWrapperForSubscriber : public publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	publishable_sample_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_sample_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return publishable_sample_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class publishable_sample_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_size = has_size_member<T>;
	static_assert( has_size, "type T must have member T::size of a type corresponding to IDL type STRUCT SIZE" );
	static constexpr bool has_chp = has_chp_member<T>;
	static_assert( has_chp, "type T must have member T::chp of a type corresponding to IDL type STRUCT CharacterParamStruct" );
	static constexpr bool has_vector_of_int = has_vector_of_int_member<T>;
	static_assert( has_vector_of_int, "type T must have member T::vector_of_int of a type corresponding to IDL type VECTOR<INTEGER>" );
	static constexpr bool has_vector_struct_point3dreal = has_vector_struct_point3dreal_member<T>;
	static_assert( has_vector_struct_point3dreal, "type T must have member T::vector_struct_point3dreal of a type corresponding to IDL type VECTOR<STRUCT POINT3DREAL>" );
	static constexpr bool has_structWithVectorOfInt = has_structWithVectorOfInt_member<T>;
	static_assert( has_structWithVectorOfInt, "type T must have member T::structWithVectorOfInt of a type corresponding to IDL type STRUCT StructWithVectorOfInt" );
	static constexpr bool has_structWithVectorOfSize = has_structWithVectorOfSize_member<T>;
	static_assert( has_structWithVectorOfSize, "type T must have member T::structWithVectorOfSize of a type corresponding to IDL type STRUCT StructWithVectorOfSize" );
	static constexpr bool has_du_one_instance = has_du_one_instance_member<T>;
	static_assert( has_du_one_instance, "type T must have member T::du_one_instance of a type corresponding to IDL type DISCRIMINATED_UNION du_one" );


public:
	static constexpr uint64_t numTypeID = 2;

	publishable_sample_WrapperForConcentrator() {}
	const char* name() {return "publishable_sample";}
	
	// Acting as publisher
	virtual void generateStateSyncMessage( ComposerT& composer ) { compose(composer); }
	template<class ComposerType>
	void compose( ComposerType& composer )
	{
		::globalmq::marshalling::impl::composeStructBegin( composer );

		::globalmq::marshalling::impl::publishableStructComposeInteger( composer, t.ID, "ID", true );

		::globalmq::marshalling::impl::publishableStructComposeString( composer, t.name, "name", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "size" );
		publishable_STRUCT_SIZE::compose( composer, t.size );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "chp" );
		publishable_STRUCT_CharacterParamStruct::compose( composer, t.chp );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( composer, t.vector_of_int, "vector_of_int", true );

		PublishableVectorProcessor::compose<ComposerT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( composer, t.vector_struct_point3dreal, "vector_struct_point3dreal", true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "structWithVectorOfInt" );
		publishable_STRUCT_StructWithVectorOfInt::compose( composer, t.structWithVectorOfInt );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "structWithVectorOfSize" );
		publishable_STRUCT_StructWithVectorOfSize::compose( composer, t.structWithVectorOfSize );
		::globalmq::marshalling::impl::composePublishableStructEnd( composer, true );

		::globalmq::marshalling::impl::composePublishableStructBegin( composer, "du_one_instance" );
		publishable_DISCRIMINATED_UNION_du_one::compose( composer, t.du_one_instance );
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
					::globalmq::marshalling::impl::publishableParseLeafeInteger<ParserT, decltype(T::ID)>( parser, &(t.ID) );
					break;
				}
				case 1:
				{
					if ( addr.size() > 1 )
						throw std::exception(); // bad format, TODO: ...
					::globalmq::marshalling::impl::publishableParseLeafeString<ParserT, decltype(T::name)>( parser, &(t.name) );
					break;
				}
				case 2:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_SIZE::parse( parser, t.size );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_SIZE::parse( parser, t.size, addr, 1 );
					}
					break;
				}
				case 3:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp, addr, 1 );
					}
					break;
				}
				case 4:
				{
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vector_of_int.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							throw std::exception(); // deeper address is unrelated to simple type of vector elements (IDL type of t.vector_of_int elements is INTEGER)
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									t.vector_of_int.erase( t.vector_of_int.begin() + pos );
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_of_int)::value_type& value = t.vector_of_int[pos];
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_of_int)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, value );
									t.vector_of_int.insert( t.vector_of_int.begin() + pos, value );
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
						PublishableVectorProcessor::parse<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.vector_of_int );
						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					break;
				}
				case 5:
				{
					if ( addr.size() > 1 ) // one of actions over elements of the vector
					{
						size_t pos = addr[1];
						if ( pos > t.vector_struct_point3dreal.size() )
							throw std::exception();
						if ( addr.size() > 2 ) // update for a member of a particular vector element
						{
							typename decltype(T::vector_struct_point3dreal)::value_type& value = t.vector_struct_point3dreal[pos];
							publishable_STRUCT_POINT3DREAL::parse<ParserT, typename decltype(T::vector_struct_point3dreal)::value_type>( parser, value, addr, 2 );
						}
						else // update of one or more elelments as a whole
						{
							size_t action;
							::globalmq::marshalling::impl::parseActionInPublishable( parser, action );
							switch ( action )
							{
								case ActionOnVector::remove_at:
								{
									t.vector_struct_point3dreal.erase( t.vector_struct_point3dreal.begin() + pos );
									break;
								}
								case ActionOnVector::update_at:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_struct_point3dreal)::value_type& value = t.vector_struct_point3dreal[pos];
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value );
									break;
								}
								case ActionOnVector::insert_single_before:
								{
									::globalmq::marshalling::impl::publishableParseLeafeValueBegin( parser );
									typename decltype(T::vector_struct_point3dreal)::value_type value;
									PublishableVectorProcessor::parseSingleValue<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, value );
									t.vector_struct_point3dreal.insert( t.vector_struct_point3dreal.begin() + pos, value );
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
						PublishableVectorProcessor::parse<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, t.vector_struct_point3dreal );
						::globalmq::marshalling::impl::publishableParseLeafeVectorEnd( parser );
					}

					break;
				}
				case 6:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt, addr, 1 );
					}
					break;
				}
				case 7:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize, addr, 1 );
					}
					break;
				}
				case 8:
				{
					if ( addr.size() == 1 ) // we have to parse and apply changes of this child
					{
						::globalmq::marshalling::impl::publishableParseLeafeStructBegin( parser );

						publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance );

						::globalmq::marshalling::impl::publishableParseLeafeStructEnd( parser );
					}
					else // let child continue parsing
					{
						publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance, addr, 1 );
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

		::globalmq::marshalling::impl::publishableParseInteger<ParserT, decltype(T::ID)>( parser, &(t.ID), "ID" );

		::globalmq::marshalling::impl::publishableParseString<ParserT, decltype(T::name)>( parser, &(t.name), "name" );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "size" );
		publishable_STRUCT_SIZE::parse( parser, t.size );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "chp" );
		publishable_STRUCT_CharacterParamStruct::parse( parser, t.chp );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseKey( parser, "vector_of_int" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vector_of_int), ::globalmq::marshalling::impl::SignedIntegralType>( parser, t.vector_of_int );

		::globalmq::marshalling::impl::parseKey( parser, "vector_struct_point3dreal" );
		PublishableVectorProcessor::parse<ParserT, decltype(T::vector_struct_point3dreal), publishable_STRUCT_POINT3DREAL>( parser, t.vector_struct_point3dreal );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "structWithVectorOfInt" );
		publishable_STRUCT_StructWithVectorOfInt::parse( parser, t.structWithVectorOfInt );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "structWithVectorOfSize" );
		publishable_STRUCT_StructWithVectorOfSize::parse( parser, t.structWithVectorOfSize );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parsePublishableStructBegin( parser, "du_one_instance" );
		publishable_DISCRIMINATED_UNION_du_one::parse( parser, t.du_one_instance );
		::globalmq::marshalling::impl::parsePublishableStructEnd( parser );

		::globalmq::marshalling::impl::parseStructEnd( parser );
	}
};

//**********************************************************************
// PUBLISHABLE publishable_html_tag (1 parameters)
// 1. STRUCT HtmlTag tag
//**********************************************************************

template<class T, class ComposerT>
class publishable_html_tag_WrapperForPublisher : public globalmq::marshalling::StatePublisherBase<ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	BufferT buffer;
	ComposerT composer;
	static constexpr bool has_tag = has_tag_member<T>;
	static_assert( has_tag, "type T must have member T::tag of a type corresponding to IDL type STRUCT HtmlTag" );


public:
	static constexpr uint64_t numTypeID = 3;
	static constexpr const char* stringTypeID = "publishable_html_tag";

	template<class ... ArgsT>
	publishable_html_tag_WrapperForPublisher( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ), composer( buffer ) {}
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
	auto get4set_tag() { return HtmlTag_RefWrapper4Set<decltype(T::tag), publishable_html_tag_WrapperForPublisher>(t.tag, *this, GMQ_COLL vector<size_t>(), 0); }

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
class publishable_html_tag_NodecppWrapperForPublisher : public publishable_html_tag_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>
{
	using ComposerT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT;
	RegistrarT& registrar;
public:
	using BufferT = typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT::BufferType;
	template<class ... ArgsT>
	publishable_html_tag_NodecppWrapperForPublisher( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_html_tag_WrapperForPublisher<T, typename GMQueueStatePublisherSubscriberTypeInfo::ComposerT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_html_tag_NodecppWrapperForPublisher()
	{ 
		registrar.remove( this );
	}

	virtual void startTick( BufferT&& buff ) { publishable_html_tag_WrapperForPublisher<T, ComposerT>::startTick( std::move( buff ) ); }
	virtual BufferT&& endTick() { return  publishable_html_tag_WrapperForPublisher<T, ComposerT>::endTick(); }
	virtual void generateStateSyncMessage(ComposerT& composer) { publishable_html_tag_WrapperForPublisher<T, ComposerT>::compose(composer); }
	virtual const char* name() { return publishable_html_tag_WrapperForPublisher<T, ComposerT>::name(); }
};

template<class T, class BufferT>
class publishable_html_tag_WrapperForSubscriber : public globalmq::marshalling::StateSubscriberBase<BufferT>
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
	static constexpr const char* stringTypeID = "publishable_html_tag";

	template<class ... ArgsT>
	publishable_html_tag_WrapperForSubscriber( ArgsT&& ... args ) : t( std::forward<ArgsT>( args )... ) {}
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
class publishable_html_tag_NodecppWrapperForSubscriber : public publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>
{
	RegistrarT& registrar;
public:
	template<class ... ArgsT>
	publishable_html_tag_NodecppWrapperForSubscriber( RegistrarT& registrar_, ArgsT&& ... args ) : publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>( std::forward<ArgsT>( args )... ), registrar( registrar_ )
	{ 
		registrar.add( this );
	}

	virtual ~publishable_html_tag_NodecppWrapperForSubscriber()
	{ 
		registrar.remove( this );
	}

	virtual void applyGmqMessageWithUpdates( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyJsonMessageWithUpdates( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::applyMessageWithUpdates(parser);
	}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser ) 
	{
		publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}

	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>& parser )
	{
		publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::parseStateSyncMessage(parser);
	}
	virtual const char* name()
	{
		return publishable_html_tag_WrapperForSubscriber<T, typename GMQueueStatePublisherSubscriberTypeInfo::BufferT>::name();
	}
	void subscribe(GMQ_COLL string path)
	{
		registrar.subscribe( this, path );
	}
};

template<class T, class InputBufferT, class ComposerT>
class publishable_html_tag_WrapperForConcentrator : public globalmq::marshalling::StateConcentratorBase<InputBufferT, ComposerT>
{
	T t;
	using BufferT = typename ComposerT::BufferType;
	static constexpr bool has_tag = has_tag_member<T>;
	static_assert( has_tag, "type T must have member T::tag of a type corresponding to IDL type STRUCT HtmlTag" );


public:
	static constexpr uint64_t numTypeID = 3;

	publishable_html_tag_WrapperForConcentrator() {}
	const char* name() {return "publishable_html_tag";}
	
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
			case 1:
				return new publishable_short_sample_WrapperForConcentrator<structures::publishable_short_sample, InputBufferT, ComposerT>;
			case 2:
				return new publishable_sample_WrapperForConcentrator<structures::publishable_sample, InputBufferT, ComposerT>;
			case 3:
				return new publishable_html_tag_WrapperForConcentrator<structures::publishable_html_tag, InputBufferT, ComposerT>;
			default:
				return nullptr;
		}
	}
};

//===============================================================================

template<class T>
class HtmlTextOrTags_RefWrapper
{
	T& t;


public:
	HtmlTextOrTags_RefWrapper( T& actual ) : t( actual ) {}
	auto get_currentVariant() { return t.currentVariant(); }
	const auto& get_str() { return t.str(); }
	auto get_tags() { return globalmq::marshalling::VectorOfStructRefWrapper<HtmlTag_RefWrapper<typename T::Case_taglists_tags_T::value_type>, typename T::Case_taglists_tags_T>(t.tags()); }
};

template<class T, class RootT>
class HtmlTextOrTags_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;


public:
	HtmlTextOrTags_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_currentVariant() { return t.currentVariant(); }
	void set_currentVariant( decltype(T::v) v ) { 
		t.initAs( v ); 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( root.getComposer(), (uint64_t)(t.currentVariant()) );
	}
	const auto& get_str() { return t.str(); }
	void set_str( typename T::Case_text_str_T val) { 
		t.str() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.str() );
	}
	auto get_tags() { return globalmq::marshalling::VectorOfStructRefWrapper<HtmlTag_RefWrapper<typename T::Case_taglists_tags_T::value_type>, typename T::Case_taglists_tags_T>(t.tags()); }
	void set_tags( typename T::Case_taglists_tags_T val) { 
		t.tags() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag>( root.getComposer(), t.tags() );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_tags() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<typename T::Case_taglists_tags_T, publishable_STRUCT_HtmlTag, RootT, HtmlTag_RefWrapper4Set<typename T::Case_taglists_tags_T::value_type, RootT>>(t.tags(), root, address, 1); }
};

template<class T>
class point3D_RefWrapper
{
	T& t;
	static constexpr bool has_x = has_x_member<T>;
	static_assert( has_x, "type T must have member T::x of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_y = has_y_member<T>;
	static_assert( has_y, "type T must have member T::y of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_z = has_z_member<T>;
	static_assert( has_z, "type T must have member T::z of a type corresponding to IDL type INTEGER" );


public:
	point3D_RefWrapper( T& actual ) : t( actual ) {}
	auto get_x() { return t.x; }
	auto get_y() { return t.y; }
	auto get_z() { return t.z; }
};

template<class T, class RootT>
class point3D_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_x = has_x_member<T>;
	static_assert( has_x, "type T must have member T::x of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_y = has_y_member<T>;
	static_assert( has_y, "type T must have member T::y of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_z = has_z_member<T>;
	static_assert( has_z, "type T must have member T::z of a type corresponding to IDL type INTEGER" );


public:
	point3D_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_x() { return t.x; }
	void set_x( decltype(T::x) val) { 
		t.x = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.x );
	}
	auto get_y() { return t.y; }
	void set_y( decltype(T::y) val) { 
		t.y = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.y );
	}
	auto get_z() { return t.z; }
	void set_z( decltype(T::z) val) { 
		t.z = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.z );
	}
};

template<class T>
class SIZE_RefWrapper
{
	T& t;
	static constexpr bool has_X = has_X_member<T>;
	static_assert( has_X, "type T must have member T::X of a type corresponding to IDL type REAL" );
	static constexpr bool has_Y = has_Y_member<T>;
	static_assert( has_Y, "type T must have member T::Y of a type corresponding to IDL type REAL" );
	static constexpr bool has_Z = has_Z_member<T>;
	static_assert( has_Z, "type T must have member T::Z of a type corresponding to IDL type REAL" );


public:
	SIZE_RefWrapper( T& actual ) : t( actual ) {}
	auto get_X() { return t.X; }
	auto get_Y() { return t.Y; }
	auto get_Z() { return t.Z; }
};

template<class T, class RootT>
class SIZE_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_X = has_X_member<T>;
	static_assert( has_X, "type T must have member T::X of a type corresponding to IDL type REAL" );
	static constexpr bool has_Y = has_Y_member<T>;
	static_assert( has_Y, "type T must have member T::Y of a type corresponding to IDL type REAL" );
	static constexpr bool has_Z = has_Z_member<T>;
	static_assert( has_Z, "type T must have member T::Z of a type corresponding to IDL type REAL" );


public:
	SIZE_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_X() { return t.X; }
	void set_X( decltype(T::X) val) { 
		t.X = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.X );
	}
	auto get_Y() { return t.Y; }
	void set_Y( decltype(T::Y) val) { 
		t.Y = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.Y );
	}
	auto get_Z() { return t.Z; }
	void set_Z( decltype(T::Z) val) { 
		t.Z = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.Z );
	}
};

template<class T>
class HtmlTag_RefWrapper
{
	T& t;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_properties = has_properties_member<T>;
	static_assert( has_properties, "type T must have member T::properties of a type corresponding to IDL type DICTIONARY<CHARACTER_STRING, CHARACTER_STRING>" );
	static constexpr bool has_tags = has_tags_member<T>;
	static_assert( has_tags, "type T must have member T::tags of a type corresponding to IDL type DISCRIMINATED_UNION HtmlTextOrTags" );


public:
	HtmlTag_RefWrapper( T& actual ) : t( actual ) {}
	const auto& get_name() { return t.name; }
	auto get_properties() { return globalmq::marshalling::DictionaryOfSimpleTypeRefWrapper(t.properties); }
	const auto& get_tags() { return t.tags; }
};

template<class T, class RootT>
class HtmlTag_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_name = has_name_member<T>;
	static_assert( has_name, "type T must have member T::name of a type corresponding to IDL type CHARACTER_STRING" );
	static constexpr bool has_properties = has_properties_member<T>;
	static_assert( has_properties, "type T must have member T::properties of a type corresponding to IDL type DICTIONARY<CHARACTER_STRING, CHARACTER_STRING>" );
	static constexpr bool has_tags = has_tags_member<T>;
	static_assert( has_tags, "type T must have member T::tags of a type corresponding to IDL type DISCRIMINATED_UNION HtmlTextOrTags" );


public:
	HtmlTag_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	const auto& get_name() { return t.name; }
	void set_name( decltype(T::name) val) { 
		t.name = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeString( root.getComposer(), t.name );
	}
	auto get_properties() { return globalmq::marshalling::DictionaryOfSimpleTypeRefWrapper(t.properties); }
	void set_properties( decltype(T::properties) val) { 
		t.properties = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableDictionaryProcessor::compose<decltype(root.getComposer()), decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType>( root.getComposer(), t.properties );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_properties() { return globalmq::marshalling::DictionaryRefWrapper4Set<decltype(T::properties), ::globalmq::marshalling::impl::StringType, ::globalmq::marshalling::impl::StringType, RootT>(t.properties, *this, GMQ_COLL vector<size_t>(), 1); }
	const auto& get_tags() { return t.tags; }
	void set_tags( decltype(T::tags) val) { 
		t.tags = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_DISCRIMINATED_UNION_HtmlTextOrTags::compose( root.getComposer(), t.tags );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_tags() { return HtmlTextOrTags_RefWrapper4Set<decltype(T::tags), RootT>(t.tags, root, address, 2); }
};

template<class T>
class du_one_RefWrapper
{
	T& t;


public:
	du_one_RefWrapper( T& actual ) : t( actual ) {}
	auto get_currentVariant() { return t.currentVariant(); }
	const auto& get_pt3d_1() { return t.pt3d_1(); }
	auto get_i_1() { return t.i_1(); }
	auto get_i_2() { return t.i_2(); }
	auto get_vp_2() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.vp_2()); }
};

template<class T, class RootT>
class du_one_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;


public:
	du_one_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_currentVariant() { return t.currentVariant(); }
	void set_currentVariant( decltype(T::v) v ) { 
		t.initAs( v ); 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeUnsignedInteger( root.getComposer(), (uint64_t)(t.currentVariant()) );
	}
	const auto& get_pt3d_1() { return t.pt3d_1(); }
	void set_pt3d_1( typename T::Case_one_pt3d_1_T val) { 
		t.pt3d_1() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_STRUCT_point3D::compose( root.getComposer(), t.pt3d_1() );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_pt3d_1() { return point3D_RefWrapper4Set<typename T::Case_one_pt3d_1_T, RootT>(t.pt3d_1(), root, address, 1); }
	auto get_i_1() { return t.i_1(); }
	void set_i_1( typename T::Case_one_i_1_T val) { 
		t.i_1() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.i_1() );
	}
	auto get_i_2() { return t.i_2(); }
	void set_i_2( typename T::Case_two_i_2_T val) { 
		t.i_2() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.i_2() );
	}
	auto get_vp_2() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.vp_2()); }
	void set_vp_2( typename T::Case_two_vp_2_T val) { 
		t.vp_2() = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType>( root.getComposer(), t.vp_2() );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_vp_2() { return globalmq::marshalling::VectorRefWrapper4Set<typename T::Case_two_vp_2_T, ::globalmq::marshalling::impl::RealType, RootT>(t.vp_2(), *this, GMQ_COLL vector<size_t>(), 2); }
};

template<class T>
class POINT3DREAL_RefWrapper
{
	T& t;
	static constexpr bool has_X = has_X_member<T>;
	static_assert( has_X, "type T must have member T::X of a type corresponding to IDL type REAL" );
	static constexpr bool has_Y = has_Y_member<T>;
	static_assert( has_Y, "type T must have member T::Y of a type corresponding to IDL type REAL" );
	static constexpr bool has_Z = has_Z_member<T>;
	static_assert( has_Z, "type T must have member T::Z of a type corresponding to IDL type REAL" );


public:
	POINT3DREAL_RefWrapper( T& actual ) : t( actual ) {}
	auto get_X() { return t.X; }
	auto get_Y() { return t.Y; }
	auto get_Z() { return t.Z; }
};

template<class T, class RootT>
class POINT3DREAL_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_X = has_X_member<T>;
	static_assert( has_X, "type T must have member T::X of a type corresponding to IDL type REAL" );
	static constexpr bool has_Y = has_Y_member<T>;
	static_assert( has_Y, "type T must have member T::Y of a type corresponding to IDL type REAL" );
	static constexpr bool has_Z = has_Z_member<T>;
	static_assert( has_Z, "type T must have member T::Z of a type corresponding to IDL type REAL" );


public:
	POINT3DREAL_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_X() { return t.X; }
	void set_X( decltype(T::X) val) { 
		t.X = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.X );
	}
	auto get_Y() { return t.Y; }
	void set_Y( decltype(T::Y) val) { 
		t.Y = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.Y );
	}
	auto get_Z() { return t.Z; }
	void set_Z( decltype(T::Z) val) { 
		t.Z = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 2 );
		::globalmq::marshalling::impl::publishableComposeLeafeReal( root.getComposer(), t.Z );
	}
};

template<class T>
class CharacterParamStruct_RefWrapper
{
	T& t;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_Size = has_Size_member<T>;
	static_assert( has_Size, "type T must have member T::Size of a type corresponding to IDL type STRUCT SIZE" );


public:
	CharacterParamStruct_RefWrapper( T& actual ) : t( actual ) {}
	auto get_ID() { return t.ID; }
	const auto& get_Size() { return t.Size; }
};

template<class T, class RootT>
class CharacterParamStruct_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_Size = has_Size_member<T>;
	static_assert( has_Size, "type T must have member T::Size of a type corresponding to IDL type STRUCT SIZE" );


public:
	CharacterParamStruct_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_ID() { return t.ID; }
	void set_ID( decltype(T::ID) val) { 
		t.ID = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.ID );
	}
	const auto& get_Size() { return t.Size; }
	void set_Size( decltype(T::Size) val) { 
		t.Size = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeStructBegin( root.getComposer() );
		publishable_STRUCT_SIZE::compose( root.getComposer(), t.Size );
		::globalmq::marshalling::impl::publishableComposeLeafeStructEnd( root.getComposer() );
	}
	auto get4set_Size() { return SIZE_RefWrapper4Set<decltype(T::Size), RootT>(t.Size, root, address, 1); }
};

template<class T>
class StructWithVectorOfSize_RefWrapper
{
	T& t;
	static constexpr bool has_sizes = has_sizes_member<T>;
	static_assert( has_sizes, "type T must have member T::sizes of a type corresponding to IDL type VECTOR<STRUCT SIZE>" );
	static constexpr bool has_NN = has_NN_member<T>;
	static_assert( has_NN, "type T must have member T::NN of a type corresponding to IDL type INTEGER" );


public:
	StructWithVectorOfSize_RefWrapper( T& actual ) : t( actual ) {}
	auto get_sizes() { return globalmq::marshalling::VectorOfStructRefWrapper<SIZE_RefWrapper<typename decltype(T::sizes)::value_type>, decltype(T::sizes)>(t.sizes); }
	auto get_NN() { return t.NN; }
};

template<class T, class RootT>
class StructWithVectorOfSize_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_sizes = has_sizes_member<T>;
	static_assert( has_sizes, "type T must have member T::sizes of a type corresponding to IDL type VECTOR<STRUCT SIZE>" );
	static constexpr bool has_NN = has_NN_member<T>;
	static_assert( has_NN, "type T must have member T::NN of a type corresponding to IDL type INTEGER" );


public:
	StructWithVectorOfSize_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_sizes() { return globalmq::marshalling::VectorOfStructRefWrapper<SIZE_RefWrapper<typename decltype(T::sizes)::value_type>, decltype(T::sizes)>(t.sizes); }
	void set_sizes( decltype(T::sizes) val) { 
		t.sizes = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), decltype(T::sizes), publishable_STRUCT_SIZE>( root.getComposer(), t.sizes );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_sizes() { return globalmq::marshalling::VectorOfStructRefWrapper4Set<decltype(T::sizes), publishable_STRUCT_SIZE, RootT, SIZE_RefWrapper4Set<typename decltype(T::sizes)::value_type, RootT>>(t.sizes, root, address, 0); }
	auto get_NN() { return t.NN; }
	void set_NN( decltype(T::NN) val) { 
		t.NN = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.NN );
	}
};

template<class T>
class StructWithVectorOfInt_RefWrapper
{
	T& t;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_signedInts = has_signedInts_member<T>;
	static_assert( has_signedInts, "type T must have member T::signedInts of a type corresponding to IDL type VECTOR<INTEGER>" );


public:
	StructWithVectorOfInt_RefWrapper( T& actual ) : t( actual ) {}
	auto get_ID() { return t.ID; }
	auto get_signedInts() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.signedInts); }
};

template<class T, class RootT>
class StructWithVectorOfInt_RefWrapper4Set
{
	T& t;
	RootT& root;
	GMQ_COLL vector<size_t> address;
	static constexpr bool has_ID = has_ID_member<T>;
	static_assert( has_ID, "type T must have member T::ID of a type corresponding to IDL type INTEGER" );
	static constexpr bool has_signedInts = has_signedInts_member<T>;
	static_assert( has_signedInts, "type T must have member T::signedInts of a type corresponding to IDL type VECTOR<INTEGER>" );


public:
	StructWithVectorOfInt_RefWrapper4Set( T& actual, RootT& root_, const GMQ_COLL vector<size_t> address_, size_t idx ) : t( actual ), root( root_ ) {
		address = address_;
		address.push_back (idx );
	}
	auto get_ID() { return t.ID; }
	void set_ID( decltype(T::ID) val) { 
		t.ID = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 0 );
		::globalmq::marshalling::impl::publishableComposeLeafeInteger( root.getComposer(), t.ID );
	}
	auto get_signedInts() { return globalmq::marshalling::VectorOfSimpleTypeRefWrapper(t.signedInts); }
	void set_signedInts( decltype(T::signedInts) val) { 
		t.signedInts = val; 
		::globalmq::marshalling::impl::composeAddressInPublishable( root.getComposer(), address, 1 );
		::globalmq::marshalling::impl::publishableComposeLeafeValueBegin( root.getComposer() );
		PublishableVectorProcessor::compose<decltype(root.getComposer()), decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType>( root.getComposer(), t.signedInts );
		::globalmq::marshalling::impl::composeStateUpdateBlockEnd( root.getComposer() );
	}
	auto get4set_signedInts() { return globalmq::marshalling::VectorRefWrapper4Set<decltype(T::signedInts), ::globalmq::marshalling::impl::SignedIntegralType, RootT>(t.signedInts, *this, GMQ_COLL vector<size_t>(), 1); }
};

//**********************************************************************
// STRUCT "CharacterParamStruct" Targets: JSON (2 parameters)
//  1. INTEGER ID (REQUIRED)
//  2. STRUCT SIZE Size (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_CharacterParamStruct_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, ID_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, Size_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "ID", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "Size", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

//**********************************************************************
// STRUCT "SIZE" Targets: JSON (3 parameters)
//  1. REAL X (REQUIRED)
//  2. REAL Y (REQUIRED)
//  3. REAL Z (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_SIZE_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, X_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, Y_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, Z_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "X", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "Y", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_3_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "Z", arg_3_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

//**********************************************************************
// STRUCT "POINT3DREAL" Targets: JSON (3 parameters)
//  1. REAL X (REQUIRED)
//  2. REAL Y (REQUIRED)
//  3. REAL Z (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_POINT3DREAL_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, X_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, Y_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::RealType, Z_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "X", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "Y", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_3_type, true, FloatingDefault<0ll,-1023ll>, int, 0>(composer, "Z", arg_3_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

//**********************************************************************
// STRUCT "LineMap" Targets: GMQ (1 parameters)
//  1. VECTOR< STRUCT Line> LineMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_LineMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, LineMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this STRUCT assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
}

//**********************************************************************
// STRUCT "Line" Targets: GMQ (2 parameters)
//  1. VECTOR<NONEXTENDABLE STRUCT Vertex> a (REQUIRED)
//  2. VECTOR<NONEXTENDABLE STRUCT Vertex> b (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_Line_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfNonextMessageTypes, a_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfNonextMessageTypes, b_Type::Name>;

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

//**********************************************************************
// STRUCT "ObstacleMap" Targets: GMQ (1 parameters)
//  1. VECTOR< STRUCT PolygonMap> _ObstacleMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_ObstacleMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, _ObstacleMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this STRUCT assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
}

//**********************************************************************
// STRUCT "PolygonMap" Targets: GMQ (1 parameters)
//  1. VECTOR<NONEXTENDABLE STRUCT Vertex> _PolygonMap (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_PolygonMap_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfNonextMessageTypes, _PolygonMap_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this STRUCT assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
}

//**********************************************************************
// STRUCT "Vertex" NONEXTENDABLE Targets: JSON GMQ (3 parameters)
//  1. INTEGER x (REQUIRED)
//  2. INTEGER y (REQUIRED)
//  3. INTEGER z (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_Vertex_compose(ComposerT& composer, Args&& ... args)
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
// DISCRIMINATED_UNION "du_one" Targets: GMQ (2 cases)
//  CASE one (2 parameters)(2 parameters)
//    1. STRUCT point3D pt3d_1 (REQUIRED)
//    2. INTEGER i_1 (REQUIRED)
//  CASE two (2 parameters)(2 parameters)
//    1. INTEGER i_2 (REQUIRED)
//    2. VECTOR<REAL> vp_2 (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void DISCRIMINATED_UNION_du_one_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, pt3d_1_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, i_1_Type::Name>;
	using arg_3_type = NamedParameterWithType<::globalmq::marshalling::impl::SignedIntegralType, i_2_Type::Name>;
	using arg_4_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::RealType>, vp_2_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_3_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_4_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::GMQ, "this DISCRIMINATED_UNION assumes only GMQ protocol" );
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_1_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_1_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_2_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_3_type, true, int64_t, int64_t, (int64_t)(0)>(composer, arg_3_type::nameAndTypeID, args...);
	::globalmq::marshalling::impl::gmq::composeParamToGmq<ComposerT, arg_4_type, true, uint64_t, uint64_t, (uint64_t)(0)>(composer, arg_4_type::nameAndTypeID, args...);
}

//**********************************************************************
// STRUCT "PolygonSt" Targets: JSON (2 parameters)
//  1. VECTOR< STRUCT Line_> portalMap (REQUIRED)
//  2. STRUCT AnimZone animZones (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_PolygonSt_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, portalMap_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, animZones_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "portalMap", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "animZones", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

//**********************************************************************
// STRUCT "AnimZone" Targets: JSON (2 parameters)
//  1. VECTOR< STRUCT Line_> animVector (REQUIRED)
//  2. VECTOR<REAL> animSpeed (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_AnimZone_compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfMessageType, animVector_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::VectorOfSympleTypes<::globalmq::marshalling::impl::RealType>, animSpeed_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "animVector", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "animSpeed", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}

//**********************************************************************
// STRUCT "Line_" Targets: JSON (2 parameters)
//  1. STRUCT Vertex a (REQUIRED)
//  2. STRUCT Vertex b (REQUIRED)

//**********************************************************************

template<class ComposerT, typename ... Args>
void STRUCT_Line__compose(ComposerT& composer, Args&& ... args)
{
	static_assert( std::is_base_of<ComposerBase, ComposerT>::value, "Composer must be one of GmqComposer<> or JsonComposer<>" );

	using arg_1_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, a_Type::Name>;
	using arg_2_type = NamedParameterWithType<::globalmq::marshalling::impl::MessageType, b_Type::Name>;

	constexpr size_t matchCount = isMatched(arg_1_type::nameAndTypeID, Args::nameAndTypeID...) + 
		isMatched(arg_2_type::nameAndTypeID, Args::nameAndTypeID...);
	constexpr size_t argCount = sizeof ... (Args);
	if constexpr ( argCount != 0 )
		ensureUniqueness(args.nameAndTypeID...);
	static_assert( argCount == matchCount, "unexpected arguments found" );

	static_assert( ComposerT::proto == Proto::JSON, "this STRUCT assumes only JSON protocol" );
	composer.buff.append( "{\n  ", sizeof("{\n  ") - 1 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_1_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "a", arg_1_type::nameAndTypeID, args...);
	composer.buff.append( ",\n  ", 4 );
	::globalmq::marshalling::impl::json::composeParamToJson<ComposerT, arg_2_type, true, int64_t, int64_t, (int64_t)(0)>(composer, "b", arg_2_type::nameAndTypeID, args...);
	composer.buff.append( "\n}", 2 );
}


} // namespace mtest

#endif // _test_marshalling_h_f36bc8bd_guard
