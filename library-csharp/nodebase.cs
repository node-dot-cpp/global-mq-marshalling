/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
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


    public abstract class NodeBase
    {
        MetaPool pool = new MetaPool();
        IPlatformSupport platform = null;
        ITransport transport = null;
        public MetaPool Pool { get { return pool; } }
        public abstract string getNodeName();
        public IPlatformSupport Platform { get { return platform; } }
        public void initPlatform(IPlatformSupport pt)
        {
            Debug.Assert(platform == null);
            this.platform = pt;
            pool.setPlatform(pt);
        }
        public ITransport Transport { get { return transport; } }
        public void initTransport(ITransport tr)
        {
            Debug.Assert(transport == null);
            this.transport = tr;
            pool.setTransport(tr);
        }
        public void onGlobalMQMessage(BufferT msg )
        {
            pool.onMessage(msg);
            // GlobalMQ: at the end of each handler cause pools to post all updates
            pool.postAllUpdates();
        }


        public void processMessagesAndOrTimeout(BufferT msg)
        {
            //timeoutManager = &(getTimeout());
            //inmediateQueue = &(getInmediateQueue());

            //EvQueue queue;
            //uint64_t now = infraGetCurrentTime();
            //timeout.infraTimeoutEvents(now, queue);
            //queue.emit();

            if (msg != null)
            {
                onGlobalMQMessage(msg);
            }

            //emitInmediates();

//            bool refed = refedTimeout();
//            uint64_t nextTimeoutAt = nextTimeout();
//            now = infraGetCurrentTime();
//            //		int timeoutToUse = getPollTimeout(nextTimeoutAt, now);
//            uint64_t timeoutToUse = nextTimeoutAt == TimeOutNever ? TimeOutNever : (nextTimeoutAt < now ? 0 : nextTimeoutAt - now);

//            timeoutManager = nullptr;
//            inmediateQueue = nullptr;
//            safememory::detail::killAllZombies();
//#ifdef NODECPP_USE_IIBMALLOC
//		::nodecpp::iibmalloc::setCurrneAllocator(formerAlloc);
//#endif

            //nodecpp::nodeLocalData = nullptr;

            //if constexpr(NodeType::has_invariant_checker)
    
            dbgInvariantChecker();

        }

        public int getNextTimeout(int now) { return 200; }
        public virtual void init() { }

        public virtual void dbgInvariantChecker() { }
    }

    public interface INodeFactory
    {
        NodeBase create();
    }


} // namespace globalmq
