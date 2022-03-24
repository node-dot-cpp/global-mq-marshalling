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

namespace
{
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
        throw std::exception();
    }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
    {
        throw std::exception();
    }
};


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
void doUpdatePublisher1(T& publ)
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
void doUpdatePublisher2(T& publ)
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

class publishable_dunion_json
{
    public:
    using ComposerT = mtest::JsonComposer;
    using ParserT = mtest::JsonParser;
    using DataT = mtest::structures::publishable_dunion;
    using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;
    using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

    static bool AreEqual(const mtest::Buffer& l, const mtest::Buffer& r)
    {
        return AreEqualIgnoreWhite(l, r);
    }
};

class publishable_dunion_gmq
{
    public:
    using ComposerT = mtest::GmqComposer;
    using ParserT = mtest::GmqParser;
    using DataT = mtest::structures::publishable_dunion;
    using PublishableT = publishable_dunion_for_test<mtest::structures::publishable_dunion, ComposerT>;
    using SubscriberT = subscriber_dunion_for_test<mtest::structures::publishable_dunion, mtest::Buffer>;

    static bool AreEqual(const mtest::Buffer& l, const mtest::Buffer& r)
    {
        return ::AreEqual(l, r);
    }
};
}

const lest::test test_publishable_dunion[] =
{
    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync0" )
    {
        testPublishableComposeStateSync<publishable_dunion_json>(JsonPath_s0, GetPublishableUnion_0, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseStateSync0" )
    {
        testPublishableParseStateSync<publishable_dunion_json>(JsonPath_s0, GetPublishableUnion_0, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync1" )
    {
        testPublishableComposeStateSync<publishable_dunion_json>(JsonPath_s1, GetPublishableUnion_1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseStateSync1" )
    {
        testPublishableParseStateSync<publishable_dunion_json>(JsonPath_s1, GetPublishableUnion_1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeStateSync2" )
    {
        testPublishableComposeStateSync<publishable_dunion_json>(JsonPath_s2, GetPublishableUnion_2, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseStateSync2" )
    {
        testPublishableParseStateSync<publishable_dunion_json>(JsonPath_s2, GetPublishableUnion_2, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeUpdate1" )
    {
        testPublishableComposeUpdate<publishable_dunion_json>(JsonPath_u1, GetPublishableUnion_0, doUpdatePublisher1<typename publishable_dunion_json::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseUpdate1" )
    {
        testPublishableParseUpdate<publishable_dunion_json>(JsonPath_u1, GetPublishableUnion_0, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonComposeUpdate2" )
    {
        testPublishableComposeUpdate<publishable_dunion_json>(JsonPath_u2, GetPublishableUnion_1, doUpdatePublisher2<typename publishable_dunion_json::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestJsonParseUpdate2" )
    {
        testPublishableParseUpdate<publishable_dunion_json>(JsonPath_u2, GetPublishableUnion_1, doUpdate2, lest_env);
    },
////////////////////////
    lest_CASE( "test_publishable_dunion.TestGmqComposeStateSync0" )
    {
        testPublishableComposeStateSync<publishable_dunion_gmq>(GmqPath_s0, GetPublishableUnion_0, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqParseStateSync0" )
    {
        testPublishableParseStateSync<publishable_dunion_gmq>(GmqPath_s0, GetPublishableUnion_0, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqComposeStateSync1" )
    {
        testPublishableComposeStateSync<publishable_dunion_gmq>(GmqPath_s1, GetPublishableUnion_1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqParseStateSync1" )
    {
        testPublishableParseStateSync<publishable_dunion_gmq>(GmqPath_s1, GetPublishableUnion_1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqComposeStateSync2" )
    {
        testPublishableComposeStateSync<publishable_dunion_gmq>(GmqPath_s2, GetPublishableUnion_2, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqParseStateSync2" )
    {
        testPublishableParseStateSync<publishable_dunion_gmq>(GmqPath_s2, GetPublishableUnion_2, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqComposeUpdate1" )
    {
        testPublishableComposeUpdate<publishable_dunion_gmq>(GmqPath_u1, GetPublishableUnion_0, doUpdatePublisher1<typename publishable_dunion_gmq::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqParseUpdate1" )
    {
        testPublishableParseUpdate<publishable_dunion_gmq>(GmqPath_u1, GetPublishableUnion_0, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqComposeUpdate2" )
    {
        testPublishableComposeUpdate<publishable_dunion_gmq>(GmqPath_u2, GetPublishableUnion_1, doUpdatePublisher2<typename publishable_dunion_gmq::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dunion.TestGmqParseUpdate2" )
    {
        testPublishableParseUpdate<publishable_dunion_gmq>(GmqPath_u2, GetPublishableUnion_1, doUpdate2, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_dunion);
