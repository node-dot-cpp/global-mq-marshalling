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

#include "test_idl_common.h"


std::string PathPubSevenJson = "test_publishable_seven.json";
std::string PathPubSevenJson1 = "test_publishable_seven_u1.json";
std::string PathPubSevenJson2 = "test_publishable_seven_u2.json";
std::string PathPubSevenJson3 = "test_publishable_seven_u3.json";
std::string PathPubSevenJson4 = "test_publishable_seven_u4.json";
std::string PathPubSevenJson5 = "test_publishable_seven_u5.json";

std::string PathPubSevenGmq = "test_publishable_seven.gmq";
std::string PathPubSevenGmq1 = "test_publishable_seven_u1.gmq";
std::string PathPubSevenGmq2 = "test_publishable_seven_u2.gmq";
std::string PathPubSevenGmq3 = "test_publishable_seven_u3.gmq";
std::string PathPubSevenGmq4 = "test_publishable_seven_u4.gmq";
std::string PathPubSevenGmq5 = "test_publishable_seven_u5.gmq";


inline
mtest::structures::publishable_seven GetPublishableSeven()
{
    mtest::structures::publishable_seven data;


    data.intVec.push_back(1);
    data.intVec.push_back(2);
    data.intVec.push_back(3);

    data.uintVec.push_back(4);
    data.uintVec.push_back(5);
    data.uintVec.push_back(6);

    data.realVec.push_back(7);
    data.realVec.push_back(8);
    data.realVec.push_back(9);

    data.strVec.push_back("one");
    data.strVec.push_back("two");
    data.strVec.push_back("three");

    data.structVec.push_back(mtest::structures::point3D{101, 102, 103});
    data.structVec.push_back(mtest::structures::point3D{201, 202, 203});

    return data;
}

inline
bool operator==(const mtest::structures::publishable_seven& l, const mtest::structures::publishable_seven& r)
{
    return (l.intVec == r.intVec) &&
            (l.uintVec == r.uintVec) &&
            (l.realVec == r.realVec) &&
            (l.strVec == r.strVec) &&
            (l.structVec == r.structVec);
}

template<class T, class ComposerT>
class publishable_seven_for_test :
    public mtest::publishable_seven_WrapperForPublisher<T, ComposerT>
{
    public:
    publishable_seven_for_test(T& data) : 
        mtest::publishable_seven_WrapperForPublisher<T, ComposerT>(data) {}

	virtual void generateStateSyncMessage(ComposerT& composer)
    {
        mtest::publishable_seven_WrapperForPublisher<T, ComposerT>::compose(composer);
    }
};

template<class T, class BufferT>
class subscriber_seven_for_test :
    public mtest::publishable_seven_WrapperForSubscriber<T, BufferT>
{
    public:
    subscriber_seven_for_test(T& data) : 
        mtest::publishable_seven_WrapperForSubscriber<T, BufferT>(data) {}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser )
    {
        mtest::publishable_seven_WrapperForSubscriber<T, BufferT>::parseStateSyncMessage(parser);
    }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
    {
        mtest::publishable_seven_WrapperForSubscriber<T, BufferT>::parseStateSyncMessage(parser);
    }
};



const lest::test test_publishable_seven[] =
{
    lest_CASE( "test_publishable_seven.TestJsonComposeStateSync" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto b2 = makeBuffer(PathPubSevenJson, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseStateSync" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        mtest::structures::publishable_seven data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonStateSyncMessage(parser);

        auto data2 = GetPublishableSeven();
        EXPECT(subs.getState() == data2);
    },

    lest_CASE( "test_publishable_seven.TestJsonComposeUpdate1" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        //modify substructure inside vector
        publ.get4set_structVec().get4set_at(0).set_y(505);
        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenJson1, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseUpdate1" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson1, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.structVec[0].y = 505;
        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestJsonComposeUpdate2" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //modify existing elements
        publ.get4set_intVec().set_at(303, 0);
        publ.get4set_uintVec().set_at(404, 1);
        publ.get4set_realVec().set_at(505, 2);
        publ.get4set_strVec().set_at("four", 1);

        publ.get4set_structVec().set_at({ 901, 902, 903 }, 0);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenJson2, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseUpdate2" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson2, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.intVec[0] = 303;
        data2.uintVec[1] = 404;
        data2.realVec[2] = 505;
        data2.strVec[1] = "four";
        data2.structVec[0] = { 901, 902, 903 };

        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestJsonComposeUpdate3" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
            //replace complete vector
        decltype(data.strVec) strVec;
        strVec.push_back("four");
        strVec.push_back("five");
        publ.set_strVec(strVec);

        decltype(data.realVec) realVec;
        publ.set_realVec(realVec);

        decltype(data.structVec) structVec;
        structVec.push_back({ 301, 302, 303 });
        publ.set_structVec(structVec);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenJson3, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseUpdate3" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson3, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        decltype(data2.strVec) strVec;
        strVec.push_back("four");
        strVec.push_back("five");
        data2.strVec = strVec;

        decltype(data2.realVec) realVec;
        data2.realVec = realVec;

        decltype(data2.structVec) structVec;
        structVec.push_back({ 301, 302, 303 });
        data2.structVec = structVec;

        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestJsonComposeUpdate4" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //erase elements in vector
        publ.get4set_intVec().remove(0);
        publ.get4set_realVec().remove(1);
        publ.get4set_structVec().remove(1);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenJson4, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseUpdate4" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson4, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.intVec.erase(data2.intVec.begin() + 0);
        data2.realVec.erase(data2.realVec.begin() + 1);
        data2.structVec.erase(data2.structVec.begin() + 1);

        EXPECT(subs.getState() == data2);
    },

    lest_CASE( "test_publishable_seven.TestJsonComposeUpdate5" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //insert elements in vector

        int64_t v1 = 77;
        uint64_t v2 = 88;
        double v3 = 99;
        GMQ_COLL string v4 = "last";

        publ.get4set_intVec().insert_before(0, v1);
        publ.get4set_uintVec().insert_before(1, v2);
        publ.get4set_realVec().insert_before(2, v3);
        publ.get4set_strVec().insert_before(3, v4);

        mtest::structures::point3D v5{301, 302, 303};
        publ.get4set_structVec().insert_before(1, v5);

        mtest::Buffer b = publ.endTick();

        auto b2 = makeBuffer(PathPubSevenJson5, lest_env);
        EXPECT(EqualsIgnoreEol(b, b2));
    },
    lest_CASE( "test_publishable_seven.TestJsonParseUpdate5" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenJson5, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        int64_t v1 = 77;
        uint64_t v2 = 88;
        double v3 = 99;
        GMQ_COLL string v4 = "last";

        data2.intVec.insert(data2.intVec.begin() + 0, v1);
        data2.uintVec.insert(data2.uintVec.begin() + 1, v2);
        data2.realVec.insert(data2.realVec.begin() + 2, v3);
        data2.strVec.insert(data2.strVec.begin() + 3, v4);

        mtest::structures::point3D v5{301, 302, 303};
        data2.structVec.insert(data2.structVec.begin() + 1, v5);

        EXPECT(subs.getState() == data2);
    },

    ////////////////// GMQ //////////////////

    lest_CASE( "test_publishable_seven.TestGmqComposeStateSync" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto b2 = makeBuffer(PathPubSevenGmq, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseStateSync" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        mtest::structures::publishable_seven data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqStateSyncMessage(parser);

        auto data2 = GetPublishableSeven();
        EXPECT(subs.getState() == data2);
    },

    lest_CASE( "test_publishable_seven.TestGmqComposeUpdate1" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        //modify substructure inside vector
        publ.get4set_structVec().get4set_at(0).set_y(505);
        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenGmq1, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseUpdate1" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq1, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.structVec[0].y = 505;
        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestGmqComposeUpdate2" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //modify existing elements
        publ.get4set_intVec().set_at(303, 0);
        publ.get4set_uintVec().set_at(404, 1);
        publ.get4set_realVec().set_at(505, 2);
        publ.get4set_strVec().set_at("four", 1);

        publ.get4set_structVec().set_at({ 901, 902, 903 }, 0);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenGmq2, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseUpdate2" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq2, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.intVec[0] = 303;
        data2.uintVec[1] = 404;
        data2.realVec[2] = 505;
        data2.strVec[1] = "four";
        data2.structVec[0] = { 901, 902, 903 };

        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestGmqComposeUpdate3" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
            //replace complete vector
        decltype(data.strVec) strVec;
        strVec.push_back("four");
        strVec.push_back("five");
        publ.set_strVec(strVec);

        decltype(data.realVec) realVec;
        publ.set_realVec(realVec);

        decltype(data.structVec) structVec;
        structVec.push_back({ 301, 302, 303 });
        publ.set_structVec(structVec);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenGmq3, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseUpdate3" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq3, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        decltype(data2.strVec) strVec;
        strVec.push_back("four");
        strVec.push_back("five");
        data2.strVec = strVec;

        decltype(data2.realVec) realVec;
        data2.realVec = realVec;

        decltype(data2.structVec) structVec;
        structVec.push_back({ 301, 302, 303 });
        data2.structVec = structVec;

        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_seven.TestGmqComposeUpdate4" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //erase elements in vector
        publ.get4set_intVec().remove(0);
        publ.get4set_realVec().remove(1);
        publ.get4set_structVec().remove(1);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubSevenGmq4, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseUpdate4" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq4, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        data2.intVec.erase(data2.intVec.begin() + 0);
        data2.realVec.erase(data2.realVec.begin() + 1);
        data2.structVec.erase(data2.structVec.begin() + 1);

        EXPECT(subs.getState() == data2);
    },

    lest_CASE( "test_publishable_seven.TestGmqComposeUpdate5" )
    {
        auto data = GetPublishableSeven();

        using ComposerT = mtest::GmqComposer<mtest::Buffer>;
        using PublishableT = publishable_seven_for_test<mtest::structures::publishable_seven, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        //insert elements in vector

        int64_t v1 = 77;
        uint64_t v2 = 88;
        double v3 = 99;
        GMQ_COLL string v4 = "last";

        publ.get4set_intVec().insert_before(0, v1);
        publ.get4set_uintVec().insert_before(1, v2);
        publ.get4set_realVec().insert_before(2, v3);
        publ.get4set_strVec().insert_before(3, v4);

        mtest::structures::point3D v5{301, 302, 303};
        publ.get4set_structVec().insert_before(1, v5);

        mtest::Buffer b = publ.endTick();

        auto b2 = makeBuffer(PathPubSevenGmq5, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_seven.TestGmqParseUpdate5" )
    {
        using SubscriberT = subscriber_seven_for_test<mtest::structures::publishable_seven, mtest::Buffer>;

        auto data = GetPublishableSeven();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubSevenGmq5, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser<mtest::Buffer> parser(it);

        subs.applyGmqMessageWithUpdates(parser);

        auto data2 = GetPublishableSeven();
        EXPECT_NOT(subs.getState() == data2);

        int64_t v1 = 77;
        uint64_t v2 = 88;
        double v3 = 99;
        GMQ_COLL string v4 = "last";

        data2.intVec.insert(data2.intVec.begin() + 0, v1);
        data2.uintVec.insert(data2.uintVec.begin() + 1, v2);
        data2.realVec.insert(data2.realVec.begin() + 2, v3);
        data2.strVec.insert(data2.strVec.begin() + 3, v4);

        mtest::structures::point3D v5{301, 302, 303};
        data2.structVec.insert(data2.structVec.begin() + 1, v5);

        EXPECT(subs.getState() == data2);
    },

};

lest_MODULE(specification(), test_publishable_seven);
