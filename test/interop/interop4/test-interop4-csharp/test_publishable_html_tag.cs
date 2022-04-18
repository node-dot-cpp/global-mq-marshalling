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

namespace test_interop4_csharp
{

    public class test_publishable_html_tag
    {
        private const string Prefix = TestCommon.DataPathPrefix + "publishable_html_tag/";

        static bool WriteFiles = false;

        static void doUpdate0(mtest.Ipublishable_html_tag data)
        {
        }


        // replace full element in dictionary
        static void doUpdate1(mtest.Ipublishable_html_tag data)
        {
            data.data.name = "first";
            data.data.tags.setCurrentVariant(mtest.HtmlTextOrTags_variants.taglists);
            data.data.tags.tags.Add(new mtest.HtmlTag());
            data.data.tags.tags[0].name = "second";
        }


        static void TestParseComposeStateSync(TestCommon.Protocol proto, String fileName, Action<mtest.Ipublishable_html_tag> updateDelegate)
        {
            mtest.publishable_html_tag_publisher publ = new mtest.publishable_html_tag_publisher();

            SimpleBuffer bufferTmp = new SimpleBuffer();
            IPublishableComposer composerTmp = TestCommon.makePublishableComposer(proto, bufferTmp);

            publ.startTick(composerTmp);

            updateDelegate(publ);

            publ.endTick();

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


            mtest.publishable_html_tag_subscriber subs = new mtest.publishable_html_tag_subscriber();

            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);
            Assert.True(publ.isEquivalent(subs));
        }

        static void TestStateSync(int id, Action<mtest.Ipublishable_html_tag> updateDelegate)
        {
            string fileName = Prefix + "state_sync_" + id.ToString();
            TestParseComposeStateSync(TestCommon.Protocol.Json, fileName + ".json", updateDelegate);
            TestParseComposeStateSync(TestCommon.Protocol.Gmq, fileName + ".gmq", updateDelegate);
        }

        static void TestParseComposeUpdate(TestCommon.Protocol proto, String fileNameInit, String fileName, Action<mtest.Ipublishable_html_tag> updateDelegate)
        {
            mtest.publishable_html_tag_publisher publ = new mtest.publishable_html_tag_publisher();

            SimpleBuffer bufferInit = SimpleBuffer.readFromFile(fileNameInit);
            IPublishableParser parserInit = TestCommon.makePublishableParser(proto, bufferInit.getReadIterator());

            publ.applyStateSyncMessage(parserInit);


            mtest.publishable_html_tag_subscriber subs = new mtest.publishable_html_tag_subscriber();


            SimpleBuffer bufferInit2 = SimpleBuffer.readFromFile(fileNameInit);
            IPublishableParser parserInit2 = TestCommon.makePublishableParser(proto, bufferInit2.getReadIterator());

            subs.applyStateSyncMessage(parserInit2);

            // init state at publ and subs are equal
            Assert.True(publ.isEquivalent(subs));


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

            // now state is different

            Assert.False(publ.isEquivalent(subs));
            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());
            subs.applyMessageWithUpdates(parser);

            // now state is equal
            Assert.True(publ.isEquivalent(subs));
        }
        static void TestUpdate(int initId, int id, Action<mtest.Ipublishable_html_tag> updateDelegate)
        {
            string fileNameInit = Prefix + "state_sync_" + initId.ToString();
            string fileName = Prefix + "update_" + id.ToString();
            TestParseComposeUpdate(TestCommon.Protocol.Json, fileNameInit + ".json", fileName + ".json", updateDelegate);
            TestParseComposeUpdate(TestCommon.Protocol.Gmq, fileNameInit + ".gmq", fileName + ".gmq", updateDelegate);
        }


        [Fact]
        public static void TestStateSync0()
        {
            TestStateSync(0, doUpdate0);
        }

        [Fact]
        public static void TestStateSync1()
        {
            TestStateSync(1, doUpdate1);
        }

        [Fact]
        public static void TestUpdate1()
        {
            TestUpdate(0, 1, doUpdate1);
        }
    }
}
