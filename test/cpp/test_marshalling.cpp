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
	fmt::print( "\nrunning runGmqSample2() \n" );
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

	uint64_t dummy = 0;
	for ( size_t i=0; i<b.size(); ++i )
		dummy = dummy + b.begin()[i] + (dummy << 32);
	fmt::print( "Buffer size = {} ({})\n", b.size(), dummy );
	for ( size_t i=0; i<b.size() / 8; ++i )
		fmt::print( "{}:\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t\n", i*8, b.begin()[8*i+0], b.begin()[8*i+1], b.begin()[8*i+2], b.begin()[8*i+3], b.begin()[8*i+4], b.begin()[8*i+5], b.begin()[8*i+6], b.begin()[8*i+7] );

	auto riter = b.getReadIter();
	mtest::GmqParser<mtest::Buffer> parser( riter );
	auto pret = mtest::test_gmq::MESSAGE_message_one_parse( parser );

	assert( pret.firstParam == 1 );
	assert( pret.fifthParam == "def" );
	assert( pret.forthParam == 3 );
	assert( pret.seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == pret.secondParam.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == pret.secondParam[i] );
	assert( vectorOfRealNumbers.size() == pret.tenthParam.size() );
	for ( size_t i=0; i<vectorOfRealNumbers.size(); ++i )
		assert( vectorOfRealNumbers[i] == pret.tenthParam[i] );
    assert( pret.sixthParam.size() == vectorOfPoints.size() );
    for ( size_t i=0; i<vectorOfPoints.size(); ++i )
    {
        assert( vectorOfPoints[i].x == pret.sixthParam[i].x );
        assert( vectorOfPoints[i].y == pret.sixthParam[i].y );
    }
	assert( pret.thirdParam.size() == vectorOfPoints3D.size() );
	for ( size_t i=0; i<vectorOfPoints3D.size(); ++i )
	{
		assert( vectorOfPoints3D[i].x == pret.thirdParam[i].x );
		assert( vectorOfPoints3D[i].y == pret.thirdParam[i].y );
		assert( vectorOfPoints3D[i].z == pret.thirdParam[i].z );
	}
	assert( pt.x == pret.eighthParam.x );
	assert( pt.y == pret.eighthParam.y );
	assert( pt3D.x == pret.ninethParam.x );
	assert( pt3D.y == pret.ninethParam.y );
	assert( pt3D.z == pret.ninethParam.z );
}

void runJsonSample()
{
	fmt::print( "\nrunning runJsonSample2() \n" );
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
	auto pret = mtest::test_json::MESSAGE_message_one_parse( parser );

	assert( pret.firstParam == 1 );
	assert( pret.fifthParam == "def" );
	assert( pret.forthParam == 3 );
	assert( pret.seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == pret.secondParam.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == pret.secondParam[i] );
	assert( vectorOfRealNumbers.size() == pret.tenthParam.size() );
	for ( size_t i=0; i<vectorOfRealNumbers.size(); ++i )
		assert( vectorOfRealNumbers[i] == pret.tenthParam[i] );
    assert( pret.sixthParam.size() == vectorOfPoints.size() );
    for ( size_t i=0; i<vectorOfPoints.size(); ++i )
    {
        assert( vectorOfPoints[i].x == pret.sixthParam[i].x );
        assert( vectorOfPoints[i].y == pret.sixthParam[i].y );
    }
	assert( pret.thirdParam.size() == vectorOfPoints3D.size() );
	for ( size_t i=0; i<vectorOfPoints3D.size(); ++i )
	{
		assert( vectorOfPoints3D[i].x == pret.thirdParam[i].x );
		assert( vectorOfPoints3D[i].y == pret.thirdParam[i].y );
		assert( vectorOfPoints3D[i].z == pret.thirdParam[i].z );
	}
	assert( pt.x == pret.eighthParam.x );
	assert( pt.y == pret.eighthParam.y );
	assert( pt3D.x == pret.ninethParam.x );
	assert( pt3D.y == pret.ninethParam.y );
	assert( pt3D.z == pret.ninethParam.z );
}

void testParsingJsonFile()
{
	fmt::print( "\nrunning testParsingJsonFile2() \n" );
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

	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser( riter );
	auto data = mtest::level_trace::MESSAGE_LevelTraceData_parse( parser );
		
	fmt::print( "Character ID = {}\n", data.CharacterParam.ID );
	fmt::print( "Character size = ({}, {}, {})\n", data.CharacterParam.Size.X, data.CharacterParam.Size.Y, data.CharacterParam.Size.Z );
	fmt::print( "# of points traced: {}\n", data.Points.size() );
	if ( data.Points.size() )
	{
		fmt::print( "Starting point: ({}, {}, {})\n", data.Points[0].X, data.Points[0].Y, data.Points[0].Z );
		fmt::print( "Ending point:   ({}, {}, {})\n", data.Points.back().X, data.Points.back().Y, data.Points.back().Z );
	}
}

void testMessageAliases()
{
	fmt::print( "\nrunning testMessageAliases2() \n" );
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::JsonComposer composer( b );
	mtest::scope_one::MESSAGE_point3D_alias_compose( composer, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z);

	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	auto riter = b.getReadIter();
	mtest::JsonParser<mtest::Buffer> parser( riter );
	auto pret = mtest::scope_one::MESSAGE_point3D_alias_parse( parser );
	assert( pt3D.x == pret.x );
	assert( pt3D.y == pret.y );
	assert( pt3D.z == pret.z );
}

void testScopedMessageComposingAndParsing()
{
	fmt::print( "\nrunning testScopedMessageComposingAndParsing2() \n" );
	Point pt = {444, 555};
	Point3D pt3D = {123, 456, 789};
	mtest::Buffer b;
	mtest::scope_one::composeMessage<mtest::scope_one::point3D_alias>( b, mtest::x = pt3D.x, mtest::y = pt3D.y, mtest::z = pt3D.z );

	mtest::structures::point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point3D_alias>([&](auto& parser){ auto back = mtest::scope_one::MESSAGE_point3D_alias_parse( parser ); pt3DBack.x = back.x; pt3DBack.y = back.y; pt3DBack.z = back.z;}),
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ ptBack = mtest::scope_one::MESSAGE_point_alias_parse( parser );}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);

	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );

	mtest::structures::point pb;
	mtest::scope_one::handleMessage( b, 
		mtest::makeMessageHandler<mtest::scope_one::point_alias>([&](auto& parser){ pb = mtest::scope_one::MESSAGE_point_alias_parse( parser );}),
		mtest::makeDefaultMessageHandler([&](auto& parser, uint64_t msgID){ fmt::print( "Unhandled message {}\n", msgID ); })
	);

}