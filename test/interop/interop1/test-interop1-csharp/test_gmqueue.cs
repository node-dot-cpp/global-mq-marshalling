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

    public class test_gmqueue
    {
        static void deliverAllMessages(MetaPool pool, BasicMtQueue<ThreadQueueItem> msgQueue, String filePrefix, ref int msgCnt)
        {
            ThreadQueueItem[] messages = new ThreadQueueItem[16];

            while (true)
            {
                int popped = msgQueue.pop_front(messages, 15, false);
                if (popped == 0)
                    break;
                for (int i = 0; i < popped; ++i)
                {
                    pool.onMessage(messages[i].msg);
                    ++msgCnt;
                    if (filePrefix != null)
                    {
                        SimpleBuffer buffer = (SimpleBuffer)messages[i].msg;
                        String fileName = TestCommon.DataPathPrefix + filePrefix + msgCnt + ".json";

                        // uncomment to update file
                        //buffer.writeToFile(fileName);

                        SimpleBuffer expected = SimpleBuffer.readFromFile(fileName);
                        Assert.True(SimpleBuffer.AreEqualIgnoreEol(expected, buffer));
                    }
                }
            }

        }

        static MetaPool initializeGmQueue(BasicMtQueue<ThreadQueueItem> msgQueue)
        {
            IPlatformSupport platform = new DefaultJsonPlatformSupport();

            GMQueue gmq = new GMQueue();
            gmq.initStateConcentratorFactory(new mtest.StateConcentratorFactory(), platform);
            gmq.setAuthority(String.Empty);


            //BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            BasicQueuePostman postMan = new BasicQueuePostman(msgQueue, 1);

            SlotIdx idx_;
            UInt64 id_ = gmq.add("test_node", postMan, out idx_);
            GMQTransportBase transport = new GMQTransportBase(gmq, "test_node", idx_, id_);

            MetaPool mp = new MetaPool();

            mp.setTransport(transport);
            mp.setPlatform(platform);

            return mp;
        }

        static String getSubscriptionAddress(String publisher)
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = publisher;
            string path = GmqPathHelper.compose(pc);

            return path;
        }

        [Fact]
        public static void TestGmQueueWithStructSix()
        {
            BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            MetaPool mp = initializeGmQueue(msgQueue);

            mtest.StructSix_publisher publ = new mtest.StructSix_publisher();

            mtest.StructSix data = test_publishable_six.GetPublishableSix();
            publ.debugOnlySetData(data);

            mp.add(publ);

            string path = getSubscriptionAddress("StructSix");
            
            mtest.StructSix_subscriber subs1 = new mtest.StructSix_subscriber();

            Assert.False(publ.isEquivalent(subs1));

            mp.add(subs1);
            mp.subscribe(subs1, path);

            int msgCnt = 0;
            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, null, ref msgCnt);

            Assert.True(publ.isEquivalent(subs1));

            mtest.StructSix_subscriber subs2 = new mtest.StructSix_subscriber();
            //mtest.StructSix data2 = (mtest.StructSix)subs2.debugOnlyGetData();

            mp.add(subs2);
            mp.subscribe(subs2, path);

            mtest.StructSix_subscriber subs3 = new mtest.StructSix_subscriber();
            //mtest.StructSix data3 = (mtest.StructSix)subs3.debugOnlyGetData();

            mp.add(subs3);
            mp.subscribe(subs3, path);

            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, null, ref msgCnt);

            Assert.True(publ.isEquivalent(subs2));
            Assert.True(publ.isEquivalent(subs3));

            test_publishable_six.doUpdate1(publ);

            Assert.False(publ.isEquivalent(subs1));
            Assert.False(publ.isEquivalent(subs2));
            Assert.False(publ.isEquivalent(subs3));

            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, null, ref msgCnt);

            Assert.True(publ.isEquivalent(subs1));
            Assert.True(publ.isEquivalent(subs2));
            Assert.True(publ.isEquivalent(subs3));
        }

        [Fact]
        public static void TestGmQueueWithMock1()
        {
            BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            MetaPool mp = initializeGmQueue(msgQueue);

            mtest.Mock_publisher publ = new mtest.Mock_publisher();

            mp.add(publ);

            string path = getSubscriptionAddress("Mock");

            mtest.Mock_subscriber subs1 = new mtest.Mock_subscriber();

            mp.add(subs1);
            mp.subscribe(subs1, path);

            int msgCnt = 0;
            //mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue1_", ref msgCnt);

            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue1_", ref msgCnt);
        }

        [Fact]
        public static void TestGmQueueWithMock2()
        {
            BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            MetaPool mp = initializeGmQueue(msgQueue);

            mtest.Mock_publisher publ = new mtest.Mock_publisher();

            mp.add(publ);

            string path = getSubscriptionAddress("Mock");

            mtest.Mock_subscriber subs1 = new mtest.Mock_subscriber();

            mp.add(subs1);
            mp.subscribe(subs1, path);

            mtest.Mock_subscriber subs2 = new mtest.Mock_subscriber();

            mp.add(subs2);
            mp.subscribe(subs2, path);

            int msgCnt = 0;
            //mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue2_", ref msgCnt);

            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue2_", ref msgCnt);
        }

        [Fact]
        public static void TestGmQueueWithMock3()
        {
            BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            MetaPool mp = initializeGmQueue(msgQueue);

            mtest.Mock_publisher publ = new mtest.Mock_publisher();

            mp.add(publ);
            mp.remove(publ);
            mp.add(publ);

            string path = getSubscriptionAddress("Mock");

            mtest.Mock_subscriber subs1 = new mtest.Mock_subscriber();

            mp.add(subs1);
            mp.remove(subs1);
            mp.add(subs1);
            mp.subscribe(subs1, path);

            int msgCnt = 0;
            //mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue3_", ref msgCnt);

            mp.postAllUpdates();
            deliverAllMessages(mp, msgQueue, "test_gmqueue3_", ref msgCnt);
        }


    }
}
