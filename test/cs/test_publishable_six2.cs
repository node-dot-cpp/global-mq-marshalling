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

    public class test_publishable_six2
    {
        private const string Path = "test_publishable_six2.gmq";
        private const string Path1 = "test_publishable_six2_update1.gmq";
        private const string Path2 = "test_publishable_six2_update2.gmq";
        private const string Path3 = "test_publishable_six2_update3.gmq";

        static IPlatformSupport gmqFactory = new DefaultGmqPlatformSupport();


        [Fact]
        public static void TestGmqComposeStateSync()
        {
            test_publishable_six.TestComposeStateSync(gmqFactory, Path);
        }

        [Fact]
        public static void TestGmqParseStateSync()
        {
            test_publishable_six.TestParseStateSync(gmqFactory, Path);
        }
        [Fact]
        public static void TestJsonComposeUpdate1()
        {
        }

        [Fact]
        public static void TestJsonParseUpdate1()
        {
            test_publishable_six.TestParseUpdate(gmqFactory, Path1, test_publishable_six.doUpdate1);
        }


        [Fact]
        public static void TestGmqComposeUpdate1()
        {
            test_publishable_six.TestComposeUpdate(gmqFactory, Path1, test_publishable_six.doUpdate1);
        }

        [Fact]
        public static void TestGmqParseUpdate1()
        {
            test_publishable_six.TestParseUpdate(gmqFactory, Path1, test_publishable_six.doUpdate1);
        }

        [Fact]
        public static void TestGmqComposeUpdate2()
        {
            test_publishable_six.TestComposeUpdate(gmqFactory, Path2, test_publishable_six.doUpdate2);
        }

        [Fact]
        public static void TestGmqParseUpdate2()
        {
            test_publishable_six.TestParseUpdate(gmqFactory, Path2, test_publishable_six.doUpdate2);
        }

        [Fact]
        public static void TestComposeNoChangeUpdate3()
        {
            test_publishable_six.TestComposeUpdate(gmqFactory, Path3, test_publishable_six.doNothing);
        }

        [Fact]
        public static void TestGmqParseNoChangeUpdate3()
        {
            test_publishable_six.TestParseUpdate(gmqFactory, Path3, null);
        }
    }

}
