using globalmq.marshalling;
using System;
using Xunit;



namespace TestProject1
{
    //using generated classes

    public class TestMessageGenerated
    {

        public static mtest.test_gmq.message_one GetSampleData()
        {
            mtest.test_gmq.message_one m = new mtest.test_gmq.message_one();
            m.firstParam = 1;
            m.secondParam = new Int64[] { 0, 1, 2, 3, 4, 5 };
            m.thirdParam = new mtest.point3D[] { new mtest.point3D { x = 0, y = 1, z = 2 }, new mtest.point3D { x = 3, y = 4, z = 5 } };
            m.forthParam = 3;
            m.fifthParam = "def";
            m.sixthParam = new mtest.point[] { new mtest.point { x = 0, y = 1 }, new mtest.point { x = 2, y = 3 }, new mtest.point { x = 4, y = 5 } };
            m.seventhParam = 3.1416;
            m.eighthParam = new mtest.point { x = 175, y = 186 };
            m.ninethParam = new mtest.point3D { x = 123, y = 456, z = 789 };
            m.tenthParam = new Double[] { 0.1, 1.2, 2.3 };

            return m;
        }

        [Fact]
        public static void TestGmq()
        {
            mtest.test_gmq.message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.test_gmq.message_one.compose(composer, m);

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.test_gmq.message_one mback;

            mtest.test_gmq.message_one.parse(parser, out mback);

            Assert.Equal(m, mback);
        }

        [Fact]
        public static void TestJson()
        {
            mtest.test_gmq.message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.test_gmq.message_one.compose(composer, m);

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.test_gmq.message_one mback;

            mtest.test_gmq.message_one.parse(parser, out mback);

            Assert.Equal(m, mback);
        }
        [Fact]
        public static void TestGmqFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.gmq");
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.test_gmq.message_one m;
            mtest.test_gmq.message_one.parse(parser, out m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);

            mtest.test_gmq.message_one.compose(composer, m);

            Assert.Equal(buffer, buffer2);

        }

        [Fact]
        public static void TestJsonFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.json");
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.test_gmq.message_one m;
            mtest.test_gmq.message_one.parse(parser, out m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer2);

            mtest.test_gmq.message_one.compose(composer, m);

            Assert.Equal(buffer, buffer2);
        }

    }

}
