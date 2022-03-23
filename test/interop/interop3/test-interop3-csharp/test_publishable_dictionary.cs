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

    public class test_publishable_dictionary
    {
        private const string Prefix = TestCommon.DataPathPrefix + "publishable_dictionary/";

        static bool WriteFiles = false;

        static mtest.publishable_dictionary GetPublishableDictionary_2()
        {
            mtest.publishable_dictionary pub = new mtest.publishable_dictionary();
            pub.name = "publishable_dictionary";
            pub.data = test_struct_dictionary.GetDictionary_2();

            return pub;
        }

        // replace full element in dictionary
        static void doUpdate21(mtest.Ipublishable_dictionary data)
        {
            mtest.IAggregateType aggr1 = new mtest.AggregateType();
            aggr1.name = "aggr1";
            aggr1.theAggregate.anInt = -15;
            aggr1.theAggregate.anUInt = 15;
            aggr1.theAggregate.aReal = 13.14;
            aggr1.theAggregate.aString = "hello2!";
            aggr1.lastValue = 199;

            data.data.dictionary_two[2] = aggr1;
        }

        static void doUpdate22(mtest.Ipublishable_dictionary data)
        {
            mtest.Idu_one du = new mtest.du_one();
            du.setCurrentVariant(mtest.Idu_one.Variants.one);
            du.D1 = -100.001;
            du.D2 = -200.002;
            du.D3 = -300.003;

            data.data.dictionary_three[-6] = du;
        }

        static void doUpdate23(mtest.Ipublishable_dictionary data)
        {
            mtest.BasicTypes bt = new mtest.BasicTypes();
            bt.anInt = -999;
            bt.aString = "someName";

            data.data.dictionary_four["someName"] = bt;
        }

        // modify sub element in dictionary

        static void doUpdate24(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_two[2].lastValue = 0;
        }

        static void doUpdate25(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_three[-6].Data.Add(-400.004);
        }

        static void doUpdate26(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_four["someName"].aReal = 9;
        }

        static void TestComposeStateSync(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState)
        {
            mtest.publishable_dictionary_publisher publ = new mtest.publishable_dictionary_publisher();
            mtest.publishable_dictionary data = getState();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = TestCommon.makePublishableComposer(proto, buffer);
            
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            if(WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            if (proto == TestCommon.Protocol.Json)
                Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
            else if (proto == TestCommon.Protocol.Gmq)
                Assert.Equal(expected, buffer);
        }

        static void TestParseStateSync(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState)
        {
            mtest.publishable_dictionary_subscriber subs = new mtest.publishable_dictionary_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            mtest.publishable_dictionary expected = getState();

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestStateSync(int id, Func<mtest.publishable_dictionary> getState)
        {
            string fileName = Prefix + "state_sync_" + id.ToString();
            TestComposeStateSync(TestCommon.Protocol.Json, fileName + ".json", getState);
            TestParseStateSync(TestCommon.Protocol.Json, fileName + ".json", getState);

            TestComposeStateSync(TestCommon.Protocol.Gmq, fileName + ".gmq", getState);
            TestParseStateSync(TestCommon.Protocol.Gmq, fileName + ".gmq", getState);
        }

        static void TestComposeUpdate(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState, Action<mtest.Ipublishable_dictionary> updateDelegate)
        {
            mtest.publishable_dictionary_publisher publ = new mtest.publishable_dictionary_publisher();
            mtest.publishable_dictionary data = getState();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = TestCommon.makePublishableComposer(proto, buffer);

            publ.startTick(composer);

            updateDelegate(publ);

            publ.endTick();

            if (WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            if (proto == TestCommon.Protocol.Json)
                Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
            else if (proto == TestCommon.Protocol.Gmq)
                Assert.Equal(expected, buffer);

        }
        static void TestParseUpdate(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState, Action<mtest.Ipublishable_dictionary> updateDelegate)
        {
            mtest.publishable_dictionary_subscriber subs = new mtest.publishable_dictionary_subscriber();
            mtest.publishable_dictionary data = getState();
            subs.debugOnlySetData(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable_dictionary expected = getState();

            Assert.False(expected.isEquivalent(subs));

            updateDelegate(expected);

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestUpdate(int id, Func<mtest.publishable_dictionary> initState, Action<mtest.Ipublishable_dictionary> updateDelegate)
        {
            string fileName = Prefix + "update_" + id.ToString();
            TestComposeUpdate(TestCommon.Protocol.Json, fileName + ".json", initState, updateDelegate);
            TestParseUpdate(TestCommon.Protocol.Json, fileName + ".json", initState, updateDelegate);

            TestComposeUpdate(TestCommon.Protocol.Gmq, fileName + ".gmq", initState, updateDelegate);
            TestParseUpdate(TestCommon.Protocol.Gmq, fileName + ".gmq", initState, updateDelegate);
        }


        [Fact]
        public static void TestStateSync2()
        {
            TestStateSync(2, GetPublishableDictionary_2);
        }

        [Fact]
        public static void TestUpdate21()
        {
            TestUpdate(21, GetPublishableDictionary_2, doUpdate21);
        }
        [Fact]
        public static void TestUpdate22()
        {
            TestUpdate(22, GetPublishableDictionary_2, doUpdate22);
        }
        [Fact]
        public static void TestUpdate23()
        {
            TestUpdate(23, GetPublishableDictionary_2, doUpdate23);
        }
        [Fact]
        public static void TestUpdate24()
        {
            TestUpdate(24, GetPublishableDictionary_2, doUpdate24);
        }
        [Fact]
        public static void TestUpdate25()
        {
            TestUpdate(25, GetPublishableDictionary_2, doUpdate25);
        }

        [Fact]
        public static void TestUpdate26()
        {
            TestUpdate(26, GetPublishableDictionary_2, doUpdate26);
        }
    }

}
