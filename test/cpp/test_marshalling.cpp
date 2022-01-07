// named_fn_params.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "nodecpp_publish_subscribe_support.h"
#include <marshalling.h>
#include "./marshalling/test_marshalling.h"

struct Point
{
	int x;
	int y;
};

struct Point64
{
	int64_t x;
	int64_t y;
};

struct Point3D
{
	int x;
	int y;
	int z;
};

void runGmqSample()
{
	std::vector<int> vectorOfNumbers = { 0, 1, 2, 3, 4, 5 };
	std::vector<double> vectorOfRealNumbers = { 0.1, 1.2, 2.3 };
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	Point pt = {175, 186};
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::GmqComposer composer( b );
	mtest::test_gmq::MESSAGE_message_one_compose( composer, 
		mtest::thirdParam = mtest::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ mtest::STRUCT_point3D_compose( c, mtest::x = vectorOfPoints3D[ordinal].x, mtest::y = vectorOfPoints3D[ordinal].y, mtest::z = vectorOfPoints3D[ordinal].z );} ), 
		mtest::firstParam = 1, mtest::fifthParam = "def", mtest::forthParam = 3, mtest::seventhParam = 3.1416, 
		mtest::eighthParam = mtest::MessageWrapperForComposing( [&](auto& c){ mtest::STRUCT_point_compose( c, mtest::x = pt.x, mtest::y = pt.y );} ),
		mtest::ninethParam = mtest::MessageWrapperForComposing( [&](auto& c){ mtest::STRUCT_point3D_compose( c, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z );} ),
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper( vectorOfRealNumbers ),
		mtest::sixthParam = mtest::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ mtest::STRUCT_point_compose( c, mtest::x = vectorOfPoints[ordinal].x, mtest::y = vectorOfPoints[ordinal].y );} )
	);

	auto riter = b.getReadIter();
	mtest::GmqParser<mtest::Buffer> parser( riter );
	int firstParam = -1;
	int forthParam = -1;
	std::string fifthParam;
	std::vector<int> vectorOfNumbersBack;
	std::vector<double> vectorOfRealNumbersBack;
	std::vector<Point> vectorOfPointsBack;
	std::vector<Point3D> vectorOfPoints3DBack;
	Point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	mtest::test_gmq::MESSAGE_message_one_parse( parser, 
		mtest::firstParam = &firstParam, mtest::forthParam = &forthParam, 
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper( vectorOfRealNumbersBack ), 
		mtest::thirdParam = mtest::CollectionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; mtest::STRUCT_point3D_parse( p, mtest::x = &(pt.x), mtest::y = &(pt.y), mtest::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		mtest::sixthParam = mtest::CollectionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; mtest::STRUCT_point_parse( p,  mtest::x = &(pt.x), mtest::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
		mtest::eighthParam = mtest::MessageWrapperForParsing( [&](auto& parser){ mtest::STRUCT_point_parse( parser, mtest::x = &(ptBack.x), mtest::y = &(ptBack.y) );} ),
		mtest::ninethParam = mtest::MessageWrapperForParsing( [&](auto& parser){ mtest::STRUCT_point3D_parse( parser, mtest::x = &(pt3DBack.x), mtest::y = &(pt3DBack.y), mtest::z = &(pt3DBack.z) );} ),
		mtest::fifthParam = &fifthParam, mtest::seventhParam = &seventhParam );

	assert( firstParam == 1 );
	assert( fifthParam == "def" );
	assert( forthParam == 3 );
	assert( seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == vectorOfNumbersBack.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == vectorOfNumbersBack[i] );
	assert( vectorOfRealNumbers.size() == vectorOfRealNumbersBack.size() );
	for ( size_t i=0; i<vectorOfRealNumbers.size(); ++i )
		assert( vectorOfRealNumbers[i] == vectorOfRealNumbersBack[i] );
    assert( vectorOfPointsBack.size() == vectorOfPoints.size() );
    for ( size_t i=0; i<vectorOfPoints.size(); ++i )
    {
        assert( vectorOfPoints[i].x == vectorOfPointsBack[i].x );
        assert( vectorOfPoints[i].y == vectorOfPointsBack[i].y );
    }
	assert( vectorOfPoints3DBack.size() == vectorOfPoints3D.size() );
	for ( size_t i=0; i<vectorOfPoints3D.size(); ++i )
	{
		assert( vectorOfPoints3D[i].x == vectorOfPoints3DBack[i].x );
		assert( vectorOfPoints3D[i].y == vectorOfPoints3DBack[i].y );
		assert( vectorOfPoints3D[i].z == vectorOfPoints3DBack[i].z );
	}
	assert( pt.x == ptBack.x );
	assert( pt.y == ptBack.y );
	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );
}

void runJsonSample()
{
	std::vector<int> vectorOfNumbers = { 0, 1, 2, 3, 4, 5 };
	std::vector<double> vectorOfRealNumbers = { 0.1, 1.2, 2.3 };
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	Point pt = {175, 186};
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::JsonComposer composer( b );
	mtest::test_json::MESSAGE_message_one_compose( composer, 
		mtest::thirdParam = mtest::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ mtest::STRUCT_point3D_compose( c, mtest::x = vectorOfPoints3D[ordinal].x, mtest::y = vectorOfPoints3D[ordinal].y, mtest::z = vectorOfPoints3D[ordinal].z );} ), 
		mtest::firstParam = 1, mtest::fifthParam = "def", mtest::forthParam = 3, mtest::seventhParam = 3.1416, 
		mtest::eighthParam = mtest::MessageWrapperForComposing( [&](auto& c){ mtest::STRUCT_point_compose( c, mtest::x = pt.x, mtest::y = pt.y );} ),
		mtest::ninethParam = mtest::MessageWrapperForComposing( [&](auto& c){ mtest::STRUCT_point3D_compose( c, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z );} ),
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper( vectorOfRealNumbers ),
		mtest::sixthParam = mtest::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ mtest::STRUCT_point_compose( c, mtest::x = vectorOfPoints[ordinal].x, mtest::y = vectorOfPoints[ordinal].y );} )
	);

	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser( riter );
	int firstParam = -1;
	int forthParam = -1;
	std::string fifthParam;
	std::vector<int> vectorOfNumbersBack;
	std::vector<double> vectorOfRealNumbersBack;
	std::vector<Point> vectorOfPointsBack;
	std::vector<Point3D> vectorOfPoints3DBack;
	Point64 ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	mtest::test_json::MESSAGE_message_one_parse( parser, 
		mtest::firstParam = &firstParam, mtest::forthParam = &forthParam, 
		mtest::secondParam = mtest::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		mtest::tenthParam = mtest::SimpleTypeCollectionWrapper( vectorOfRealNumbersBack ),
		mtest::thirdParam = mtest::CollectionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; mtest::STRUCT_point3D_parse( p, mtest::x = &(pt.x), mtest::y = &(pt.y), mtest::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		mtest::sixthParam = mtest::CollectionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; mtest::STRUCT_point_parse( p,  mtest::x = &(pt.x), mtest::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
		mtest::eighthParam = mtest::MessageWrapperForParsing( [&](auto& parser){ mtest::STRUCT_point_parse( parser, mtest::x = &(ptBack.x), mtest::y = &(ptBack.y) );} ),
		mtest::ninethParam = mtest::MessageWrapperForParsing( [&](auto& parser){ mtest::STRUCT_point3D_parse( parser, mtest::x = &(pt3DBack.x), mtest::y = &(pt3DBack.y), mtest::z = &(pt3DBack.z) );} ),
		mtest::fifthParam = &fifthParam, mtest::seventhParam = &seventhParam );

	assert( firstParam == 1 );
	assert( fifthParam == "def" );
	assert( forthParam == 3 );
	assert( seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == vectorOfNumbersBack.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == vectorOfNumbersBack[i] );
	assert( vectorOfRealNumbers.size() == vectorOfRealNumbersBack.size() );
	for ( size_t i=0; i<vectorOfRealNumbers.size(); ++i )
		assert( vectorOfRealNumbers[i] == vectorOfRealNumbersBack[i] );
    assert( vectorOfPointsBack.size() == vectorOfPoints.size() );
    for ( size_t i=0; i<vectorOfPoints.size(); ++i )
    {
        assert( vectorOfPoints[i].x == vectorOfPointsBack[i].x );
        assert( vectorOfPoints[i].y == vectorOfPointsBack[i].y );
    }
	assert( vectorOfPoints3DBack.size() == vectorOfPoints3D.size() );
	for ( size_t i=0; i<vectorOfPoints3D.size(); ++i )
	{
		assert( vectorOfPoints3D[i].x == vectorOfPoints3DBack[i].x );
		assert( vectorOfPoints3D[i].y == vectorOfPoints3DBack[i].y );
		assert( vectorOfPoints3D[i].z == vectorOfPoints3DBack[i].z );
	}
	assert( pt.x == ptBack.x );
	assert( pt.y == ptBack.y );
	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );
}

struct Point3DReal
{
	float x;
	float y;
	float z;
};

struct Size3D
{
	float x;
	float y;
	float z;
};

struct CharacterParams
{
	size_t id;
	Size3D size;
};

struct LevelTraceData
{
	CharacterParams charParams;
	std::vector<Point3DReal> points;
};

void testParsingJsonFile()
{
	// NOTE: nodecpp provides proper means for file reading
	//       here we just do all things manually
	const char* path = "../sample_json.txt";
	FILE* f = fopen( path, "rb" );
	if ( f == nullptr )
	{
		fmt::print( "failed to open test file at \"{}\". Exiting\n", path );
		return;
	}
	int ret = fseek( f, 0, SEEK_END );
	if ( ret )
	{
		fmt::print( "failed to access test file at \"{}\". Exiting\n", path );
		fclose( f );
		return;
	}
	long int fsz = ftell( f );
	if ( fsz < 0 )
	{
		fmt::print( "failed to access test file at \"{}\". Exiting\n", path );
		fclose( f );
		return;
	}
	fseek( f, 0, SEEK_SET );

	mtest::FileReadBuffer b( fsz + 1 );

	size_t rres = fread( b.begin(), 1, fsz, f );
	fclose( f );
	if ( rres != fsz )
	{
		fmt::print( "failed to read from test file at \"{}\". Exiting\n", path );
		return;
	}
	b.set_size( rres );
	b.appendUint8( 0 );

	LevelTraceData data;
	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser( riter );
	mtest::level_trace::MESSAGE_LevelTraceData_parse( 
		parser, 
		mtest::CharacterParam = mtest::MessageWrapperForParsing( [&](auto& parser)
			{ 
				mtest::STRUCT_CharacterParamStruct_parse( 
					parser, 
					mtest::ID = &(data.charParams.id), 
					mtest::Size = mtest::MessageWrapperForParsing( [&](auto& parser){ mtest::STRUCT_SIZE_parse( parser, mtest::X = &(data.charParams.size.x), mtest::Y = &(data.charParams.size.y), mtest::Z = &(data.charParams.size.z) );} ) 
				);
			} 
		), 
		mtest::Points = mtest::CollectionWrapperForParsing( [&](size_t sz){data.points.reserve( sz );}, [&](auto& p, size_t ordinal){ Point3DReal pt; mtest::STRUCT_POINT3DREAL_parse( p,  mtest::X = &(pt.x), mtest::Y = &(pt.y), mtest::Z = &(pt.z) ); data.points.push_back( pt );} )
	);
		
	fmt::print( "Character ID = {}\n", data.charParams.id );
	fmt::print( "Character size = ({}, {}, {})\n", data.charParams.size.x, data.charParams.size.y, data.charParams.size.z );
	fmt::print( "# of points traced: {}\n", data.points.size() );
	if ( data.points.size() )
	{
		fmt::print( "Starting point: ({}, {}, {})\n", data.points[0].x, data.points[0].y, data.points[0].z );
		fmt::print( "Ending point:   ({}, {}, {})\n", data.points.back().x, data.points.back().y, data.points.back().z );
	}
}

void testMessageAliases()
{
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::JsonComposer composer( b );
	mtest::scope_one::MESSAGE_point3D_alias_compose( composer, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z);

	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser( riter );
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	mtest::scope_one::MESSAGE_point3D_alias_parse( parser, mtest::x = &(pt3DBack.x), mtest::y = &(pt3DBack.y), mtest::z = &(pt3DBack.z) );
	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );
}

void testScopedMessageComposingAndParsing()
{
	/*Point pt = {444, 555};
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::scope_one::composeMessage<mtest::scope_one::point3D_alias>( b, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z );

	Point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point3D_alias>([&](auto& parser){ mtest::STRUCT_point3D_parse( parser, mtest::x = &(pt3DBack.x), mtest::y = &(pt3DBack.y), mtest::z = &(pt3DBack.z) );}),
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ mtest::STRUCT_point_parse( parser, mtest::x = &(ptBack.x), mtest::y = &(ptBack.y) );}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);

	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );

	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ mtest::STRUCT_point_parse( parser, mtest::x = &(ptBack.x), mtest::y = &(ptBack.y) );}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);*/

}
void testScopedMessageComposingAndParsing2()
{
	Point pt = {444, 555};
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::scope_one::composeMessage<mtest::scope_one::point3D_alias>( b, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z );

	mtest::structures::point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point3D_alias>([&](auto& parser){ auto back = mtest::infrastructural::MESSAGE_point3D_parse2( parser ); pt3DBack.x = back.pt.x; pt3DBack.y = back.pt.y; pt3DBack.z = back.pt.z;}),
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ ptBack = mtest::infrastructural::MESSAGE_point_parse2( parser ).point;}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);

	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );

	mtest::structures::point pb;
	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ pb = mtest::infrastructural::MESSAGE_point_parse2( parser ).point;}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);

}