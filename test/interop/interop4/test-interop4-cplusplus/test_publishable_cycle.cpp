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


using namespace std;

namespace
{
std::string Prefix = "data/publishable_cycle/";

mtest::structures::publishable_cycle GetPublishableCycle_0()
{
    return mtest::structures::publishable_cycle{};
}
void doUpdate1(mtest::structures::publishable_cycle& data)
{
    data.data.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs.push_back(mtest::structures::Cicle_1());

    data.data.childs[1].child.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs[1].child.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs[1].child.childs.push_back(mtest::structures::Cicle_1());

    data.data.childs[1].child.childs[1].child.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs[1].child.childs[1].child.childs.push_back(mtest::structures::Cicle_1());
    data.data.childs[1].child.childs[1].child.childs.push_back(mtest::structures::Cicle_1());
}

template<class ComposerT>
void doUpdatePublisher1(mtest::publishable_cycle_WrapperForPublisher<ComposerT>& publ)
{
    publ.get4set_data().get4set_childs().insert_before(0, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().insert_before(1, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().insert_before(2, mtest::structures::Cicle_1());



    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(0, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(1, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(2, mtest::structures::Cicle_1());


    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(0, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(1, mtest::structures::Cicle_1());
    publ.get4set_data().get4set_childs().get4set_at(1).get4set_child().get4set_childs().get4set_at(1).get4set_child().get4set_childs().insert_before(2, mtest::structures::Cicle_1());
}



class publishable_cycle_json : public types_json
{
    public:
    using DataT = mtest::structures::publishable_cycle;
    using PublishableT = mtest::publishable_cycle_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_cycle_subscriber;
};

class publishable_cycle_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::publishable_cycle;
    using PublishableT = mtest::publishable_cycle_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_cycle_subscriber;
};
}



const lest::test test_publishable_cycle[] =
{
    lest_CASE( "test_publishable_cycle.TestStateSync0" )
    {
        testStateSync2<publishable_cycle_json>(Prefix + "state_sync_0.json", GetPublishableCycle_0, lest_env);
        testStateSync2<publishable_cycle_gmq>(Prefix + "state_sync_0.gmq", GetPublishableCycle_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate1" )
    {
        testUpdate2<publishable_cycle_json>(Prefix + "state_sync_0.json", Prefix + "update_1.json", GetPublishableCycle_0, doUpdatePublisher1<typename publishable_cycle_json::ComposerT>, doUpdate1, lest_env);
        testUpdate2<publishable_cycle_gmq>(Prefix + "state_sync_0.gmq", Prefix + "update_1.gmq", GetPublishableCycle_0, doUpdatePublisher1<typename publishable_cycle_gmq::ComposerT>, doUpdate1, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_cycle);
