/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
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
using NUnit.Framework;
using System;
using System.Collections.Generic;

namespace test_interop3_csharp
{
    [TestFixture]
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

            mtest.IAggregateType aggr2 = new mtest.AggregateType();
            aggr2.name = "aggr1";
            aggr2.theAggregate.anInt = -5;
            aggr2.theAggregate.anUInt = 5;
            aggr2.theAggregate.aReal = 3.14;
            aggr2.theAggregate.aString = "hello!";
            aggr2.lastValue = 99;

            mtest.IAggregateType aggr3 = new mtest.AggregateType();
            aggr3.name = "aggr1";
            aggr3.theAggregate.anInt = -5;
            aggr3.theAggregate.anUInt = 5;
            aggr3.theAggregate.aReal = 3.14;
            aggr3.theAggregate.aString = "hello!";
            aggr3.lastValue = 99;

            data.dictionary_two.Add(2, aggr1);
            data.dictionary_two.Add(3, aggr2);
            data.dictionary_two.Add(4, aggr3);

            mtest.Idu_one du1 = new mtest.du_one();
            du1.setCurrentVariant(mtest.du_one_variants.two);
            du1.Data.Add(-100.001);
            du1.Data.Add(-200.002);
            du1.Data.Add(-300.003);

            mtest.Idu_one du2 = new mtest.du_one();
            du2.setCurrentVariant(mtest.du_one_variants.two);
            du2.Data.Add(-100.001);
            du2.Data.Add(-200.002);
            du2.Data.Add(-300.003);

            mtest.Idu_one du3 = new mtest.du_one();
            du3.setCurrentVariant(mtest.du_one_variants.two);
            du3.Data.Add(-100.001);
            du3.Data.Add(-200.002);
            du3.Data.Add(-300.003);

            data.dictionary_three.Add(-5, du1);
            data.dictionary_three.Add(-6, du2);
            data.dictionary_three.Add(-7, du3);

            mtest.BasicTypes bt = new mtest.BasicTypes();
            bt.anInt = -99;
            bt.aString = "someName";

            mtest.BasicTypes bt2 = new mtest.BasicTypes();
            bt2.anInt = -98;
            bt2.aString = "otherName";

            data.dictionary_four.Add("someName", bt);
            data.dictionary_four.Add("otherName", bt2);

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
            Assert.AreEqual(expected, buffer);
        }

        static void TestJsonParse(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_dictionary msg = new mtest.struct_dictionary();
            mtest.struct_dictionary_message.parse(parser, msg);

            Assert.AreEqual(msg, sampleData());
        }

        static void TestGmqParse(Func<mtest.struct_dictionary> sampleData, string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_dictionary msg = new mtest.struct_dictionary();
            mtest.struct_dictionary_message.parse(parser, msg);

            Assert.AreEqual(sampleData(), msg);
        }

        [Test]
        public static void TestJsonCompose0()
        {
            TestJsonCompose(GetDictionary_2, JsonPath1);
        }

        [Test]
        public static void TestJsonParse0()
        {
            TestJsonParse(GetDictionary_2, JsonPath1);
        }

        [Test]
        public static void TestGmqCompose0()
        {
            TestGmqCompose(GetDictionary_2, GmqPath1);
        }

        [Test]
        public static void TestGmqParse0()
        {
            TestGmqParse(GetDictionary_2, GmqPath1);
        }

    }

}
