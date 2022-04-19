/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
* 
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

    public class test_publishable_dictionary
    {
        private const string Prefix = TestCommon.DataPathPrefix + "publishable_cycle/";

        static bool WriteFiles = true;

        static mtest.publishable_cycle GetPublishableCycle_0()
        {
            return new mtest.publishable_cycle();
        }

        // replace full element in dictionary
        static void doUpdate1(mtest.Ipublishable_cycle data)
        {
            data.data.childs.Add(new mtest.Cicle_1());
            data.data.childs.Add(new mtest.Cicle_1());
            data.data.childs.Add(new mtest.Cicle_1());

            data.data.childs[1].child.childs.Add(new mtest.Cicle_1());
            data.data.childs[1].child.childs.Add(new mtest.Cicle_1());
            data.data.childs[1].child.childs.Add(new mtest.Cicle_1());

            data.data.childs[1].child.childs[1].child.childs.Add(new mtest.Cicle_1());
            data.data.childs[1].child.childs[1].child.childs.Add(new mtest.Cicle_1());
            data.data.childs[1].child.childs[1].child.childs.Add(new mtest.Cicle_1());

        }


        static void TestComposeStateSync(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_cycle> getState)
        {
            mtest.publishable_cycle_publisher publ = new mtest.publishable_cycle_publisher();

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

        static void TestParseStateSync(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_cycle> getState)
        {
            mtest.publishable_cycle_subscriber subs = new mtest.publishable_cycle_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());

            subs.applyStateSyncMessage(parser);

            mtest.publishable_cycle expected = getState();

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestStateSync(int id, Func<mtest.publishable_cycle> getState)
        {
            string fileName = Prefix + "state_sync_" + id.ToString();
            TestComposeStateSync(TestCommon.Protocol.Json, fileName + ".json", getState);
            TestParseStateSync(TestCommon.Protocol.Json, fileName + ".json", getState);

            TestComposeStateSync(TestCommon.Protocol.Gmq, fileName + ".gmq", getState);
            TestParseStateSync(TestCommon.Protocol.Gmq, fileName + ".gmq", getState);
        }

        static void TestComposeUpdate(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_cycle> getState, Action<mtest.publishable_cycle_publisher> updateDelegate)
        {
            mtest.publishable_cycle_publisher publ = new mtest.publishable_cycle_publisher();

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
        static void TestParseUpdate(TestCommon.Protocol proto, String fileName, Func<mtest.publishable_cycle> getState, Action<mtest.Ipublishable_cycle> updateDelegate)
        {
            mtest.publishable_cycle_subscriber subs = new mtest.publishable_cycle_subscriber();

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            IPublishableParser parser = TestCommon.makePublishableParser(proto, buffer.getReadIterator());

            subs.applyMessageWithUpdates(parser);

            mtest.publishable_cycle expected = getState();

            Assert.False(expected.isEquivalent(subs));

            updateDelegate(expected);

            Assert.True(expected.isEquivalent(subs));
        }

        static void TestUpdate(int id, Func<mtest.publishable_cycle> initState, Action<mtest.Ipublishable_cycle> updateDelegate)
        {
            string fileName = Prefix + "update_" + id.ToString();
            TestComposeUpdate(TestCommon.Protocol.Json, fileName + ".json", initState, updateDelegate);
            TestParseUpdate(TestCommon.Protocol.Json, fileName + ".json", initState, updateDelegate);

            TestComposeUpdate(TestCommon.Protocol.Gmq, fileName + ".gmq", initState, updateDelegate);
            TestParseUpdate(TestCommon.Protocol.Gmq, fileName + ".gmq", initState, updateDelegate);
        }


        [Fact]
        public static void TestStateSync0()
        {
            TestStateSync(0, GetPublishableCycle_0);
        }

        [Fact]
        public static void TestUpdate1()
        {
            TestUpdate(1, GetPublishableCycle_0, doUpdate1);
        }
    }
}
