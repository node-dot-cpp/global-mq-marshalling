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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Text;

namespace globalmq.marshalling
{
    public class StateSubscriberData
    {
        public UInt64 IdInPool; // for indexing purposes
        public UInt64 ref_id_at_subscriber;

        public StateSubscriberData(UInt64 IdInPool, UInt64 ref_id_at_subscriber)
        {
            this.IdInPool = IdInPool;
            this.ref_id_at_subscriber = ref_id_at_subscriber;
        }
    };

    public interface StatePublisherBase
    {
        int idx { get; set; } // for use in pools, etc
        void generateStateSyncMessage(IPublishableComposer composer);
        void startTick(IPublishableComposer composer);
        IPublishableComposer endTick();
        String statePublisherName();
        UInt64 stateTypeID();
    };

    public class StatePublisherData
    {

        public StatePublisherBase publisher = null;
        public List<StateSubscriberData> subscribers = new List<StateSubscriberData>();
        public int idx; // in pool

        public StatePublisherData(int idx_, StatePublisherBase publisher_)
        {
            Debug.Assert(publisher_ != null);
            this.publisher = publisher_;
            this.idx = idx_;
            publisher.idx = idx_;
            //BufferT buff; // just empty
            //publisher.startTick(buff);
        }
        public void setPublisher(StatePublisherBase publisher_)
        {
            Debug.Assert(this.publisher == null);
            Debug.Assert(publisher_ != null);
            this.publisher = publisher_;
            publisher.idx = idx;
            //BufferT buff; // just empty
            //publisher.startTick(buff);
        }
        public void setUnused(StatePublisherBase publisher_)
        {
            Debug.Assert(this.publisher == publisher_);
            this.publisher = null;
        }
        public bool isUsed() { return publisher != null; }
        public UInt64 stateTypeID()
        {
            return publisher.stateTypeID();
        }

        // processing requests (by now they seem to be independent on state wrappers)
        public int onSubscriptionRequest(UInt64 IdInPool, UInt64 ref_id_at_subscriber)
        {
            // TODO: who will check uniqueness?
            subscribers.Add(new StateSubscriberData(IdInPool, ref_id_at_subscriber));
            return subscribers.Count - 1;
        }
        public void generateStateSyncMessage(IPublishableComposer composer)
        {
            publisher.generateStateSyncMessage(composer);
        }
        public BufferT getStateUpdateBuff() { return publisher.endTick().getBuffer(); }
        public void startTick(IPublishableComposer composer) { publisher.startTick(composer); }
        BufferT endTick() { return publisher.endTick().getBuffer(); }
    };

    class StatePublisherPool
    {
        IPlatformSupport platform = null;
        GMQTransportBase transport = null;

        List<StatePublisherData> publishers = new List<StatePublisherData>();
        Dictionary<string, StatePublisherData> name2publisherMapping = new Dictionary<string, StatePublisherData>();
        Dictionary<UInt64, (int, int)> ID2PublisherAndItsSubscriberMapping = new Dictionary<UInt64, (int, int)>();
        UInt64 publisherAndItsSubscriberBase = 0;

        public int add(StatePublisherBase publisher)
        {
            for (int i = 0; i < publishers.Count; ++i)
            {
                if (!publishers[i].isUsed())
                {
                    publishers[i].setPublisher(publisher);
                    /*auto ins = */
                    name2publisherMapping.Add(publisher.statePublisherName(), (publishers[i]));
                    //assert(ins.second); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
                    BufferT buff2 = platform.makeBuffer();
                    IPublishableComposer composer2 = platform.makePublishableComposer(buff2);
                    publisher.startTick(composer2);
                    
                    return i;
                }
            }
            StatePublisherData data = new StatePublisherData(publishers.Count, publisher);
            publishers.Add(data);
            /*auto ins = */
            name2publisherMapping.Add(publisher.statePublisherName(), data);
            //assert(ins.second); // this should never happen as all names are distinct and we assume only a single state of a particular type in a given pool
            BufferT buff = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buff);
            publisher.startTick(composer);

            return publishers.Count - 1;
        }
        public void remove(StatePublisherBase publisher)
        {
            bool res = name2publisherMapping.Remove(publisher.statePublisherName());
            Debug.Assert(res);
            Debug.Assert(publisher.idx < publishers.Count);
            foreach (StateSubscriberData subscriber in publishers[publisher.idx].subscribers)
            {
                res = ID2PublisherAndItsSubscriberMapping.Remove(subscriber.IdInPool);
                Debug.Assert(res);
            }
            publishers[publisher.idx].setUnused(publisher);
        }

        public void setTransport(GMQTransportBase tr) { transport = tr; }
        public void setPlatform(IPlatformSupport pl) { platform = pl; }

        public void onMessage(IPublishableParser parser)
        {
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            GMQueue.helperParsePublishableStateMessageBegin(parser, ref mh);
            switch (mh.type)
            {
                case PublishableStateMessageHeader.MsgType.subscriptionRequest:
                    {
                        GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
                        pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
                        bool pathOK = GmqPathHelper.parse(mh.path, pc);
                        if (!pathOK)
                            throw new Exception(); // TODO: ... (bad path)
                        string publisherName = pc.statePublisherOrConnectionType;

                        StatePublisherData findres = name2publisherMapping[publisherName];
                        //if (findres == name2publisherMapping.end())
                        //	throw std::exception(); // not found / misdirected

                        UInt64 id = ++publisherAndItsSubscriberBase;
                        int refIdAtPublisher = findres.onSubscriptionRequest(id, mh.ref_id_at_subscriber);
                        /*auto ret = */
                        ID2PublisherAndItsSubscriberMapping[id] = (findres.idx, refIdAtPublisher);
                        //assert(ret.second);

                        PublishableStateMessageHeader hdrBack = new PublishableStateMessageHeader();
                        hdrBack.type = PublishableStateMessageHeader.MsgType.subscriptionResponse;
                        hdrBack.state_type_id_or_direction = findres.stateTypeID();
                        hdrBack.priority = mh.priority;
                        hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
                        hdrBack.ref_id_at_publisher = id;

                        BufferT msgBack = platform.makeBuffer();
                        IPublishableComposer composer = platform.makePublishableComposer(msgBack);
                        GMQueue.helperComposePublishableStateMessageBegin(composer, hdrBack);
                        findres.generateStateSyncMessage(composer);
                        GMQueue.helperComposePublishableStateMessageEnd(composer);
                        //assert(transport != nullptr);
                        transport.postMessage(msgBack);

                        break;
                    }
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            GMQueue.helperParsePublishableStateMessageEnd(parser);
        }

        public void postAllUpdates()
        {
            foreach (StatePublisherData publisher in publishers)
            {
                PublishableStateMessageHeader mhBase = new PublishableStateMessageHeader();
                mhBase.type = PublishableStateMessageHeader.MsgType.stateUpdate;
                mhBase.state_type_id_or_direction = publisher.stateTypeID();
                mhBase.priority = 0; // TODO: source
                mhBase.ref_id_at_subscriber = 0; // later
                mhBase.ref_id_at_publisher = 0; // later

                BufferT stateUpdateBuff = publisher.getStateUpdateBuff();

                BufferT msgBase = platform.makeBuffer();
                IPublishableComposer composer = platform.makePublishableComposer(msgBase);
                GMQueue.helperComposePublishableStateMessageBegin(composer, mhBase);
                ReadIteratorT riter = stateUpdateBuff.getReadIterator();
                composer.appendRaw(riter);
                GMQueue.helperComposePublishableStateMessageEnd(composer);

                foreach (StateSubscriberData subscriber in publisher.subscribers)
                {
                    PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                    ud.ref_id_at_publisher = subscriber.IdInPool;
                    ud.update_ref_id_at_publisher = true;
                    ud.update_ref_id_at_subscriber = true;
                    //				ud.ref_id_at_subscriber = subscriber.info.ref_id_at_subscriber;
                    ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
                    BufferT msgForward = platform.makeBuffer();
                    GMQueue.helperParseAndUpdatePublishableStateMessage(platform, msgBase, msgForward, ud);

                    //				PlatformSupportT::sendMsgFromPublisherToSubscriber( msgForward, subscriber.address.nodeAddr );
                    //assert(transport != nullptr);
                    transport.postMessage(msgForward);
                }
                BufferT newBuff = platform.makeBuffer(); // just empty
                IPublishableComposer composerNew = platform.makePublishableComposer(newBuff);
                publisher.startTick(composerNew);
            }
        }
    };

    public interface StateSubscriberBase
    {
        //void applyGmqMessageWithUpdates(IPublishableParser parser);
        //void applyJsonMessageWithUpdates(IPublishableParser parser);
        //void applyGmqStateSyncMessage(IPublishableParser parser);
        //void applyJsonStateSyncMessage(IPublishableParser parser);

        void applyMessageWithUpdates(IPublishableParser parser);
        void applyStateSyncMessage(IPublishableParser parser);
        String stateSubscriberName();
        UInt64 stateTypeID();
    };

    class StateSubscriberPool
    {

        class Subscriber
        {
            public StateSubscriberBase subscriber;
            public ulong ref_id_at_subscriber;
            public ulong ref_id_at_publisher;
        };

        List<Subscriber> subscribers = new List<Subscriber>(); // TODO: consider mapping ID -> ptr, if states are supposed to be added and removede dynamically

        GMQTransportBase transport = null;
        IPlatformSupport platform = null;

        public void add(StateSubscriberBase subscriber)
        {
            // TODO: revise for performance
            Subscriber s = new Subscriber();
            s.subscriber = subscriber;
            s.ref_id_at_subscriber = (ulong)subscribers.Count;
            s.ref_id_at_publisher = 0;
            subscribers.Add(s);
        }
        public void remove(StateSubscriberBase subscriber)
        {
            for (int i = 0; i < subscribers.Count; ++i)
            {
                if (subscribers[i].subscriber == subscriber)
                {
                    // TODO: if unsubscribeMessaage is to be sent, it's probably the right place
                    subscribers.RemoveAt(i);
                    return;
                }
            }
            Debug.Assert(false); // not found
        }

        public void setTransport(GMQTransportBase tr) { transport = tr; }
        public void setPlatform(IPlatformSupport pl) { platform = pl; }

        public void subscribe(StateSubscriberBase subscriber, string path)
        {
            for (int i = 0; i < subscribers.Count; ++i)
                if (subscribers[i].subscriber == subscriber)
                {
                    BufferT buff = platform.makeBuffer();
                    IPublishableComposer composer = platform.makePublishableComposer(buff);
                    PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
                    mh.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
                    mh.priority = 0; // TODO: source
                    mh.state_type_id_or_direction = subscriber.stateTypeID();
                    mh.path = path;
                    Debug.Assert(subscribers[i].ref_id_at_subscriber == (ulong)i);
                    mh.ref_id_at_subscriber = subscribers[i].ref_id_at_subscriber;
                    GMQueue.helperComposePublishableStateMessageBegin(composer, mh);
                    GMQueue.helperComposePublishableStateMessageEnd(composer);
                    //assert(transport != nullptr);
                    transport.postMessage(buff);
                    return;
                }
            Debug.Assert(false); // not found
        }
        public void onMessage(IPublishableParser parser)
        {
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            GMQueue.helperParsePublishableStateMessageBegin(parser, ref mh);
            switch (mh.type)
            {
                case PublishableStateMessageHeader.MsgType.subscriptionResponse:
                    //if (mh.ref_id_at_subscriber >= subscribers.size())
                    //	throw std::exception(); // TODO: ... (invalid ID)
                    subscribers[(int)(mh.ref_id_at_subscriber)].ref_id_at_publisher = mh.ref_id_at_publisher;
                    //if constexpr(ParserT::proto == globalmq::marshalling::Proto::JSON)
                    //	subscribers[mh.ref_id_at_subscriber].subscriber->applyJsonStateSyncMessage(parser);
                    //else
                    //{
                    //	static_assert(ParserT::proto == globalmq::marshalling::Proto::GMQ);
                    //	subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqStateSyncMessage(parser);
                    //}
                    subscribers[(int)mh.ref_id_at_subscriber].subscriber.applyStateSyncMessage(parser);
                    break;
                case PublishableStateMessageHeader.MsgType.stateUpdate:
                    //if (mh.ref_id_at_subscriber >= subscribers.size())
                    //	throw std::exception(); // TODO: ... (invalid ID)
                    // TODO: consider the following:
                    //if ( subscribers[mh.ref_id_at_subscriber].ref_id_at_publisher != mh.ref_id_at_publisher )
                    //	throw std::exception(); // TODO: ... (invalid source)
                    //if constexpr(ParserT::proto == globalmq::marshalling::Proto::JSON)
                    //	subscribers[mh.ref_id_at_subscriber].subscriber->applyJsonMessageWithUpdates(parser);
                    //else
                    //{
                    //	static_assert(ParserT::proto == globalmq::marshalling::Proto::GMQ);
                    //	subscribers[mh.ref_id_at_subscriber].subscriber->applyGmqMessageWithUpdates(parser);
                    //}
                    subscribers[(int)mh.ref_id_at_subscriber].subscriber.applyMessageWithUpdates(parser);
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            GMQueue.helperParsePublishableStateMessageEnd(parser);
        }
    };

    public interface ClientPoolT
    {
        void postMessage(UInt64 connID, BufferT msgBuff);
        void connect(UInt64 connID, string path);

    }

    public interface ServerPoolT
    {
        void postMessage(UInt64 connID, BufferT msgBuff);
    }


    public abstract class ClientConnectionBase
    {
        public enum Status { uninitialized = 0, addedToPool, connRequestSent, connected, removedFromPool }; // TODO: add states as necessary; consider making specific to a particular connection
        Status status = Status.uninitialized;

        ClientPoolT pool = null;
        UInt64 connID = 0;
        public UInt64 getConnID() { return connID; }


        public void addedToPool(ClientPoolT pool, UInt64 connID)
        {
            this.pool = pool;
            this.connID = connID;
            this.status = Status.addedToPool;
        }
        void setServerConnected(ClientPoolT pool, UInt64 connID)
        {
            this.pool = pool;
            this.connID = connID;
            this.status = Status.connected;
        }

        public void setRemovedFromPool()
        {
            this.status = Status.removedFromPool;
        }
        public void setRequestSent()
        {
            this.status = Status.connRequestSent;
        }

        public void setConnected()
        {
            if (status != Status.connRequestSent)
                throw new Exception(); // well, we have not requested connection yet

            status = Status.connected;
        }
        public bool isConnected() { return status == Status.connected; }
        public void postMessage(BufferT buff)
        {
            Debug.Assert(isConnected());
            //assert(pool != nullptr);
            pool.postMessage(connID, buff);
        }

        public void connect(string path)
        {
            Debug.Assert(status == Status.addedToPool);
            pool.connect(connID, path);
        }
        public virtual void onConnectionAccepted() { }
        public abstract void onMessage(ReadIteratorT parser);

        //virtual ~ClientConnectionBase()
        //{
        //	// TODO: check status for being disconnected
        //}
    };



    public abstract class ServerConnectionBase
    {
        public enum Status { uninitialized = 0, connected }; // TODO: add states as necessary; consider making specific to a particular connection
        Status status = Status.uninitialized;

        ServerPoolT pool = null;
        UInt64 connID = 0;
        public UInt64 getConnID() { return connID; }


        public void setServerConnected(ServerPoolT pool, UInt64 connID)
        {
            this.pool = pool;
            this.connID = connID;
            this.status = Status.connected;
        }

        public bool isConnected() { return status == Status.connected; }
        public abstract void onMessage(IPublishableParser parser);
        public void postMessage(BufferT buff)
        {
            Debug.Assert(isConnected());
            //assert(pool != nullptr);
            pool.postMessage(connID, buff);
        }

    };

    public class ClientSimpleConnectionPool : ClientPoolT
    {
        //protected:
        //using BufferT = typename PlatformSupportT::BufferT;
        //using ParserT = typename PlatformSupportT::ParserT;
        //using ComposerT = typename PlatformSupportT::ComposerT;
        //using ConnectionT = globalmq::marshalling::ClientConnectionBase<PlatformSupportT>;

        class ClientConnection
        {
            public ClientConnectionBase connection;
            public UInt64 ref_id_at_server;
            public UInt64 ref_id_at_client; // that is, local id
        };

        Dictionary<UInt64, ClientConnection> connections = new Dictionary<UInt64, ClientConnection>();
        UInt64 connIdxBase = 0;

        GMQTransportBase transport = null;
        IPlatformSupport platform = null;

        public void setTransport(GMQTransportBase tr) { transport = tr; }
        public void setPlatform(IPlatformSupport pl) { platform = pl; }

        public UInt64 add(ClientConnectionBase connection) // returns connection ID
        {
            // TODO: revise for performance
            ClientConnection cc = new ClientConnection();
            cc.connection = connection;
            cc.ref_id_at_client = ++connIdxBase;
            cc.ref_id_at_server = 0;
            /*auto ins = */
            connections.Add(cc.ref_id_at_client, cc);
            //assert(ins.second);
            connection.addedToPool(this, cc.ref_id_at_client);
            return cc.ref_id_at_client;
        }
        public void remove(ClientConnectionBase connection)
        {
            connections.Remove(connection.getConnID());
            connection.setRemovedFromPool();
        }

        public void connect(UInt64 connID, string path)
        {
            ClientConnection conn = connections[connID];
            //assert(f != connections.end());
            //auto & conn = f->second;
            Debug.Assert(conn.ref_id_at_client == connID); // self-consistency
            BufferT buff = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buff);
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            mh.type = PublishableStateMessageHeader.MsgType.connectionRequest;
            mh.priority = 0; // TODO: source
            mh.state_type_id_or_direction = (UInt64)PublishableStateMessageHeader.ConnMsgDirection.toServer;
            mh.path = path;
            Debug.Assert(conn.ref_id_at_client == connID);
            mh.ref_id_at_subscriber = conn.ref_id_at_client;
            GMQueue.helperComposePublishableStateMessageBegin(composer, mh);
            GMQueue.helperComposePublishableStateMessageEnd(composer);
            transport.postMessage(buff);
            conn.connection.setRequestSent();
        }

        public void postMessage(UInt64 connID, BufferT msgBuff)
        {
            //auto f = connections.find(connID);
            //assert(f != connections.end());
            //auto & conn = f->second;
            ClientConnection conn = connections[connID];
            Debug.Assert(conn.ref_id_at_client == connID); // self-consistency
            BufferT buff = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buff);
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            mh.type = PublishableStateMessageHeader.MsgType.connectionMessage;
            mh.priority = 0; // TODO: source
            mh.state_type_id_or_direction = (UInt64)PublishableStateMessageHeader.ConnMsgDirection.toServer;
            Debug.Assert(conn.ref_id_at_client == connID);
            mh.ref_id_at_subscriber = conn.ref_id_at_client;
            Debug.Assert(conn.ref_id_at_server != 0);
            mh.ref_id_at_publisher = conn.ref_id_at_server;
            GMQueue.helperComposePublishableStateMessageBegin(composer, mh);
            ReadIteratorT riter = msgBuff.getReadIterator();

            composer.appendRaw(riter);

            GMQueue.helperComposePublishableStateMessageEnd(composer);
            //assert(transport != nullptr);
            transport.postMessage(buff);
        }

        public void onMessage(IPublishableParser parser)
        {
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            GMQueue.helperParsePublishableStateMessageBegin(parser, ref mh);
            switch (mh.type)
            {
                case PublishableStateMessageHeader.MsgType.connectionAccepted:
                    {
                        //auto f = connections.find(mh.ref_id_at_subscriber);
                        //if (f == connections.end())
                        //	throw std::exception();
                        //auto & conn = f->second;
                        ClientConnection conn = connections[mh.ref_id_at_subscriber];
                        Debug.Assert(conn.ref_id_at_client == mh.ref_id_at_subscriber); // self-consistency
                        conn.ref_id_at_server = mh.ref_id_at_publisher;
                        //if (conn.connection->status != ConnectionT::Status::connRequestSent)
                        //	throw std::exception(); // well, we have not requested connection yet
                        //conn.connection->status = ConnectionT::Status::connected;
                        conn.connection.setConnected();
                        conn.connection.onConnectionAccepted();
                        break;
                    }
                case PublishableStateMessageHeader.MsgType.connectionMessage:
                    {
                        Debug.Assert(mh.state_type_id_or_direction == (ulong)PublishableStateMessageHeader.ConnMsgDirection.toClient);
                        //auto f = connections.find(mh.ref_id_at_subscriber);
                        //if (f == connections.end())
                        //	throw std::exception();
                        //auto & conn = f->second;
                        ClientConnection conn = connections[mh.ref_id_at_subscriber];
                        if (conn.connection.isConnected())
                            throw new Exception(); // TODO: revise
                        Debug.Assert(conn.ref_id_at_server == mh.ref_id_at_publisher); // self-consistency
                                                                                       //ReadIteratorT riter = parser.getIterator();
                        conn.connection.onMessage(parser.getIterator());
                        //parser = riter;
                        break;
                    }
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            GMQueue.helperParsePublishableStateMessageEnd(parser);
        }
    };

    public interface IConnectionFactory
    {
        ServerConnectionBase create();
    };

    class ServerSimpleConnectionPool : ServerPoolT
    {
        //	protected:
        //using BufferT = typename PlatformSupportT::BufferT;
        //using ParserT = typename PlatformSupportT::ParserT;
        //using ComposerT = typename PlatformSupportT::ComposerT;
        //using ConnectionT = globalmq::marshalling::ServerConnectionBase<PlatformSupportT>;
        //using OwningPtrToConnection = typename PlatformSupportT::template OwningPtrT<ServerConnectionBase<PlatformSupportT>>;

        class ServerConnection
        {
            public ServerConnectionBase connection;
            public UInt64 ref_id_at_server = 0; // that is, local id
            public UInt64 ref_id_at_client = 0;
        };

        Dictionary<UInt64, ServerConnection> connections = new Dictionary<ulong, ServerConnection>();
        UInt64 connIdxBase = 0;

        Dictionary<string, IConnectionFactory> connFactories = new Dictionary<string, IConnectionFactory>();
        GMQTransportBase transport = null;
        IPlatformSupport platform = null;

        //public:
        //ServerSimpleConnectionPool() { }
        //~ServerSimpleConnectionPool() { }

        public void addSimpleConnectionFactory(IConnectionFactory connFactory, string name)
        {
            Debug.Assert(connFactory != null);
            /*auto ins = */
            connFactories.Add(name, connFactory);
            //assert(ins.second);
        }
        public void setTransport(GMQTransportBase tr) { transport = tr; }
        public void setPlatform(IPlatformSupport pl) { platform = pl; }

        public void postMessage(UInt64 connID, BufferT msgBuff)
        {
            //auto f = connections.find(connID);
            //assert(f != connections.end());
            //auto & conn = f->second;
            ServerConnection conn = connections[connID];
            Debug.Assert(conn.ref_id_at_server == connID); // self-consistency
            BufferT buff = platform.makeBuffer();
            IPublishableComposer composer = platform.makePublishableComposer(buff);
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            mh.type = PublishableStateMessageHeader.MsgType.connectionMessage;
            mh.priority = 0; // TODO: source
            mh.state_type_id_or_direction = (ulong)PublishableStateMessageHeader.ConnMsgDirection.toClient;
            mh.ref_id_at_subscriber = conn.ref_id_at_client;
            mh.ref_id_at_publisher = conn.ref_id_at_server;
            GMQueue.helperComposePublishableStateMessageBegin(composer, mh);

            composer.appendRaw(msgBuff.getReadIterator());

            GMQueue.helperComposePublishableStateMessageEnd(composer);
            //assert(transport != nullptr);
            transport.postMessage(buff);
        }

        public void onMessage(IPublishableParser parser)
        {
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            GMQueue.helperParsePublishableStateMessageBegin(parser, ref mh);
            switch (mh.type)
            {
                case PublishableStateMessageHeader.MsgType.connectionRequest:
                    {
                        GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
                        pc.type = PublishableStateMessageHeader.MsgType.connectionRequest;
                        bool pathOK = GmqPathHelper.parse(mh.path, pc);
                        if (!pathOK)
                            throw new Exception(); // TODO: ... (bad path)

                        //auto f = connFactories.find(pc.statePublisherOrConnectionType);
                        //if (f == connFactories.end())
                        //	throw std::exception(); // TODO:  ... (no factory for conn name)
                        //auto & connFactory = *f->second;
                        IConnectionFactory connFactory = connFactories[pc.statePublisherOrConnectionType];

                        ServerConnection sc = new ServerConnection();
                        sc.connection = connFactory.create();
                        sc.ref_id_at_client = mh.ref_id_at_subscriber;
                        sc.ref_id_at_server = ++connIdxBase;
                        //sc.connection->pool = this;
                        //sc.connection->connID = sc.ref_id_at_server;
                        //sc.connection->status = ConnectionT::Status::connected;
                        sc.connection.setServerConnected(this, sc.ref_id_at_server);
                        /*auto ins = */
                        connections.Add(sc.ref_id_at_server, sc);
                        //assert(ins.second);

                        PublishableStateMessageHeader hdrBack = new PublishableStateMessageHeader();
                        hdrBack.type = PublishableStateMessageHeader.MsgType.connectionAccepted;
                        hdrBack.state_type_id_or_direction = (ulong)PublishableStateMessageHeader.ConnMsgDirection.toClient;
                        hdrBack.priority = mh.priority; // TODO: source?
                        hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
                        hdrBack.ref_id_at_publisher = sc.ref_id_at_server;

                        BufferT msgBack = platform.makeBuffer();
                        IPublishableComposer composer = platform.makePublishableComposer(msgBack);
                        GMQueue.helperComposePublishableStateMessageBegin(composer, hdrBack);
                        GMQueue.helperComposePublishableStateMessageEnd(composer);
                        //assert(transport != nullptr);
                        transport.postMessage(msgBack);

                        break;
                    }
                case PublishableStateMessageHeader.MsgType.connectionMessage:
                    {
                        Debug.Assert(mh.state_type_id_or_direction == (ulong)PublishableStateMessageHeader.ConnMsgDirection.toServer);
                        //auto f = connections.find(mh.ref_id_at_publisher);
                        //if (f == connections.end())
                        //	throw std::exception();
                        //auto & conn = f->second;
                        ServerConnection conn = connections[mh.ref_id_at_publisher];
                        Debug.Assert(conn.ref_id_at_server == mh.ref_id_at_publisher); // self-consistency
                                                                                       //auto riter = parser.getIterator();
                        conn.connection.onMessage(parser);
                        //parser = riter;
                        break;
                    }
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            GMQueue.helperParsePublishableStateMessageEnd(parser);
        }
    };

    public class MetaPool
    {
        //using BufferT = typename PlatformSupportT::BufferT;
        //using ParserT = typename PlatformSupportT::ParserT;
        //using ComposerT = typename PlatformSupportT::ComposerT;
        //using StateSubscriberT = globalmq::marshalling::StateSubscriberBase<BufferT>;
        //using StatePublisherT = globalmq::marshalling::StatePublisherBase<BufferT>;
        //	using ConnectionT = globalmq::marshalling::ConnectionBase<BufferT>;
        IPlatformSupport platform = null;
        StatePublisherPool pubPool = new StatePublisherPool();
        StateSubscriberPool subPool = new StateSubscriberPool();
        ClientSimpleConnectionPool cliPool = new ClientSimpleConnectionPool();
        ServerSimpleConnectionPool srvPool = new ServerSimpleConnectionPool();

        public void setTransport(GMQTransportBase tr)
        {
            pubPool.setTransport(tr);
            subPool.setTransport(tr);
            cliPool.setTransport(tr);
            srvPool.setTransport(tr);
        }
        public void setPlatform(IPlatformSupport pl)
        {
            this.platform = pl;
            pubPool.setPlatform(pl);
            subPool.setPlatform(pl);
            cliPool.setPlatform(pl);
            srvPool.setPlatform(pl);
        }


        void addSimpleConnectionFactory(IConnectionFactory connFactory, string name)
        {
            srvPool.addSimpleConnectionFactory(connFactory, name);
        }


        public void add(StatePublisherBase obj)
        {
            pubPool.add(obj);
        }

        public void add(StateSubscriberBase obj)
        {
            subPool.add(obj);
        }
        void add(ClientConnectionBase obj)
        {
            cliPool.add(obj);
        }

        public void remove(StatePublisherBase obj)
        {
            pubPool.remove(obj);
        }

        public void remove(StateSubscriberBase obj)
        {
            subPool.remove(obj);
        }
        void remove(ClientConnectionBase obj)
        {
            cliPool.remove(obj);
        }
        public void subscribe(StateSubscriberBase subscriber, string path)
        {
            subPool.subscribe(subscriber, path);
        }

        void onMessage(IPublishableParser parser)
        {
            ReadIteratorT riter = parser.getIterator().shallowClone();

            IPublishableParser parser1 = platform.makePublishableParser(riter);
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            GMQueue.helperParsePublishableStateMessageBegin(parser1, ref mh);
            switch (mh.type)
            {
                case PublishableStateMessageHeader.MsgType.subscriptionResponse:
                case PublishableStateMessageHeader.MsgType.stateUpdate:
                    subPool.onMessage(parser);
                    break;
                case PublishableStateMessageHeader.MsgType.subscriptionRequest:
                    pubPool.onMessage(parser);
                    break;
                case PublishableStateMessageHeader.MsgType.connectionRequest:
                    srvPool.onMessage(parser);
                    break;
                case PublishableStateMessageHeader.MsgType.connectionAccepted:
                    cliPool.onMessage(parser);
                    break;
                case PublishableStateMessageHeader.MsgType.connectionMessage:
                    switch ((PublishableStateMessageHeader.ConnMsgDirection)mh.state_type_id_or_direction)
                    {
                        case PublishableStateMessageHeader.ConnMsgDirection.toClient:
                            cliPool.onMessage(parser);
                            break;
                        case PublishableStateMessageHeader.ConnMsgDirection.toServer:
                            srvPool.onMessage(parser);
                            break;
                        default:
                            throw new Exception(); // TODO: ... (unknown msg type)
                    }
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
        }

        public void onMessage(BufferT buffer)
        {
            ReadIteratorT riter = buffer.getReadIterator();
            IPublishableParser parser = platform.makePublishableParser(riter);
            onMessage(parser);
        }

        public void postAllUpdates()
        {
            pubPool.postAllUpdates();
        }

    }
} //namespace globalmq.marshalling