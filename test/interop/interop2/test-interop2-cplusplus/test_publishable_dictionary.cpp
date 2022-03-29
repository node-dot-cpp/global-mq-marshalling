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

class publishable_dictionary_json : public types_json
{
    public:
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = publishable_dictionary_for_test<mtest::structures::publishable_dictionary, ComposerT>;
    using SubscriberT = subscriber_dictionary_for_test<mtest::structures::publishable_dictionary, mtest::Buffer>;
};

class publishable_dictionary_gmq : public types_gmq
{
    public:
    using DataT = mtest::structures::publishable_dictionary;
    using PublishableT = publishable_dictionary_for_test<mtest::structures::publishable_dictionary, ComposerT>;
    using SubscriberT = subscriber_dictionary_for_test<mtest::structures::publishable_dictionary, mtest::Buffer>;
};
}




const lest::test test_publishable_dictionary[] =
{
    lest_CASE( "test_publishable_dictionary.TestStateSync0" )
    {
        testStateSync<publishable_dictionary_json>(Prefix + "state_sync_0.json", GetPublishableDictionary_0, lest_env);
        testStateSync<publishable_dictionary_gmq>(Prefix + "state_sync_0.gmq", GetPublishableDictionary_0, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestStateSync1" )
    {
        testStateSync<publishable_dictionary_json>(Prefix + "state_sync_1.json", GetPublishableDictionary_1, lest_env);
        testStateSync<publishable_dictionary_gmq>(Prefix + "state_sync_1.gmq", GetPublishableDictionary_1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate1" )
    {
        testUpdate<publishable_dictionary_json>(Prefix + "update_1.json", GetPublishableDictionary_0, doUpdatePublisher1<typename publishable_dictionary_json::PublishableT>, doUpdate1, lest_env);
        testUpdate<publishable_dictionary_gmq>(Prefix + "update_1.gmq", GetPublishableDictionary_0, doUpdatePublisher1<typename publishable_dictionary_gmq::PublishableT>, doUpdate1, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate2" )
    {
        testUpdate<publishable_dictionary_json>(Prefix + "update_2.json", GetPublishableDictionary_1, doUpdatePublisher2<typename publishable_dictionary_json::PublishableT>, doUpdate2, lest_env);
        testUpdate<publishable_dictionary_gmq>(Prefix + "update_2.gmq", GetPublishableDictionary_1, doUpdatePublisher2<typename publishable_dictionary_gmq::PublishableT>, doUpdate2, lest_env);
    },
    lest_CASE( "test_publishable_dictionary.TestUpdate3" )
    {
        testUpdate<publishable_dictionary_json>(Prefix + "update_3.json", GetPublishableDictionary_1, doUpdatePublisher3<typename publishable_dictionary_json::PublishableT>, doUpdate3, lest_env);
        testUpdate<publishable_dictionary_gmq>(Prefix + "update_3.gmq", GetPublishableDictionary_1, doUpdatePublisher3<typename publishable_dictionary_gmq::PublishableT>, doUpdate3, lest_env);
    },
};

lest_MODULE(specification(), test_publishable_dictionary);
