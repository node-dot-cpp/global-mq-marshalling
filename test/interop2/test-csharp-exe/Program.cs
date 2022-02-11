using globalmq.marshalling;
using System;
using System.Runtime.InteropServices;
using interopbufferdotnet;
using System.Diagnostics;

namespace interop_csharp_exe
{
    public class TestInterop2_handler : mtest.TestInterop2_subscriber
    {
        string description;
        public TestInterop2_handler(string description)
        {
            this.description = description;
        }
        public override void notifyUpdated_number(Int64 old)
        {
            Console.WriteLine("At {0}: {1} => {2}", description, old, number);
        }
        public override void notifyUpdated_name(String old)
        {
            Console.WriteLine("At {0}: {1} => {2}", description, old, name);
        }

}
class Program
    {

        unsafe public delegate void GmqPostMessage(UInt64 recipientID, UInt64 size, UInt64 capacity, byte* data);

        [DllImport("test-cpp-dll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GmqInit(UInt64 id, UInt64 slotId, UInt64 reincarnation, GmqPostMessage postFunc);

        [DllImport("test-cpp-dll.dll", CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void GmqHandleMessage(UInt64 size, UInt64 capacity, byte* data);
        [DllImport("test-cpp-dll.dll", CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void EndTick();
        [DllImport("test-cpp-dll.dll", CallingConvention = CallingConvention.Cdecl)]
        unsafe public static extern void TestUpdatePublisher(Int64 number);


        unsafe static void deliverAllMessages(String filePrefix, ref int msgCnt)
        {
            ThreadQueueItem[] messages = new ThreadQueueItem[16];

            while (true)
            {
                int popped = msgQueue.pop_front(messages, 15, false);
                if (popped == 0)
                    break;
                for (int i = 0; i < popped; ++i)
                {

                    ++msgCnt;
                    if (filePrefix != null)
                    {
                        InteropBuffer buffer = (InteropBuffer)messages[i].msg;
                        String fileName = filePrefix + msgCnt + ".json";

                        buffer.writeToFile(fileName);
                    }

                    if (messages[i].recipientID == 2)
                        mp.onMessage(messages[i].msg);
                    else if (messages[i].recipientID == 3)
                    {
                        InteropBufferMemento memento = ((InteropBuffer)(messages[i].msg)).releaseToMemento();
                        GmqHandleMessage(memento.size, memento.capacity, memento.buffer);
                    }
                    else
                        Debug.Assert(false);

                }
            }
        }

        unsafe static void callbackToCpp(UInt64 recipientID, UInt64 size, UInt64 capacity, byte* data)
        {
            Console.WriteLine("callback from c++");
            InteropBufferMemento memento = new InteropBufferMemento();
            memento.size = size;
            memento.capacity = capacity;
            memento.buffer = data;
            InteropBuffer buffer = InteropBuffer.makeFromMemento(memento);

            transport3.postMessage(buffer);
        }

        static BasicMtQueue<ThreadQueueItem> msgQueue;
        static MetaPool mp;
        static GMQueue gmq;
        static GMQTransportBase transport2;
        static GMQTransportBase transport3;

        public class InteropPlatform : IPlatformSupport
        {
            public BufferT makeBuffer() { return new InteropBuffer(); }
            public IPublishableComposer makePublishableComposer(BufferT buffer) { return new JsonPublishableComposer(buffer); }
            public IPublishableParser makePublishableParser(ReadIteratorT readIter) { return new JsonPublishableParser(readIter); }
            public InProcessMessagePostmanBase allocPostman(Object[] args) { throw new NotImplementedException(); }
        }



        unsafe static void Main(string[] args)
        {
            msgQueue = new BasicMtQueue<ThreadQueueItem>();

            IPlatformSupport platform = new InteropPlatform();

            gmq = new GMQueue();
            gmq.initStateConcentratorFactory(new mtest.StateConcentratorFactory(), platform);
            gmq.setAuthority(String.Empty);


            //BasicMtQueue<ThreadQueueItem> msgQueue = new BasicMtQueue<ThreadQueueItem>();
            BasicQueuePostman postMan3 = new BasicQueuePostman(msgQueue, 3);
            SlotIdx idx_3;
            UInt64 id_3 = gmq.add("test_cpp", postMan3, out idx_3);


            BasicQueuePostman postMan2 = new BasicQueuePostman(msgQueue, 2);
            SlotIdx idx_2;
            UInt64 id_2 = gmq.add("test_cs", postMan2, out idx_2);


            transport2 = new GMQTransportBase(gmq, "test_cs", idx_2, id_2);
            transport3 = new GMQTransportBase(gmq, "test_cpp", idx_3, id_3);



            mp = new MetaPool();

            mp.setTransport(transport2);
            mp.setPlatform(platform);

            mtest.TestInterop2_publisher publ = new mtest.TestInterop2_publisher();

            //mtest.StructSix data = test_publishable_six.GetPublishableSix();
            //publ.debugOnlySetData(data);

            mp.add(publ);


            int msgCnt = 0;


            GmqInit(id_3, (UInt64)idx_3.idx, idx_3.reincarnation, callbackToCpp);

            deliverAllMessages("test_interop2_", ref msgCnt);




            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
            pc.nodeName = "test_cpp";
            pc.statePublisherOrConnectionType = "TestInterop2";
            string path = GmqPathHelper.compose(pc);

            TestInterop2_handler subs1 = new TestInterop2_handler("C# subs1");

            mp.add(subs1);
            mp.subscribe(subs1, path);


            deliverAllMessages("test_interop2_", ref msgCnt);
            EndTick();
            deliverAllMessages("test_interop2_", ref msgCnt);
            TestUpdatePublisher(55);
            EndTick();
            deliverAllMessages("test_interop2_", ref msgCnt);
        }
    }
}
