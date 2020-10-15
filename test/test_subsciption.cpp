
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
		b.appendString( "level 1 msg 1" );
		source.insertNode( {}, 0, std::move( b ) );

		b.clear();
		b.appendString( "level 1 msg 2" );
		source.insertNode( {}, 0, std::move( b ) );

		b.clear();
		b.appendString( "level 2 msg 1" );
		source.insertNode( {1}, 0, std::move( b ) );

		source.dbgPrintTree();
	}
};

void testSubscriptionTest()
{
	SubscriptionPairTest test;
	test.test();
}