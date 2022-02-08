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

    public class TestUnitGmqPathHelper
    {
        static void checkParser(GmqPathHelper.PathComponents pc, String path, PublishableStateMessageHeader.MsgType msgType)
        {
            GmqPathHelper.PathComponents result = new GmqPathHelper.PathComponents();
            result.type = msgType;
            Assert.True(GmqPathHelper.parse(path, result));

            Assert.Equal(pc, result);

        }


        [Fact]
        public static void TestException1()
        {
            Assert.Throws<ArgumentException>(() => { GmqPathHelper.compose(null); });
        }
        [Fact]
        public static void TestException2()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();

            Assert.Throws<ArgumentException>(() => { GmqPathHelper.compose(pc); });
        }

        [Fact]
        public static void TestException3()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.undefined;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";

            Assert.Throws<ArgumentException>(() => { GmqPathHelper.compose(pc); });
        }
        [Fact]
        public static void TestException4()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "";

            Assert.Throws<ArgumentException>(() => { GmqPathHelper.compose(pc); });
        }
        [Fact]
        public static void TestException5()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "";
            pc.statePublisherOrConnectionType = "StructSix";

            Assert.Throws<ArgumentException>(() => { GmqPathHelper.compose(pc); });
        }
        [Fact]
        public static void Test10()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";
            
            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq:/test_node?sp=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.subscriptionRequest);
        }
        [Fact]
        public static void Test11()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.connectionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";

            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq:/test_node?ct=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.connectionRequest);
        }
        [Fact]
        public static void Test12()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";
            pc.authority = "authority";

            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq://authority/test_node?sp=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.subscriptionRequest);
        }
        [Fact]
        public static void Test13()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";
            pc.authority = "authority";
            pc.furtherResolution = true;

            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq://authority!gmq/test_node?sp=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.subscriptionRequest);
        }
        [Fact]
        public static void Test14()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";
            pc.authority = "authority";
            pc.hasPort = true;
            pc.port = 8080;

            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq://authority:8080/test_node?sp=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.subscriptionRequest);
        }
        [Fact]
        public static void Test15()
        {
            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_node";
            pc.statePublisherOrConnectionType = "StructSix";
            pc.authority = "authority";
            pc.furtherResolution = true;
            pc.hasPort = true;
            pc.port = 8080;

            string path = GmqPathHelper.compose(pc);

            Assert.Equal("globalmq://authority!gmq:8080/test_node?sp=StructSix", path);

            checkParser(pc, path, PublishableStateMessageHeader.MsgType.subscriptionRequest);
        }
    }

}
