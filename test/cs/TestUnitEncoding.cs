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
* ON ANY Test OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/

using globalmq.marshalling;
using NUnit.Framework;
using System;
using System.Collections.Generic;


namespace TestProject1
{
    /// <summary>
    /// Tests to match Json parser/composer from C# to C++
    /// </summary>
    [TestFixture]
    public class TestUnitEncoding
    {

        [Test]
        [TestCase(0UL)]
        [TestCase(1UL)]
        [TestCase(2UL)]
        [TestCase(126UL)]
        [TestCase(127UL)]
        [TestCase(128UL)]
        [TestCase(UInt64.MaxValue)]
        public static void TestVlqEncoding(UInt64 val)
        {
            SimpleBuffer buffer = new SimpleBuffer();
            IntegralVlq.writeVlqIntegral(buffer, val);

            UInt64 result = IntegralVlq.readVlqIntegral(buffer.getReadIterator());

            Assert.AreEqual(val, result);
        }

        [Test]
        [TestCase(0L)]
        [TestCase(1L)]
        [TestCase(2L)]
        [TestCase(126L)]
        [TestCase(127L)]
        [TestCase(128L)]
        [TestCase(Int64.MaxValue)]
        [TestCase(Int64.MinValue)]
        [TestCase(-128L)]
        [TestCase(-127L)]
        [TestCase(-1L)]
        public static void TestZigZagEncoding(Int64 val)
        {
            UInt64 uns = ZigZag.encode(val);
            Int64 result = ZigZag.decode(uns);

            Assert.AreEqual(val, result);
        }

        [Test]
        [TestCase("hello")]
        [TestCase("\thi\r\nbye")]
        public static void TestAsciiAddressEncoding(String val)
        {
            UInt64[] baseAddr = new UInt64[1];
            UInt64[] addr = AsciiAddress.encode(baseAddr, val);
            int refIndex = 1;
            String result = AsciiAddress.decode(addr, ref refIndex);

            Assert.AreEqual(val, result);
        }


        [Test]
        [TestCase("hello\0")]
        public static void TestAsciiAddressEncodingFail(String val)
        {
            try
            {
                UInt64[] baseAddr = new UInt64[1];
                UInt64[] addr = AsciiAddress.encode(baseAddr, val);
            }
            catch(Exception)
            {
                return;
            }
            Assert.True(false);
        }
    }

}
