using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace TestProject1
{
    //public class UnitTest1
    //{
    //    [Fact]
    //    public void Test1()
    //    {

    //    }
    //}

    public class TestMessageCustom
    {
        public struct Point
        {
            public Int64 x;
            public Int64 y;
        }

        public struct Point3D
        {
            public Int64 x;
            public Int64 y;
            public Int64 z;
        }
        public class message_one
        {
            public long one;
            public string five;
            public UInt64 four;
            public Double seven;
            public int[] two;
            public List<double> ten;
            public Point eight;
            public Point3D nine;
            public Point[] six;
            public List<Point3D> three;
        }
        public static void parse(ParserBase parser, message_one msg)
        {
            msg.three = new List<Point3D>();
            msg.ten = new List<Double>();

            mtest.test_gmq.message_one.parse(parser,
                firstParam: ref msg.one, forthParam: ref msg.four,
                secondParam: new CollectionWrapperForParsing((int size) => { msg.two = new int[size]; }, (ParserBase parser, int ordinal) => { parser.parseSignedInteger(out msg.two[ordinal]); }),
                tenthParam: SimpleTypeCollection.makeParser(msg.ten),
                thirdParam: new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Point3D pt = new Point3D(); mtest.point3D.parse(parser, x: ref pt.x, y: ref pt.y, z: ref pt.z); msg.three.Add(pt); }),
                sixthParam: new CollectionWrapperForParsing((int size) => { msg.six = new Point[size]; }, (ParserBase parser, int ordinal) => { Point pt = new Point(); mtest.point.parse(parser, x: ref pt.x, y: ref pt.y); msg.six[ordinal] = pt; }),
                eighthParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.point.parse(parser, x: ref msg.eight.x, y: ref msg.eight.y); }),
                ninethParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.point3D.parse(parser, x: ref msg.nine.x, y: ref msg.nine.y, z: ref msg.nine.z); }),
                fifthParam: ref msg.five, seventhParam: ref msg.seven
                );
        }

        public static void compose(ComposerBase composer, message_one msg)
        {
            mtest.test_gmq.message_one.compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.three.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.three[ordinal].x, y: msg.three[ordinal].y, z: msg.three[ordinal].z); }),
                firstParam: msg.one, fifthParam: msg.five, forthParam: msg.four, seventhParam: msg.seven,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eight.x, y: msg.eight.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.nine.x, y: msg.nine.y, z: msg.nine.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.two),
                tenthParam: SimpleTypeCollection.makeComposer(msg.ten),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.six.Length; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.six[ordinal].x, y: msg.six[ordinal].y); })
            );
        }

        public static void setSampleData(message_one msg)
        {
            msg.one = 1;
            msg.five = "def";
            msg.four = 3;
            msg.seven = 3.1416;
            msg.two = new int[] { 0, 1, 2, 3, 4, 5 };
            msg.ten = new List<double> { 0.1, 1.2, 2.3 };
            msg.eight = new Point { x = 175, y = 186 };
            msg.nine = new Point3D { x = 123, y = 456, z = 789 };
            msg.six = new Point[] { new Point { x = 0, y = 1 }, new Point { x = 2, y = 3 }, new Point { x = 4, y = 5 } };
            msg.three = new System.Collections.Generic.List<Point3D> { new Point3D { x = 0, y = 1, z = 2 }, new Point3D { x = 3, y = 4, z = 5 } };
        }

        public static bool AreEqual(message_one l, message_one r)
        {
            return l.one == r.one &&
                   Enumerable.SequenceEqual(l.two, r.two) &&
                   Enumerable.SequenceEqual(l.three, r.three) &&
                   l.four == r.four &&
                   l.five == r.five &&
                   Enumerable.SequenceEqual(l.six, r.six) &&
                   l.seven == r.seven &&
                   l.eight.Equals(r.eight) &&
                   l.nine.Equals(r.nine) &&
                   Enumerable.SequenceEqual(l.ten, r.ten);
        }

        [Fact]
        public static void TestGmq()
        {
            message_one m = new message_one();
            setSampleData(m);

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            compose(composer, m);

            //buffer.writeToFile("message_one.gmq");

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            message_one mback = new message_one();
            parse(parser, mback);

            Assert.True(AreEqual(m, mback));
        }

        [Fact]
        public static void TestJson()
        {

            message_one m = new message_one();
            setSampleData(m);

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            compose(composer, m);

            //buffer.writeToFile("message_one.json");

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            message_one mback = new message_one();
            parse(parser, mback);

            Assert.True(AreEqual(m, mback));
        }

        [Fact]
        public static void TestGmqFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.gmq");
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            message_one m = new message_one();
            parse(parser, m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);

            compose(composer, m);

            Assert.Equal(buffer, buffer2);

        }

        [Fact]
        public static void TestJsonFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.json");
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            message_one m = new message_one();
            parse(parser, m);

            SimpleBuffer buffer2 = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer2);

            compose(composer, m);

            Assert.Equal(buffer, buffer2);
        }

    }

}
