
#include <stdio.h>
#include <subscription_state.h>

/*template<class T>
class C
{
public:
	T t;

	bool hasX()
	{
		if constexpr requires
	}
};*/

#include <concepts>
/*template<class T>
std::string optionalToString(T* obj)
{
    constexpr bool has_toString = requires(const T& t) {
        t.toString();
    };

    if constexpr (has_toString)
        return obj->toString();
    else
        return "toString not defined";
}

class X
{
public:
	GMQ_COLL string toString() { return "X: I have it!"; }
};

class Y
{
public:
	GMQ_COLL string toString_() { return "Y: I have it!"; }
};*/

struct A
{
	int x;
};

struct B
{
	float y;
};

struct S
{
	std::string s;
	A a;
	B b;
};

////////////////////////////////////////////////////

template<typename T> concept has_x_member = requires { { T::x }; };
template<class TA>
class A_Ref_Wrapper
{
	TA& a;
	static constexpr bool has_x = has_x_member<TA>;
	static_assert( has_x, "type TA must have member TA::x of a type corresponding to IDL type INTEGER" );
	static_assert( std::is_integral<decltype(TA::x)>::value, "member TA::x must be INTEGER type" );
public:
	A_Ref_Wrapper( TA& actual ) : a( actual ) {}
	auto& get_x() { return a.x; }
};
	
template<typename T> concept has_y_member = requires { { T::y }; };
template<class TB>
class B_Ref_Wrapper
{
	TB& b;
	static constexpr bool has_y = has_y_member<TB>;
	static_assert( has_y, "type TA must have member TA::x of atype corresponding to IDL type REAL" );
	static_assert( std::is_floating_point<decltype(TB::y)>::value, "member TB::y must be REAL type" );
public:
	B_Ref_Wrapper( TB& actual ) : b( actual ) {}
	auto& get_y() { return b.y; }
};

template<typename T> concept has_A_value_member = requires { { T::a }; };
template<typename T> concept has_B_value_member = requires { { T::b }; };
template<typename T> concept has_string_value_member = requires { { T::s }; };

template<class T>
class S_Wrapper
{
	T t;
	static constexpr bool has_a = has_A_value_member<T>;
	static_assert( has_a, "type T must have member T::a of a type corresponding to IDL type PUBLISHABLE A" );
	static constexpr bool has_b = has_B_value_member<T>;
	static_assert( has_b, "type T must have member T::b of a type corresponding to IDL type PUBLISHABLE B" );
	static constexpr bool has_s = has_string_value_member<T>;
	static_assert( has_s, "member T::a of type CHARACTER-STRING is expected" );
	static_assert( std::is_convertible<decltype(T::s), GMQ_COLL string>::value, "member TB::s must be of type convertible to GMQ_COLL string" );

public:
	S_Wrapper() {}
	auto& get_s() { return t.s; }
	auto get_a() { return A_Ref_Wrapper<decltype(T::a)>(t.a); }
	auto get_b() { return B_Ref_Wrapper<decltype(T::b)>(t.b); }
};

void test()
{
	S_Wrapper<S> s;
	printf( "%d\n", s.get_a().get_x() );
	printf( "%f\n", s.get_b().get_y() );
}

template<typename T, typename U>
concept can_add = requires(T t, U u) { t + u; };

template<typename T>
concept has_bool_value_member = requires { { T::value } -> std::convertible_to<bool>; };

template<typename T>
concept has_count_call_member = requires { { T::count() } -> std::convertible_to<int>; };

template<typename T>
concept has_count_call_member2 = requires { { T::count() }; };

template<typename T>
concept has_count_call_member3 = requires(T t) { { t.count() }; };

template <typename T>
int foo(T t)
{
    if constexpr (requires (T t){ { t.count() } -> std::convertible_to<int>; })
        return t.count();
    else
        return 0;
}

/*template <typename T>
int foo(T t)
{
    if constexpr (requires (T t){ { t.count() } -> std::convertible_to<int>; })
        return t.count();
    else
        return 0;
}*/

class SubscriptionPairTest
{
	GmqSubscriptionState source;
	GmqSubscriptionState replica;

public:
	void test()
	{
		m::Buffer b;
		GmqSubscriptionState::PositionVectorT v;

		b.appendString( "root msg" );
		source.updateNode( {}, std::move( b ) );

		b.clear();
		b.appendString( "level 1 msg 1" );
		source.insertNode( {}, 0, std::move( b ) );

		b.clear();
		b.appendString( "level 1 msg 2" );
		source.insertNode( {}, 0, std::move( b ) );

		b.clear();
		b.appendString( "level 1 msg 3" );
		source.insertNode( {}, 2, std::move( b ) );

		b.clear();
		b.appendString( "level 2 msg 1" );
		source.insertNode( {1}, 0, std::move( b ) );

		source.dbgPrintTree();

		source.removeNode( {0} );

		source.dbgPrintTree();
	}
};

void testSubscriptionTest()
{
	/*SubscriptionPairTest test;
	test.test();*/
/*	X x;
	Y y;
	printf( "%s\n", optionalToString( &x ).c_str() );
	printf( "%s\n", optionalToString( &y ).c_str() );
    printf( "%d\n", foo(1) );*/

	test();

    struct bar
    {
		bool x;
        int count() const { return 6; }
    };
    struct bar1
    {
		bool value;
        int count_() const { return 6; }
//        static int count() { return 6; }
        static void count() { return; }
    };
	constexpr bool has_bool = has_bool_value_member<bar>;
	constexpr bool has_bool1 = has_bool_value_member<bar1>;
	printf( "%s, %s\n", has_bool ? "Y" : "N", has_bool1 ? "Y" : "N" );

	constexpr bool has_count = has_count_call_member<bar>;
	constexpr bool has_count1 = has_count_call_member<bar1>;
	printf( "%s, %s\n", has_count ? "Y" : "N", has_count1 ? "Y" : "N" );

	constexpr bool has_2count = has_count_call_member3<bar>;
	constexpr bool has_2count1 = has_count_call_member3<bar1>;
	printf( "%s, %s\n", has_2count ? "Y" : "N", has_2count1 ? "Y" : "N" );

	constexpr bool has_3count = has_count_call_member3<bar>;
	constexpr bool has_3count1 = has_count_call_member3<bar1>;
	printf( "%s, %s\n", has_3count ? "Y" : "N", has_3count1 ? "Y" : "N" );


//    printf( "foo(bar{}): : %d\n", foo(bar{}) );
}