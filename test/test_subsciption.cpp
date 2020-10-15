
#include <stdio.h>
#include <subscription_state.h>

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
		b.appendString( "level msg" );
		source.updateNode( {}, std::move( b ) );
	}
};

void testSubscriptionTest()
{
	SubscriptionPairTest test;
	test.test();
}