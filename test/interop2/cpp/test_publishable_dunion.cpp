/* -------------------------------------------------------------------------------
* Copyright (c) 2022, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

#include "test_common.h"

using namespace std;

std::string Prefix = "data/publishable_dunion/";

std::string JsonPath_s0 = Prefix + "state_sync_0.json";
std::string JsonPath_s1 = Prefix + "state_sync_1.json";
std::string JsonPath_s2 = Prefix + "state_sync_2.json";
std::string JsonPath_u1 = Prefix + "update_1.json";
std::string JsonPath_u2 = Prefix + "update_2.json";

std::string GmqPath_s0 = Prefix + "state_sync_0.gmq";
std::string GmqPath_s1 = Prefix + "state_sync_1.gmq";
std::string GmqPath_s2 = Prefix + "state_sync_2.gmq";
std::string GmqPath_u1 = Prefix + "update_1.gmq";
std::string GmqPath_u2 = Prefix + "update_2.gmq";

 enum Protocol { Json, Gmq };

template<class T, class ComposerT>
class publishable_dunion_for_test :
    public mtest::publishable_dunion_WrapperForPublisher<T, ComposerT>
{
    public:
    publishable_dunion_for_test(T& data) : 
        mtest::publishable_dunion_WrapperForPublisher<T, ComposerT>(data) {}

	virtual void generateStateSyncMessage(ComposerT& composer)
    {
        mtest::publishable_dunion_WrapperForPublisher<T, ComposerT>::compose(composer);
    }
};

template<class T, class BufferT>
class subscriber_dunion_for_test :
    public mtest::publishable_dunion_WrapperForSubscriber<T, BufferT>
{
    public:
    subscriber_dunion_for_test(T& data) : 
        mtest::publishable_dunion_WrapperForSubscriber<T, BufferT>(data) {}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser )
    {
        mtest::publishable_dunion_WrapperForSubscriber<T, BufferT>::parseStateSyncMessage(parser);
    }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
    {
        mtest::publishable_dunion_WrapperForSubscriber<T, BufferT>::parseStateSyncMessage(parser);
    }
};


inline
bool operator==(const mtest::structures::publishable_dunion& l, const mtest::structures::publishable_dunion& r)
{
    if(l.name != r.name)
        return false;
    if(l.anUnion.currentVariant() != r.anUnion.currentVariant())
        return false;
    
    if(l.anUnion.currentVariant() == mtest::structures::du_one::Variants::one)
        return l.anUnion.D1() == r.anUnion.D1() && r.anUnion.D2() == r.anUnion.D2() &&
            l.anUnion.D3() == r.anUnion.D3();
    else if(l.anUnion.currentVariant() == mtest::structures::du_one::Variants::two)
        return l.anUnion.Data() == r.anUnion.Data();
    else
        return false;
}


mtest::structures::publishable_dunion GetPublishableUnion_0()
{
    mtest::structures::publishable_dunion data{};

    data.name = "dunion";

    return data;
}

void doUpdate1(mtest::structures::publishable_dunion& data)
{
    data.anUnion.initAs(mtest::structures::du_one::Variants::one);

    data.anUnion.D1() = 3.14;
    data.anUnion.D2() = -3.14;
    data.anUnion.D3() = -3.14e-20;
}

template<class T>
void doUpdate1(T& publ)
{
    //modify substructure inside vector
    publ.get4set_anUnion().set_currentVariant(mtest::structures::du_one::Variants::one);

    publ.get4set_anUnion().set_D1(3.14);
    publ.get4set_anUnion().set_D2(-3.14);
    publ.get4set_anUnion().set_D3(-3.14e-20);
}

mtest::structures::publishable_dunion GetPublishableUnion_1()
{
    mtest::structures::publishable_dunion data{};
    doUpdate1(data);
    return data;
}

void doUpdate2(mtest::structures::publishable_dunion& data)
{
    data.anUnion.initAs(mtest::structures::du_one::Variants::two);

    data.anUnion.Data().push_back(3.14);
    data.anUnion.Data().push_back(-3.14);
    data.anUnion.Data().push_back(-3.14e-20);
}

template<class T>
void doUpdate2(T& publ)
{
    publ.get4set_anUnion().set_currentVariant(mtest::structures::du_one::Variants::two);

    // double d1 = 3.14;
    // double d2 = -3.14;
    // double d3 = -3.14e-20;
    publ.get4set_anUnion().get4set_Data().insert_before(0, 3.14);
    publ.get4set_anUnion().get4set_Data().insert_before(1, -3.14);
    publ.get4set_anUnion().get4set_Data().insert_before(2, -3.14e-20);
}

mtest::structures::publishable_dunion GetPublishableUnion_2()
{
    mtest::structures::publishable_dunion data{};
    doUpdate2(data);
    return data;
}

const lest::test test_publishable_dunion[] =
{
    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync0" )
    {
        auto data = GetPublishableUnion_0();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(JsonPath_s0, lest_env);
        EXPECT(AreEqualIgnoreEol(expected, b));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseStateSync0" )
    {
        using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

        mtest::structures::publishable_dunion data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(JsonPath_s0, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonStateSyncMessage(parser);

        auto expected = GetPublishableUnion_0();
        EXPECT(expected != subs.getState());
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync1" )
    {
        auto data = GetPublishableUnion_1();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(JsonPath_s1, lest_env);
        EXPECT(AreEqualIgnoreEol(expected, b));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseStateSync1" )
    {
        using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

        mtest::structures::publishable_dunion data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(JsonPath_s1, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonStateSyncMessage(parser);

        auto expected = GetPublishableUnion_1();
        EXPECT(expected == subs.getState());
    },

    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync2" )
    {
        auto data = GetPublishableUnion_2();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(JsonPath_s2, lest_env);
        EXPECT(AreEqualIgnoreEol(expected, b));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseStateSync2" )
    {
        using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

        mtest::structures::publishable_dunion data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(JsonPath_s2, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonStateSyncMessage(parser);

        auto expected = GetPublishableUnion_2();
        EXPECT(expected == subs.getState());
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeUpdate1" )
    {
        auto data = GetPublishableUnion_0();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        doUpdate1(publ);
        mtest::Buffer b = publ.endTick();

        auto expected = makeBuffer(JsonPath_u1, lest_env);
        EXPECT(AreEqualIgnoreEol(expected, b));
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseUpdate1" )
    {
        using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

        auto data = GetPublishableUnion_0();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(JsonPath_u1, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableUnion_0();
        EXPECT_NOT(subs.getState() == data2);

        doUpdate1(data2);
        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeUpdate2" )
    {
        auto data = GetPublishableUnion_1();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        doUpdate2(publ);
        mtest::Buffer b = publ.endTick();

        auto expected = makeBuffer(JsonPath_u2, lest_env);
        EXPECT(AreEqualIgnoreEol(expected, b));
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseUpdate2" )
    {
        using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

        auto data = GetPublishableUnion_1();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(JsonPath_u2, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableUnion_1();
        EXPECT_NOT(subs.getState() == data2);

        doUpdate2(data2);
        EXPECT(subs.getState() == data2);
    },

};

lest_MODULE(specification(), test_publishable_dunion);
