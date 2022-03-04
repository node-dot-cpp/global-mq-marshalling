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

    public class test_publishable_dictionary
    {
        private const string Prefix = TestCommon.DataPathPrefix + "publishable_dictionary/";

        internal const string JsonPath_s0 = Prefix + "state_sync_0.json";
        internal const string JsonPath_s1 = Prefix + "state_sync_1.json";
        internal const string JsonPath_u1 = Prefix + "update_1.json";
        internal const string JsonPath_u2 = Prefix + "update_2.json";
        internal const string JsonPath_u3 = Prefix + "update_3.json";

        private const string GmqPath_s0 = Prefix + "state_sync_0.gmq";
        private const string GmqPath_s1 = Prefix + "state_sync_1.gmq";
        private const string GmqPath_u1 = Prefix + "update_1.gmq";
        private const string GmqPath_u2 = Prefix + "update_2.gmq";
        private const string GmqPath_u3 = Prefix + "update_3.gmq";


        public static bool WriteFiles = false;
        public enum Protocol { Json, Gmq };
        public static BufferT makeBuffer() { return new SimpleBuffer(); }
        public static IPublishableComposer makePublishableComposer(Protocol proto, BufferT buffer)
        {
            if (proto == Protocol.Json)
                return new JsonPublishableComposer(buffer);
            else if (proto == Protocol.Gmq)
                return new GmqPublishableComposer(buffer);
            else
                throw new Exception();
        }
        public static IPublishableParser makePublishableParser(Protocol proto,  ReadIteratorT readIter)
        {
            if (proto == Protocol.Json)
                return new JsonPublishableParser(readIter);
            else if (proto == Protocol.Gmq)
                return new GmqPublishableParser(readIter);
            else
                throw new Exception();
        }

        public static mtest.publishable_dictionary GetPublishableDictionary_0()
        {
            mtest.publishable_dictionary data = new mtest.publishable_dictionary();

            data.name = "dictionary";

            return data;
        }

        static void doUpdate1(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_one.Add("hello", "world");
            data.data.dictionary_one.Add("red", "blue");
            data.data.dictionary_one.Add("dog", "cat");
        }

        static void doUpdate2(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_one["hello"] = "goodbye";
        }

        static void doUpdate3(mtest.Ipublishable_dictionary data)
        {
            data.data.dictionary_one.Remove("red");
        }

        public static mtest.publishable_dictionary GetPublishableDictionary_1()
        {
            mtest.publishable_dictionary data = GetPublishableDictionary_0();
            doUpdate1(data);
            return data;
        }


        static void TestComposeStateSync(Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState)
        {
            mtest.publishable_dictionary_publisher publ = new mtest.publishable_dictionary_publisher();
            mtest.publishable_dictionary data = getState();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = makePublishableComposer(proto, buffer);
            
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            if(WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            if (proto == Protocol.Json)
                Assert.Equal(expected, buffer);
            else if (proto == Protocol.Gmq)
                Assert.Equal(expected, buffer);
        }

        static void TestParseStateSync(Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState)
        {
            mtest.publishable_dictionary_subscriber subs = new mtest.publishable_dictionary_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = makePublishableParser(proto, buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            mtest.publishable_dictionary expected = getState();

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestComposeUpdate(Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState, Action<mtest.Ipublishable_dictionary> updateDelegate)
        {
            mtest.publishable_dictionary_publisher publ = new mtest.publishable_dictionary_publisher();
            mtest.publishable_dictionary data = getState();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = makePublishableComposer(proto, buffer);

            publ.startTick(composer);

            updateDelegate(publ);

            publ.endTick();

            if (WriteFiles)
                buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }
        static void TestParseUpdate(Protocol proto, String fileName, Func<mtest.publishable_dictionary> getState, Action<mtest.Ipublishable_dictionary> updateDelegate)
        {
            mtest.publishable_dictionary_subscriber subs = new mtest.publishable_dictionary_subscriber();
            mtest.publishable_dictionary data = getState();
            subs.debugOnlySetData(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = makePublishableParser(proto, buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable_dictionary expected = getState();

            Assert.False(expected.isEquivalent(subs));

            updateDelegate(expected);

            Assert.True(expected.isEquivalent(subs));
        }

        [Fact]
        public static void TestJsonComposeStateSync0()
        {
            TestComposeStateSync(Protocol.Json, JsonPath_s0, GetPublishableDictionary_0);
        }

        [Fact]
        public static void TestJsonParseStateSync0()
        {
            TestParseStateSync(Protocol.Json, JsonPath_s0, GetPublishableDictionary_0);
        }
        [Fact]
        public static void TestJsonComposeStateSync1()
        {
            TestComposeStateSync(Protocol.Json, JsonPath_s1, GetPublishableDictionary_1);
        }

        [Fact]
        public static void TestJsonParseStateSync1()
        {
            TestParseStateSync(Protocol.Json, JsonPath_s1, GetPublishableDictionary_1);
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestComposeUpdate(Protocol.Json, JsonPath_u1, GetPublishableDictionary_0, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestParseUpdate(Protocol.Json, JsonPath_u1, GetPublishableDictionary_0, doUpdate1);
        }


        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestComposeUpdate(Protocol.Json, JsonPath_u2, GetPublishableDictionary_1, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestParseUpdate(Protocol.Json, JsonPath_u2, GetPublishableDictionary_1, doUpdate2);
        }

        [Fact]
        public static void TestJsonComposeUpdate3()
        {
            TestComposeUpdate(Protocol.Json, JsonPath_u3, GetPublishableDictionary_1, doUpdate3);
        }

        [Fact]
        public static void TestJsonParseUpdate3()
        {
            TestParseUpdate(Protocol.Json, JsonPath_u3, GetPublishableDictionary_1, doUpdate3);
        }


        ///////////////////// gmq //////////////////
        [Fact]
        public static void TestGmqComposeStateSync0()
        {
            TestComposeStateSync(Protocol.Gmq, GmqPath_s0, GetPublishableDictionary_0);
        }

        [Fact]
        public static void TestGmqParseStateSync0()
        {
            TestParseStateSync(Protocol.Gmq, GmqPath_s0, GetPublishableDictionary_0);
        }
        [Fact]
        public static void TestGmqComposeStateSync1()
        {
            TestComposeStateSync(Protocol.Gmq, GmqPath_s1, GetPublishableDictionary_1);
        }

        [Fact]
        public static void TestGmqParseStateSync1()
        {
            TestParseStateSync(Protocol.Gmq, GmqPath_s1, GetPublishableDictionary_1);
        }

        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            TestComposeUpdate(Protocol.Gmq, GmqPath_u1, GetPublishableDictionary_0, doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            TestParseUpdate(Protocol.Gmq, GmqPath_u1, GetPublishableDictionary_0, doUpdate1);
        }


        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            TestComposeUpdate(Protocol.Gmq, GmqPath_u2, GetPublishableDictionary_1, doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            TestParseUpdate(Protocol.Gmq, GmqPath_u2, GetPublishableDictionary_1, doUpdate2);
        }
        [Fact]
        public static void TestGmqComposeUpdate3()
        {
            TestComposeUpdate(Protocol.Gmq, GmqPath_u3, GetPublishableDictionary_1, doUpdate3);
        }

        [Fact]
        public static void TestGmqParseUpdate3()
        {
            TestParseUpdate(Protocol.Gmq, GmqPath_u3, GetPublishableDictionary_1, doUpdate3);
        }
    }

}
