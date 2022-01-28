/* -------------------------------------------------------------------------------
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


    public class ThreadQueueItem
    {
        const int invalidRecipientID = int.MaxValue;
        public BufferT msg;
        int recipientID = invalidRecipientID;
        public ThreadQueueItem() { }
        public ThreadQueueItem(BufferT msg_, int recipientID_)
        {
            this.msg = msg_;
            this.recipientID = recipientID_;
        }
    };

    class CircularBuffer<T>
    {
        const int bufsz = 32;
        const int maxsz = bufsz - 1;
        int head = 0;
        int tail = 0;
        T[] buffer = new T[bufsz];

        public int size()
        {
            return (head >= tail) ? head - tail : head - tail + bufsz;
        }

        public bool is_full() { return size() == maxsz; }

        public void push_back(T t)
        {
            Debug.Assert(size() < maxsz);
            buffer[head] = t;
            head = (head + 1) % bufsz;
        }

        public T pop_front()
        {
            Debug.Assert(size() > 0);
            T ret = buffer[tail];
            buffer[tail] = default(T);
            tail = (tail + 1) % bufsz;
            return ret;
        }
    };

    //mb: this implementation is very basic, just for first tests.
    public class BasicMtQueue<T>
    {
        object mx = new object();
        CircularBuffer<T> coll = new CircularBuffer<T>();
        bool killflag = false;

        //stats:
        int nfulls = 0;
        int hwmsize = 0;//high watermark on queue size

        public void push_back(T it)
        {
            //if the queue is full, BLOCKS until some space is freed
            lock (mx)
            {
                while (coll.is_full() && !killflag)
                {
                    Monitor.Wait(mx);
                    ++nfulls;
                }

                if (killflag)
                    return;

                Debug.Assert(!coll.is_full());
                coll.push_back(it);
                int sz = coll.size();
                hwmsize = Math.Max(hwmsize, sz);

                Monitor.Pulse(mx);
            }//unlocking mx
        }

        public int pop_front(T[] messages, int count)
        {
            lock (mx)
            {
                if (coll.size() == 0 || killflag)
                    return 0;

                Debug.Assert(coll.size() > 0);
                Debug.Assert(count <= messages.Length);
                int sz2move = count <= coll.size() ? count : coll.size();
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
                killflag = true;
                Monitor.Pulse(mx);
            }//unlocking mx
        }
    };

    public class ThreadQueuePostman : InProcessMessagePostmanBase
    {
        BasicMtQueue<ThreadQueueItem> msgQueue;
        int recipientID;

        public ThreadQueuePostman(BasicMtQueue<ThreadQueueItem> msgQueue_, int recipientID_)
        {
            this.msgQueue = msgQueue_;
            this.recipientID = recipientID_;
        }
        public void postMessage(BufferT msg)
        {
            msgQueue.push_back(new ThreadQueueItem(msg, recipientID));
        }
    };

    public class GMQThreadQueueTransport : GMQTransportBase
    {

        public GMQThreadQueueTransport(GMQueue gmq, string name, BasicMtQueue<ThreadQueueItem> queue, int recipientID) : base(gmq, name, new ThreadQueuePostman(queue, recipientID)) { }
        public GMQThreadQueueTransport(GMQueue gmq, BasicMtQueue<ThreadQueueItem> queue, int recipientID) : base(gmq, new ThreadQueuePostman(queue, recipientID)) { }
    };

} // namespace globalmq
