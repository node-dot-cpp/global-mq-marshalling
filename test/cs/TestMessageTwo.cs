using globalmq.marshalling;
using System;
using System.Collections.Generic;
using Xunit;

namespace TestProject1
{

    public class TestMessageTwo
    {
        private static void ComposeMessageTwo(BufferT buffer, mtest.struct_one msg)
        {
            mtest.test_gmq.composeMessage_message_two(buffer,
                thirdParam: new CollectionWrapperForComposing(() => { return msg.thirdParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point3D.compose(composer, x: msg.thirdParam[ordinal].x, y: msg.thirdParam[ordinal].y, z: msg.thirdParam[ordinal].z); }),
                firstParam: msg.firstParam, fifthParam: msg.fifthParam, forthParam: msg.forthParam, seventhParam: msg.seventhParam,
                eighthParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point.compose(composer, x: msg.eighthParam.x, y: msg.eighthParam.y); }),
                ninethParam: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.ninethParam.x, y: msg.ninethParam.y, z: msg.ninethParam.z); }),
                secondParam: SimpleTypeCollection.makeComposer(msg.secondParam),
                tenthParam: SimpleTypeCollection.makeComposer(msg.tenthParam),
                sixthParam: new CollectionWrapperForComposing(() => { return msg.sixthParam.Count; }, (ComposerBase composer, int ordinal) => { mtest.point.compose(composer, x: msg.sixthParam[ordinal].x, y: msg.sixthParam[ordinal].y); })
            );
        }

        [Fact]
        public static void TestGmqCompose()
        {
            mtest.struct_one msg = TestStructOne.GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();

            ComposeMessageTwo(buffer, msg);

            // uncomment to update file
            //buffer.writeToFile("message_two.gmq");

            Assert.Equal(buffer, SimpleBuffer.readFromFile("message_two.gmq"));
        }


        [Fact]
        public static void TestGmqHandle()
        {
            mtest.struct_one msg = TestStructOne.GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();

            ComposeMessageTwo(buffer, msg);

            bool condition = false;

            MessageHandlerArray handlers = MessageHandlerArray.make(new MessageHandler[] {
                new MessageHandler((UInt64)mtest.test_gmq.MsgIds.message_two, (ParserBase parser) => {
                    mtest.struct_one msg = mtest.struct_one.parse(parser);
                    condition = msg.Equals(TestStructOne.GetSampleData());
                }),
                new MessageHandler((UInt64)mtest.test_gmq.MsgIds.message_four, (ParserBase parser) => { /* nothing */ }),
                new MessageHandler(MessageHandler.DefaultHandler, (ParserBase parser) => { /* nothing */ })
            });

            mtest.test_gmq.handleMessage(buffer, handlers);

            Assert.True(condition);
        }

        [Fact]
        public static void TestGmqHandleDefault()
        {
            mtest.struct_one msg = TestStructOne.GetSampleData();

            SimpleBuffer buffer = new SimpleBuffer();

            ComposeMessageTwo(buffer, msg);

            bool condition = false;

            MessageHandlerArray handlers = MessageHandlerArray.make(new MessageHandler[] {
                new MessageHandler((UInt64)mtest.test_gmq.MsgIds.message_four, (ParserBase parser) => { /* nothing */ }),
                new MessageHandler(MessageHandler.DefaultHandler, (ParserBase parser) => {
                    //mb we need to remove data from stream, otherwise we get an exception from parser
                    mtest.struct_one.parse(parser);
                    condition = true;
                })
            });

            mtest.test_gmq.handleMessage(buffer, handlers);

            Assert.True(condition);
        }

    }

}
