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

std::string PathMsg3Json = DataPrefix + "test_message_three.json";

template<class BufferT>
void ComposeMessageThree(BufferT& buffer, mtest::structures::struct_one& msg)
{
	mtest::test_json::composeMessage<mtest::test_json::message_three>(buffer,
		mtest::thirdParam = msg.thirdParam,
		mtest::firstParam = msg.firstParam,
        mtest::fifthParam = msg.fifthParam,
        mtest::forthParam = msg.forthParam,
        mtest::seventhParam = msg.seventhParam,
		mtest::eighthParam = msg.eighthParam,
		mtest::ninethParam = msg.ninethParam,
		mtest::secondParam = msg.secondParam,
		mtest::tenthParam = msg.tenthParam,
		mtest::sixthParam = msg.sixthParam
	);
}


const lest::test test_message_three[] =
{
    lest_CASE( "TestJsonCompose" )
    {
        auto msg = GetSampleStructOne();

        mtest::JsonComposer composer;

        ComposeMessageThree(composer, msg);

        mtest::Buffer b = composer.getBuffer();
        auto expected = makeBuffer(PathMsg3Json, lest_env);
        EXPECT(AreEqualIgnoreWhite(expected, b));
    },
    lest_CASE( "TestJsonHandle" )
    {
        mtest::Buffer b = makeBuffer(PathMsg3Json, lest_env);
        auto iter = b.getReadIter();
        mtest::JsonParser parser(iter);

        bool condition = false;

        mtest::test_json::handleMessage(parser,
            mtest::makeMessageHandler<mtest::test_json::message_three>(
                [&](auto& parser) {
                    auto msg = mtest::test_json::MESSAGE_message_three_parse(parser);
                    auto msg2 = GetSampleStructOne();
                    condition = (msg == msg2);
                }),
            mtest::makeMessageHandler<mtest::test_json::message_five>(
                [&](auto& parser) {
                    EXPECT(false);
                }),
            mtest::makeDefaultMessageHandler(
                [&](auto& parser, uint64_t msgID) {
                    EXPECT(false);
                })
        );
            
        EXPECT(condition);
    },
    lest_CASE( "TestJsonHandleDefault" )
    {
        mtest::Buffer b = makeBuffer(PathMsg3Json, lest_env);
        auto iter = b.getReadIter();
        mtest::JsonParser parser(iter);

        bool condition = false;

        mtest::test_json::handleMessage(parser,
            // mtest::makeMessageHandler<mtest::test_json::message_three>(
            //     [&](auto& parser) {
            //         auto msg2 = GetSampleStructOne();
            //         condition = (msg == msg2);
            //     }),
            mtest::makeMessageHandler<mtest::test_json::message_five>(
                [&](auto& parser) {
                    EXPECT(false);
                }),
            mtest::makeDefaultMessageHandler(
                [&](auto& parser, uint64_t msgID) {
                    // TODO if we don't remove msg from buffer we get an exception
                    auto msg = mtest::test_json::MESSAGE_message_three_parse(parser);
                    condition = true;
                })
        );
            
        EXPECT(condition);
    },
};

lest_MODULE(specification(), test_message_three);
