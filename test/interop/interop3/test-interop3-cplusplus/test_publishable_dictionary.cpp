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
std::string Prefix = "data/publishable_dictionary/";

std::string JsonPath_s0 = Prefix + "state_sync_0.json";
std::string JsonPath_s1 = Prefix + "state_sync_1.json";
std::string JsonPath_u1 = Prefix + "update_1.json";
std::string JsonPath_u2 = Prefix + "update_2.json";
std::string JsonPath_u3 = Prefix + "update_3.json";

std::string GmqPath_s0 = Prefix + "state_sync_0.gmq";
std::string GmqPath_s1 = Prefix + "state_sync_1.gmq";
std::string GmqPath_u1 = Prefix + "update_1.gmq";
std::string GmqPath_u2 = Prefix + "update_2.gmq";
std::string GmqPath_u3 = Prefix + "update_3.gmq";



template<class T, class ComposerT>
class publishable_dictionary_for_test :
    public mtest::publishable_dictionary_WrapperForPublisher<T, ComposerT>
{
    public:
    publishable_dictionary_for_test(T& data) : 
        mtest::publishable_dictionary_WrapperForPublisher<T, ComposerT>(data) {}

	virtual void generateStateSyncMessage(ComposerT& composer)
    {
        mtest::publishable_dictionary_WrapperForPublisher<T, ComposerT>::compose(composer);
    }
};

template<class T, class BufferT>
class subscriber_dictionary_for_test :
    public mtest::publishable_dictionary_WrapperForSubscriber<T, BufferT>
{
    public:
    subscriber_dictionary_for_test(T& data) : 
        mtest::publishable_dictionary_WrapperForSubscriber<T, BufferT>(data) {}

	virtual void applyGmqStateSyncMessage( globalmq::marshalling::GmqParser<BufferT>& parser )
    {
        throw std::exception();
    }
	virtual void applyJsonStateSyncMessage( globalmq::marshalling::JsonParser<BufferT>& parser )
    {
        throw std::exception();
    }
};


bool operator==(const mtest::structures::publishable_dictionary& l, const mtest::structures::publishable_dictionary& r)
{
    if(l.name != r.name)
        return false;
    if(l.data.dictionary_one != r.data.dictionary_one)
        return false;
    return true;
}


mtest::structures::publishable_dictionary GetPublishableDictionary_0()
{
    mtest::structures::publishable_dictionary data{};

    data.name = "dictionary";

    return data;
}

void doUpdate1(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_one["hello"] = "world";
    data.data.dictionary_one["red"] = "blue";
    data.data.dictionary_one["dog"] = "cat";
}

template<class T>
void doUpdatePublisher1(T& publ)
{
    publ.get4set_data().get4set_dictionary_one().insert("hello", "world");
    publ.get4set_data().get4set_dictionary_one().insert("red", "blue");
    publ.get4set_data().get4set_dictionary_one().insert("dog", "cat");
}


mtest::structures::publishable_dictionary GetPublishableDictionary_1()
{
    mtest::structures::publishable_dictionary data = GetPublishableDictionary_0();
    doUpdate1(data);
    return data;
}

void doUpdate2(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_one["hello"] = "goodbye";
}

template<class T>
void doUpdatePublisher2(T& publ)
{
    publ.get4set_data().get4set_dictionary_one().update_value("hello", "goodbye");
}

void doUpdate3(mtest::structures::publishable_dictionary& data)
{
    data.data.dictionary_one.erase("red");
}

template<class T>
void doUpdatePublisher3(T& publ)
{
    publ.get4set_data().get4set_dictionary_one().remove("red");
}

class publishable_dictionary_json
{
    public:
    using ComposerT = mtest::JsonComposer;
    using ParserT = mtest::JsonParser;
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = publishable_dictionary_for_test<mtest::structures::publishable_dictionary, ComposerT>;
    using SubscriberT = subscriber_dictionary_for_test<mtest::structures::publishable_dictionary, mtest::Buffer>;

    static bool AreEqual(const mtest::Buffer& l, const mtest::Buffer& r)
    {
        return AreEqualIgnoreEol(l, r);
    }
};

class publishable_dictionary_gmq
{
    public:
    using ComposerT = mtest::GmqComposer;
    using ParserT = mtest::GmqParser;
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = publishable_dictionary_for_test<mtest::structures::publishable_dictionary, ComposerT>;
    using SubscriberT = subscriber_dictionary_for_test<mtest::structures::publishable_dictionary, mtest::Buffer>;

    static bool AreEqual(const mtest::Buffer& l, const mtest::Buffer& r)
    {
        return ::operator==(l, r);
    }
};
}

const lest::test test_publishable_dictionary[] =
{
    lest_CASE( "test_publishable_dictionary.TestJsonComposeStateSync0" )
    {
        testPublishableComposeStateSync<publishable_dictionary_json>(JsonPath_s0, GetPublishableDictionary_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonComposeStateSync1" )
    {
        testPublishableComposeStateSync<publishable_dictionary_json>(JsonPath_s1, GetPublishableDictionary_1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonParseStateSync0" )
    {
        testPublishableParseStateSync<publishable_dictionary_json>(JsonPath_s0, GetPublishableDictionary_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonParseStateSync1" )
    {
        testPublishableParseStateSync<publishable_dictionary_json>(JsonPath_s1, GetPublishableDictionary_1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonComposeUpdate1" )
    {
        testPublishableComposeUpdate<publishable_dictionary_json>(JsonPath_u1, GetPublishableDictionary_0, doUpdatePublisher1<typename publishable_dictionary_json::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonParseUpdate1" )
    {
        testPublishableParseUpdate<publishable_dictionary_json>(JsonPath_u1, GetPublishableDictionary_0, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonComposeUpdate2" )
    {
        testPublishableComposeUpdate<publishable_dictionary_json>(JsonPath_u2, GetPublishableDictionary_1, doUpdatePublisher2<typename publishable_dictionary_json::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonParseUpdate2" )
    {
        testPublishableParseUpdate<publishable_dictionary_json>(JsonPath_u2, GetPublishableDictionary_1, doUpdate2, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonComposeUpdate3" )
    {
        testPublishableComposeUpdate<publishable_dictionary_json>(JsonPath_u3, GetPublishableDictionary_1, doUpdatePublisher3<typename publishable_dictionary_json::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestJsonParseUpdate3" )
    {
        testPublishableParseUpdate<publishable_dictionary_json>(JsonPath_u3, GetPublishableDictionary_1, doUpdate3, lest_env);
    },
////////////////////////
    lest_CASE( "test_publishable_dictionary.TestGmqComposeStateSync0" )
    {
        testPublishableComposeStateSync<publishable_dictionary_gmq>(GmqPath_s0, GetPublishableDictionary_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqParseStateSync0" )
    {
        testPublishableParseStateSync<publishable_dictionary_gmq>(GmqPath_s0, GetPublishableDictionary_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqComposeStateSync1" )
    {
        testPublishableComposeStateSync<publishable_dictionary_gmq>(GmqPath_s1, GetPublishableDictionary_1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqParseStateSync1" )
    {
        testPublishableParseStateSync<publishable_dictionary_gmq>(GmqPath_s1, GetPublishableDictionary_1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqComposeUpdate1" )
    {
        testPublishableComposeUpdate<publishable_dictionary_gmq>(GmqPath_u1, GetPublishableDictionary_0, doUpdatePublisher1<typename publishable_dictionary_gmq::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqParseUpdate1" )
    {
        testPublishableParseUpdate<publishable_dictionary_gmq>(GmqPath_u1, GetPublishableDictionary_0, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqComposeUpdate2" )
    {
        testPublishableComposeUpdate<publishable_dictionary_gmq>(GmqPath_u2, GetPublishableDictionary_1, doUpdatePublisher2<typename publishable_dictionary_gmq::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqParseUpdate2" )
    {
        testPublishableParseUpdate<publishable_dictionary_gmq>(GmqPath_u2, GetPublishableDictionary_1, doUpdate2, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqComposeUpdate3" )
    {
        testPublishableComposeUpdate<publishable_dictionary_gmq>(GmqPath_u3, GetPublishableDictionary_1, doUpdatePublisher3<typename publishable_dictionary_gmq::PublishableT>, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestGmqParseUpdate3" )
    {
        testPublishableParseUpdate<publishable_dictionary_gmq>(GmqPath_u3, GetPublishableDictionary_1, doUpdate3, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_dictionary);
