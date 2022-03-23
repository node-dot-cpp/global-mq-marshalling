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

namespace test_interop3_csharp
{

    public class test_message_dictionary
    {
        private const string Prefix = TestCommon.DataPathPrefix + "message_dictionary/";

        private const string JsonPath_0 = Prefix + "message_0.json";
        private const string GmqPath_0 = Prefix + "message_0.gmq";

        public static bool WriteFiles = false;

        public static mtest.struct_dictionary1 GetDictionary_0()
        {
            mtest.struct_dictionary1 data = new mtest.struct_dictionary1();

            data.dictionary_one.Add("hello", "world");
            data.dictionary_one.Add("red", "blue");
            data.dictionary_one.Add("dog", "cat");

            return data;
        }


        [Fact]
        public static void TestJsonCompose()
        {
            mtest.struct_dictionary1 msg = GetDictionary_0();

            SimpleBuffer buffer = new SimpleBuffer();

            mtest.json_scope.composeMessage_message_dictionary1_json(buffer, msg.dictionary_one);

            if(WriteFiles)
                buffer.writeToFile(JsonPath_0);

            SimpleBuffer expected = SimpleBuffer.readFromFile(JsonPath_0);
            Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
        }

        [Fact]
        public static void TestJsonHandle()
        {
            bool condition = false;

            SimpleBuffer buffer = SimpleBuffer.readFromFile(JsonPath_0);

            mtest.json_scope.handleMessage(buffer,
                mtest.json_scope.makeMessageHandler(mtest.json_scope.MsgId.message_dictionary1_json, (JsonParser parser, ulong id) => {
                    mtest.struct_dictionary1 msg = mtest.json_scope.parseMessage_message_dictionary1_json(parser);
                    condition = msg.isEquivalent(GetDictionary_0());
                }),
                mtest.json_scope.makeDefaultMessageHandler((JsonParser parser, ulong id) => { Assert.True(false); })
            );

            Assert.True(condition);
        }

        [Fact]
        public static void TestGmqCompose()
        {
            mtest.struct_dictionary1 msg = GetDictionary_0();

            SimpleBuffer buffer = new SimpleBuffer();

            mtest.gmq_scope.composeMessage_message_dictionary1_gmq(buffer, msg.dictionary_one);

            if (WriteFiles)
                buffer.writeToFile(GmqPath_0);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(GmqPath_0));
        }


        [Fact]
        public static void TestGmqHandle()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(GmqPath_0);

            bool condition = false;

            mtest.gmq_scope.handleMessage(buffer,
                mtest.gmq_scope.makeMessageHandler(mtest.gmq_scope.MsgId.message_dictionary1_gmq, (GmqParser parser, ulong id) => {
                    mtest.struct_dictionary1 msg = mtest.gmq_scope.parseMessage_message_dictionary1_gmq(parser);
                    condition = msg.Equals(GetDictionary_0());
                }),
                mtest.gmq_scope.makeDefaultMessageHandler((GmqParser parser, ulong id) => { Assert.True(false); })
            );

            Assert.True(condition);
        }

    }

}
