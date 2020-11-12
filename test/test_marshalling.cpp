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

#if 0
void runGmqSample()
{
	std::vector<int> vectorOfNumbers = { 0, 1, 2, 3, 4, 5 };
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	m::Buffer b;
	m::GmqComposer composer( b );
	m::message_one_compose( composer, 
		m::thirdParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ m::point3D_compose( c, m::x = vectorOfPoints3D[ordinal].x, m::y = vectorOfPoints3D[ordinal].y, m::z = vectorOfPoints3D[ordinal].z );} ), 
		m::firstParam = 1, m::fifthParam = "def", m::forthParam = 3, m::seventhParam = 3.1416, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		m::sixthParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ m::point_compose( c, m::x = vectorOfPoints[ordinal].x, m::y = vectorOfPoints[ordinal].y );} )
	);

	m::GmqParser parser( b );
	int firstParam = -1;
	int forthParam = -1;
	std::string_view fifthParam;
	std::vector<int> vectorOfNumbersBack;
	std::vector<Point> vectorOfPointsBack;
	std::vector<Point3D> vectorOfPoints3DBack;
	double seventhParam = 0;
	m::message_one_parse( parser, 
		m::firstParam = &firstParam, m::forthParam = &forthParam, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		m::thirdParam = m::CollactionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; m::point3D_parse( p, m::x = &(pt.x), m::y = &(pt.y), m::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		m::sixthParam = m::CollactionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; m::point_parse( p,  m::x = &(pt.x), m::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
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
}

void runJsonSample()
{
	std::vector<int> vectorOfNumbers = { 0, 1, 2, 3, 4, 5 };
	std::vector<Point> vectorOfPoints = { {0, 1}, {2, 3}, {4, 5} };
	std::vector<Point3D> vectorOfPoints3D = { {0, 1, 2}, {3, 4, 5} };
	m::Buffer b;
	m::JsonComposer composer( b );
	m::message_one_compose( composer, 
		m::thirdParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints3D.size(); }, [&](auto& c, size_t ordinal){ m::point3D_compose( c, m::x = vectorOfPoints3D[ordinal].x, m::y = vectorOfPoints3D[ordinal].y, m::z = vectorOfPoints3D[ordinal].z );} ), 
		m::firstParam = 1, m::fifthParam = "def", m::forthParam = 3, m::seventhParam = 3.1416, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbers ),
		m::sixthParam = m::CollectionWrapperForComposing( [&]() { return vectorOfPoints.size(); }, [&](auto& c, size_t ordinal){ m::point_compose( c, m::x = vectorOfPoints[ordinal].x, m::y = vectorOfPoints[ordinal].y );} )
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
	double seventhParam = 0;
	m::message_one_parse( parser, 
		m::firstParam = &firstParam, m::forthParam = &forthParam, 
		m::secondParam = m::SimpleTypeCollectionWrapper( vectorOfNumbersBack ), 
		m::thirdParam = m::CollactionWrapperForParsing( nullptr, [&](auto& p, size_t ordinal){ Point3D pt; m::point3D_parse( p, m::x = &(pt.x), m::y = &(pt.y), m::z = &(pt.z) ); vectorOfPoints3DBack.push_back( pt );} ), 
		m::sixthParam = m::CollactionWrapperForParsing( [&](size_t sz){vectorOfPointsBack.reserve( sz );}, [&](auto& p, size_t ordinal){ Point pt; m::point_parse( p,  m::x = &(pt.x), m::y = &(pt.y) ); vectorOfPointsBack.push_back( pt );} ), 
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
}
#endif // 0