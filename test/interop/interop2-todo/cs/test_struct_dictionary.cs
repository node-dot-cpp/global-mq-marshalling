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

namespace test_interop2_csharp_todo
{

    public class test_struct_dictionary
    {
        private const string Prefix = TestCommon.DataPathPrefix + "struct_dictionary/";

        private const string JsonPath1 = Prefix + "struct_2.json";
        private const string GmqPath1 = Prefix + "struct_2.gmq";

        public static bool WriteFiles = false;

        public static mtest.struct_dictionary GetDictionary_2()
        {
            mtest.struct_dictionary data = new mtest.struct_dictionary();

            data.dictionary_one.Add("hello", "world");
            data.dictionary_one.Add("red", "blue");
            data.dictionary_one.Add("dog", "cat");


            mtest.IAggregateType aggr1 = new mtest.AggregateType();
            aggr1.name = "aggr1";
            aggr1.theAggregate.anInt = -5;
            aggr1.theAggregate.anUInt = 5;
            aggr1.theAggregate.aReal = 3.14;
            aggr1.theAggregate.aString = "hello!";
            aggr1.lastValue = 99;

            data.dictionary_two.Add(2, aggr1);
            data.dictionary_two.Add(3, aggr1);
            data.dictionary_two.Add(4, aggr1);

            mtest.Idu_one du = new mtest.du_one();
            du.setCurrentVariant(mtest.du_one_variants.two);
            du.Data.Add(-100.001);
            du.Data.Add(-200.002);
            du.Data.Add(-300.003);

            data.dictionary_three.Add(-5, du);
            data.dictionary_three.Add(-6, du);
            data.dictionary_three.Add(-7, du);

            mtest.BasicTypes bt = new mtest.BasicTypes();
            bt.anInt = -99;
            bt.aString = "someName";

            data.dictionary_four.Add("someName", bt);

            mtest.BasicTypes bt2 = new mtest.BasicTypes();
            bt.anInt = -98;
            bt.aString = "otherName";

            data.dictionary_four.Add("otherName", bt);

            return data;
        }

        static void TestJsonCompose(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            mtest.struct_dictionary msg = sampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.struct_dictionary_message.compose(composer, msg);

            if (WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
        }

        static void TestGmqCompose(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            mtest.struct_dictionary msg = sampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_dictionary_message.compose(composer, msg);


            if (WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            Assert.Equal(expected, buffer);
        }

        static void TestJsonParse(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_dictionary msg = new mtest.struct_dictionary();
            mtest.struct_dictionary_message.parse(parser, msg);

            Assert.Equal(msg, sampleData());
        }

        static void TestGmqParse(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_dictionary msg = new mtest.struct_dictionary();
            mtest.struct_dictionary_message.parse(parser, msg);

            Assert.Equal(sampleData(), msg);
        }

        [Fact]
        public static void TestJsonCompose0()
        {
            TestJsonCompose(GetDictionary_2, JsonPath1);
        }

        [Fact]
        public static void TestJsonParse0()
        {
            TestJsonParse(GetDictionary_2, JsonPath1);
        }

        [Fact]
        public static void TestGmqCompose0()
        {
            TestGmqCompose(GetDictionary_2, GmqPath1);
        }

        [Fact]
        public static void TestGmqParse0()
        {
            TestGmqParse(GetDictionary_2, GmqPath1);
        }

    }

}
