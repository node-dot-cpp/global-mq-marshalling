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

std::string PathPubGmq = DataPrefix + "test_publishable_six2.gmq";
std::string PathPubGmq1 = DataPrefix + "test_publishable_six2_update1.gmq";
std::string PathPubGmq2 = DataPrefix + "test_publishable_six2_update2.gmq";
std::string PathPubGmq3 = DataPrefix + "test_publishable_six2_update3.gmq";

const lest::test test_publishable_six2[] =
{
    lest_CASE( "test_publishable_six.TestGmqComposeStateSync" )
    {
        auto data = GetPublishableSix();

        using ComposerT = mtest::GmqComposer;
        using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;

        PublishableT publ(data);

        ComposerT composer;

        publ.generateStateSyncMessage(composer);

        mtest::Buffer b = composer.getBuffer();
        auto b2 = makeBuffer(PathPubGmq, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_six.TestGmqParseStateSync" )
    {
        using SubscriberT = subscriber_six_for_test<mtest::structures::StructSix, mtest::Buffer>;

        mtest::structures::StructSix data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubGmq, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser parser(it);

        subs.applyStateSyncMessage(parser);

        auto data2 = GetPublishableSix();
        EXPECT(subs.getState() == data2);
    },

    lest_CASE( "test_publishable_six.TestGmqComposeUpdate1" )
    {
        auto data = GetPublishableSix();

        using ComposerT = mtest::GmqComposer;
        using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        publ.get4set_aggregate().get4set_theAggregate().set_anInt(-101);
        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubGmq1, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_six.TestGmqParseUpdate1" )
    {
        using SubscriberT = subscriber_six_for_test<mtest::structures::StructSix, mtest::Buffer>;

        auto data = GetPublishableSix();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubGmq1, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser parser(it);

        subs.applyMessageWithUpdates(parser);

        auto data2 = GetPublishableSix();
        EXPECT_NOT(subs.getState() == data2);

        data2.aggregate.theAggregate.anInt = -101;
        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_six.TestGmqComposeUpdate2" )
    {
        auto data = GetPublishableSix();

        using ComposerT = mtest::GmqComposer;
        using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());

        publ.get4set_aggregate().set_name("changed name");

        publ.get4set_basic().set_anInt(-2);
        publ.get4set_basic().set_anUInt(3);
        publ.get4set_basic().set_aReal(4.0);

        mtest::structures::BasicTypes  aggr;
        aggr.anInt = -200;
        aggr.anUInt = 300;
        aggr.aReal = 400.0;
        aggr.aString = "new part";
        publ.get4set_aggregate().set_theAggregate(aggr);

        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubGmq2, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_six.TestGmqParseUpdate2" )
    {
        using SubscriberT = subscriber_six_for_test<mtest::structures::StructSix, mtest::Buffer>;

        auto data = GetPublishableSix();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubGmq2, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser parser(it);

        subs.applyMessageWithUpdates(parser);

        auto data2 = GetPublishableSix();
        EXPECT_NOT(subs.getState() == data2);

        data2.aggregate.name = "changed name";

        data2.basic.anInt = -2;
        data2.basic.anUInt = 3;
        data2.basic.aReal = 4.0;

        data2.aggregate.theAggregate.anInt = -200;
        data2.aggregate.theAggregate.anUInt = 300;
        data2.aggregate.theAggregate.aReal = 400.0;
        data2.aggregate.theAggregate.aString = "new part";

        EXPECT(subs.getState() == data2);
    },
    lest_CASE( "test_publishable_six.TestComposeNoChangeUpdate3" )
    {
        auto data = GetPublishableSix();

        using ComposerT = mtest::GmqComposer;
        using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;

        PublishableT publ(data);

        publ.startTick(mtest::Buffer());
        // no changes
        mtest::Buffer b = publ.endTick();


        auto b2 = makeBuffer(PathPubGmq3, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "test_publishable_six.TestGmqParseNoChangeUpdate3" )
    {
        using SubscriberT = subscriber_six_for_test<mtest::structures::StructSix, mtest::Buffer>;

        auto data = GetPublishableSix();

        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(PathPubGmq3, lest_env);
        auto it = b.getReadIter();
        mtest::GmqParser parser(it);

        subs.applyMessageWithUpdates(parser);

        auto data2 = GetPublishableSix();
        EXPECT(subs.getState() == data2);
    },

};

lest_MODULE(specification(), test_publishable_six2);
