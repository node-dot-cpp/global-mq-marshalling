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

    public class test_publishable_six
    {
        private const string Prefix = TestCommon.DataPathPrefix + "";

        internal const string Path = Prefix + "test_publishable_six.json";
        internal const string Path1 = Prefix + "test_publishable_six_update1.json";
        internal const string Path2 = Prefix + "test_publishable_six_update2.json";
        internal const string Path3 = Prefix + "test_publishable_six_update3.json";

        internal const string GmqPath_s0 = Prefix + "test_publishable_six2.gmq";
        internal const string GmqPath_u1 = Prefix + "test_publishable_six2_update1.gmq";
        internal const string GmqPath_u2 = Prefix + "test_publishable_six2_update2.gmq";
        internal const string GmqPath_u3 = Prefix + "test_publishable_six2_update3.gmq";


        static bool WriteFiles = false;
        static ITestPlatformSupport JsonPlatform = new TestJsonPlatform();
        static ITestPlatformSupport GmqPlatform = new TestGmqPlatform();

        internal static  mtest.StructSix GetPublishableSix()
        {
             mtest.StructSix data = new  mtest.StructSix();

            data.name = "TheName";
            
            data.basic = new  mtest.BasicTypes();
            data.basic.anInt = -100000;
            data.basic.anUInt = 100000;
            data.basic.aReal = 3.14;
            data.basic.aString = "basic string";

            data.aggregate = new  mtest.AggregateType();
            data.aggregate.name = "aggregate name";

            data.aggregate.theAggregate = new  mtest.BasicTypes();
            data.aggregate.theAggregate.anInt = -100;
            data.aggregate.theAggregate.anUInt = 100;
            data.aggregate.theAggregate.aReal = 100;
            data.aggregate.theAggregate.aString = "basic string inside aggregate";
            data.aggregate.lastValue = 0;

            return data;
        }


        internal static void TestComposeStateSync(ITestPlatformSupport platform, String fileName)
        {

            mtest.StructSix_publisher publ = new  mtest.StructSix_publisher();
            mtest.StructSix data = GetPublishableSix();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);

            publ.generateStateSyncMessage(composer);

            if (WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            Assert.True(platform.AreEqual(expected, buffer));
        }

        internal static void TestParseStateSync(ITestPlatformSupport platform, String fileName)
        {
             mtest.StructSix_subscriber subs = new  mtest.StructSix_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = platform.makePublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            Assert.True(subs.isEquivalent(GetPublishableSix()));
        }


        internal static void TestComposeUpdate(ITestPlatformSupport platform, String fileName, Action< mtest.IStructSix> updateDelegate)
        {
            mtest.StructSix_publisher publ = new mtest.StructSix_publisher();
            mtest.StructSix data = GetPublishableSix();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);

            publ.startTick(composer);

            updateDelegate(publ);

            publ.endTick();

            if (WriteFiles)
                buffer.writeToFile(fileName);

            SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
            Assert.True(platform.AreEqual(expected, buffer));
        }
        internal static void TestParseUpdate(ITestPlatformSupport platform, String fileName, Action< mtest.IStructSix> updateDelegate)
        {
             mtest.StructSix_subscriber subs = new  mtest.StructSix_subscriber();
            subs.debugOnlySetData(GetPublishableSix());

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = platform.makePublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

             //mtest.StructSix actual = ( mtest.StructSix)subs.debugOnlyGetData();
             mtest.StructSix expected = GetPublishableSix();

            if(updateDelegate != null)
            {
                // not equal yet
                Assert.False(subs.isEquivalent(expected));

                updateDelegate(expected);
            }

            Assert.True(subs.isEquivalent(expected));
        }

        internal static void doUpdate1( mtest.IStructSix data)
        {
            data.aggregate.theAggregate.anInt = -101;
        }

        internal static void doUpdate2( mtest.IStructSix data)
        {
            //modify substructure inside vector
            data.aggregate.name = "changed name";

            data.basic.anInt = -2;
            data.basic.anUInt = 3;
            data.basic.aReal = 4.0;

             mtest.IBasicTypes aggr = new  mtest.BasicTypes();
            aggr.anInt = -200;
            aggr.anUInt = 300;
            aggr.aReal = 400.0;
            aggr.aString = "new part";
            data.aggregate.theAggregate = aggr;
        }

        internal static void doNothing( mtest.IStructSix data) { }

        [Fact]
        public static void TestJsonComposeStateSync()
        {
            TestComposeStateSync(JsonPlatform, Path);
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            TestParseStateSync(JsonPlatform, Path);
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestComposeUpdate(JsonPlatform, Path1, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestParseUpdate(JsonPlatform, Path1, doUpdate1);
        }

        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestComposeUpdate(JsonPlatform, Path2, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestParseUpdate(JsonPlatform, Path2, doUpdate2);
        }


        [Fact]
        public static void TestJsonComposeNoChangeUpdate3()
        {
            TestComposeUpdate(JsonPlatform, Path3, doNothing);
        }

        [Fact]
        public static void TestJsonParseNoChangeUpdate3()
        {
            TestParseUpdate(JsonPlatform, Path3, null);
        }
//////////////////////
        [Fact]
        public static void TestGmqComposeStateSync()
        {
            TestComposeStateSync(GmqPlatform, GmqPath_s0);
        }

        [Fact]
        public static void TestGmqParseStateSync()
        {
            TestParseStateSync(GmqPlatform, GmqPath_s0);
        }

        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            TestComposeUpdate(GmqPlatform, GmqPath_u1, doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            TestParseUpdate(GmqPlatform, GmqPath_u1, doUpdate1);
        }

        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            TestComposeUpdate(GmqPlatform, GmqPath_u2, doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            TestParseUpdate(GmqPlatform, GmqPath_u2, doUpdate2);
        }


        [Fact]
        public static void TestGmqComposeNoChangeUpdate3()
        {
            TestComposeUpdate(GmqPlatform, GmqPath_u3, doNothing);
        }

        [Fact]
        public static void TestGmqParseNoChangeUpdate3()
        {
            TestParseUpdate(GmqPlatform, GmqPath_u3, null);
        }
    }
}
