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
        private const string JsonPath = "test_publishable_seven.json";
        private const string JsonPath1 = "test_publishable_seven_u1.json";
        private const string JsonPath2 = "test_publishable_seven_u2.json";
        private const string JsonPath3 = "test_publishable_seven_u3.json";
        private const string JsonPath4 = "test_publishable_seven_u4.json";
        private const string JsonPath5 = "test_publishable_seven_u5.json";

        private const string GmqPath = "test_publishable_seven.gmq";
        private const string GmqPath1 = "test_publishable_seven_u1.gmq";
        private const string GmqPath2 = "test_publishable_seven_u2.gmq";
        private const string GmqPath3 = "test_publishable_seven_u3.gmq";
        private const string GmqPath4 = "test_publishable_seven_u4.gmq";
        private const string GmqPath5 = "test_publishable_seven_u5.gmq";

        public static mtest.publishable.publishable_seven_impl GetPublishableSeven()
        {
            mtest.publishable.publishable_seven_impl data = new mtest.publishable.publishable_seven_impl();

            data.intVec = data.make_intVec();
            data.intVec.Add(1);
            data.intVec.Add(2);
            data.intVec.Add(3);

            data.uintVec = data.make_uintVec();
            data.uintVec.Add(4);
            data.uintVec.Add(5);
            data.uintVec.Add(6);

            data.realVec = data.make_realVec();
            data.realVec.Add(7);
            data.realVec.Add(8);
            data.realVec.Add(9);

            data.strVec = data.make_strVec();
            data.strVec.Add("one");
            data.strVec.Add("two");
            data.strVec.Add("three");

            data.structVec = data.make_structVec();

            mtest.publishable.point3D e1 = data.make_structVec_element();
            e1.x = 101;
            e1.y = 102;
            e1.z = 103;
            data.structVec.Add(e1);

            mtest.publishable.point3D e2 = data.make_structVec_element();
            e2.x = 201;
            e2.y = 202;
            e2.z = 203;
            data.structVec.Add(e2);

            return data;
        }

        [Fact]
        public static void TestJsonComposeStateSync()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, null, new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();
            JsonPublishableComposer composer = new JsonPublishableComposer();
            
            composer.startTick(buffer);
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            //buffer.writeToFile(JsonPath);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(JsonPath));
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            mtest.publishable.publishable_seven_impl data = new mtest.publishable.publishable_seven_impl();

            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(JsonPath);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            Assert.Equal(data, GetPublishableSeven());
        }
        static void TestJsonComposeUpdate(String fileName, Action<mtest.publishable.publishable_seven> updateDelegate)
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, new JsonPublishableComposer(), new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();

            publ.startTick(buffer);

            updateDelegate(publ);

            publ.endTick();

            // uncomment to update files
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }
        static void TestJsonParseUpdate(String fileName, Action<mtest.publishable.publishable_seven> updateDelegate)
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();
            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable.publishable_seven_impl data2 = GetPublishableSeven();

            Assert.NotEqual(data, data2);

            updateDelegate(data2);

            Assert.Equal(data, data2);
        }

        static void doUpdate1(mtest.publishable.publishable_seven data)
        {
            //modify substructure inside vector
            data.structVec[0].y = 505;
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            TestJsonComposeUpdate(JsonPath1, doUpdate1);
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            TestJsonParseUpdate(JsonPath1, doUpdate1);
        }

        static void doUpdate2(mtest.publishable.publishable_seven data)
        {
            //modify existing elements
            data.intVec[0] = 303;
            data.uintVec[1] = 404;
            data.realVec[2] = 505;
            data.strVec[1] = "four";

            mtest.publishable.point3D e1 = data.make_structVec_element();
            e1.x = 901;
            e1.y = 902;
            e1.z = 903;
            data.structVec[0] = e1;
        }

        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            TestJsonComposeUpdate(JsonPath2, doUpdate2);
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            TestJsonParseUpdate(JsonPath2, doUpdate2);
        }

        static void doUpdate3(mtest.publishable.publishable_seven data)
        {
            //replace complete vector
            IList<String> strVec = data.make_strVec();
            strVec.Add("four");
            strVec.Add("five");
            data.strVec = strVec;

            data.realVec = data.make_realVec();

            IList<mtest.publishable.point3D> vec = data.make_structVec();
            mtest.publishable.point3D e1 = data.make_structVec_element();
            e1.x = 301;
            e1.y = 302;
            e1.z = 303;
            vec.Add(e1);
            data.structVec = vec;
        }
        [Fact]
        public static void TestJsonComposeUpdate3()
        {
            TestJsonComposeUpdate(JsonPath3, doUpdate3);
        }

        [Fact]
        public static void TestJsonParseUpdate3()
        {
            TestJsonParseUpdate(JsonPath3, doUpdate3);
        }
        static void doUpdate4(mtest.publishable.publishable_seven data)
        {
            //erase elements in vector
            data.intVec.RemoveAt(0);
            data.realVec.RemoveAt(1);
            data.structVec.RemoveAt(1);
        }

        [Fact]
        public static void TestJsonComposeUpdate4()
        {
            TestJsonComposeUpdate(JsonPath4, doUpdate4);
        }

        [Fact]
        public static void TestJsonParseUpdate4()
        {
            TestJsonParseUpdate(JsonPath4, doUpdate4);
        }
        static void doUpdate5(mtest.publishable.publishable_seven data)
        {
            //insert elements in vector
            data.intVec.Insert(0, 77);
            data.uintVec.Insert(1, 88);
            data.realVec.Insert(2, 99);
            data.strVec.Insert(3, "last");

            mtest.publishable.point3D e1 = data.make_structVec_element();
            e1.x = 301;
            e1.y = 302;
            e1.z = 303;
            data.structVec.Insert(1, e1);
        }
        [Fact]
        public static void TestJsonComposeUpdate5()
        {
            TestJsonComposeUpdate(JsonPath5, doUpdate5);
        }

        [Fact]
        public static void TestJsonParseUpdate5()
        {
            TestJsonParseUpdate(JsonPath5, doUpdate5);
        }


        ///////////////////// gmq //////////////////
        [Fact]
        public static void TestGmqComposeStateSync()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, null, new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();
            GmqPublishableComposer composer = new GmqPublishableComposer();

            composer.startTick(buffer);
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            //buffer.writeToFile(GmqPath);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(GmqPath));
        }

        [Fact]
        public static void TestGmqParseStateSync()
        {
            mtest.publishable.publishable_seven_impl data = new mtest.publishable.publishable_seven_impl();

            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(GmqPath);
            GmqPublishableParser parser = new GmqPublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            Assert.Equal(data, GetPublishableSeven());
        }
        static void TestGmqComposeUpdate(String fileName, Action<mtest.publishable.publishable_seven> updateDelegate)
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, new GmqPublishableComposer(), new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();

            publ.startTick(buffer);

            updateDelegate(publ);

            publ.endTick();

            // uncomment to update file
            //buffer.writeToFile(fileName);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(fileName));
        }
        static void TestGmqParseUpdate(String fileName, Action<mtest.publishable.publishable_seven> updateDelegate)
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();
            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            GmqPublishableParser parser = new GmqPublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable.publishable_seven_impl data2 = GetPublishableSeven();

            Assert.NotEqual(data, data2);

            updateDelegate(data2);

            Assert.Equal(data, data2);
        }


        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            TestGmqComposeUpdate(GmqPath1, doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            TestGmqParseUpdate(GmqPath1, doUpdate1);
        }

        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            TestGmqComposeUpdate(GmqPath2, doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            TestGmqParseUpdate(GmqPath2, doUpdate2);
        }
        [Fact]
        public static void TestGmqComposeUpdate3()
        {
            TestGmqComposeUpdate(GmqPath3, doUpdate3);
        }

        [Fact]
        public static void TestGmqParseUpdate3()
        {
            TestGmqParseUpdate(GmqPath3, doUpdate3);
        }

        [Fact]
        public static void TestGmqComposeUpdate4()
        {
            TestGmqComposeUpdate(GmqPath4, doUpdate4);
        }

        [Fact]
        public static void TestGmqParseUpdate4()
        {
            TestGmqParseUpdate(GmqPath4, doUpdate4);
        }
        [Fact]
        public static void TestGmqComposeUpdate5()
        {
            TestGmqComposeUpdate(GmqPath5, doUpdate5);
        }

        [Fact]
        public static void TestGmqParseUpdate5()
        {
            TestGmqParseUpdate(GmqPath5, doUpdate5);
        }

    }

}
