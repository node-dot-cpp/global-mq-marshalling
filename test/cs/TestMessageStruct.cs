using globalmq.marshalling;
using System;
using System.Collections.Generic;
using Xunit;

namespace TestProject1
{

    public class TestMessageStruct
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
            public List<int> two;
            public List<double> ten;
            public Point eight;
            public Point3D nine;
            public List<Point> six;
            public List<Point3D> three;
        }

        public static message_one getSampleData()
        {
            //create some sample data to be written to message
    
            message_one msg = new message_one();

            msg.one = 1;
            msg.five = "def";
            msg.four = 3;
            msg.seven = 3.1416;
            msg.two = new List<int> { 0, 1, 2, 3, 4, 5 };
            msg.ten = new List<double> { 0.1, 1.2, 2.3 };
            msg.eight = new Point { x = 175, y = 186 };
            msg.nine = new Point3D { x = 123, y = 456, z = 789 };
            msg.six = new List<Point> { new Point { x = 0, y = 1 }, new Point { x = 2, y = 3 }, new Point { x = 4, y = 5 } };
            msg.three = new System.Collections.Generic.List<Point3D> { new Point3D { x = 0, y = 1, z = 2 }, new Point3D { x = 3, y = 4, z = 5 } };

            return msg;
        }

        public static void CreateGmq()
        {
            message_one msg = getSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_one.compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.three.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.three[ordinal].x, y: msg.three[ordinal].y, z: msg.three[ordinal].z); }),
                firstParam: msg.one, fifthParam: msg.five, forthParam: msg.four, seventhParam: msg.seven,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eight.x, y: msg.eight.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.nine.x, y: msg.nine.y, z: msg.nine.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.two),
                tenthParam: SimpleTypeCollection.makeComposer(msg.ten),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.six.Count; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.six[ordinal].x, y: msg.six[ordinal].y); })
            );

            buffer.writeToFile("struct_one.gmq");
        }

        public static void CreateJson()
        {

            message_one msg = getSampleData();

            SimpleBuffer buffer = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer);

            mtest.struct_one.compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.three.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.three[ordinal].x, y: msg.three[ordinal].y, z: msg.three[ordinal].z); }),
                firstParam: msg.one, fifthParam: msg.five, forthParam: msg.four, seventhParam: msg.seven,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eight.x, y: msg.eight.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.nine.x, y: msg.nine.y, z: msg.nine.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.two),
                tenthParam: SimpleTypeCollection.makeComposer(msg.ten),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.six.Count; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.six[ordinal].x, y: msg.six[ordinal].y); })
            );

            buffer.writeToFile("struct_one.json");
        }

        [Fact]
        public static void TestGmqFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("struct_one.gmq");
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_one msg = mtest.struct_one.parse(parser);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);

            mtest.struct_one.compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.thirdParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.thirdParam[ordinal].x, y: msg.thirdParam[ordinal].y, z: msg.thirdParam[ordinal].z); }),
                firstParam: msg.firstParam, fifthParam: msg.fifthParam, forthParam: msg.forthParam, seventhParam: msg.seventhParam,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eighthParam.x, y: msg.eighthParam.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.ninethParam.x, y: msg.ninethParam.y, z: msg.ninethParam.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
                tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.sixthParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.sixthParam[ordinal].x, y: msg.sixthParam[ordinal].y); })
            );

            Assert.Equal(buffer, buffer2);
        }

        [Fact]
        public static void TestJsonFile()
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile("struct_one.json");
            JsonParser parser = new JsonParser(buffer.getReadIterator());

            mtest.struct_one msg = mtest.struct_one.parse(parser);

            SimpleBuffer buffer2 = new SimpleBuffer();
            JsonComposer composer = new JsonComposer(buffer2);

            mtest.struct_one.compose(composer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.thirdParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.thirdParam[ordinal].x, y: msg.thirdParam[ordinal].y, z: msg.thirdParam[ordinal].z); }),
                firstParam: msg.firstParam, fifthParam: msg.fifthParam, forthParam: msg.forthParam, seventhParam: msg.seventhParam,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eighthParam.x, y: msg.eighthParam.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.ninethParam.x, y: msg.ninethParam.y, z: msg.ninethParam.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
                tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.sixthParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.sixthParam[ordinal].x, y: msg.sixthParam[ordinal].y); })
            );

            Assert.Equal(buffer, buffer2);
        }

    }

}
