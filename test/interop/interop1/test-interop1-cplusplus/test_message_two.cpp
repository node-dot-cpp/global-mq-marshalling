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

std::string PathMsg2Gmq = DataPrefix + "test_message_two.gmq";

template<class BufferT>
void ComposeMessageTwo(BufferT& buffer, mtest::structures::struct_one& msg)
{
	mtest::test_gmq::composeMessage<mtest::test_gmq::message_two>(buffer,
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


const lest::test test_message_two[] =
{
    lest_CASE( "TestGmqCompose" )
    {
        auto msg = GetSampleStructOne();

        mtest::Buffer b;
        mtest::GmqComposer composer(b);

        ComposeMessageTwo(composer, msg);

        auto b2 = makeBuffer(PathMsg2Gmq, lest_env);
        EXPECT(AreEqual(b, b2));
    },
    lest_CASE( "TestGmqHandle" )
    {
        mtest::Buffer b = makeBuffer(PathMsg2Gmq, lest_env);
        auto iter = b.getReadIter();
        mtest::GmqParser parser(iter);

        bool condition = false;

        mtest::test_gmq::handleMessage(parser,
            mtest::makeMessageHandler<mtest::test_gmq::message_two>(
                [&](auto& parser) {
                    auto msg = mtest::test_gmq::MESSAGE_message_two_parse(parser);
                    auto msg2 = GetSampleStructOne();
                    condition = (msg == msg2);
                }),
            mtest::makeMessageHandler<mtest::test_gmq::message_four>(
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
    lest_CASE( "TestGmqHandleDefault" )
    {
        mtest::Buffer b = makeBuffer(PathMsg2Gmq, lest_env);
        auto iter = b.getReadIter();
        mtest::GmqParser parser(iter);

        bool condition = false;

        mtest::test_gmq::handleMessage(parser,
            // mtest::makeMessageHandler<mtest::test_gmq::message_two>(
            //     [&](auto& parser) {
            //         auto msg2 = GetSampleStructOne();
            //         condition = (msg == msg2);
            //     }),
            mtest::makeMessageHandler<mtest::test_gmq::message_four>(
                [&](auto& parser) {
                    EXPECT(false);
                }),
            mtest::makeDefaultMessageHandler(
                [&](auto& parser, uint64_t msgID) {
                    // TODO if we don't remove msg from buffer we get an exception
                    auto msg = mtest::test_gmq::MESSAGE_message_two_parse(parser);
                    condition = true;
                })
        );
            
        EXPECT(condition);
    },
};

lest_MODULE(specification(), test_message_two);
