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
        internal const string Path = "test_publishable_six.json";
        internal const string Path1 = "test_publishable_six_update1.json";
        internal const string Path2 = "test_publishable_six_update2.json";
        internal const string Path3 = "test_publishable_six_update3.json";


        static IPlatformSupport jsonFactory = new DefaultJsonPlatformSupport();

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


        internal static void TestComposeStateSync(IPlatformSupport platform, String fileName)
        {

            mtest.StructSix_publisher publ = new  mtest.StructSix_publisher();
            mtest.StructSix data = GetPublishableSix();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);

            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            //buffer.writeToFile(Path);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }

        internal static void TestParseStateSync(IPlatformSupport platform, String fileName)
        {
             mtest.StructSix_subscriber subs = new  mtest.StructSix_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = platform.makePublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

             //mtest.StructSix actual = ( mtest.StructSix)subs.debugOnlyGetData();
             //mtest.StructSix expected = GetPublishableSix();
            Assert.True(subs.isEquivalent(GetPublishableSix()));
        }


        internal static void TestComposeUpdate(IPlatformSupport platform, String fileName, Action< mtest.IStructSix> updateDelegate)
        {
            mtest.StructSix_publisher publ = new mtest.StructSix_publisher();
            mtest.StructSix data = GetPublishableSix();
            publ.debugOnlySetData(data);

            BufferT buffer = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);

            publ.startTick(composer);

            updateDelegate(publ);

            publ.endTick();

            // uncomment to update file
            //buffer.writeToFile(Path1);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }
        internal static void TestParseUpdate(IPlatformSupport platform, String fileName, Action< mtest.IStructSix> updateDelegate)
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
            TestComposeStateSync(jsonFactory, Path);
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            TestParseStateSync(jsonFactory, Path);
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestComposeUpdate(jsonFactory, Path1, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestParseUpdate(jsonFactory, Path1, doUpdate1);
        }

        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestComposeUpdate(jsonFactory, Path2, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestParseUpdate(jsonFactory, Path2, doUpdate2);
        }


        [Fact]
        public static void TestJsonComposeNoChangeUpdate3()
        {
            TestComposeUpdate(jsonFactory, Path3, doNothing);
        }

        [Fact]
        public static void TestJsonParseNoChangeUpdate3()
        {
            TestParseUpdate(jsonFactory, Path3, null);
        }
    }
}
