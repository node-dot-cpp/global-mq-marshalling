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

    public class TestMessageDu
    {
        public static void CreateGmq1()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_du.compose(composer,
                pt: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: 5, y: 6, z: 7); }),
                disc_union: new MessageWrapperForComposing((ComposerBase composer) => { mtest.du_one.compose_one(composer, D1: 2.3, D2: 4.5, D3: 6.7); })
                );

            buffer.writeToFile("struct_du_1.gmq");
        }

        public static void CreateGmq2()
        {
            SimpleBuffer buffer = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer);

            mtest.struct_du.compose(composer,
                pt: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: 5, y: 6, z: 7); }),
                disc_union: new MessageWrapperForComposing((ComposerBase composer) =>
                {
                    mtest.du_one.compose_two(composer,
Data: SimpleTypeCollection.makeComposer(new List<Single> { 2.3f, 4.5f, 6.7f })
);
                })
                );

            buffer.writeToFile("struct_du_2.gmq");
        }


        private static void TestGmqFile(string fileName)
        {
            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            GmqParser parser = new GmqParser(buffer.getReadIterator());

            mtest.struct_du msg = mtest.struct_du.parse(parser);

            SimpleBuffer buffer2 = new SimpleBuffer();
            GmqComposer composer = new GmqComposer(buffer2);


            mtest.struct_du.compose(composer,
            pt: new MessageWrapperForComposing((ComposerBase composer) => { mtest.point3D.compose(composer, x: msg.pt.x, y: msg.pt.y, z: msg.pt.z); }),
            disc_union: new MessageWrapperForComposing((ComposerBase composer) =>
            {
                if (msg.disc_union.currentVariant() == mtest.du_one.Variants.one)
                    mtest.du_one.compose_one(composer, D1: msg.disc_union.D1, D2: msg.disc_union.D2, D3: msg.disc_union.D3);
                else if (msg.disc_union.currentVariant() == mtest.du_one.Variants.two)
                    mtest.du_one.compose_two(composer,
                        Data: SimpleTypeCollection.makeComposer(msg.disc_union.Data));
            })
            );
 
            Assert.Equal(buffer, buffer2);
        }

        [Fact]
        public static void TestGmqFile1()
        {
            TestGmqFile("struct_du_1.gmq");
        }

        [Fact]
        public static void TestGmqFile2()
        {
            TestGmqFile("struct_du_1.gmq");
        }


    }

}
