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
	template<typename This, typename ComparserT>
	static void rw(This& obj, ComparserT& comparser )
	{
		comparser.template beginStruct("A");
		GMQ_SERIALIZE(INT, "ii", obj.ii);
		GMQ_SERIALIZE(UINT, "nn", obj.nn);
		GMQ_SERIALIZE(REAL, "ff", obj.ff);
		GMQ_SERIALIZE(STRING, "ss", obj.ss);
//		comparser.rw<ComparserT::VofUINT>( "vnn", vnn );
		GMQ_SERIALIZE(VofUINT, "vnn", obj.vnn);
		GMQ_SERIALIZE(BOOLEAN, "bb", obj.bb);
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
	std::vector<std::string> vs;
	std::vector<bool> vb;
	std::unique_ptr<int[]> ints;

	template<typename Object, typename ComparserT>
	static void rw(Object& obj, ComparserT& comparser )
	{
		comparser.template beginStruct("B");
		GMQ_SERIALIZE(ENUM, "e", obj.e);
		GMQ_SERIALIZE(INT, "i", obj.i);
		GMQ_SERIALIZE(UINT, "n", obj.n);
		GMQ_SERIALIZE(REAL, "f", obj.f);
		GMQ_SERIALIZE(STRUCT, "a", obj.a);
		GMQ_SERIALIZE(STRING, "s", obj.s);
		GMQ_SERIALIZE(VofSTRUCT, "va", obj.va);
		GMQ_SERIALIZE(VofSTRING, "vs", obj.vs);
		GMQ_SERIALIZE(VofBOOLEAN, "vb", obj.vb);

		comparser.beginNamedValue("ints");
		comparser.beginArray();
		for (size_t i = 0; i != 100; ++i)
			GMQ_SERIALIZE_VALUE(INT, obj.ints[i]);
		comparser.endArray();
		comparser.endNamedValue();

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
		for ( size_t i=0; i!=0; ++i)
			assert(ints[i] == other.ints[i]);
		for ( size_t i=0; i!=vs.size(); ++i)
			assert(vs[i] == other.vs[i]);
		for ( size_t i=0; i!=vb.size(); ++i)
			assert(vb[i] == other.vb[i]);
	}
};

// Check that rw can be called on const qualified variable
template<typename Composer>
inline void test_const(const B& b, Composer& composer) {
	B::rw(b, composer);
}

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
	b.vs.emplace_back("string 0");
	b.vs.emplace_back("string 1");
	b.vb.emplace_back(false);
	b.vb.emplace_back(true);
	b.ints = std::unique_ptr<int[]>( new int[100] );
	for (int i = 0; i != 100; ++i) {
		b.ints[i] = i;
	}

	globalmq::marshalling::Buffer buff;
	JsonComposer2 composer( buff );

	test_const(b, composer);

	std::string_view sview( reinterpret_cast<const char*>(buff.begin()), buff.size() );
	fmt::print( "{}\n", sview );

	auto riter = buff.getReadIter();
	JsonParser2<globalmq::marshalling::Buffer> parser( riter );
	B b1;
	b1.ints = std::unique_ptr<int[]>( new int[100] );
	B::rw(b1, parser);

	b.assertIsSameAs( b1 );
}

#endif // COMPARSERS_TEST_H
