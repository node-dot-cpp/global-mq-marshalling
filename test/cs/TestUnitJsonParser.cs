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
    /// <summary>
    /// Tests to match Json parser/composer from C# to C++
    /// </summary>
    public class TestUnitJsonParser
    {

        [Theory]
        [InlineData(" { \"addr\": [ 1, 2, 3],", new UInt64[] { 1, 2, 3 })]
        [InlineData(" { \"addr\": [ ],", new UInt64[] { })]
        public static void TestParseAddress(String asText, UInt64[] parsed)
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(asText);

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.True(parser.parseAddress(ref addr));
            Assert.Equal<UInt64>(parsed, addr);
        }


        [Fact]
        public static void TestParseEmptyAddress()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(" { }");

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.False(parser.parseAddress(ref addr));
        }

        [Theory]
        [InlineData(" ] }")]
        [InlineData(" { \"addr\": "
            )]
        public static void TestParseFail(String asText)
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(asText);

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.Throws<Exception>(() => { parser.parseAddress(ref addr); });
        }


        //mb: TODO add significant values
        [Theory]
        [InlineData(0, "0 ")]
        [InlineData(1, "1 ")]
        [InlineData(1.1, "1.1 ")]
        [InlineData(1.1e4, "11000 ")]
        [InlineData(-1.1e4, "-11000 ")]
        [InlineData(-1.1e-4, "-0.00011 ")]

        public static void TestParseFloat(double value, String asText)
        {
            SimpleBuffer buffer = new SimpleBuffer();

            JsonComposer composer = new JsonComposer(buffer);
            composer.composeReal(value);
            composer.append(' ');

            Assert.Equal(asText, buffer.toDebugString());

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            double res;
            parser.parseReal(out res);

            Assert.Equal(value, res);
        }

        [Theory]
        [InlineData("hola mundo!", "\"hola mundo!\"")]
        [InlineData("hola \"mundo\"!", "\"hola \\\"mundo\\\"!\"")]
        [InlineData("hola\r\n\tmundo!", "\"hola\\r\\n\\tmundo!\"")]
        [InlineData("hola\\mundo!", "\"hola\\\\mundo!\"")]
        public static void TestEscapeString(String value, String escaped)
        {
            SimpleBuffer buffer = new SimpleBuffer();

            JsonComposer composer = new JsonComposer(buffer);
            composer.composeString(value);

            Assert.Equal(escaped, buffer.toDebugString());

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            String res;
            parser.parseString(out res);

            Assert.Equal(value, res);
        }
    }

}
