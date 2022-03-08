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

namespace test_interop2_csharp
{

    public class test_struct_du
    {
        private const string Prefix = TestCommon.DataPathPrefix + "data/dunion/";
        private const string PathJson0 = Prefix + "test_struct_du_0.json";
        private const string PathJson1 = Prefix + "test_struct_du_1.json";
        private const string PathJson2 = Prefix + "test_struct_du_2.json";
        private const string PathGmq0 = Prefix + "test_struct_du_0.gmq";
        private const string PathGmq1 = Prefix + "test_struct_du_1.gmq";
        private const string PathGmq2 = Prefix + "test_struct_du_2.gmq";

        static mtest.struct_du GetStructDu0()
        {
            mtest.struct_du msg = new mtest.struct_du();

            return msg;
        }

        static mtest.struct_du GetStructDu1()
        {
            mtest.struct_du msg = new mtest.struct_du();

            msg.disc_union.setCurrentVariant(mtest.Idu_one.Variants.one);
            msg.disc_union.D1 = 1.2;
            msg.disc_union.D2 = 3.4;
            msg.disc_union.D3 = 5.6;

            return msg;
        }

        static mtest.struct_du GetStructDu2()
        {
            mtest.struct_du msg = new mtest.struct_du();

            msg.disc_union.setCurrentVariant(mtest.Idu_one.Variants.two);
            msg.disc_union.Data.Add(1.2);
            msg.disc_union.Data.Add(3.4);
            msg.disc_union.Data.Add(5.6);

            return msg;
        }

        static void TestJsonCompose(Func<mtest.struct_du> sampleData, string fileName)
        {
            mtest.struct_du msg = sampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.struct_du_message.compose(composer, msg);

            // uncomment to update file
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }

        static void TestGmqCompose(Func<mtest.struct_du> sampleData, string fileName)
        {
            mtest.struct_du msg = sampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_du_message.compose(composer, msg);


            // uncomment to update file
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }

        static void TestJsonParse(Func<mtest.struct_du> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_du msg = new mtest.struct_du();
            mtest.struct_du_message.parse(parser, msg);

            Assert.Equal(msg, sampleData());
        }

        static void TestGmqParse(Func<mtest.struct_du> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_du msg = new mtest.struct_du();
            mtest.struct_du_message.parse(parser, msg);

            Assert.Equal(sampleData(), msg);
        }

        //[Fact]
        //public static void TestJsonCompose0()
        //{
        //    TestJsonCompose(GetStructDu0, PathJson0);
        //}
        [Fact]
        public static void TestJsonCompose1()
        {
            TestJsonCompose(GetStructDu1, PathJson1);
        }

        [Fact]
        public static void TestJsonCompose2()
        {
            TestJsonCompose(GetStructDu2, PathJson2);
        }

        //[Fact]
        //public static void TestGmqCompose0()
        //{
        //    TestGmqCompose(GetStructDu0, PathGmq0);
        //}
        [Fact]
        public static void TestGmqCompose1()
        {
            TestGmqCompose(GetStructDu1, PathGmq1);
        }

        [Fact]
        public static void TestGmqCompose2()
        {
            TestGmqCompose(GetStructDu2, PathGmq2);
        }

        [Fact]
        public static void TestJsonParse1()
        {
            TestJsonParse(GetStructDu1, PathJson1);
        }

        [Fact]
        public static void TestJsonParse2()
        {
            TestJsonParse(GetStructDu2, PathJson2);
        }

        [Fact]
        public static void TestGmqParse1()
        {
            TestGmqParse(GetStructDu1, PathGmq1);
        }

        [Fact]
        public static void TestGmqParse2()
        {
            TestGmqParse(GetStructDu2, PathGmq2);
        }

    }

}
