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

    public class TestPublishableStructSix
    {
        public static mtest.publishable.StructSix_impl GetSampleData()
        {
            mtest.publishable.StructSix_impl data = new mtest.publishable.StructSix_impl();

            data.name = "TheName";
            
            data.basic = data.make_basic();
            data.basic.anInt = -100000;
            data.basic.anUInt = 100000;
            data.basic.aReal = 3.14;
            data.basic.aString = "basic string";

            data.aggregate = data.make_aggregate();
            data.aggregate.name = "aggregate name";

            data.aggregate.theAggregate = data.aggregate.make_theAggregate();
            data.aggregate.theAggregate.anInt = -100;
            data.aggregate.theAggregate.anUInt = 100;
            data.aggregate.theAggregate.aReal = 100;
            data.aggregate.theAggregate.aString = "basic string inside aggregate";
            data.aggregate.lastValue = 0;

            return data;
        }

        [Fact]
        public static void TestJsonComposeStateSync()
        {
            mtest.publishable.StructSix_impl data = GetSampleData();

            mtest.publishable.StructSix_publ publ = new mtest.publishable.StructSix_publ(data, null, new UInt64[] { });

            SimpleBuffer buffer = new SimpleBuffer();
            JsonPublishableComposer composer = new JsonPublishableComposer();
            
            composer.startTick(buffer);
            publ.generateStateSyncMessage(composer);

            // uncomment to update file
            //buffer.writeToFile("publishable_struct_six.json");

            Assert.Equal(buffer, SimpleBuffer.readFromFile("publishable_struct_six.json"));
        }

        [Fact]
        public static void TestJsonParseStateSync()
        {
            mtest.publishable.StructSix_impl data = new mtest.publishable.StructSix_impl();

            mtest.publishable.StructSix_subs subs = new mtest.publishable.StructSix_subs(data);

            SimpleBuffer buffer = SimpleBuffer.readFromFile("publishable_struct_six.json");
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            // TODO add object Equals
            Assert.Equal(subs.aggregate.theAggregate.aString, data.aggregate.theAggregate.aString);
        }

        //[Fact]
        //public static void TestGmqParse()
        //{
        //    SimpleBuffer buffer = SimpleBuffer.readFromFile("struct_one.gmq");
        //    GmqParser parser = new GmqParser(buffer.getReadIterator());

        //    mtest.struct_one msg = mtest.struct_one.parse(parser);

        //    Assert.Equal(msg, GetSampleData());
        //}

        //[Fact]
        //public static void TestJsonParse()
        //{
        //    SimpleBuffer buffer = SimpleBuffer.readFromFile("struct_one.json");
        //    JsonParser parser = new JsonParser(buffer.getReadIterator());

        //    mtest.struct_one msg = mtest.struct_one.parse(parser);

        //    Assert.Equal(msg, GetSampleData());
        //}
        //[Fact]
        //public static void TestGmqComposeAndParse()
        //{
        //    mtest.struct_one msg = GetSampleData();

        //    SimpleBuffer buffer = new SimpleBuffer();
        //    GmqComposer composer = new GmqComposer(buffer);

        //    ComposeStructOne(composer, msg);

        //    GmqParser parser = new GmqParser(buffer.getReadIterator());

        //    mtest.struct_one msg2 = mtest.struct_one.parse(parser);

        //    Assert.Equal(msg, msg2);
        //}

        //[Fact]
        //public static void TestJsonComposeAndParse()
        //{
        //    mtest.struct_one msg = GetSampleData();

        //    SimpleBuffer buffer = new SimpleBuffer();
        //    JsonComposer composer = new JsonComposer(buffer);

        //    ComposeStructOne(composer, msg);

        //    JsonParser parser = new JsonParser(buffer.getReadIterator());

        //    mtest.struct_one msg2 = mtest.struct_one.parse(parser);

        //    Assert.Equal(msg, msg2);
        //}

    }

}
