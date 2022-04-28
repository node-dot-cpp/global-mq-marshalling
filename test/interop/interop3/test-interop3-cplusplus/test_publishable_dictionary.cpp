/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
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
#include "struct_dictionary.h"


using namespace std;

namespace
{
std::string Prefix = "data/publishable_dictionary/";

mtest::structures::publishable_dictionary GetPublishableDictionary_2()
{
    mtest::structures::publishable_dictionary data{};

    data.name = "publishable_dictionary";
    data.data = GetDictionary_2();

    return data;
}

// replace full element in dictionary
void doUpdate21(mtest::structures::publishable_dictionary& data)
{
    mtest::structures::AggregateType aggr1;
    aggr1.name = "aggr1";
    aggr1.theAggregate.anInt = -15;
    aggr1.theAggregate.anUInt = 15;
    aggr1.theAggregate.aReal = 13.14;
    aggr1.theAggregate.aString = "hello2!";
    aggr1.lastValue = 199;

    data.data.dictionary_two[2] = aggr1;

}

template<class T>
void doUpdatePublisher21(T& publ)
{
    mtest::structures::AggregateType aggr1;
    aggr1.name = "aggr1";
    aggr1.theAggregate.anInt = -15;
    aggr1.theAggregate.anUInt = 15;
    aggr1.theAggregate.aReal = 13.14;
    aggr1.theAggregate.aString = "hello2!";
    aggr1.lastValue = 199;

    publ.get4set_data().get4set_dictionary_two().update_value(2, aggr1);
}

void doUpdate22(mtest::structures::publishable_dictionary& data)
{
    mtest::structures::du_one du;
    du.initAs(mtest::structures::du_one::Variants::one);
    du.D1() = -100.001;
    du.D2() = -200.002;
    du.D3() = -300.003;

    data.data.dictionary_three[-6] = du;
}

template<class T>
void doUpdatePublisher22(T& publ)
{
    mtest::structures::du_one du;
    du.initAs(mtest::structures::du_one::Variants::one);
    du.D1() = -100.001;
    du.D2() = -200.002;
    du.D3() = -300.003;

    publ.get4set_data().get4set_dictionary_three().update_value(-6, du);
}

void doUpdate23(mtest::structures::publishable_dictionary& data)
{
    mtest::structures::BasicTypes bt;
    bt.anInt = -999;
    bt.aString = "someName";

    data.data.dictionary_four["someName"] = bt;
}

template<class T>
void doUpdatePublisher23(T& publ)
{
    mtest::structures::BasicTypes bt;
    bt.anInt = -999;
    bt.aString = "someName";

    publ.get4set_data().get4set_dictionary_four().update_value("someName", bt);
}

// modify sub element in dictionary

void doUpdate24(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_two[2].lastValue = 0;
}

template<class T>
void doUpdatePublisher24(T& publ)
{
    publ.get4set_data().get4set_dictionary_two().get4set_at(2).set_lastValue(0);
}

void doUpdate25(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_three[-6].Data().push_back(-400.004);
}

template<class T>
void doUpdatePublisher25(T& publ)
{
    size_t sz = publ.get_data().dictionary_three.at(-6).Data().size();

    publ.get4set_data().get4set_dictionary_three().get4set_at(-6).get4set_Data().insert_before(sz, -400.004);
}

void doUpdate26(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_four["someName"].aReal = 9;
}

template<class T>
void doUpdatePublisher26(T& publ)
{
    publ.get4set_data().get4set_dictionary_four().get4set_at("someName").set_aReal(9);
}


class publishable_dictionary_json : public types_json
{
    public:
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = mtest::publishable_dictionary_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_dictionary_subscriber;
};

class publishable_dictionary_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = mtest::publishable_dictionary_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_dictionary_subscriber;
};
}




const lest::test test_publishable_dictionary[] =
{
    lest_CASE( "test_publishable_dictionary.TestStateSync2" )
    {
        testStateSync2<publishable_dictionary_json>(Prefix + "state_sync_2.json", GetPublishableDictionary_2, lest_env);
        testStateSync2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", GetPublishableDictionary_2, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate21" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_21.json", GetPublishableDictionary_2, doUpdatePublisher21<typename publishable_dictionary_json::PublishableT>, doUpdate21, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_21.gmq", GetPublishableDictionary_2, doUpdatePublisher21<typename publishable_dictionary_gmq::PublishableT>, doUpdate21, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate22" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_22.json", GetPublishableDictionary_2, doUpdatePublisher22<typename publishable_dictionary_json::PublishableT>, doUpdate22, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_22.gmq", GetPublishableDictionary_2, doUpdatePublisher22<typename publishable_dictionary_gmq::PublishableT>, doUpdate22, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate23" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_23.json", GetPublishableDictionary_2, doUpdatePublisher23<typename publishable_dictionary_json::PublishableT>, doUpdate23, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_23.gmq", GetPublishableDictionary_2, doUpdatePublisher23<typename publishable_dictionary_gmq::PublishableT>, doUpdate23, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate24" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_24.json", GetPublishableDictionary_2, doUpdatePublisher24<typename publishable_dictionary_json::PublishableT>, doUpdate24, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_24.gmq", GetPublishableDictionary_2, doUpdatePublisher24<typename publishable_dictionary_gmq::PublishableT>, doUpdate24, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate25" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_25.json", GetPublishableDictionary_2, doUpdatePublisher25<typename publishable_dictionary_json::PublishableT>, doUpdate25, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_25.gmq", GetPublishableDictionary_2, doUpdatePublisher25<typename publishable_dictionary_gmq::PublishableT>, doUpdate25, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate26" )
    {
        testUpdate2<publishable_dictionary_json>(Prefix + "state_sync_2.json", Prefix + "update_26.json", GetPublishableDictionary_2, doUpdatePublisher26<typename publishable_dictionary_json::PublishableT>, doUpdate26, lest_env);
        testUpdate2<publishable_dictionary_gmq>(Prefix + "state_sync_2.gmq", Prefix + "update_26.gmq", GetPublishableDictionary_2, doUpdatePublisher26<typename publishable_dictionary_gmq::PublishableT>, doUpdate26, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_dictionary);
