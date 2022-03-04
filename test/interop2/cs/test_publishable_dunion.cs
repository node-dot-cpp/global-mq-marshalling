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

    public class test_publishable_dunion
    {
        private const string Prefix = TestCommon.DataPathPrefix + "publishable_dunion/";

        internal const string JsonPath_s0 = Prefix + "state_sync_0.json";
        internal const string JsonPath_s1 = Prefix + "state_sync_1.json";
        internal const string JsonPath_s2 = Prefix + "state_sync_2.json";
        internal const string JsonPath_u1 = Prefix + "update_1.json";
        internal const string JsonPath_u2 = Prefix + "update_2.json";

        private const string GmqPath_s0 = Prefix + "state_sync_0.gmq";
        private const string GmqPath_s1 = Prefix + "state_sync_1.gmq";
        private const string GmqPath_s2 = Prefix + "state_sync_2.gmq";
        private const string GmqPath_u1 = Prefix + "update_1.gmq";
        private const string GmqPath_u2 = Prefix + "update_2.gmq";


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

        public static mtest.publishable_dunion GetPublishableUnion_0()
        {
            mtest.publishable_dunion data = new mtest.publishable_dunion();

            data.name = "dunion";

            return data;
        }

        static void doUpdate1(mtest.Ipublishable_dunion data)
        {
            //modify substructure inside vector
            data.anUnion.setCurrentVariant(mtest.Idu_one.Variants.one);

            data.anUnion.D1 = 3.14;
            data.anUnion.D2 = -3.14;
            data.anUnion.D3 = -3.14e-20;
        }

        static void doUpdate2(mtest.Ipublishable_dunion data)
        {
            //modify existing elements
            data.anUnion.setCurrentVariant(mtest.Idu_one.Variants.two);

            data.anUnion.Data.Add(3.14);
            data.anUnion.Data.Add(-3.14);
            data.anUnion.Data.Add(-3.14e-20);
        }

        public static mtest.publishable_dunion GetPublishableUnion_1()
        {
            mtest.publishable_dunion data = new mtest.publishable_dunion();
            doUpdate1(data);
            return data;
        }

        public static mtest.publishable_dunion GetPublishableUnion_2()
        {
            mtest.publishable_dunion data = new mtest.publishable_dunion();
            doUpdate2(data);
            return data;
        }

  
        static void TestComposeStateSync(Protocol proto, String fileName, Func<mtest.publishable_dunion> getState)
        {
            mtest.publishable_dunion_publisher publ = new mtest.publishable_dunion_publisher();
            mtest.publishable_dunion data = getState();
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

        static void TestParseStateSync(Protocol proto, String fileName, Func<mtest.publishable_dunion> getState)
        {
            mtest.publishable_dunion_subscriber subs = new mtest.publishable_dunion_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = makePublishableParser(proto, buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            mtest.publishable_dunion expected = getState();

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestComposeUpdate(Protocol proto, String fileName, Func<mtest.publishable_dunion> getState, Action<mtest.Ipublishable_dunion> updateDelegate)
        {
            mtest.publishable_dunion_publisher publ = new mtest.publishable_dunion_publisher();
            mtest.publishable_dunion data = getState();
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
        static void TestParseUpdate(Protocol proto, String fileName, Func<mtest.publishable_dunion> getState, Action<mtest.Ipublishable_dunion> updateDelegate)
        {
            mtest.publishable_dunion_subscriber subs = new mtest.publishable_dunion_subscriber();
            mtest.publishable_dunion data = getState();
            subs.debugOnlySetData(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = makePublishableParser(proto, buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable_dunion expected = getState();

            Assert.False(expected.isEquivalent(subs));

            updateDelegate(expected);

            Assert.True(expected.isEquivalent(subs));
        }

        [Fact]
        public static void TestJsonComposeStateSync0()
        {
            TestComposeStateSync(Protocol.Json, JsonPath_s0, GetPublishableUnion_0);
        }

        [Fact]
        public static void TestJsonParseStateSync0()
        {
            TestParseStateSync(Protocol.Json, JsonPath_s0, GetPublishableUnion_0);
        }
        [Fact]
        public static void TestJsonComposeStateSync1()
        {
            TestComposeStateSync(Protocol.Json, JsonPath_s1, GetPublishableUnion_1);
        }

        [Fact]
        public static void TestJsonParseStateSync1()
        {
            TestParseStateSync(Protocol.Json, JsonPath_s1, GetPublishableUnion_1);
        }
        [Fact]
        public static void TestJsonComposeStateSync2()
        {
            TestComposeStateSync(Protocol.Json, JsonPath_s2, GetPublishableUnion_2);
        }

        [Fact]
        public static void TestJsonParseStateSync2()
        {
            TestParseStateSync(Protocol.Json, JsonPath_s2, GetPublishableUnion_2);
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestComposeUpdate(Protocol.Json, JsonPath_u1, GetPublishableUnion_0, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestParseUpdate(Protocol.Json, JsonPath_u1, GetPublishableUnion_0, doUpdate1);
        }


        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestComposeUpdate(Protocol.Json, JsonPath_u2, GetPublishableUnion_1, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestParseUpdate(Protocol.Json, JsonPath_u2, GetPublishableUnion_1, doUpdate2);
        }



        ///////////////////// gmq //////////////////
        [Fact]
        public static void TestGmqComposeStateSync0()
        {
            TestComposeStateSync(Protocol.Gmq, GmqPath_s0, GetPublishableUnion_0);
        }

        [Fact]
        public static void TestGmqParseStateSync0()
        {
            TestParseStateSync(Protocol.Gmq, GmqPath_s0, GetPublishableUnion_0);
        }
        [Fact]
        public static void TestGmqComposeStateSync1()
        {
            TestComposeStateSync(Protocol.Gmq, GmqPath_s1, GetPublishableUnion_1);
        }

        [Fact]
        public static void TestGmqParseStateSync1()
        {
            TestParseStateSync(Protocol.Gmq, GmqPath_s1, GetPublishableUnion_1);
        }
        [Fact]
        public static void TestGmqComposeStateSync2()
        {
            TestComposeStateSync(Protocol.Gmq, GmqPath_s2, GetPublishableUnion_2);
        }

        [Fact]
        public static void TestGmqParseStateSync2()
        {
            TestParseStateSync(Protocol.Gmq, GmqPath_s2, GetPublishableUnion_2);
        }

        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            TestComposeUpdate(Protocol.Gmq, GmqPath_u1, GetPublishableUnion_0, doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            TestParseUpdate(Protocol.Gmq, GmqPath_u1, GetPublishableUnion_0, doUpdate1);
        }


        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            TestComposeUpdate(Protocol.Gmq, GmqPath_u2, GetPublishableUnion_1, doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            TestParseUpdate(Protocol.Gmq, GmqPath_u2, GetPublishableUnion_1, doUpdate2);
        }
    }

}
