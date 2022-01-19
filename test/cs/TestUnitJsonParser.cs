using globalmq.marshalling;
using System;
using System.Collections.Generic;
using Xunit;

namespace TestProject1
{

    public class TestUnitJsonParser
    {
        [Fact]
        public static void TestParseAddress1()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(" \"addr\": [ 1, 2, 3],");

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.True(parser.parseAddress(ref addr));
            Assert.Equal<UInt64>(addr, new UInt64[]{ 1, 2, 3});
        }

        [Fact]
        public static void TestParseAddress2()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(" { } ] }");

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.False(parser.parseAddress(ref addr));
        }
        [Fact]
        public static void TestParseAddress3()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(" ] }");

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.False(parser.parseAddress(ref addr));
        }

        [Fact]
        public static void TestParseAddress4()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            buffer.appendAscii(" \"addr\": [ ],");

            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            UInt64[] addr = null;
            Assert.True(parser.parseAddress(ref addr));
            Assert.Equal<UInt64>(addr, new UInt64[] { });
        }
    }

}
