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

#include "test_common2.h"

namespace
{

std::string PathPubSixJson = DataPrefix + "test_publishable_six.json";
std::string PathPubSixJson1 = DataPrefix + "test_publishable_six_update1.json";
std::string PathPubSixJson2 = DataPrefix + "test_publishable_six_update2.json";
std::string PathPubSixJson3 = DataPrefix + "test_publishable_six_update3.json";

std::string PathPubSixGmq = DataPrefix + "test_publishable_six2.gmq";
std::string PathPubSixGmq1 = DataPrefix + "test_publishable_six2_update1.gmq";
std::string PathPubSixGmq2 = DataPrefix + "test_publishable_six2_update2.gmq";
std::string PathPubSixGmq3 = DataPrefix + "test_publishable_six2_update3.gmq";

inline
mtest::structures::StructSix GetPublishableSix()
{
    mtest::structures::StructSix data;

    data.name = "TheName";
            
    // data.basic = data.make_basic();
    data.basic.anInt = -100000;
    data.basic.anUInt = 100000;
    data.basic.aReal = 3.14;
    data.basic.aString = "basic string";

    // data.aggregate = data.make_aggregate();
    data.aggregate.name = "aggregate name";

    // data.aggregate.theAggregate = data.aggregate.make_theAggregate();
    data.aggregate.theAggregate.anInt = -100;
    data.aggregate.theAggregate.anUInt = 100;
    data.aggregate.theAggregate.aReal = 100;
    data.aggregate.theAggregate.aString = "basic string inside aggregate";
    data.aggregate.lastValue = 0;

    return data;
}

template<class T, class ComposerT>
class publishable_six_for_test :
    public mtest::StructSix_WrapperForPublisher<T, ComposerT>
{
    public:
    publishable_six_for_test(T& data) : 
        mtest::StructSix_WrapperForPublisher<T, ComposerT>(data) {}

	virtual void generateStateSyncMessage(ComposerT& composer)
    {
        mtest::StructSix_WrapperForPublisher<T, ComposerT>::compose(composer);
    }
};


enum Events6 {
    notifyUpdated_anInt, notifyUpdated_anUInt, notifyUpdated_aReal, notifyUpdated_aString,
    notifyUpdated_name, notifyUpdated_theAggregate, notifyUpdated_lastValue,
    notifyUpdated_basic, notifyUpdated_aggregate, notifyUpdated, notifyFullyUpdated
};
class user_BasicTypes_handler : public  mtest::BasicTypes_subscriber
{
    public:
    std::vector<Events6>& handled;

    user_BasicTypes_handler(std::vector<Events6>& handled) : handled(handled) {}

    void notifyUpdated_anInt(int64_t old) override { handled.push_back(Events6::notifyUpdated_anInt); }
    void notifyUpdated_anUInt(uint64_t old) override { handled.push_back(Events6::notifyUpdated_anUInt); }
    void notifyUpdated_aReal(double old) override { handled.push_back(Events6::notifyUpdated_aReal); }
    void notifyUpdated_aString(GMQ_COLL string old) override { handled.push_back(Events6::notifyUpdated_aString); }

};
class user_AggregateType_handler : public  mtest::AggregateType_subscriber
{
    public:
    std::vector<Events6>& handled;

    user_AggregateType_handler(std::vector<Events6>& handled) : handled(handled) {}

    GMQ_COLL unique_ptr<mtest::BasicTypes_subscriber> make_theAggregate() override { return GMQ_COLL unique_ptr<mtest::BasicTypes_subscriber>{new user_BasicTypes_handler(handled)}; }

    void notifyUpdated_name(GMQ_COLL string old) override { handled.push_back(Events6::notifyUpdated_name); }
    void notifyUpdated_theAggregate() override { handled.push_back(Events6::notifyUpdated_theAggregate); }
    void notifyUpdated_lastValue(int64_t old) override { handled.push_back(Events6::notifyUpdated_lastValue); }


};

class user_StructSix_handler : public mtest::StructSix_subscriber
{
    public:
    std::vector<Events6> handled;

    user_StructSix_handler() {}

    const std::vector<Events6>& getEvents() const { return handled; }

    GMQ_COLL unique_ptr<mtest::BasicTypes_subscriber> make_basic() override { return GMQ_COLL unique_ptr<mtest::BasicTypes_subscriber>{new user_BasicTypes_handler(handled)}; }
    GMQ_COLL unique_ptr<mtest::AggregateType_subscriber> make_aggregate() override { return GMQ_COLL unique_ptr<mtest::AggregateType_subscriber>{new user_AggregateType_handler(handled)}; }

    void notifyFullyUpdated() override { handled.push_back(Events6::notifyFullyUpdated); }
    void notifyUpdated() override { handled.push_back(Events6::notifyUpdated); }
    void notifyUpdated_name(GMQ_COLL string old) override { handled.push_back(Events6::notifyUpdated_name); }
    void notifyUpdated_basic() override { handled.push_back(Events6::notifyUpdated_basic); }
    void notifyUpdated_aggregate() override { handled.push_back(Events6::notifyUpdated_aggregate); }

};


class publishable_six_json : public types_json
{
    public:
    using DataT = mtest::structures::StructSix;
    using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;
    using SubscriberT = user_StructSix_handler;
};

class publishable_six_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::StructSix;
    using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;
    using SubscriberT = user_StructSix_handler;
};

std::vector<Events6> events0 = {
                    Events6::notifyFullyUpdated
                };

void doUpdate1(mtest::structures::StructSix& data)
{
    data.aggregate.theAggregate.anInt = -101;
}

template<class T>
void doUpdatePublisher1(T& publ)
{
    publ.get4set_aggregate().get4set_theAggregate().set_anInt(-101);
}

std::vector<Events6> events1 = {
                    Events6::notifyFullyUpdated,
                    Events6::notifyUpdated_anInt,
                    Events6::notifyUpdated_theAggregate,
                    Events6::notifyUpdated_aggregate,
                    Events6::notifyUpdated
                };

void doUpdate2(mtest::structures::StructSix& data)
{
    data.aggregate.name = "changed name";

    data.basic.anInt = -2;
    data.basic.anUInt = 3;
    data.basic.aReal = 4.0;

    data.aggregate.theAggregate.anInt = -200;
    data.aggregate.theAggregate.anUInt = 300;
    data.aggregate.theAggregate.aReal = 400.0;
    data.aggregate.theAggregate.aString = "new part";
}

template<class T>
void doUpdatePublisher2(T& publ)
{
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
}

std::vector<Events6> events2 = {

                Events6::notifyFullyUpdated,
                Events6::notifyUpdated_name,
                Events6::notifyUpdated_aggregate,
                Events6::notifyUpdated,
                Events6::notifyUpdated_anInt,
                Events6::notifyUpdated_basic,
                Events6::notifyUpdated,
                Events6::notifyUpdated_anUInt,
                Events6::notifyUpdated_basic,
                Events6::notifyUpdated,
                Events6::notifyUpdated_aReal,
                Events6::notifyUpdated_basic,
                Events6::notifyUpdated,
                Events6::notifyUpdated_anInt,
                Events6::notifyUpdated_anUInt,
                Events6::notifyUpdated_aReal,
                Events6::notifyUpdated_aString,
                Events6::notifyUpdated_theAggregate,
                Events6::notifyUpdated_aggregate,
                Events6::notifyUpdated
                };
}


const lest::test test_publishable_six[] =
{
        lest_CASE( "test_publishable_six.TestStateSync" )
    {
        testStateSync2<publishable_six_json>(PathPubSixJson, GetPublishableSix, lest_env);
        testStateSync2<publishable_six_gmq>(PathPubSixGmq, GetPublishableSix, lest_env);
    },
    lest_CASE( "test_publishable_six.TestUpdate1" )
    {
        testUpdate2<publishable_six_json>(PathPubSixJson, PathPubSixJson1, GetPublishableSix, doUpdatePublisher1<typename publishable_six_json::PublishableT>, doUpdate1, lest_env);
        testUpdate2<publishable_six_gmq>(PathPubSixGmq, PathPubSixGmq1, GetPublishableSix, doUpdatePublisher1<typename publishable_six_gmq::PublishableT>, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_six.TestUpdate2" )
    {
        testUpdate2<publishable_six_json>(PathPubSixJson, PathPubSixJson2, GetPublishableSix, doUpdatePublisher2<typename publishable_six_json::PublishableT>, doUpdate2, lest_env);
        testUpdate2<publishable_six_gmq>(PathPubSixGmq, PathPubSixGmq2, GetPublishableSix, doUpdatePublisher2<typename publishable_six_gmq::PublishableT>, doUpdate2, lest_env);
    },
    lest_CASE( "test_publishable_six.TestUpdate3" )
    {
        testEmptyUpdate2<publishable_six_json>(PathPubSixJson, PathPubSixJson3, GetPublishableSix, lest_env);
        testEmptyUpdate2<publishable_six_gmq>(PathPubSixGmq, PathPubSixGmq3, GetPublishableSix, lest_env);
    },
    lest_CASE( "test_publishable_six.TestNotifyStateSync" )
    {
        testNotify2<publishable_six_json>(PathPubSixJson, "", events0, lest_env);
        testNotify2<publishable_six_gmq>(PathPubSixGmq, "", events0, lest_env);
    },
    lest_CASE( "test_publishable_six.TestNotifyUpdate1" )
    {
        testNotify2<publishable_six_json>(PathPubSixJson, PathPubSixJson1, events1, lest_env);
        testNotify2<publishable_six_gmq>(PathPubSixGmq, PathPubSixGmq1, events1, lest_env);
    },
    lest_CASE( "test_publishable_six.TestNotifyUpdate2" )
    {
        testNotify2<publishable_six_json>(PathPubSixJson, PathPubSixJson2, events2, lest_env);
        testNotify2<publishable_six_gmq>(PathPubSixGmq, PathPubSixGmq2, events2, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_six);
