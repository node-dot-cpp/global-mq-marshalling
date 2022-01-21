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

#include "test_idl_common.h"

std::string Path = "test_publishable_six.json";
std::string Path1 = "test_publishable_six_update1.json";
std::string Path2 = "test_publishable_six_update2.json";
std::string Path3 = "test_publishable_six_update3.json";


const lest::test test_publishable_six[] =
{
    lest_CASE( "TestJsonComposeStateSync" )
    {
        auto data = GetPublishableSix();

        using ComposerT = mtest::JsonComposer<mtest::Buffer>;
        using PublishableT = publishable_six_for_test<mtest::structures::StructSix, ComposerT>;

        PublishableT publ(data);

        mtest::Buffer b;
        ComposerT composer(b);

        publ.generateStateSyncMessage(composer);

        auto b2 = makeBuffer(Path, lest_env);
        EXPECT(b == b2);
    },
    lest_CASE( "TestJsonParseStateSync" )
    {
        using SubscriberT = subscriber_six_for_test<mtest::structures::StructSix, mtest::Buffer>;

        mtest::structures::StructSix data;
        SubscriberT subs(data);

        mtest::Buffer b = makeBuffer(Path, lest_env);
        auto it = b.getReadIter();
        mtest::JsonParser<mtest::Buffer> parser(it);

        subs.applyJsonStateSyncMessage(parser);

        auto data2 = GetPublishableSix();
        EXPECT(subs.getState() == data2);
    },
};

lest_MODULE(specification(), test_publishable_six);
