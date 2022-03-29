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

#ifndef TEST_COMMON_H_INCLUDED
#define TEST_COMMON_H_INCLUDED


#include "../../3rdparty/lest/include/lest/lest.hpp"
#include <global_mq_common.h>
#include <globalmq/marshalling2/marshalling2.h>
#include <marshalling_impl.h>


using BufferT = globalmq::marshalling::Buffer;

lest::tests& specification();

inline
BufferT makeBuffer(const std::string& filename)
{
    FILE* input_file = fopen(filename.c_str(), "rb");
    if(!input_file)
        throw lest::failure{ lest_LOCATION, "fopen", "\"" + filename + "\""};


    BufferT b;
    b.read_file(input_file);
    fclose(input_file);

    return b;
}

inline
std::string buffersToStringBinary(const BufferT& l, const BufferT& r)
{
    std::string result = "[ ";

    auto it1 = const_cast<BufferT&>(l).getReadIter();
    while(it1.isData())
    {
        result += fmt::format("{:02x} ", *it1);
        ++it1;
    }

    result += "] != [ ";

    auto it2 = const_cast<BufferT&>(r).getReadIter();
    while(it2.isData())
    {
        result += fmt::format("{:02x} ", *it2);
        ++it2;
    }

    result += "]";
    return result;
}

inline
std::string buffersToStringText(const BufferT& l, const BufferT& r)
{
    return "\"" + globalmq::marshalling::impl::json::string2JsonString(l) +
        "\" != \"" + globalmq::marshalling::impl::json::string2JsonString(r) + "\"";
}

inline
void AreEqualBinary(const BufferT& l, const BufferT& r)
{

    auto it1 = const_cast<BufferT&>(l).getReadIter();
    auto it2 = const_cast<BufferT&>(r).getReadIter();

    while(it1.isData() && it2.isData())
    {
        if(*it1 != *it2)
            throw lest::failure{ lest_LOCATION, "AreEqualBinary", buffersToStringBinary(l, r) };

        ++it1;
        ++it2;
    }

    if(it1.isData() || it2.isData())
        throw lest::failure{ lest_LOCATION, "AreEqualBinary", buffersToStringBinary(l, r) };
}

inline
void AreEqualIgnoreWhite(const BufferT& l, const BufferT& r)
{

    auto it1 = const_cast<BufferT&>(l).getReadIter();
    auto it2 = const_cast<BufferT&>(r).getReadIter();

    while (it1.isData() && it2.isData())
    {
        if(*it1 != *it2)
        {
            while(it1.isData() && (*it1 == '\r' || *it1 == '\n'|| *it1 == '\t'|| *it1 == ' '))
                ++it1;

            while(it2.isData() && (*it2 == '\r' || *it2 == '\n'|| *it2 == '\t'|| *it2 == ' '))
                ++it2;

            if(!it1.isData() && !it2.isData())
                return;

            if(!it1.isData() || !it2.isData())
                throw lest::failure{ lest_LOCATION, "AreEqualIgnoreWhite", buffersToStringText(l, r) };

            if(*it1 != *it2)
                throw lest::failure{ lest_LOCATION, "AreEqualIgnoreWhite", buffersToStringText(l, r) };
        }

        ++it1;
        ++it2;
    }

    if(it1.isData() || it2.isData())
        throw lest::failure{ lest_LOCATION, "AreEqualIgnoreWhite", buffersToStringText(l, r) };
}


class types_json
{
    public:
    using ComposerT = globalmq::marshalling2::JsonComposer2<BufferT>;
    using ParserT = globalmq::marshalling2::JsonParser2<BufferT>;

    static void ExpectAreEqual(const BufferT& l, const BufferT& r)
    {
        ::AreEqualIgnoreWhite(l, r);
    }
};

class types_gmq
{
    public:
    using ComposerT = globalmq::marshalling2::GmqComposer2<BufferT>;
    using ParserT =globalmq::marshalling2::GmqParser2<BufferT>;

    static void ExpectAreEqual(const BufferT& l, const BufferT& r)
    {
        ::AreEqualBinary(l, r);
    }
};


template<typename Types>
void testPublishableComposeStateSync(std::string fileName, std::function<typename Types::DataT()> getState, lest::env & lest_env)
{
        auto data = getState();

        typename Types::PublishableT publ(data);

        BufferT b;
        typename Types::ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(fileName);
        Types::ExpectAreEqual(expected, b);
}

template<typename Types>
void testPublishableParseStateSync(std::string fileName, std::function<typename Types::DataT()> getState, lest::env & lest_env)
{

        typename Types::DataT data;
        typename Types::SubscriberT subs(data);

        BufferT b = makeBuffer(fileName);
        auto it = b.getReadIter();
        typename Types::ParserT parser(it);

        subs.parseStateSyncMessage(parser);

        auto expected = getState();
        EXPECT(expected == subs.getState());
}

template<typename TypesA, typename TypesB>
void testPublishableStateSync(std::string fileName, std::function<typename TypesA::DataT()> getState, lest::env & lest_env)
{
    testPublishableComposeStateSync<TypesA>(fileName + TypesA::Extension, getState, lest_env);
    testPublishableParseStateSync<TypesA>(fileName + TypesA::Extension, getState, lest_env);
    testPublishableComposeStateSync<TypesB>(fileName + TypesB::Extension, getState, lest_env);
    testPublishableParseStateSync<TypesB>(fileName + TypesB::Extension, getState, lest_env);
}

template<typename Types>
void testPublishableComposeUpdate(std::string fileName, std::function<typename Types::DataT()> getInitState,
                        std::function<void(typename Types::PublishableT&)> doUpdate, lest::env & lest_env)
{
    auto data = getInitState();

    typename Types::PublishableT publ(data);

    publ.startTick(BufferT());
    doUpdate(publ);
    BufferT b = publ.endTick();

    auto expected = makeBuffer(fileName);
    Types::ExpectAreEqual(expected, b);
}

template<typename Types>
void testPublishableParseUpdate(std::string fileName, std::function<typename Types::DataT()> getInitState,
                        std::function<void(typename Types::DataT&)> doUpdate, lest::env & lest_env)
{

        auto data = getInitState();

        typename Types::SubscriberT subs(data);

        BufferT b = makeBuffer(fileName);
        auto it = b.getReadIter();
        typename Types::ParserT parser(it);

        subs.applyMessageWithUpdates(parser);

        auto data2 = getInitState();
        EXPECT_NOT(subs.getState() == data2);

        doUpdate(data2);
        EXPECT(subs.getState() == data2);
}

template<typename TypesA, typename TypesB>
void testPublishableUpdate(std::string fileName, std::function<typename TypesA::DataT()> getInitState,
                        std::function<void(typename TypesA::DataT&)> doUpdate, lest::env & lest_env)
{
    testPublishableComposeUpdate<TypesA>(fileName + TypesA::Extension, getInitState, doUpdate, lest_env);
    testPublishableParseUpdate<TypesA>(fileName + TypesA::Extension, getInitState, doUpdate, lest_env);
    testPublishableComposeUpdate<TypesB>(fileName + TypesB::Extension, getInitState, doUpdate, lest_env);
    testPublishableParseUpdate<TypesB>(fileName + TypesB::Extension, getInitState, doUpdate, lest_env);
}

template<typename Types>
void testStateSync(std::string fileName, std::function<typename Types::DataT()> getState, lest::env & lest_env)
{
        auto data = getState();

        typename Types::PublishableT publ(data);

        BufferT b;
        typename Types::ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(fileName);
        Types::ExpectAreEqual(expected, b);



        typename Types::DataT data2;
        typename Types::SubscriberT subs(data2);

        auto it = b.getReadIter();
        typename Types::ParserT parser(it);

        subs.parseStateSyncMessage(parser);

        typename Types::DataT data3 = getState();
        EXPECT(data3 == subs.getState());
}


template<typename Types>
void testUpdate(std::string fileName, std::function<typename Types::DataT()> getInitState,
                        std::function<void(typename Types::PublishableT&)> doUpdatePub,
                        std::function<void(typename Types::DataT&)> doUpdateData, lest::env& lest_env)
{
    auto data = getInitState();

    typename Types::PublishableT publ(data);

    publ.startTick(BufferT());
    doUpdatePub(publ);
    BufferT b = publ.endTick();

    auto expected = makeBuffer(fileName);
    Types::ExpectAreEqual(expected, b);


    auto data2 = getInitState();
    typename Types::SubscriberT subs(data2);

    auto it = b.getReadIter();
    typename Types::ParserT parser(it);

    subs.applyMessageWithUpdates(parser);

    auto data3 = getInitState();
    EXPECT_NOT(subs.getState() == data3);

    doUpdateData(data3);
    EXPECT(subs.getState() == data3);

}



#endif // TEST_COMMON_H_INCLUDED
