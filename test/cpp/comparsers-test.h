#ifndef COMPARSERS_TEST_H
#define COMPARSERS_TEST_H

#include "global_mq_common.h"
#include "comparsers.h"

struct A
{
	int ii;
	uint32_t nn;
	float ff;
	std::string ss;
	std::vector<uint64_t> vnn;
	bool bb;
	template<class ComparserT>
	void rw( ComparserT& comparser )
	{
		comparser.beginStruct();
		comparser.rw<ComparserT::INT>( "ii", ii );
		comparser.rw<ComparserT::UINT>( "nn", nn );
		comparser.rw<ComparserT::REAL>( "ff", ff );
		comparser.rw<ComparserT::STRING>( "ss", ss );
//		comparser.rw<ComparserT::UINT>( "vnn", vnn );
		comparser.rw<ComparserT::UINT>( "vnn", vnn, [&comparser](uint64_t& val){comparser.rw<ComparserT::UINT>(val);} );
		comparser.rw<ComparserT::BOOLEAN>( "bb", bb );
		comparser.endStruct();
	}
	void assertIsSameAs( const A& other ) const
	{
		assert( ii == other.ii );
		assert( nn == other.nn );
		assert( ff == other.ff );
		assert( ss == other.ss );
		assert( bb == other.bb );
		assert( vnn.size() == other.vnn.size() );
		for ( size_t i=0; i<vnn.size(); ++i )
			assert( vnn[i] == other.vnn[i] );
	}
};

struct B
{
	enum E {v1, v2, v3};
	E e;
	int i;
	uint32_t n;
	std::string s;
	A a;
	float f;
	std::vector<A> va;
	std::vector<std::vector<int>> vvi;

	template<class ComparserT>
	void rw( ComparserT& comparser )
	{
		comparser.beginStruct();
		comparser.rw<ComparserT::ENUM>( "e", e );
		comparser.rw<ComparserT::INT>( "i", i );
		comparser.rw<ComparserT::UINT>( "n", n );
		comparser.rw<ComparserT::REAL>( "f", f );
		comparser.rw<ComparserT::STRUCT>( "a", a );
		comparser.rw<ComparserT::STRING>( "s", s );
		comparser.rw<ComparserT::STRUCT>( "va", va );
		comparser.rw<ComparserT::ARRAY>( "vvi", vvi, [&comparser]( std::vector<int>& val ){ comparser.rw<ComparserT::INT>( val, [&comparser](int& val){comparser.rw<ComparserT::INT>(val);} ); } );
		comparser.endStruct();
	}
	void assertIsSameAs( const B& other ) const
	{
		assert( e == other.e );
		assert( i == other.i );
		assert( n == other.n );
		assert( f == other.f );
		assert( s == other.s );
		a.assertIsSameAs( other.a );
		assert( va.size() == other.va.size() );
		for ( size_t i=0; i<va.size(); ++i )
			va[i].assertIsSameAs( other.va[i] );
		assert( vvi.size() == other.vvi.size() );
		for ( size_t i=0; i<vvi.size(); ++i )
		{
			assert( vvi[i].size() == other.vvi[i].size() );
			for ( size_t j=0; j<vvi[i].size(); ++j )
				assert( vvi[i][j] == other.vvi[i][j] );
		}
	}
};

inline void comparsers_test()
{
	B b;
	b.e = B::E::v2;
	b.i = -17;
	b.n = 17;
	b.s = "high!";
	b.a.ii = -71;
	b.a.nn = 71;
	b.a.ff = 3.1416;
	b.a.ss = "high again!";
	b.a.vnn = {1,2,3,4,5};
	b.a.bb = false;
	b.f = 2.71;
	b.va.push_back({-171,171,13.1416,"high again!",{5,4,3},true});
	b.va.push_back(b.a);
	b.vvi = {{55,56,57},{66,67,68}};

	globalmq::marshalling::Buffer buff;
	JsonComposer2 composer( buff );
	b.rw( composer );

	std::string_view sview( reinterpret_cast<const char*>(buff.begin()), buff.size() );
	fmt::print( "{}\n", sview );

	auto riter = buff.getReadIter();
	JsonParser2<globalmq::marshalling::Buffer> parser( riter );
	B b1;
	b1.rw( parser );

	b.assertIsSameAs( b1 );
}

#endif // COMPARSERS_TEST_H
