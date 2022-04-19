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
std::string Prefix = "data/publishable_html_tag/";


template<class ComposerT>
void doUpdatePublisher0(mtest::publishable_html_tag_WrapperForPublisher<ComposerT>& publ)
{
}

template<class ComposerT>
void doUpdatePublisher1(mtest::publishable_html_tag_WrapperForPublisher<ComposerT>& publ)
{
    publ.get4set_data().set_name("first");
    publ.get4set_data().get4set_tags().set_currentVariant(mtest::structures::HtmlTextOrTags::Variants::taglists);
    publ.get4set_data().get4set_tags().get4set_tags().insert_before(0, mtest::structures::HtmlTag());
    publ.get4set_data().get4set_tags().get4set_tags().get4set_at(0).set_name("second");
}



class publishable_html_tag_json : public types_json
{
    public:
    using DataT = mtest::structures::publishable_html_tag;
    using PublishableT = mtest::publishable_html_tag_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_html_tag_subscriber;
};

class publishable_html_tag_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::publishable_html_tag;
    using PublishableT = mtest::publishable_html_tag_WrapperForPublisher<ComposerT>;
    using SubscriberT = mtest::publishable_html_tag_subscriber;
};
}



const lest::test test_publishable_html_tag[] =
{
    lest_CASE( "test_publishable_html_tag.TestStateSync0" )
    {
        testStateSync3<publishable_html_tag_json>(Prefix + "state_sync_0.json", doUpdatePublisher0<typename publishable_html_tag_json::ComposerT>, lest_env);
        testStateSync3<publishable_html_tag_gmq>(Prefix + "state_sync_0.gmq", doUpdatePublisher0<typename publishable_html_tag_gmq::ComposerT>, lest_env);
    },
    lest_CASE( "test_publishable_html_tag.TestStateSync1" )
    {
        testStateSync3<publishable_html_tag_json>(Prefix + "state_sync_1.json", doUpdatePublisher1<typename publishable_html_tag_json::ComposerT>, lest_env);
        testStateSync3<publishable_html_tag_gmq>(Prefix + "state_sync_1.gmq", doUpdatePublisher1<typename publishable_html_tag_gmq::ComposerT>, lest_env);
    },
    lest_CASE( "test_publishable_html_tag.TestUpdate1" )
    {
        testUpdate3<publishable_html_tag_json>(Prefix + "state_sync_0.json", Prefix + "update_1.json", doUpdatePublisher1<typename publishable_html_tag_json::ComposerT>, lest_env);
        testUpdate3<publishable_html_tag_gmq>(Prefix + "state_sync_0.gmq", Prefix + "update_1.gmq", doUpdatePublisher1<typename publishable_html_tag_gmq::ComposerT>, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_html_tag);
