// named_fn_params.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <marshalling.h>
#include "./marshalling/test_marshalling.h"

struct Point
{
	int x;
	int y;
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
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	Point pt = {175, 186};
	Point3D pt3D = {123, 456, 789};
	m::Buffer b;
	m::GmqComposer composer( b );
	m::MESSAGE_message_one_compose( composer, 
		m::thirdParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ m::STRUCT_point3D_compose( c, m::x = vectorOfPoints3D[ordinal].x, m::y = vectorOfPoints3D[ordinal].y, m::z = vectorOfPoints3D[ordinal].z );} ), 
		m::firstParam = 1, m::fifthParam = "def", m::forthParam = 3, m::seventhParam = 3.1416, 
		m::eighthParam = m::MessageWrapperForComposing( [&](auto& c){ m::STRUCT_point_compose( c, m::x = pt.x, m::y = pt.y );} ),
		m::ninethParam = m::MessageWrapperForComposing( [&](auto& c){ m::STRUCT_point3D_compose( c, m::x = pt3D.x, m::y = pt3D.y, m::z = pt3D.z );} ),
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		m::sixthParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ m::STRUCT_point_compose( c, m::x = vectorOfPoints[ordinal].x, m::y = vectorOfPoints[ordinal].y );} )
	);

	m::GmqParser parser( b );
	int firstParam = -1;
	int forthParam = -1;
	std::string_view fifthParam;
	std::vector<int> vectorOfNumbersBack;
	std::vector<Point> vectorOfPointsBack;
	std::vector<Point3D> vectorOfPoints3DBack;
	Point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	m::MESSAGE_message_one_parse( parser, 
		m::firstParam = &firstParam, m::forthParam = &forthParam, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		m::thirdParam = m::CollactionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; m::STRUCT_point3D_parse( p, m::x = &(pt.x), m::y = &(pt.y), m::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		m::sixthParam = m::CollactionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; m::STRUCT_point_parse( p,  m::x = &(pt.x), m::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
		m::eighthParam = m::MessageWrapperForParsing( [&](auto& parser){ m::STRUCT_point_parse( parser, m::x = &(ptBack.x), m::y = &(ptBack.y) );} ),
		m::ninethParam = m::MessageWrapperForParsing( [&](auto& parser){ m::STRUCT_point3D_parse( parser, m::x = &(pt3DBack.x), m::y = &(pt3DBack.y), m::z = &(pt3DBack.z) );} ),
		m::fifthParam = &fifthParam, m::seventhParam = &seventhParam );

	assert( firstParam == 1 );
	assert( fifthParam == "def" );
	assert( forthParam == 3 );
	assert( seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == vectorOfNumbersBack.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == vectorOfNumbersBack[i] );
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
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	Point pt = {175, 186};
	Point3D pt3D = {123, 456, 789};
	m::Buffer b;
	m::JsonComposer composer( b );
	m::MESSAGE_message_one_compose( composer, 
		m::thirdParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ m::STRUCT_point3D_compose( c, m::x = vectorOfPoints3D[ordinal].x, m::y = vectorOfPoints3D[ordinal].y, m::z = vectorOfPoints3D[ordinal].z );} ), 
		m::firstParam = 1, m::fifthParam = "def", m::forthParam = 3, m::seventhParam = 3.1416, 
		m::eighthParam = m::MessageWrapperForComposing( [&](auto& c){ m::STRUCT_point_compose( c, m::x = pt.x, m::y = pt.y );} ),
		m::ninethParam = m::MessageWrapperForComposing( [&](auto& c){ m::STRUCT_point3D_compose( c, m::x = pt3D.x, m::y = pt3D.y, m::z = pt3D.z );} ),
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		m::sixthParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ m::STRUCT_point_compose( c, m::x = vectorOfPoints[ordinal].x, m::y = vectorOfPoints[ordinal].y );} )
	);

	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	m::JsonParser parser( b );
	int firstParam = -1;
	int forthParam = -1;
	std::string_view fifthParam;
	std::vector<int> vectorOfNumbersBack;
	std::vector<Point> vectorOfPointsBack;
	std::vector<Point3D> vectorOfPoints3DBack;
	Point ptBack = {0, 0};
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	m::MESSAGE_message_one_parse( parser, 
		m::firstParam = &firstParam, m::forthParam = &forthParam, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		m::thirdParam = m::CollactionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; m::STRUCT_point3D_parse( p, m::x = &(pt.x), m::y = &(pt.y), m::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		m::sixthParam = m::CollactionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; m::STRUCT_point_parse( p,  m::x = &(pt.x), m::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
		m::eighthParam = m::MessageWrapperForParsing( [&](auto& parser){ m::STRUCT_point_parse( parser, m::x = &(ptBack.x), m::y = &(ptBack.y) );} ),
		m::ninethParam = m::MessageWrapperForParsing( [&](auto& parser){ m::STRUCT_point3D_parse( parser, m::x = &(pt3DBack.x), m::y = &(pt3DBack.y), m::z = &(pt3DBack.z) );} ),
		m::fifthParam = &fifthParam, m::seventhParam = &seventhParam );

	assert( firstParam == 1 );
	assert( fifthParam == "def" );
	assert( forthParam == 3 );
	assert( seventhParam == 3.1416 );
	assert( vectorOfNumbers.size() == vectorOfNumbersBack.size() );
	for ( size_t i=0; i<vectorOfNumbers.size(); ++i )
		assert( vectorOfNumbers[i] == vectorOfNumbersBack[i] );
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

	m::Buffer b( fsz + 1 );

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
	m::JsonParser parser( b );
	m::MESSAGE_LevelTraceData_parse( 
		parser, 
		m::CharacterParam = m::MessageWrapperForParsing( [&](auto& parser)
			{ 
				m::STRUCT_CharacterParam_parse( 
					parser, 
					m::ID = &(data.charParams.id), 
					m::Size = m::MessageWrapperForParsing( [&](auto& parser){ m::STRUCT_Size_parse( parser, m::X = &(data.charParams.size.x), m::Y = &(data.charParams.size.y), m::Z = &(data.charParams.size.z) );} ) 
				);
			} 
		), 
		m::Points = m::CollactionWrapperForParsing( [&](size_t sz){data.points.reserve( sz );}, [&](auto& p, size_t ordinal){ Point3DReal pt; m::STRUCT_POINT3DREAL_parse( p,  m::X = &(pt.x), m::Y = &(pt.y), m::Z = &(pt.z) ); data.points.push_back( pt );} )
	);
		
	fmt::print( "Character ID = {}\n", data.charParams.id );
	fmt::print( "Character size = ({}, {}, {})\n", data.charParams.size.x, data.charParams.size.y, data.charParams.size.z );
	fmt::print( "# of points traced: {}\n", data.points.size() );
	if ( data.points.size() )
	{
		fmt::print( "Starting point: ({}, {}, {})\n", data.points[0].x, data.points[0].y, data.points[0].z );
		fmt::print( "Ending point:   ({}, {}, {})\n", data.points.front().x, data.points.front().y, data.points.front().z );
	}
}

void testMessageAliases()
{
	Point3D pt3D = {123, 456, 789};
	m::Buffer b;
	m::JsonComposer composer( b );
	m::MESSAGE_point32_alias_compose( composer, m::x = pt3D.x, m::y = pt3D.y, m::z = pt3D.z);

	std::string_view sview( reinterpret_cast<const char*>(b.begin()), b.size() );
	fmt::print( "{}\n", sview );

	m::JsonParser parser( b );
	Point3D pt3DBack = {0, 0, 0};
	double seventhParam = 0;
	m::MESSAGE_point32_alias_parse( parser, m::x = &(pt3DBack.x), m::y = &(pt3DBack.y), m::z = &(pt3DBack.z) );
	assert( pt3D.x == pt3DBack.x );
	assert( pt3D.y == pt3DBack.y );
	assert( pt3D.z == pt3DBack.z );
}
