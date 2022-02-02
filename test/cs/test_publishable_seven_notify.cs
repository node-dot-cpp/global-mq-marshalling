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

    public class test_publishable_seven_notify
    {
        public enum Events
        {
            notifyUpdated_x, notifyUpdated_y, notifyUpdated_z,
            notifyUpdated_intVec, notifyElementUpdated_intVec, notifyInserted_intVec, notifyErased_intVec,
            notifyUpdated_uintVec, notifyElementUpdated_uintVec, notifyInserted_uintVec, notifyErased_uintVec,
            notifyUpdated_realVec, notifyElementUpdated_realVec, notifyInserted_realVec, notifyErased_realVec,
            notifyUpdated_strVec, notifyElementUpdated_strVec, notifyInserted_strVec, notifyErased_strVec,
            notifyUpdated_structVec, notifyElementUpdated_structVec, notifyInserted_structVec, notifyErased_structVec,
            notifyFullyUpdated
        };

        class point3D_stub :  mtest.point3D_subscriber
        {
            List<Events> handled;

            public point3D_stub( mtest.Ipoint3D data, List<Events> handled) : base(data)
            {
                this.handled = handled;
            }

            public override void notifyUpdated_x(Int64 old) { handled.Add(Events.notifyUpdated_x); }
            public override void notifyUpdated_y(Int64 old) { handled.Add(Events.notifyUpdated_y); }
            public override void notifyUpdated_z(Int64 old) { handled.Add(Events.notifyUpdated_z); }
        }

            class publishable_seven_stub :  mtest.publishable_seven_subscriber
        {
            List<Events> handled;

            public publishable_seven_stub(List<Events> handled)
            {
                this.handled = handled;
            }

            public override  mtest.point3D_subscriber makeElementHandler_structVec( mtest.Ipoint3D data) { return new point3D_stub(data, handled); }



            public override void notifyFullyUpdated() { handled.Add(Events.notifyFullyUpdated); }
            public override void notifyUpdated_intVec() { handled.Add(Events.notifyUpdated_intVec); }
            public override void notifyElementUpdated_intVec(int index, Int64 old) { handled.Add(Events.notifyElementUpdated_intVec); }
            public override void notifyInserted_intVec(int index) { handled.Add(Events.notifyInserted_intVec); }
            public override void notifyErased_intVec(int index) { handled.Add(Events.notifyErased_intVec); }
            public override void notifyUpdated_uintVec() { handled.Add(Events.notifyUpdated_uintVec); }
            public override void notifyElementUpdated_uintVec(int index, UInt64 old) { handled.Add(Events.notifyElementUpdated_uintVec); }
            public override void notifyInserted_uintVec(int index) { handled.Add(Events.notifyInserted_uintVec); }
            public override void notifyErased_uintVec(int index) { handled.Add(Events.notifyErased_uintVec); }
            public override void notifyUpdated_realVec() { handled.Add(Events.notifyUpdated_realVec); }
            public override void notifyElementUpdated_realVec(int index, Double old) { handled.Add(Events.notifyElementUpdated_realVec); }
            public override void notifyInserted_realVec(int index) { handled.Add(Events.notifyInserted_realVec); }
            public override void notifyErased_realVec(int index) { handled.Add(Events.notifyErased_realVec); }
            public override void notifyUpdated_strVec() { handled.Add(Events.notifyUpdated_strVec); }
            public override void notifyElementUpdated_strVec(int index, String old) { handled.Add(Events.notifyElementUpdated_strVec); }
            public override void notifyInserted_strVec(int index) { handled.Add(Events.notifyInserted_strVec); }
            public override void notifyErased_strVec(int index) { handled.Add(Events.notifyErased_strVec); }
            public override void notifyUpdated_structVec() { handled.Add(Events.notifyUpdated_structVec); }
            public override void notifyElementUpdated_structVec(int index) { handled.Add(Events.notifyElementUpdated_structVec); }
            public override void notifyInserted_structVec(int index) { handled.Add(Events.notifyInserted_structVec); }
            public override void notifyErased_structVec(int index) { handled.Add(Events.notifyErased_structVec); }

        }



        static void TestJsonParseUpdate(String fileInitial, String fileUpdate, Events[] events)
        {
            List<Events> handled = new List<Events>();
            publishable_seven_stub stub = new publishable_seven_stub(handled);

            SimpleBuffer bufferInitial = SimpleBuffer.readFromFile(fileInitial);
            JsonPublishableParser parserInitial = new JsonPublishableParser(bufferInitial.getReadIterator());

            stub.applyStateSyncMessage(parserInitial);

            Assert.Equal(test_publishable_seven.GetPublishableSeven(), ( mtest.publishable_seven)stub.debugOnlyGetData());

            if (fileUpdate != null)
            {
                SimpleBuffer bufferUpdate = SimpleBuffer.readFromFile(fileUpdate);
                JsonPublishableParser parserUpdate = new JsonPublishableParser(bufferUpdate.getReadIterator());

                stub.applyMessageWithUpdates(parserUpdate);
            }

            Assert.Equal(handled.ToArray(), events);
        }

        [Fact]
        public static void TestJsonNotifyStateSync()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                null,
                new Events[] {
                    Events.notifyFullyUpdated
                }
            );
        }

        [Fact]
        public static void TestJsonNotifyUpdate1()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                test_publishable_seven.JsonPath1,
                new Events[] {
                    Events.notifyFullyUpdated,
                    Events.notifyUpdated_y,
                    Events.notifyUpdated_structVec
                }
            );
        }

        [Fact]
        public static void TestJsonNotifyUpdate2()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                test_publishable_seven.JsonPath2,
                new Events[] {
                    Events.notifyFullyUpdated,
                    Events.notifyElementUpdated_intVec,
                    Events.notifyUpdated_intVec,
                    Events.notifyElementUpdated_uintVec,
                    Events.notifyUpdated_uintVec,
                    Events.notifyElementUpdated_realVec,
                    Events.notifyUpdated_realVec,
                    Events.notifyElementUpdated_strVec,
                    Events.notifyUpdated_strVec,
                    Events.notifyUpdated_x,
                    Events.notifyUpdated_y,
                    Events.notifyUpdated_z,
                    Events.notifyElementUpdated_structVec,
                    Events.notifyUpdated_structVec
                }
            );
        }
        [Fact]
        public static void TestJsonNotifyUpdate3()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                test_publishable_seven.JsonPath3,
                new Events[] {
                    Events.notifyFullyUpdated,
                    Events.notifyUpdated_strVec,
                    Events.notifyUpdated_realVec,
                    Events.notifyUpdated_structVec
                }
            );
        }

        [Fact]
        public static void TestJsonNotifyUpdate4()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                test_publishable_seven.JsonPath4,
                new Events[] {
                    Events.notifyFullyUpdated,
                    Events.notifyErased_intVec,
                    Events.notifyUpdated_intVec,
                    Events.notifyErased_realVec,
                    Events.notifyUpdated_realVec,
                    Events.notifyErased_structVec,
                    Events.notifyUpdated_structVec
                }
            );
        }

        [Fact]
        public static void TestJsonNotifyUpdate5()
        {
            TestJsonParseUpdate(
                test_publishable_seven.JsonPath,
                test_publishable_seven.JsonPath5,
                new Events[] {
                    Events.notifyFullyUpdated,
                    Events.notifyInserted_intVec,
                    Events.notifyUpdated_intVec,
                    Events.notifyInserted_uintVec,
                    Events.notifyUpdated_uintVec,
                    Events.notifyInserted_realVec,
                    Events.notifyUpdated_realVec,
                    Events.notifyInserted_strVec,
                    Events.notifyUpdated_strVec,
                    Events.notifyUpdated_x,
                    Events.notifyUpdated_y,
                    Events.notifyUpdated_z,
                    Events.notifyInserted_structVec,
                    Events.notifyUpdated_structVec
                }
            );
        }


    }

}
