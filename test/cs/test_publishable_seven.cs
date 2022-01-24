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
        private const string Path = "test_publishable_seven.json";
        private const string Path1 = "test_publishable_seven_u1.json";
        private const string Path2 = "test_publishable_seven_u2.json";
        private const string Path3 = "test_publishable_seven_u3.json";

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
            e1.x = 102;
            e1.x = 103;
            data.structVec.Add(e1);

            mtest.publishable.point3D e2 = data.make_structVec_element();
            e1.x = 201;
            e1.x = 202;
            e1.x = 303;
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
            //buffer.writeToFile(Path);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(Path));
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            mtest.publishable.publishable_seven_impl data = new mtest.publishable.publishable_seven_impl();

            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(Path);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            Assert.Equal(data, GetPublishableSeven());
        }

        [Fact]
        public static void TestJsonComposeUpdate1()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, new JsonPublishableComposer(), new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();
            //JsonPublishableComposer composer = new JsonPublishableComposer();

            //modify substructure inside vector
            publ.startTick(buffer);
            publ.structVec[0].y = 505;
            publ.endTick();

            // uncomment to update file
            //buffer.writeToFile(Path1);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(Path1));
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();
            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(Path1);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable.publishable_seven_impl data2 = GetPublishableSeven();

            Assert.NotEqual(data, data2);

            data2.structVec[0].y = 505;
            Assert.Equal(data, data2);
        }

        [Fact]
        public static void TestJsonComposeUpdate2()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

            mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, new JsonPublishableComposer(), new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();
            //JsonPublishableComposer composer = new JsonPublishableComposer();

            publ.startTick(buffer);

            publ.intVec[0] = 303;
            publ.uintVec[1] = 404;
            publ.realVec[2] = 505;
            publ.strVec[1] = "four";

            mtest.publishable.point3D e1 = publ.make_structVec_element();
            e1.x = 901;
            e1.x = 902;
            e1.x = 903;
            publ.structVec[0] = e1;


            publ.endTick();

            //uncomment to update file
            //buffer.writeToFile(Path2);

            Assert.Equal(buffer, SimpleBuffer.readFromFile(Path2));
        }

        [Fact]
        public static void TestJsonParseUpdate2()
        {
            mtest.publishable.publishable_seven_impl data = GetPublishableSeven();
            mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(Path2);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable.publishable_seven_impl data2 = GetPublishableSeven();

            Assert.NotEqual(data, data2);

            data2.intVec[0] = 303;
            data2.uintVec[1] = 404;
            data2.realVec[2] = 505;
            data2.strVec[1] = "four";

            mtest.publishable.point3D e1 = data2.make_structVec_element();
            e1.x = 901;
            e1.x = 902;
            e1.x = 903;
            data2.structVec[0] = e1;

            Assert.Equal(data, data2);
        }

        //[Fact]
        //public static void TestComposeNoChangeUpdate3()
        //{
        //    mtest.publishable.publishable_seven_impl data = GetPublishableSeven();

        //    mtest.publishable.publishable_seven_publ publ = new mtest.publishable.publishable_seven_publ(data, new JsonPublishableComposer(), new UInt64[] { });

        //    SimpleBuffer buffer = new SimpleBuffer();
        //    //JsonPublishableComposer composer = new JsonPublishableComposer();

        //    publ.startTick(buffer);
        //    // no change
        //    publ.endTick();

        //    // uncomment to update file
        //    //buffer.writeToFile(Path3);

        //    Assert.Equal(buffer, SimpleBuffer.readFromFile(Path3));
        //}

        //[Fact]
        //public static void TestJsonParseNoChangeUpdate3()
        //{
        //    mtest.publishable.publishable_seven_impl data = GetPublishableSeven();
        //    mtest.publishable.publishable_seven_subs subs = new mtest.publishable.publishable_seven_subs(data);

        //    SimpleBuffer buffer = SimpleBuffer.readFromFile(Path3);
        //    JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

        //    subs.applyMessageWithUpdates(parser);

        //    mtest.publishable.publishable_seven_impl data2 = GetPublishableSeven();

        //    Assert.Equal(data, data2);
        //}
    }

}
