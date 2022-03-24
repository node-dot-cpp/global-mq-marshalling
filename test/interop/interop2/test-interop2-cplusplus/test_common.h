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

#ifndef TEST_IDL_COMMON_H_INCLUDED
#define TEST_IDL_COMMON_H_INCLUDED


#include "../../3rdparty/lest/include/lest/lest.hpp"
#include "generated_interop2.h"

lest::tests& specification();


inline
mtest::Buffer makeBuffer(const std::string& filename, lest::env & lest_env)
{
    FILE* input_file = fopen(filename.c_str(), "rb");
    if(!input_file)
        throw lest::failure{ lest_LOCATION, "fopen", "\"" + filename + "\""};


    mtest::Buffer b;
    b.read_file(input_file);
    fclose(input_file);

    return b;
}

inline
std::string buffersToStringBinary(const mtest::Buffer& l, const mtest::Buffer& r)
{
    std::string result = "[ ";

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    while(it1.isData())
    {
        result += fmt::format("{:02x} ", *it1);
        ++it1;
    }

    result += "] != [ ";

    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();
    while(it2.isData())
    {
        result += fmt::format("{:02x} ", *it2);
        ++it2;
    }

    result += "]";
    return result;
}

inline
std::string buffersToStringText(const mtest::Buffer& l, const mtest::Buffer& r)
{
    return "\"" + globalmq::marshalling::impl::json::string2JsonString(l) +
        "\" != \"" + globalmq::marshalling::impl::json::string2JsonString(r) + "\"";
}

inline
void AreEqualBinary(const mtest::Buffer& l, const mtest::Buffer& r)
{

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();

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
void AreEqualIgnoreWhite(const mtest::Buffer& l, const mtest::Buffer& r)
{

    auto it1 = const_cast<mtest::Buffer&>(l).getReadIter();
    auto it2 = const_cast<mtest::Buffer&>(r).getReadIter();

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

template<typename Types>
void testPublishableComposeStateSync(std::string fileName, std::function<typename Types::DataT()> getState, lest::env & lest_env)
{
        auto data = getState();

        typename Types::PublishableT publ(data);

        mtest::Buffer b;
        typename Types::ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto expected = makeBuffer(fileName, lest_env);
        Types::ExpectAreEqual(expected, b);
}

template<typename Types>
void testPublishableParseStateSync(std::string fileName, std::function<typename Types::DataT()> getState, lest::env & lest_env)
{

        typename Types::DataT data;
        typename Types::SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(fileName, lest_env);
        auto it = b.getReadIter();
        typename Types::ParserT parser(it);

        subs.parseStateSyncMessage(parser);

        auto expected = getState();
        EXPECT(expected == subs.getState());
}

template<typename Types>
void testPublishableComposeUpdate(std::string fileName, std::function<typename Types::DataT()> getInitState,
                        std::function<void(typename Types::PublishableT&)> doUpdate, lest::env & lest_env)
{
    auto data = getInitState();

    typename Types::PublishableT publ(data);

    publ.startTick(mtest::Buffer());
    doUpdate(publ);
    mtest::Buffer b = publ.endTick();

    auto expected = makeBuffer(fileName, lest_env);
    Types::ExpectAreEqual(expected, b);
}

template<typename Types>
void testPublishableParseUpdate(std::string fileName, std::function<typename Types::DataT()> getInitState,
                        std::function<void(typename Types::DataT&)> doUpdate, lest::env & lest_env)
{

        auto data = getInitState();

        typename Types::SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(fileName, lest_env);
        auto it = b.getReadIter();
        typename Types::ParserT parser(it);

        subs.applyMessageWithUpdates(parser);

        auto data2 = getInitState();
        EXPECT_NOT(subs.getState() == data2);

        doUpdate(data2);
        EXPECT(subs.getState() == data2);
}

#endif // TEST_IDL_COMMON_H_INCLUDED
