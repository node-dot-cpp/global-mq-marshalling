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
    public class test_simple
    {

        private const string PathJson = TestCommon.DataPathPrefix + "simple/sample_1.json";
        private const string PathGmq = TestCommon.DataPathPrefix + "simple/sample_1.gmq";

        static mtest.SimpleStruct GetSample1()
        {
            mtest.SimpleStruct msg = new mtest.SimpleStruct();

            msg.name = "Hello world!";
            msg.id = 1;

            return msg;
        }

        [Fact]
        public static void TestJsonCompose()
        {
            mtest.SimpleStruct data = GetSample1();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.SimpleStruct_message.compose(composer, data);


            // uncomment to update file
            //buffer.writeToFile(PathJson);

            SimpleBuffer expected = SimpleBuffer.readFromFile(PathJson);
            Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
        }

        [Fact]
        public static void TestJsonParse()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathJson);
            JsonParser parser = new JsonParser(buffer.getReadIterator());
            mtest.SimpleStruct msg = new mtest.SimpleStruct();
            mtest.SimpleStruct_message.parse(parser, msg);
            
            Assert.True(GetSample1().isEquivalent(msg));
         }



        [Fact]
        public static void TestGmqCompose()
        {
            mtest.SimpleStruct data = GetSample1();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.SimpleStruct_message.compose(composer, data);

            // uncomment to update file
            //buffer.writeToFile(PathGmq);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(PathGmq));
        }

        [Fact]
        public static void TestGmqParse()
        {

            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathGmq);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.SimpleStruct msg = new mtest.SimpleStruct();
            mtest.SimpleStruct_message.parse(parser, msg);

            Assert.True(GetSample1().isEquivalent(msg));
        }

    }

}
