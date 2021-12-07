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

    public class UnitTest1
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

            public void parse(ParserBase parser)
            {
                three = new List<Point3D>();
                ten = new List<Double>();

                mtest.MESSAGE_message_one_parse(parser,
                    firstParam: ref one, forthParam: ref four,
                    secondParam: new CollectionWrapperForParsing((int size) => { two = new int[size]; }, (ParserBase parser, int ordinal) => { parser.parseSignedInteger(out two[ordinal]); }),
                    tenthParam: SimpleTypeCollection.makeParser(ten),
                    thirdParam: new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Point3D pt; mtest.STRUCT_point3D_parse(parser, x: out pt.x, y: out pt.y, z: out pt.z); three.Add(pt); }),
                    sixthParam: new CollectionWrapperForParsing((int size) => { six = new Point[size]; }, (ParserBase parser, int ordinal) => { Point pt; mtest.STRUCT_point_parse(parser, x: out pt.x, y: out pt.y); six[ordinal] = pt; }),
                    eighthParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point_parse(parser, x: out eight.x, y: out eight.y); }),
                    ninethParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point3D_parse(parser, x: out nine.x, y: out nine.y, z: out nine.z); }),
                    fifthParam: ref five, seventhParam: ref seven
                   );
            }

            public void compose(ComposerBase composer)
            {
                mtest.MESSAGE_message_one_compose(composer,
                    thirdParam: new CollectionWrapperForComposing(() => { return three.Count; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point3D_compose(composer, x: three[ordinal].x, y: three[ordinal].y, z: three[ordinal].z); }),
                    firstParam: one, fifthParam: five, forthParam: four, seventhParam: seven,
                    eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point_compose(composer, x: eight.x, y: eight.y); }),
                    ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point3D_compose(composer, x: nine.x, y: nine.y, z: nine.z); }),
                    secondParam: SimpleTypeCollection.makeComposer(two),
                    tenthParam: SimpleTypeCollection.makeComposer(ten),
                    sixthParam: new CollectionWrapperForComposing(() => { return six.Length; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point_compose(composer, x: six[ordinal].x, y: six[ordinal].y); })
                );
            }

            public void setSampleData()
            {
                one = 1;
                five = "def";
                four = 3;
                seven = 3.1416;
                two = new int[] { 0, 1, 2, 3, 4, 5 };
                ten = new List<double> { 0.1, 1.2, 2.3 };
                eight = new Point { x = 175, y = 186 };
                nine = new Point3D { x = 123, y = 456, z = 789 };
                six = new Point[] { new Point { x = 0, y = 1 }, new Point { x = 2, y = 3 }, new Point { x = 4, y = 5 } };
                three = new System.Collections.Generic.List<Point3D> { new Point3D { x = 0, y = 1, z = 2 }, new Point3D { x = 3, y = 4, z = 5 } };
            }

            public override bool Equals(object obj)
            {
                return obj is message_one msg &&
                       one == msg.one &&
                       Enumerable.SequenceEqual(two, msg.two) &&
                       Enumerable.SequenceEqual(three, msg.three) &&
                       four == msg.four &&
                       five == msg.five &&
                       Enumerable.SequenceEqual(six, msg.six) &&
                       seven == msg.seven &&
                       eight.Equals(msg.eight) &&
                       nine.Equals(msg.nine) &&
                       Enumerable.SequenceEqual(ten, msg.ten);
            }

        }

        public static message_one GetSampleData()
        {
            message_one m = new message_one();
            m.one = 1;
            m.five = "def";
            m.four = 3;
            m.seven = 3.1416;
            m.two = new int[] { 0, 1, 2, 3, 4, 5 };
            m.ten = new List<double> { 0.1, 1.2, 2.3 };
            m.eight = new Point { x = 175, y = 186 };
            m.nine = new Point3D { x = 123, y = 456, z = 789 };
            m.six = new Point[] { new Point { x = 0, y = 1 }, new Point { x = 2, y = 3 }, new Point { x = 4, y = 5 } };
            m.three = new System.Collections.Generic.List<Point3D> { new Point3D { x = 0, y = 1, z = 2 }, new Point3D { x = 3, y = 4, z = 5 } };

            return m;
        }
        public static void DebugAssertEquals(message_one left, message_one right)
        {
            Assert.Equal(left.one, right.one);
            Assert.True(left.two.SequenceEqual(right.two));
            Assert.True(left.three.SequenceEqual(right.three));
            Assert.True(left.four == right.four);
            Assert.True(left.five.Equals(right.five));
            Assert.True(left.six.SequenceEqual(right.six));
            Assert.True(left.seven == right.seven);
            Assert.True(left.eight.Equals(right.eight));
            Assert.True(left.nine.Equals(right.nine));
            Assert.True(left.ten.SequenceEqual(right.ten));
        }

        [Fact]
        public static void TestGmq()
        {
            message_one m = new message_one();
            m.setSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            m.compose(composer);

            //buffer.writeToFile("message_one.gmq");

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            message_one mback = new message_one();
            mback.parse(parser);

            Assert.Equal(m, mback);
        }

        [Fact]
        public static void TestJson()
        {

            message_one m = new message_one();
            m.setSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            m.compose(composer);

            //buffer.writeToFile("message_one.json");

            JsonParser parser = new JsonParser(buffer.getReadIterator());

            message_one mback = new message_one();
            mback.parse(parser);

            Assert.Equal(m, mback);
        }

        [Fact]
        public static void TestGmqFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.gmq");
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            message_one m = new message_one();
            m.parse(parser);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);

            m.compose(composer);

            Assert.Equal(buffer, buffer2);

        }

        [Fact]
        public static void TestJsonFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("message_one.json");
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            message_one m = new message_one();
            m.parse(parser);

            SimpleBuffer buffer2 = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer2);

            m.compose(composer);

            Assert.Equal(buffer, buffer2);
        }

    }

}
