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

    public class test_publishable_six_notify
    {
        public enum Events {
            notifyUpdated_anInt, notifyUpdated_anUInt, notifyUpdated_aReal, notifyUpdated_aString,
            notifyUpdated_name, notifyUpdated_theAggregate, notifyUpdated_lastValue,
            notifyUpdated_basic, notifyUpdated_aggregate, notifyUpdated, notifyFullyUpdated
        };
        public class BasicTypes_stub :   mtest.BasicTypes_subscriber
        {
            List<Events> handled;

            public BasicTypes_stub(  mtest.IBasicTypes data, List<Events> handled) : base(data)
            {
                this.handled = handled;
            }


            public override void notifyUpdated() { handled.Add(Events.notifyUpdated); }
            public override void notifyUpdated_anInt(Int64 old) { handled.Add(Events.notifyUpdated_anInt); }
            public override void notifyUpdated_anUInt(UInt64 old) { handled.Add(Events.notifyUpdated_anUInt); }
            public override void notifyUpdated_aReal(Double old) { handled.Add(Events.notifyUpdated_aReal); }
            public override void notifyUpdated_aString(String old) { handled.Add(Events.notifyUpdated_aString); }

        }
        public class AggregateType_stub :   mtest.AggregateType_subscriber
        {
            List<Events> handled;

            public AggregateType_stub(  mtest.IAggregateType data, List<Events> handled) : base(data)
            {
                this.handled = handled;
            }

            public override   mtest.BasicTypes_subscriber makeHandler_theAggregate(  mtest.IBasicTypes data) { return new BasicTypes_stub(data, handled); }

            public override void notifyUpdated() { handled.Add(Events.notifyUpdated); }
            public override void notifyUpdated_name(String old) { handled.Add(Events.notifyUpdated_name); }
            public override void notifyUpdated_theAggregate() { handled.Add(Events.notifyUpdated_theAggregate); }
            public override void notifyUpdated_lastValue(Int64 old) { handled.Add(Events.notifyUpdated_lastValue); }


        }

        public class StructSix_stub :   mtest.StructSix_subscriber
        {
            List<Events> handled;

            public StructSix_stub(List<Events> handled)
            {
                this.handled = handled;
            }

            public override   mtest.BasicTypes_subscriber makeHandler_basic(  mtest.IBasicTypes data) { return new BasicTypes_stub(data, handled); }
            public override   mtest.AggregateType_subscriber makeHandler_aggregate(  mtest.IAggregateType data) { return new AggregateType_stub(data, handled); }

            public override void notifyFullyUpdated() { handled.Add(Events.notifyFullyUpdated); }
            public override void notifyUpdated() { handled.Add(Events.notifyUpdated); }
            public override void notifyUpdated_name(String old) { handled.Add(Events.notifyUpdated_name); }
            public override void notifyUpdated_basic() { handled.Add(Events.notifyUpdated_basic); }
            public override void notifyUpdated_aggregate() { handled.Add(Events.notifyUpdated_aggregate); }

        }


        [Fact]
        public static void TestJsonNotifyStateSync()
        {
            List<Events> handled = new List<Events>();
            StructSix_stub stub = new StructSix_stub(handled);

            SimpleBuffer buffer = SimpleBuffer.readFromFile(test_publishable_six.Path);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            stub.applyStateSyncMessage(parser);

            Assert.Equal(handled.ToArray(), new Events[]{ Events.notifyFullyUpdated });
        }

        static void TestJsonParseUpdate(String fileName, Events[] events)
        {
            List<Events> handled = new List<Events>();
            StructSix_stub stub = new StructSix_stub(handled); 

            SimpleBuffer buffer = SimpleBuffer.readFromFile(fileName);
            JsonPublishableParser parser = new JsonPublishableParser(buffer.getReadIterator());

            stub.applyMessageWithUpdates(parser);

            Assert.Equal(events, handled.ToArray());
        }

        [Fact]
        public static void TestJsonNotifyUpdate1()
        {
            TestJsonParseUpdate(test_publishable_six.Path1, new Events[] {
                Events.notifyUpdated_anInt,
                Events.notifyUpdated,
                Events.notifyUpdated_theAggregate,
                Events.notifyUpdated,
                Events.notifyUpdated_aggregate,
                Events.notifyUpdated
            });
        }


        [Fact]
        public static void TestJsonNotifyUpdate2()
        {
            TestJsonParseUpdate(test_publishable_six.Path2, new Events[] {
                Events.notifyUpdated_name,
                Events.notifyUpdated,
                Events.notifyUpdated_aggregate,
                Events.notifyUpdated,
                Events.notifyUpdated_anInt,
                Events.notifyUpdated,
                Events.notifyUpdated_basic,
                Events.notifyUpdated,
                Events.notifyUpdated_anUInt,
                Events.notifyUpdated,
                Events.notifyUpdated_basic,
                Events.notifyUpdated,
                Events.notifyUpdated_aReal,
                Events.notifyUpdated,
                Events.notifyUpdated_basic,
                Events.notifyUpdated,
                Events.notifyUpdated_anInt,
                Events.notifyUpdated_anUInt,
                Events.notifyUpdated_aReal,
                Events.notifyUpdated_aString,
                Events.notifyUpdated,
                Events.notifyUpdated_theAggregate,
                Events.notifyUpdated,
                Events.notifyUpdated_aggregate,
                Events.notifyUpdated
            });
        }

        [Fact]
        public static void TestJsonNotifyUpdate3()
        {
            TestJsonParseUpdate(test_publishable_six.Path3, new Events[] { });
        }

    }
}
