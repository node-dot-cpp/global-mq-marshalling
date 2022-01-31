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

    public class TestUnitGmQueue
    {
        static void deliverAllMessages(MetaPool pool, BasicMtQueue<ThreadQueueItem> msgQueue)
        {
            ThreadQueueItem[] messages = new ThreadQueueItem[16];

            pool.postAllUpdates();

            int msgCnt = 0;
            while (true)
            {
                msgCnt = msgQueue.pop_front(messages, 15, false);
                if (msgCnt == 0)
                    break;
                for (int i = 0; i < msgCnt; ++i)
                {
                    pool.onMessage(messages[i].msg);
                }
            }

        }
        [Fact]
        public static void TestGmQueue1()
        {
            IPlatformSupport platform = new DefaultJsonPlatformSupport();

            GMQueue gmq = new GMQueue();
            gmq.initStateConcentratorFactory(new mtest.publishable.StateConcentratorFactory(), platform);
            gmq.setAuthority(String.Empty);

            MetaPool mp = new MetaPool();

            BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            GMQThreadQueueTransport transport = new GMQThreadQueueTransport(gmq, "myNode", msgQueue, 1);

            mp.setTransport(transport);
            mp.setPlatform(new DefaultJsonPlatformSupport());

            mtest.publishable.StructSix_impl data = test_publishable_six.GetPublishableSix();
            mtest.publishable.StructSix_publisher publ = new mtest.publishable.StructSix_publisher(data, platform.makePublishableComposer(null), new UInt64[] { });

            mp.add(publ);

            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.authority = "";
            pc.nodeName = "myNode";
            pc.statePublisherOrConnectionType = "StructSix";
            string path = GmqPathHelper.compose(pc);

            
            mtest.publishable.StructSix_impl data1 = new mtest.publishable.StructSix_impl();
            mtest.publishable.StructSix_subscriber subs1 = new mtest.publishable.StructSix_subscriber(data1);

            Assert.NotEqual(data, data1);

            mp.add(subs1);
            mp.subscribe(subs1, path);

            deliverAllMessages(mp, msgQueue);

            Assert.Equal(data, data1);

            mtest.publishable.StructSix_impl data2 = new mtest.publishable.StructSix_impl();
            mtest.publishable.StructSix_subscriber subs2 = new mtest.publishable.StructSix_subscriber(data2);

            mp.add(subs2);
            mp.subscribe(subs2, path);

            mtest.publishable.StructSix_impl data3 = new mtest.publishable.StructSix_impl();
            mtest.publishable.StructSix_subscriber subs3 = new mtest.publishable.StructSix_subscriber(data3);

            mp.add(subs3);
            mp.subscribe(subs3, path);

            deliverAllMessages(mp, msgQueue);

            Assert.Equal(data, data2);
            Assert.Equal(data, data3);

            test_publishable_six.doUpdate2(publ);

            Assert.NotEqual(data, data1);
            Assert.NotEqual(data, data2);
            Assert.NotEqual(data, data3);

            deliverAllMessages(mp, msgQueue);

            Assert.Equal(data, data1);
            Assert.Equal(data, data2);
            Assert.Equal(data, data3);
        }
    }
}
