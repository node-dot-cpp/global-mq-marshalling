using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;


namespace TestProject1
{
    //using generated classes

    public class TestMessageGenerated
    {

        public static mtest.message_one GetSampleData()
        {
            mtest.message_one m = new mtest.message_one();
            m.firstParam = 1;
            m.secondParam = new Int64[] { 0, 1, 2, 3, 4, 5 };
            m.thirdParam = new mtest.Point3D[] { new mtest.Point3D { x = 0, y = 1, z = 2 }, new mtest.Point3D { x = 3, y = 4, z = 5 } };
            m.forthParam = 3;
            m.fifthParam = "def";
            m.sixthParam = new mtest.Point[] { new mtest.Point { x = 0, y = 1 }, new mtest.Point { x = 2, y = 3 }, new mtest.Point { x = 4, y = 5 } };
            m.seventhParam = 3.1416;
            m.eighthParam = new mtest.Point { x = 175, y = 186 };
            m.ninethParam = new mtest.Point3D { x = 123, y = 456, z = 789 };
            m.tenthParam = new Double[] { 0.1, 1.2, 2.3 };

            return m;
        }

        [Fact]
        public static void TestGmq()
        {
            mtest.message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.MESSAGE_message_one_compose(composer, m);

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.message_one mback = new mtest.message_one();

            mtest.MESSAGE_message_one_parse(parser, mback);

            Assert.Equal(m, mback);
        }

        [Fact]
        public static void TestJson()
        {
            mtest.message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.MESSAGE_message_one_compose(composer, m);

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.message_one mback = new mtest.message_one();

            mtest.MESSAGE_message_one_parse(parser, mback);

            Assert.Equal(m, mback);
        }
        [Fact]
        public static void TestGmqFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.gmq");
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.message_one m = new mtest.message_one();
            mtest.MESSAGE_message_one_parse(parser, m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);

            mtest.MESSAGE_message_one_compose(composer, m);

            Assert.Equal(buffer, buffer2);

        }

        [Fact]
        public static void TestJsonFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.json");
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.message_one m = new mtest.message_one();
            mtest.MESSAGE_message_one_parse(parser, m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer2);

            mtest.MESSAGE_message_one_compose(composer, m);

            Assert.Equal(buffer, buffer2);
        }

    }

}
