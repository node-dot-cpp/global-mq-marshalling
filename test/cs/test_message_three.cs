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

using globalmq.marshalling;
using System;
using System.Collections.Generic;
using Xunit;

namespace TestProject1
{

    public class test_message_three
    {
        private const string PathJson = "test_message_three.json";
        private static void ComposeMessageThree(BufferT buffer, mtest.struct_one msg)
        {
            mtest.test_json.composeMessage_message_three(buffer,
                thirdParam:msg.thirdParam,
                firstParam: msg.firstParam, fifthParam: msg.fifthParam, forthParam: msg.forthParam, seventhParam: msg.seventhParam,
                eighthParam: msg.eighthParam,
                ninethParam: msg.ninethParam,
                secondParam: msg.secondParam,
                tenthParam: msg.tenthParam,
                sixthParam: msg.sixthParam
            );
        }

        [Fact]
        public static void TestJsonCompose()
        {
            mtest.struct_one msg = test_struct_one.GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();

            ComposeMessageThree(buffer, msg);

            // uncomment to update file
            //buffer.writeToFile(PathJson);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(PathJson));
        }

        [Fact]
        public static void TestJsonHandle()
        {
            bool condition = false;

            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathJson);

            mtest.test_json.handleMessage(buffer,
                mtest.test_json.makeMessageHandler(mtest.test_json.MsgId.message_three, (ParserBase parser) => {
                    mtest.struct_one msg = mtest.test_json.message_three_message.parse(parser);
                    condition = msg.Equals(test_struct_one.GetSampleData());
                }),
                mtest.test_json.makeMessageHandler(mtest.test_json.MsgId.message_five, (ParserBase parser) => { Assert.True(false); }),
                mtest.test_json.makeDefaultMessageHandler((ParserBase parser) => { Assert.True(false); })
            );

            Assert.True(condition);
        }

        [Fact]
        public static void TestJsonHandleDefault()
        {
            bool condition = false;

            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathJson);

            mtest.test_json.handleMessage(buffer,
                mtest.test_json.makeMessageHandler(mtest.test_json.MsgId.message_five, (ParserBase parser) => { Assert.True(false); }),
                mtest.test_json.makeDefaultMessageHandler((ParserBase parser) =>
                {
                    //mb we need to remove data from stream, otherwise we get an exception from parser
                    // TODO see what we should really do in that case
                    mtest.test_json.message_three_message.parse(parser);
                    condition = true;
                })
            );

            Assert.True(condition);
        }
    }
}
