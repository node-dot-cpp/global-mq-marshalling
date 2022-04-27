/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
*
* Copyright (c) 2021, OLogN Technologies AG
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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;

namespace globalmq.marshalling
{


    public struct ThreadQueueItem
    {
        public BufferT msg;
        public int recipientID;
        public enum ThreadQueueItemType { Undefined, GlobalMQ, UserDefined };
        public ThreadQueueItemType msgType;

        public ThreadQueueItem(BufferT msg_, ThreadQueueItemType msgType_, int recipientID_)
        {
            this.msg = msg_;
            this.msgType = msgType_;
            this.recipientID = recipientID_;
        }

        public static ThreadQueueItem makeGlobalMq(BufferT msg, int recipientID) { return new ThreadQueueItem(msg, ThreadQueueItemType.GlobalMQ, recipientID); }
        public static ThreadQueueItem makeNull() { return new ThreadQueueItem(null, ThreadQueueItemType.Undefined, 0); }
    };

    class CircularBuffer
    {
        private int bufsz;
        private int maxsz;
        private int mask;
        private int head = 0;
        private int tail = 0;

        ThreadQueueItem[] buffer;

        public CircularBuffer(int szExp)
        {
            this.bufsz = 1 << szExp;
            this.maxsz = bufsz - 1;
            //-1 to make sure that head==tail always means 'empty'
            this.mask = maxsz;
            this.buffer = new ThreadQueueItem[bufsz];

        }
        public int size()
        {
            return (head >= tail) ? head - tail : head - tail + bufsz;
        }

        public bool is_full() { return size() == maxsz; }

        public void push_back(ThreadQueueItem t)
        {
            Debug.Assert(size() < maxsz);
            buffer[head] = t;
            head = (head + 1) & mask;
        }

        public ThreadQueueItem pop_front()
        {
            Debug.Assert(size() > 0);
            ThreadQueueItem ret = buffer[tail];
            buffer[tail] = ThreadQueueItem.makeNull();
            tail = (tail + 1) & mask;
            return ret;
        }
    };

    //mb: this implementation is very basic, just for first tests.
    public class MwsrThreadQueue
    {
        object mx = new object();
        CircularBuffer coll;
        bool stillRunning = true;

        //stats:
        int hwmsize = 0;//high watermark on queue size

        public MwsrThreadQueue(int szExp)
        {
            this.coll = new CircularBuffer(szExp);
        }
        public void push_back(ThreadQueueItem it)
        {
            //if the queue is full, just throw
            lock (mx)
            {
                while (coll.is_full() && stillRunning)
                    Monitor.Wait(mx);

                if (!stillRunning)
                    return;

                Debug.Assert(!coll.is_full());
                coll.push_back(it);
                int sz = coll.size();
                hwmsize = Math.Max(hwmsize, sz);

                Monitor.Pulse(mx);
            }//unlocking mx
        }

        public int pop_front(ThreadQueueItem[] messages)
        {
            return pop_front(messages, 0);
        }

        public int pop_front(ThreadQueueItem[] messages, int timeout)
        {
            lock (mx)
            {
                if (coll.size() == 0)
                {
                    Monitor.Wait(mx, timeout);
                }

                if (coll.size() == 0)
                    return 0;

                int sz2move = messages.Length <= coll.size() ? messages.Length : coll.size();
                for (int i = 0; i < sz2move; ++i)
                    messages[i] = coll.pop_front();

                Monitor.Pulse(mx);
                return sz2move;
            }
        }

        public void kill()
        {
            lock (mx)
            {
                stillRunning = false;
                Monitor.Pulse(mx);
            }//unlocking mx
        }
    };

    public class ThreadQueuePostman : IPostman
    {
        MwsrThreadQueue msgQueue;
        int recipientID;

        public ThreadQueuePostman(MwsrThreadQueue msgQueue_, int recipientID_)
        {
            this.msgQueue = msgQueue_;
            this.recipientID = recipientID_;
        }
        public void postMessage(BufferT msg)
        {
            msgQueue.push_back(ThreadQueueItem.makeGlobalMq(msg, recipientID));
        }
    };

    public class ThreadQueueProcessor
    {
        MwsrThreadQueue msgQueue;
        List<NodeBase> nodes = new List<NodeBase>();
        bool stillRunning = true;

        public ThreadQueueProcessor(MwsrThreadQueue msgQueue)
        {
            this.msgQueue = msgQueue;
        }
        public int addNode(NodeBase n)
        {
            int ix = nodes.Count;
            nodes.Add(n);
            return ix;
        }

        int getNextTimeout(int now)
        {
            int nextTimeout = nodes[0].getNextTimeout(now);
            for (int i = 1; i < nodes.Count; ++i)
            {
                nextTimeout = Math.Min(nextTimeout, nodes[1].getNextTimeout(now));
            }

            return nextTimeout;
        }
        public void init()
        {
            if (nodes.Count == 0)
                throw new Exception("No nodes in this queue");

            foreach(NodeBase each in nodes)
                each.init();

        }

        public void run()
        {
            while (stillRunning)
            {
                const int maxMsgCnt = 8;
                ThreadQueueItem[] thq = new ThreadQueueItem[maxMsgCnt];
                //int actualFromQueue = timeoutToUse == TimeOutNever ? popFrontFromThisThreadQueue(thq, maxMsgCnt) : popFrontFromThisThreadQueue(thq, maxMsgCnt, timeoutToUse / 1000);
                int nextTimeout = getNextTimeout(0);
                int actualFromQueue = msgQueue.pop_front(thq, nextTimeout);

                for (int i = 0; i < actualFromQueue; ++i)
                {
                    Debug.Assert(thq[i].recipientID < nodes.Count); // current version limitation: just a single node per thread
                    Debug.Assert(thq[i].msgType == ThreadQueueItem.ThreadQueueItemType.GlobalMQ); // current version limitation
                    nodes[thq[i].recipientID].processMessagesAndOrTimeout(thq[i].msg);
                }

                foreach (NodeBase each in nodes)
                    each.processMessagesAndOrTimeout(null);

            }
        }
    }

    public class ThreadQueueManager
    {
        public const int MAX_THREADS = 128;
        public const int DEFAULT_QUEUE_SZEXP = 5;

        static ThreadQueueManager instance = new ThreadQueueManager();

        public static ThreadQueueManager getInstance() { return instance; }
        class InterThreadCommData
        {
            public UInt64 reincarnation = 0;
            public int csharpThreadId = 0;
            public Thread thread = null;
            public enum Status { unused, acquired, running, terminating };
            public Status status = Status.unused;
            public MwsrThreadQueue queue;
        }

        [ThreadStatic]
        static InterThreadCommData currentThreadQueue = null;

        object mx = new object();
        InterThreadCommData[] threadQueues = new InterThreadCommData[MAX_THREADS];

        /// <summary>
        /// Creates the thread queue and assigns some 'slot' id to it.
        /// This is a simplification from C++ counterpart, where this is
        /// supposed to be called only by the thread owner of the queue
        /// after launch, and not by the main thread as it is on C++
        /// </summary>
        /// <returns></returns>
        public MwsrThreadQueue createQueueForCurrentThread()
        {
            lock (mx)
            {
                Debug.Assert(currentThreadQueue == null);

                for (int i = 1; i < threadQueues.Length; ++i)
                {
                    if (threadQueues[i] == null)
                    {
                        threadQueues[i] = new InterThreadCommData();
                    }

                    if (threadQueues[i].status == InterThreadCommData.Status.unused)
                    {
                        ++(threadQueues[i].reincarnation);
                        threadQueues[i].csharpThreadId = Thread.CurrentThread.ManagedThreadId;
                        threadQueues[i].thread = Thread.CurrentThread;
                        threadQueues[i].status = InterThreadCommData.Status.acquired;
                        threadQueues[i].queue = new MwsrThreadQueue(DEFAULT_QUEUE_SZEXP);

                        currentThreadQueue = threadQueues[i];

                        return threadQueues[i].queue;
                    }
                }
            }
            throw new Exception("All thread slots already in use");
        }

        struct ThreadStartInfo
        {
            public IPlatformSupport platform;
            public GMQueue gmq;
            public INodeFactory[] factories;
        }

        public static void nodeThreadQueueMain(Object obj)
        {
            ThreadStartInfo info = (ThreadStartInfo)obj;

            MwsrThreadQueue queue = getInstance().createQueueForCurrentThread();
            ThreadQueueProcessor proc = new ThreadQueueProcessor(queue);

            foreach(INodeFactory each in info.factories)
            {
                NodeBase node = each.create();
                int ix = proc.addNode(node);

                ThreadQueuePostman postMan = new ThreadQueuePostman(queue, ix);

                SlotIdx idx_;
                UInt64 id_ = info.gmq.add(node.getNodeName(), postMan, out idx_);
                GMQTransport transport = new GMQTransport(info.gmq, node.getNodeName(), idx_, id_);


                node.initPlatform(info.platform);
                node.initTransport(transport);
            }

            proc.init();
            proc.run();

        }

        public static void runNodeInAnotherThread(IPlatformSupport platform, GMQueue gmq, INodeFactory factory)
        {
            ThreadStartInfo info = new ThreadStartInfo();

            info.platform = platform;
            info.gmq = gmq;
            info.factories = new INodeFactory[] { factory };

            Thread th = new Thread(nodeThreadQueueMain);
            th.Start(info);
        }

        public static void runNodeInThisThread(IPlatformSupport platform, GMQueue gmq, INodeFactory factory)
        {
            ThreadStartInfo info = new ThreadStartInfo();

            info.platform = platform;
            info.gmq = gmq;
            info.factories = new INodeFactory[] { factory };

            nodeThreadQueueMain(info);
        }
    }



} // namespace globalmq
