using globalmq.marshalling;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
            public Int32 x;
            public Int32 y;
            public Int32 z;
        }
        public class message_one
        {
            public long one;
            public string five;
            public UInt64 four;
            public Double seven;
            public int[] two;
            public double[] ten;
            public Point eight;
            public Point3D nine;
            public Point[] six;
            public List<Point3D> three;
        }

        public static message_one GetSampleData()
        {
            message_one m = new message_one();
            m.one = 1;
            m.five = "def";
            m.four = 3;
            m.seven = 3.1416;
            m.two = new int[] { 0, 1, 2, 3, 4, 5 };
            m.ten = new double[] { 0.1, 1.2, 2.3 };
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
            message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.MESSAGE_message_one_compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return m.three.Count; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point3D_compose(composer, x: m.three[ordinal].x, y: m.three[ordinal].y, z: m.three[ordinal].z); }),
                firstParam: m.one, fifthParam: m.five, forthParam: m.four, seventhParam: m.seven,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point_compose(composer, x: m.eight.x, y: m.eight.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point3D_compose(composer, x: m.nine.x, y: m.nine.y, z: m.nine.z); }),
                secondParam: new CollectionWrapperForComposing(() => { return m.two.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(m.two[ordinal]); }),
                tenthParam: new CollectionWrapperForComposing(() => { return m.ten.Length; }, (ComposerBase composer, int ordinal) => { composer.composeReal(m.ten[ordinal]); }),
                sixthParam: new CollectionWrapperForComposing(() => { return m.six.Length; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point_compose(composer, x: m.six[ordinal].x, y: m.six[ordinal].y); })
            );

            GmqParser parser = new GmqParser(buffer.getReadIterator());

            message_one mback = new message_one();
            mback.three = new List<Point3D>();

            mtest.MESSAGE_message_one_parse(parser,
                firstParam: ref mback.one, forthParam: ref mback.four,
                secondParam: new CollectionWrapperForParsing((int size) => { mback.two = new int[size]; }, (ParserBase parser, int ordinal) => { parser.parseSignedInteger(out mback.two[ordinal]); }),
                tenthParam: new CollectionWrapperForParsing((int size) => { mback.ten = new double[size]; }, (ParserBase parser, int ordinal) => { parser.parseReal(out mback.ten[ordinal]); }),
                thirdParam: new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Point3D pt; mtest.STRUCT_point3D_parse(parser, x: out pt.x, y: out pt.y, z: out pt.z); mback.three.Add(pt); }),
                sixthParam: new CollectionWrapperForParsing((int size) => { mback.six = new Point[size]; }, (ParserBase parser, int ordinal) => { Point pt; mtest.STRUCT_point_parse(parser, x: out pt.x, y: out pt.y); mback.six[ordinal] = pt; }),
                eighthParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point_parse(parser, x: out mback.eight.x, y: out mback.eight.y); }),
                ninethParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point3D_parse(parser, x: out mback.nine.x, y: out mback.nine.y, z: out mback.nine.z); }),
                fifthParam: ref mback.five, seventhParam: ref mback.seven
               );

            DebugAssertEquals(m, mback);

        }

        [Fact]
        public static void TestJson()
        {
            message_one m = GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.MESSAGE_message_one_compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return m.three.Count; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point3D_compose(composer, x: m.three[ordinal].x, y: m.three[ordinal].y, z: m.three[ordinal].z); }),
                firstParam: m.one, fifthParam: m.five, forthParam: m.four, seventhParam: m.seven,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point_compose(composer, x: m.eight.x, y: m.eight.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.STRUCT_point3D_compose(composer, x: m.nine.x, y: m.nine.y, z: m.nine.z); }),
                secondParam: new CollectionWrapperForComposing(() => { return m.two.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(m.two[ordinal]); }),
                tenthParam: new CollectionWrapperForComposing(() => { return m.ten.Length; }, (ComposerBase composer, int ordinal) => { composer.composeReal(m.ten[ordinal]); }),
                sixthParam: new CollectionWrapperForComposing(() => { return m.six.Length; }, (ComposerBase composer, int ordinal) => { mtest.STRUCT_point_compose(composer, x: m.six[ordinal].x, y: m.six[ordinal].y); })
            );

            ReadIterT i = buffer.getReadIterator();
            StringBuilder sb = new StringBuilder();
            while (i.isData())
            {
                sb.Append(i.getChar());
                i.inc();
            }

            string s = sb.ToString();



            JsonParser parser = new JsonParser(buffer.getReadIterator());



            message_one mback = new message_one();
            mback.three = new List<Point3D>();

            mtest.MESSAGE_message_one_parse(parser,
                firstParam: ref mback.one, forthParam: ref mback.four,
                secondParam: new CollectionWrapperForParsing((int size) => { mback.two = new int[size]; }, (ParserBase parser, int ordinal) => { parser.parseSignedInteger(out mback.two[ordinal]); }),
                tenthParam: new CollectionWrapperForParsing((int size) => { mback.ten = new double[size]; }, (ParserBase parser, int ordinal) => { parser.parseReal(out mback.ten[ordinal]); }),
                thirdParam: new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Point3D pt; mtest.STRUCT_point3D_parse(parser, x: out pt.x, y: out pt.y, z: out pt.z); mback.three.Add(pt); }),
                sixthParam: new CollectionWrapperForParsing((int size) => { mback.six = new Point[size]; }, (ParserBase parser, int ordinal) => { Point pt; mtest.STRUCT_point_parse(parser, x: out pt.x, y: out pt.y); mback.six[ordinal] = pt; }),
                eighthParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point_parse(parser, x: out mback.eight.x, y: out mback.eight.y); }),
                ninethParam: new MessageWrapperForParsing((ParserBase parser) => { mtest.STRUCT_point3D_parse(parser, x: out mback.nine.x, y: out mback.nine.y, z: out mback.nine.z); }),
                fifthParam: ref mback.five, seventhParam: ref mback.seven
               );

            DebugAssertEquals(m, mback);
        }
    }

}
