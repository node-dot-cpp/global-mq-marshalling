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

    public class test_publishable_seven
    {
        internal const string JsonPath = "test_publishable_seven.json";
        internal const string JsonPath1 = "test_publishable_seven_u1.json";
        internal const string JsonPath2 = "test_publishable_seven_u2.json";
        internal const string JsonPath3 = "test_publishable_seven_u3.json";
        internal const string JsonPath4 = "test_publishable_seven_u4.json";
        internal const string JsonPath5 = "test_publishable_seven_u5.json";

        private const string GmqPath = "test_publishable_seven.gmq";
        private const string GmqPath1 = "test_publishable_seven_u1.gmq";
        private const string GmqPath2 = "test_publishable_seven_u2.gmq";
        private const string GmqPath3 = "test_publishable_seven_u3.gmq";
        private const string GmqPath4 = "test_publishable_seven_u4.gmq";
        private const string GmqPath5 = "test_publishable_seven_u5.gmq";

        static IPlatformSupport JsonFactory = new DefaultJsonPlatformSupport();
        static IPlatformSupport GmqFactory = new DefaultGmqPlatformSupport();


        public static mtest.publishable_seven GetPublishableSeven()
        {
            mtest.publishable_seven data = new mtest.publishable_seven();

            data.intVec = new List<Int64>();
            data.intVec.Add(1);
            data.intVec.Add(2);
            data.intVec.Add(3);

            data.uintVec = new List<UInt64>();
            data.uintVec.Add(4);
            data.uintVec.Add(5);
            data.uintVec.Add(6);

            data.realVec = new List<Double>();
            data.realVec.Add(7);
            data.realVec.Add(8);
            data.realVec.Add(9);

            data.strVec = new List<String>();
            data.strVec.Add("one");
            data.strVec.Add("two");
            data.strVec.Add("three");

            data.structVec = new List<mtest.Ipoint3D>();

            mtest.Ipoint3D e1 = new mtest.point3D();
            e1.x = 101;
            e1.y = 102;
            e1.z = 103;
            data.structVec.Add(e1);

            mtest.Ipoint3D e2 = new mtest.point3D();
            e2.x = 201;
            e2.y = 202;
            e2.z = 203;
            data.structVec.Add(e2);

            return data;
        }

        static void TestComposeStateSync(IPlatformSupport platform, String fileName)
        {

            mtest.publishable_seven_publisher publ = new mtest.publishable_seven_publisher();
            mtest.publishable_seven data = GetPublishableSeven();
            publ.debugOnlySetData(data);

            SimpleBuffer buffer = new SimpleBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);
            
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }

        static void TestParseStateSync(IPlatformSupport platform, String fileName)
        {
            mtest.publishable_seven_subscriber subs = new mtest.publishable_seven_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = platform.makePublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            //mtest.publishable_seven actual = (mtest.publishable_seven)subs.debugOnlyGetData();
            mtest.publishable_seven expected = GetPublishableSeven();

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestComposeUpdate(IPlatformSupport platform, String fileName, Action<mtest.Ipublishable_seven> updateDelegate)
        {
            mtest.publishable_seven_publisher publ = new mtest.publishable_seven_publisher();
            mtest.publishable_seven data = GetPublishableSeven();
            publ.debugOnlySetData(data);

            BufferT buffer = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buffer);

            publ.startTick(composer);

            updateDelegate(publ);

            publ.endTick();

            // uncomment to update files
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }
        static void TestParseUpdate(IPlatformSupport platform, String fileName, Action<mtest.Ipublishable_seven> updateDelegate)
        {
            mtest.publishable_seven_subscriber subs = new mtest.publishable_seven_subscriber();
            subs.debugOnlySetData(GetPublishableSeven());

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = platform.makePublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            //mtest.publishable_seven actual = (mtest.publishable_seven)subs.debugOnlyGetData();
            mtest.publishable_seven expected = GetPublishableSeven();

            Assert.False(expected.isEquivalent(subs));

            updateDelegate(expected);

            Assert.True(expected.isEquivalent(subs));
        }

        static void doUpdate1(mtest.Ipublishable_seven data)
        {
            //modify substructure inside vector
            data.structVec[0].y = 505;
        }

        static void doUpdate2(mtest.Ipublishable_seven data)
        {
            //modify existing elements
            data.intVec[0] = 303;
            data.uintVec[1] = 404;
            data.realVec[2] = 505;
            data.strVec[1] = "four";

            mtest.Ipoint3D e1 = new mtest.point3D();
            e1.x = 901;
            e1.y = 902;
            e1.z = 903;
            data.structVec[0] = e1;
        }

        static void doUpdate3(mtest.Ipublishable_seven data)
        {
            //replace complete vector
            IList<String> strVec = new List<String>();
            strVec.Add("four");
            strVec.Add("five");
            data.strVec = strVec;

            data.realVec = new List<Double>();

            IList<mtest.Ipoint3D> vec = new List<mtest.Ipoint3D>();
            mtest.Ipoint3D e1 = new mtest.point3D();
            e1.x = 301;
            e1.y = 302;
            e1.z = 303;
            vec.Add(e1);
            data.structVec = vec;
        }
        static void doUpdate4(mtest.Ipublishable_seven data)
        {
            //erase elements in vector
            data.intVec.RemoveAt(0);
            data.realVec.RemoveAt(1);
            data.structVec.RemoveAt(1);
        }

        static void doUpdate5(mtest.Ipublishable_seven data)
        {
            //insert elements in vector
            data.intVec.Insert(0, 77);
            data.uintVec.Insert(1, 88);
            data.realVec.Insert(2, 99);
            data.strVec.Insert(3, "last");

            mtest.Ipoint3D e1 = new mtest.point3D();
            e1.x = 301;
            e1.y = 302;
            e1.z = 303;
            data.structVec.Insert(1, e1);
        }

        [Fact]
        public static void TestJsonComposeStateSync()
        {
            TestComposeStateSync(JsonFactory, JsonPath);
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            TestParseStateSync(JsonFactory, JsonPath);
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestComposeUpdate(JsonFactory, JsonPath1, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestParseUpdate(JsonFactory, JsonPath1, doUpdate1);
        }


        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestComposeUpdate(JsonFactory, JsonPath2, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestParseUpdate(JsonFactory, JsonPath2, doUpdate2);
        }

        [Fact]
        public static void TestJsonComposeUpdate3()
        {
            TestComposeUpdate(JsonFactory, JsonPath3, doUpdate3);
        }

        [Fact]
        public static void TestJsonParseUpdate3()
        {
            TestParseUpdate(JsonFactory, JsonPath3, doUpdate3);
        }

        [Fact]
        public static void TestJsonComposeUpdate4()
        {
            TestComposeUpdate(JsonFactory, JsonPath4, doUpdate4);
        }

        [Fact]
        public static void TestJsonParseUpdate4()
        {
            TestParseUpdate(JsonFactory, JsonPath4, doUpdate4);
        }
        [Fact]
        public static void TestJsonComposeUpdate5()
        {
            TestComposeUpdate(JsonFactory, JsonPath5, doUpdate5);
        }

        [Fact]
        public static void TestJsonParseUpdate5()
        {
            TestParseUpdate(JsonFactory, JsonPath5, doUpdate5);
        }


        ///////////////////// gmq //////////////////
        [Fact]
        public static void TestGmqComposeStateSync()
        {
            TestComposeStateSync(GmqFactory, GmqPath);
        }

        [Fact]
        public static void TestGmqParseStateSync()
        {
            TestParseStateSync(GmqFactory, GmqPath);
        }

        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            TestComposeUpdate(GmqFactory, GmqPath1, doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            TestParseUpdate(GmqFactory, GmqPath1, doUpdate1);
        }

        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            TestComposeUpdate(GmqFactory, GmqPath2, doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            TestParseUpdate(GmqFactory, GmqPath2, doUpdate2);
        }
        [Fact]
        public static void TestGmqComposeUpdate3()
        {
            TestComposeUpdate(GmqFactory, GmqPath3, doUpdate3);
        }

        [Fact]
        public static void TestGmqParseUpdate3()
        {
            TestParseUpdate(GmqFactory, GmqPath3, doUpdate3);
        }

        [Fact]
        public static void TestGmqComposeUpdate4()
        {
            TestComposeUpdate(GmqFactory, GmqPath4, doUpdate4);
        }

        [Fact]
        public static void TestGmqParseUpdate4()
        {
            TestParseUpdate(GmqFactory, GmqPath4, doUpdate4);
        }
        [Fact]
        public static void TestGmqComposeUpdate5()
        {
            TestComposeUpdate(GmqFactory, GmqPath5, doUpdate5);
        }

        [Fact]
        public static void TestGmqParseUpdate5()
        {
            TestParseUpdate(GmqFactory, GmqPath5, doUpdate5);
        }

    }

}
