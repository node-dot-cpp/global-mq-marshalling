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

    public class test_struct_one
    {
        private const string PathJson = "test_struct_one.json";
        private const string PathGmq = "test_struct_one.gmq";
        public static mtest.struct_one GetSampleData()
        {
            //create some sample data to be written to message

            mtest.struct_one msg = new mtest.struct_one();

            msg.firstParam = 1;
            msg.secondParam = new List<long> { 0, 1, 2, 3, 4, 5 };
            msg.thirdParam = new List<mtest.Ipoint3D> { new mtest.point3D { x = 0, y = 1, z = 2 }, new mtest.point3D { x = 3, y = 4, z = 5 } };
            msg.forthParam = 3;
            msg.fifthParam = "def";
            msg.sixthParam = new List<mtest.Ipoint> { new mtest.point { x = 0, y = 1 }, new mtest.point { x = 2, y = 3 }, new mtest.point { x = 4, y = 5 } };
            msg.seventhParam = 3.1416;
            msg.eighthParam = new mtest.point { x = 175, y = 186 };
            msg.ninethParam = new mtest.point3D { x = 123, y = 456, z = 789 };
            msg.tenthParam = new List<double> { 0.1, 1.2, 2.3 };

            return msg;
        }

        [Fact]
        public static void TestGmqCompose()
        {
            mtest.struct_one msg = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_one_message.compose(composer, msg);


            // uncomment to update file
            //buffer.writeToFile(PathGmq);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(PathGmq));
        }

        [Fact]
        public static void TestJsonCompose()
        {
            mtest.struct_one msg = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.struct_one_message.compose(composer, msg);

            // uncomment to update file
            //buffer.writeToFile(PathJson);

            Assert.True(buffer.EqualsIgnoreEol(SimpleBuffer.readFromFile(PathJson)));
        }

        [Fact]
        public static void TestGmqParse()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathGmq);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_one msg = new mtest.struct_one();
            mtest.struct_one_message.parse(parser, msg);

            Assert.Equal(msg, GetSampleData());
        }

        [Fact]
        public static void TestJsonParse()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(PathJson);
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_one msg = new mtest.struct_one();
            mtest.struct_one_message.parse(parser, msg);

            Assert.Equal(msg, GetSampleData());
        }
        [Fact]
        public static void TestGmqComposeAndParse()
        {
            mtest.struct_one msg = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_one_message.compose(composer, msg);

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_one msg2 = new mtest.struct_one();
            mtest.struct_one_message.parse(parser, msg2);

            Assert.Equal(msg, msg2);
        }

        [Fact]
        public static void TestJsonComposeAndParse()
        {
            mtest.struct_one msg = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.struct_one_message.compose(composer, msg);

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_one msg2 = new mtest.struct_one();
            mtest.struct_one_message.parse(parser, msg2);

            Assert.Equal(msg, msg2);
        }

    }

}
