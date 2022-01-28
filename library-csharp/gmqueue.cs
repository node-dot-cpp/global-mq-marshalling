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
    public interface IPlatformSupport
    {
        BufferT makeBuffer();
        IPublishableComposer makePublishableComposer(BufferT buffer);
        IPublishableParser makePublishableParser(ReadIteratorT readIter);
        InProcessMessagePostmanBase allocPostman(Object[] args);

    }

    public class DefaultJsonPlatformSupport : IPlatformSupport
    {
        public BufferT makeBuffer() { return new SimpleBuffer(); }
        public IPublishableComposer makePublishableComposer(BufferT buffer) { return new JsonPublishableComposer(buffer); }
        public IPublishableParser makePublishableParser(ReadIteratorT readIter) { return new JsonPublishableParser(readIter); }
        public InProcessMessagePostmanBase allocPostman(Object[] args) { throw new NotImplementedException(); }
    }


    public struct PublishableStateMessageHeader
    {
        public enum MsgType { undefined = 0, subscriptionRequest = 1, subscriptionResponse = 2, stateUpdate = 3, connectionRequest = 4, connectionAccepted = 5, connectionMessage = 6 };
        public enum ConnMsgDirection { toServer = 0, toClient = 1 };
        public MsgType type;
        public UInt64 state_type_id_or_direction; // Note: may be removed in future versions
        public UInt64 priority;
        public string path;  // subscriptionRequest only
        public UInt64 ref_id_at_subscriber; // updatable
        public UInt64 ref_id_at_publisher; // updatable

        public struct UpdatedData
        {
            public UInt64 ref_id_at_subscriber;
            public UInt64 ref_id_at_publisher;
            public bool update_ref_id_at_subscriber;
            public bool update_ref_id_at_publisher;
        };
        public void parse(IPublishableParser parser)
        {
            parser.parsePublishableStructBegin("hdr");

            UInt64 msgType = parser.parseUnsigned("msg_type");
            switch ((MsgType)msgType)
            {
                case MsgType.subscriptionRequest:
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                    state_type_id_or_direction = parser.parseUnsigned("state_type_id");
                    break;
                case MsgType.connectionRequest:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    state_type_id_or_direction = parser.parseUnsigned("direction");
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            priority = parser.parseUnsigned("priority");
            type = (MsgType)(msgType);
            switch (type)
            {
                case MsgType.subscriptionRequest:
                case MsgType.connectionRequest:
                    path = parser.parseString("path");
                    ref_id_at_subscriber = parser.parseUnsigned("ref_id_at_subscriber");
                    break;
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    ref_id_at_subscriber = parser.parseUnsigned("ref_id_at_subscriber");
                    ref_id_at_publisher = parser.parseUnsigned("ref_id_at_publisher");
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            parser.parsePublishableStructEnd();
        }

        public static void parseAndUpdate(ReadIteratorT msgStart, IPublishableParser parser, IPublishableComposer composer, UpdatedData udata)
        {
            //		ParserT parser2 = parser;
            parser.parsePublishableStructBegin("hdr");
            //size_t msgType;
            UInt64 msgType = parser.parseUnsigned("msg_type");
            UInt64 dummy;
            string dummyStr;
            switch ((MsgType)msgType)
            {
                case MsgType.subscriptionRequest:
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                    dummy = parser.parseUnsigned("state_type_id");
                    break;
                case MsgType.connectionRequest:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    dummy = parser.parseUnsigned("direction");
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            dummy = parser.parseUnsigned("priority");
            switch ((MsgType)msgType)
            {
                case MsgType.subscriptionRequest:
                case MsgType.connectionRequest:
                    {
                        dummyStr = parser.parseString("path");
                        Debug.Assert(!udata.update_ref_id_at_publisher);

                        int offset = parser.getIterator().offset();
                        composer.appendRaw(msgStart, offset);

                        dummy = parser.parseUnsigned("ref_id_at_subscriber");
                        if (udata.update_ref_id_at_subscriber)
                            composer.composeUnsigned("ref_id_at_subscriber", udata.ref_id_at_subscriber, false);
                        else
                            composer.composeUnsigned("ref_id_at_subscriber", dummy, false);
                        break;
                    }
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    {
                        int offset = parser.getIterator().offset();
                        composer.appendRaw(msgStart, offset);

                        dummy = parser.parseUnsigned("ref_id_at_subscriber");
                        if (udata.update_ref_id_at_subscriber)
                            composer.composeUnsigned("ref_id_at_subscriber", udata.ref_id_at_subscriber, false);
                        else
                            composer.composeUnsigned("ref_id_at_subscriber", dummy, false);

                        dummy = parser.parseUnsigned("ref_id_at_publisher");
                        if (udata.update_ref_id_at_publisher)
                            composer.composeUnsigned("ref_id_at_publisher", udata.ref_id_at_publisher, false);
                        else
                            composer.composeUnsigned("ref_id_at_publisher", dummy, false);
                        break;
                    }
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            composer.appendRaw(parser.getIterator());
        }

        public void compose(IPublishableComposer composer, bool addSeparator)
        {
            composer.composePublishableStructBegin("hdr");
            composer.composeUnsigned("msg_type", (UInt32)type, true);
            switch (type)
            {
                case MsgType.subscriptionRequest:
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                    composer.composeUnsigned("state_type_id", state_type_id_or_direction, true);
                    break;
                case MsgType.connectionRequest:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    composer.composeUnsigned("direction", state_type_id_or_direction, true);
                    break;
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            composer.composeUnsigned("priority", priority, true);
            switch (type)
            {
                case MsgType.subscriptionRequest:
                case MsgType.connectionRequest:
                    {
                        composer.composeString("path", path, true);
                        composer.composeUnsigned("ref_id_at_subscriber", ref_id_at_subscriber, false);
                        break;
                    }
                case MsgType.subscriptionResponse:
                case MsgType.stateUpdate:
                case MsgType.connectionAccepted:
                case MsgType.connectionMessage:
                    {
                        composer.composeUnsigned("ref_id_at_subscriber", ref_id_at_subscriber, true);
                        composer.composeUnsigned("ref_id_at_publisher", ref_id_at_publisher, false);
                        break;
                    }
                default:
                    throw new Exception(); // TODO: ... (unknown msg type)
            }
            composer.composePublishableStructEnd(addSeparator);
        }

    }
    public class GmqPathHelper
    {
        /*
			(global)
			globalmq://example.com/node23?sp=pub1
			sp stands for State Publisher 
			or (another global, with different - future - resolution)
			globalmq://ourname!gmq:4567/somenode?sp=somepub 
			!gmq is a non-standard extension which means we'll resolve the name using our own means
			or (local)
			globalmq:/lobbynode?sp=worldlist
		*/
        public class PathComponents
        {
            public PublishableStateMessageHeader.MsgType type = PublishableStateMessageHeader.MsgType.undefined;
            public string authority;
            public bool furtherResolution = false;
            public bool hasPort = false;
            public UInt16 port = 0xFFFF;
            public string nodeName;
            public string statePublisherOrConnectionType; // for subscription request
        };

        public static string compose(PathComponents components)
        {
            // TODO: check components
            StringBuilder ret = new StringBuilder();
            ret.Append("globalmq:");
            if (components.authority != null)
            {
                ret.Append("//");
                ret.Append(components.authority);
            }
            if (components.furtherResolution)
                ret.Append("!gmq");
            if (components.hasPort)
            {
                //auto str = fmt::format(, components.port);
                ret.Append(":");
                ret.Append(components.port);
            }
            ret.Append('/');
            ret.Append(localPart(components));
            return ret.ToString();
        }

        public static string localPart(PathComponents components)
        {
            Debug.Assert(components.nodeName.Length != 0);
            Debug.Assert(components.statePublisherOrConnectionType.Length != 0);
            switch (components.type)
            {
                case PublishableStateMessageHeader.MsgType.subscriptionRequest:
                    return String.Format("{0}?sp={1}", components.nodeName, components.statePublisherOrConnectionType);
                case PublishableStateMessageHeader.MsgType.connectionRequest:
                    return String.Format("{0}?ct={1}", components.nodeName, components.statePublisherOrConnectionType);
                default:
                    throw new Exception();
            }
        }

        public static bool parse(string path, PathComponents components)
        {
            int pos = path.IndexOf("globalmq:");
            if (pos != 0)
                return false;
            pos += 9; /* "globalmq:" */
            if (path.Length <= pos)
                return false;
            if (path[pos++] != '/')
                return false;
            if (path[pos] == '/') // double-slash, authority component is present
            {
                ++pos;
                int pos2 = path.IndexOf('/', pos);
                if (pos2 == -1)
                    return false;
                components.authority = path.Substring(pos, pos2 - pos);
                pos = pos2 + 1;
                int pos3 = components.authority.LastIndexOf(':');
                if (pos3 != -1)
                {

                    //char* end = nullptr;
                    //size_t port = strtol(components.authority.c_str() + pos3 + 1, &end, 10);
                    string portStr = components.authority.Substring(pos3 + 1);
                    UInt16 port;
                    bool good = UInt16.TryParse(portStr, out port);

                    //if (components.authority.c_str() + pos3 + 1 == end)
                    //	return false;
                    //if (end < components.authority.c_str() + components.authority.size()) // there are remaining chars
                    //	return false;
                    //if (port >= UINT16_MAX)
                    //	return false;
                    if (!good)
                        return false;

                    components.hasPort = true;
                    components.port = port;
                    components.authority = components.authority.Substring(0, pos3);
                }

                else
                {
                    components.hasPort = false;
                    components.port = 0xFFFF;
                }

                int pos4 = components.authority.LastIndexOf('!');
                if (pos4 != -1)
                {
                    if (components.authority.Length - pos4 < 3 /* "gmq" */ )
                        return false;
                    if (components.authority.Substring(pos4 + 1) != "gmq")
                        return false;
                    components.furtherResolution = true;
                    components.authority = components.authority.Substring(0, pos4);
                }

                else
                {
                    components.furtherResolution = false;
                }
            }
            else
            {
                components.authority = "";
                components.hasPort = false;
                components.furtherResolution = false;
                components.port = 0xFFFF;
            }

            // node name
            int pos1 = path.IndexOf('?', pos);
            if (pos1 == -1)
                return false;
            components.nodeName = path.Substring(pos, pos1 - pos);

            int pos5 = pos1;
            // statePublisherOrConnectionType
            switch (components.type)
            {
                case PublishableStateMessageHeader.MsgType.subscriptionRequest:
                    pos5 = path.IndexOf("sp=", pos5);
                    if (pos5 == -1)
                        return false;
                    pos5 += 3; /* "sp=" */;
                    break;
                case PublishableStateMessageHeader.MsgType.connectionRequest:
                    pos5 = path.IndexOf("ct=", pos5);
                    if (pos5 == -1)
                        return false;
                    pos5 += 3; /* "sp=" */;
                    break;
                default:
                    throw new Exception();
            }
            //if (pos5 == GMQ_COLL string::npos )
            //			return false;
            int pos6 = path.IndexOf('&', pos5);
            if (pos6 == -1)
                components.statePublisherOrConnectionType = path.Substring(pos5);
            else
                components.statePublisherOrConnectionType = path.Substring(pos5, pos6 - pos5);
            return true;
        }
    };
    public interface IStateConcentrator
    {
        string stateSubscriberName();
        UInt64 stateTypeID();
        // as subscriber
        void applyMessageWithUpdates(IPublishableParser parser);
        void applyStateSyncMessage(IPublishableParser parser);
        // as publisher
        void generateStateSyncMessage(IPublishableComposer composer);
    };

    public interface IStateConcentratorFactory
    {
        IStateConcentrator createConcentrator(UInt64 typeID);

    }
    public interface InProcessMessagePostmanBase
    {
        void postMessage(BufferT buffer);
    };

    public class AddressableLocation
    {
        public InProcessMessagePostmanBase postman;
        public UInt64 reincarnation;

        public AddressableLocation(InProcessMessagePostmanBase postman, UInt64 reincarnation)
        {
            this.postman = postman;
            this.reincarnation = reincarnation;
        }
    };
    public struct SlotIdx
    {
        public static int invalid_idx = int.MaxValue;
        public static UInt64 invalid_reincarnation = UInt64.MaxValue;

        public int idx;// = invalid_idx;
        public UInt64 reincarnation;// = invalid_reincarnation;
        public bool isInitialized() { return idx != invalid_idx && reincarnation != invalid_reincarnation; }
        public void invalidate() { idx = invalid_idx; reincarnation = invalid_reincarnation; }
        public SlotIdx(int idx, UInt64 reincarnation)
        {
            this.idx = idx;
            this.reincarnation = reincarnation;
        }
        public static SlotIdx Invalid { get { return new SlotIdx(invalid_idx, invalid_reincarnation); } }
        public static bool operator ==(SlotIdx left, SlotIdx right)
        {
            return left.idx == right.idx &&
                left.reincarnation == right.reincarnation;
        }
        public static bool operator !=(SlotIdx left, SlotIdx right)
        {
            return !(left == right);
        }
        public override int GetHashCode()
        {
            // TODO
            throw new InvalidOperationException();
        }
        override public bool Equals(object other) { throw new NotImplementedException(); }
    }

    public class AddressableLocations // one per process; provides process-unique Slot with Postman and returns its SlotIdx
    {
        List<AddressableLocation> slots = new List<AddressableLocation>();
        public SlotIdx add(InProcessMessagePostmanBase postman)
        {
            // essentially add to slots and return its idx
            for (int i = 0; i < slots.Count; ++i)
                if (slots[i].postman == null)
                {
                    slots[i].postman = postman;
                    ++(slots[i].reincarnation);
                    return new SlotIdx(i, slots[i].reincarnation);
                }
            slots.Add(new AddressableLocation(postman, 0));
            return new SlotIdx(slots.Count - 1, 0);
        }
        public void remove(SlotIdx idx)
        {
            // find by idx.idx, check reincarnaion, set postman to null
            Debug.Assert(idx.idx < slots.Count);
            Debug.Assert(idx.reincarnation == slots[idx.idx].reincarnation);
            slots[idx.idx].postman = null;
        }
        public InProcessMessagePostmanBase getPostman(SlotIdx idx)
        {
            // access, verify, return
            Debug.Assert(idx.idx < slots.Count);
            Debug.Assert(idx.reincarnation == slots[idx.idx].reincarnation);
            return slots[idx.idx].postman;
        }
    }

    public class GMQueue
    {
        //using InputBufferT = typename PlatformSupportT::BufferT;
        //using ComposerT = typename PlatformSupportT::ComposerT;
        //using ParserT = typename PlatformSupportT::ParserT;

        AddressableLocations addressableLocations = new AddressableLocations();

        string myAuthority;
        bool isMyAuthority(string authority)
        {
            // TODO: regexp comparison (note: myAuthority could be '*', etc );
            return authority == myAuthority || authority.Length == 0;
        }

        class ConcentratorWrapper
        {
            //friend class GMQueue<PlatformSupportT>;

            IStateConcentrator ptr = null;
            bool subscriptionResponseReceived = false;
            UInt64 idAtPublisher; // one for all subscriber using this concentrator

            public struct SubscriberData
            {
                public UInt64 ref_id_at_subscriber;
                public UInt64 ref_id_at_publisher;
                public SlotIdx senderSlotIdx;
            };
            public List<SubscriberData> subscribers = new List<SubscriberData>();

            public ConcentratorWrapper(IStateConcentrator ptr) { this.ptr = ptr; }
            //~ConcentratorWrapper() { if (ptr) delete ptr; }

            // Gmqueue part (indeed, we need it only if 'remove concentrator' event may actually happen (conditions?))
            public string address;
            public UInt64 idInQueue; // used as a key for finding this concentrator; reported to publisher as a ref_id_at_subscriber (note: concentrator plays role of subscriber in this case)

            public UInt64 addSubscriber(SubscriberData sd)
            {
                subscribers.Add(sd);
                return (UInt64)subscribers.Count - 1;
            }

            public bool isSsubscriptionResponseReceived() { return subscriptionResponseReceived; }

            public void generateStateSyncMessage(IPublishableComposer composer)
            {
                Debug.Assert(ptr != null);
                Debug.Assert(subscriptionResponseReceived);
                ptr.generateStateSyncMessage(composer);
            }

            public void onSubscriptionResponseMessage(IPublishableParser parser, UInt64 idAtPublisher_)
            {
                Debug.Assert(ptr != null);
                Debug.Assert(subscribers.Count != 0); // current implementation does not practically assume removing subscribers 
                //if constexpr(ParserT::proto == globalmq::marshalling::Proto::JSON)
                //		ptr->applyJsonStateSyncMessage(parser);
                //else
                //{
                //	static_assert(ParserT::proto == globalmq::marshalling::Proto::GMQ);
                //	ptr->applyGmqStateSyncMessage(parser);
                //}
                ptr.applyStateSyncMessage(parser);
                subscriptionResponseReceived = true;
                idAtPublisher = idAtPublisher_;
            }

            public void onStateUpdateMessage(IPublishableParser parser)
            {
                Debug.Assert(ptr != null);
                //if constexpr(ParserT::proto == globalmq::marshalling::Proto::JSON)
                //		ptr->applyJsonMessageWithUpdates(parser);
                //else
                //{
                //	static_assert(ParserT::proto == globalmq::marshalling::Proto::GMQ);
                //	ptr->applyGmqMessageWithUpdates(parser);
                //}
                ptr.applyMessageWithUpdates(parser);
            }
        };

        public class Connections
        {
            public class Connection
            {
                public enum Status { uninitialized = 0, connRequestSent, connected };
                public Status status;// = Status::uninitialized;

                // conn initiator info
                public UInt64 ref_id_at_conn_initiator;
                public UInt64 ref_id_at_conn_acceptor;
                public UInt64 idInQueueForInitiator;// = 0; // reported to initiator
                public UInt64 idInQueueForAcceptor;// = 0; // reported to aceptor

                public SlotIdx initiatorSlotIdx;
                public SlotIdx acceptorSlotIdx;

                public string address;
            };

            Dictionary<UInt64, Connection> idToConnectionStorage = new Dictionary<UInt64, Connection>(); // used as storage, mx-protected
            Dictionary<UInt64, Connection> idToConnection = new Dictionary<UInt64, Connection>(); // used for searching, mx-protected
            UInt64 connectionIDBase = 0;

            public Connections() { }
            //Connections( const Connections& ) = delete;
            //	Connections& operator = ( const Connections& ) = delete;
            //	Connections(Connections&& ) = delete;
            //	Connections& operator = (Connections&& ) = delete;
            //	~Connections() { }

            public struct FieldsForSending
            {
                public UInt64 idAtSource;
                public UInt64 idAtTarget;
                public SlotIdx targetSlotIdx;
            };

            public FieldsForSending onConnRequest(UInt64 ref_id_at_conn_initiator, SlotIdx initiatorSlotIdx, SlotIdx acceptorSlotIdx, string address)
            {
                Connection conn = new Connection();
                conn.status = Connection.Status.connRequestSent;
                conn.ref_id_at_conn_initiator = ref_id_at_conn_initiator;
                conn.initiatorSlotIdx = initiatorSlotIdx;
                conn.acceptorSlotIdx = acceptorSlotIdx;
                conn.address = address;
                conn.idInQueueForAcceptor = ++connectionIDBase;
                idToConnectionStorage.Add(conn.idInQueueForAcceptor, conn);
                //assert(ins.second);
                idToConnection.Add(conn.idInQueueForAcceptor, conn);
                //assert(ins2.second);
                FieldsForSending ffs;
                ffs.idAtSource = conn.idInQueueForAcceptor;
                ffs.idAtTarget = 0; // TODO: invalid value
                ffs.targetSlotIdx = acceptorSlotIdx;
                return ffs;
            }

            public FieldsForSending onConnAccepted(UInt64 connID, UInt64 ref_id_at_conn_acceptor, SlotIdx acceptorSlotIdx)
            {
                //if (f == idToConnection.end())
                //	throw std::exception();
                //auto & conn = *(f->second);
                Connection conn = idToConnection[connID];
                Debug.Assert(connID == conn.idInQueueForAcceptor); // self-consistency
                                                                   //conn.status = Connection.Status.connRequestSent;
                if (conn.status != Connection.Status.connRequestSent)
                    throw new Exception();
                conn.status = Connection.Status.connected;
                if (conn.acceptorSlotIdx != acceptorSlotIdx)
                    throw new Exception();
                conn.ref_id_at_conn_acceptor = ref_id_at_conn_acceptor;
                conn.idInQueueForInitiator = ++connectionIDBase;
                idToConnection.Add(conn.idInQueueForInitiator, conn);
                //assert(ins.second);
                FieldsForSending ffs;
                ffs.idAtSource = conn.idInQueueForInitiator;
                ffs.idAtTarget = conn.ref_id_at_conn_initiator;
                ffs.targetSlotIdx = conn.initiatorSlotIdx;
                return ffs;
            }
            public FieldsForSending onConnMsg(UInt64 connID, UInt64 refIdAtCaller, SlotIdx callerSlotIdx)
            {
                //auto f = idToConnection.find(connID);
                //if (f == idToConnection.end())
                //	throw std::exception();
                //auto & conn = *(f->second);
                Connection conn = idToConnection[connID];
                Debug.Assert(connID == conn.idInQueueForInitiator || connID == conn.idInQueueForAcceptor);
                if (conn.status != Connection.Status.connected)
                    throw new Exception();
                FieldsForSending ffs;
                if (connID == conn.idInQueueForAcceptor) // from acceptor to initiator
                {
                    if (refIdAtCaller != conn.ref_id_at_conn_acceptor)
                        throw new Exception();
                    if (callerSlotIdx != conn.acceptorSlotIdx)
                        throw new Exception();
                    ffs.idAtSource = conn.idInQueueForInitiator;
                    ffs.idAtTarget = conn.ref_id_at_conn_initiator;
                    ffs.targetSlotIdx = conn.initiatorSlotIdx;
                }
                else
                {
                    if (refIdAtCaller != conn.ref_id_at_conn_initiator)
                        throw new Exception();
                    if (callerSlotIdx != conn.initiatorSlotIdx)
                        throw new Exception();
                    ffs.idAtSource = conn.idInQueueForAcceptor;
                    ffs.idAtTarget = conn.ref_id_at_conn_acceptor;
                    ffs.targetSlotIdx = conn.acceptorSlotIdx;
                }
                return ffs;
            }
        };
        Connections connections = new Connections();

        //std::mutex mx;
        Object mx = new Object();

        Dictionary<string, ConcentratorWrapper> addressesToStateConcentrators = new Dictionary<string, ConcentratorWrapper>(); // address to concentrator mapping, 1 - 1, mx-protected
        Dictionary<UInt64, ConcentratorWrapper> idToStateConcentrators = new Dictionary<UInt64, ConcentratorWrapper>(); // id to concentrator mapping, many - 1, mx-protected
        UInt64 concentratorIDBase = 0;

        //	GMQ_COLL unordered_map<GMQ_COLL string, AddressableLocation> namedRecipients; // node name to location, mx-protected
        Dictionary<string, SlotIdx> namedRecipients = new Dictionary<string, SlotIdx>(); // node name to location, mx-protected

        Dictionary<UInt64, SlotIdx> senders = new Dictionary<UInt64, SlotIdx>(); // node name to location, mx-protected
        UInt64 senderIDBase = 0;

        Dictionary<UInt64, Tuple<UInt64, UInt64>> ID2ConcentratorSubscriberPairMapping = new Dictionary<UInt64, Tuple<UInt64, UInt64>>();
        UInt64 publisherAndItsConcentratorBase = 0;

        IStateConcentratorFactory stateConcentratorFactory = null;
        IPlatformSupport platformSupport = null;

        void addConcentratorSubscriberPair(UInt64 id, UInt64 concentratorID, UInt64 subscriberDataID)
        {
            /*auto ins = */
            ID2ConcentratorSubscriberPairMapping.Add(id, new Tuple<UInt64, UInt64>(concentratorID, subscriberDataID));
            //assert(ins.second);
        }
        void removeConcentratorSubscriberPair(UInt64 ID)
        {
            ID2ConcentratorSubscriberPairMapping.Remove(ID);
        }
        Tuple<UInt64, UInt64> findConcentratorSubscriberPair(UInt64 ID)
        {
            //auto f = ID2ConcentratorSubscriberPairMapping.find(ID);
            //if (f != ID2ConcentratorSubscriberPairMapping.end())
            //	return f->second;
            //else
            //	throw std::exception();
            return ID2ConcentratorSubscriberPairMapping[ID];
        }

        // concentrators (address2concentrators)
        ValueTuple<ConcentratorWrapper, bool> findOrAddStateConcentrator(string path, UInt64 stateTypeID)
        {
            Debug.Assert(path.Length != 0);

            if (addressesToStateConcentrators.ContainsKey(path))
                //return std::make_pair(&(f->second), true);
                return new ValueTuple<ConcentratorWrapper, bool>(addressesToStateConcentrators[path], true);

            else
            {
                //Debug.Assert(!path.empty());
                Debug.Assert(stateConcentratorFactory != null);
                IStateConcentrator concentrator = stateConcentratorFactory.createConcentrator(stateTypeID);
                Debug.Assert(concentrator != null);
                ConcentratorWrapper cwrapper = new ConcentratorWrapper(concentrator);
                /*auto ins = */
                addressesToStateConcentrators.Add(path, cwrapper);
                //assert(ins.second);
                //ConcentratorWrapper* c = &(ins.first->second);
                UInt64 concentratorID = ++concentratorIDBase;
                /*auto ins1 = */
                idToStateConcentrators.Add(concentratorID, cwrapper);
                //assert(ins1.second);
                cwrapper.address = path;
                cwrapper.idInQueue = concentratorID;
                return new ValueTuple<ConcentratorWrapper, bool>(cwrapper, false);
            }
        }
        ConcentratorWrapper findStateConcentrator(UInt64 id)
        {
            Debug.Assert(id != 0);
            //auto f = idToStateConcentrators.find(id);
            //if (f != idToStateConcentrators.end())
            //	return f->second;
            //else
            //	return nullptr;

            return idToStateConcentrators.ContainsKey(id) ? idToStateConcentrators[id] : null;
        }
        /*void removeStateConcentrator( GMQ_COLL string path ) { // TODO: rework
            assert( !path.empty() );
            std::unique_lock<std::mutex> lock(mxAddressesToStateConcentrators);
            addressesToStateConcentrators.erase( path );
            idToStateConcentrators.erase( id );
        }*/

        // named local objects (namedRecipients)
        void addNamedLocation(string name, SlotIdx idx)
        {
            Debug.Assert(name.Length != 0);
            /*auto ins = */
            namedRecipients.Add(name, idx);
            //assert(ins.second);
        }
        void removeNamedLocation(string name)
        {
            Debug.Assert(name.Length != 0);
            namedRecipients.Remove(name);
        }

        public SlotIdx locationNameToSlotIdx(string name)
        {
            Debug.Assert(name.Length != 0);
            //		auto f = namedRecipients.find(name);
            //if (f != namedRecipients.end())
            //	return f->second;
            //else
            //	return SlotIdx();
            lock (mx)
            {
                return namedRecipients.ContainsKey(name) ? namedRecipients[name] : SlotIdx.Invalid;
            }
        }

        UInt64 addSender(SlotIdx idx)
        {
            UInt64 id = ++senderIDBase;
            /*auto ins = */
            senders.Add(id, idx);
            //assert(ins.second);
            return id;
        }
        void removeSender(UInt64 id, SlotIdx idx)
        {
            //auto f = senders.find(id);
            //assert(f != senders.end());
            //assert(f->second.idx == idx.idx);
            //assert(f->second.reincarnation == idx.reincarnation);
            Debug.Assert(senders[id].idx == idx.idx);
            Debug.Assert(senders[id].reincarnation == idx.reincarnation);
            senders.Remove(id);
        }
        public SlotIdx senderIDToSlotIdx(UInt64 id)
        {
            //auto f = senders.find(id);
            //if (f != senders.end())
            //	return f->second;
            //else
            //	return SlotIdx();
            lock (mx)
            {
                return senders.ContainsKey(id) ? senders[id] : SlotIdx.Invalid;
            }

        }

        public GMQueue() { }
        //~GMQueue()
        //	{
        //	std::unique_lock < std::mutex > lock (mx) ;

        //	if (stateConcentratorFactory != nullptr)
        //		delete stateConcentratorFactory;
        //}

        //template <class StateFactoryT>
        public void initStateConcentratorFactory(IStateConcentratorFactory factory, IPlatformSupport platform) // Note: potentially, temporary solution
        {
            //std::unique_lock < std::mutex > lock (mx) ;
            lock (mx)
            {
                Debug.Assert(stateConcentratorFactory == null); // must be called just once
                stateConcentratorFactory = factory;

                Debug.Assert(platformSupport == null); // must be called just once
                platformSupport = platform;
            }
        }
        public void setAuthority(string authority)
        {
            //std::unique_lock < std::mutex > lock (mx) ;
            lock (mx)
            {

                Debug.Assert(myAuthority == null); // set just once
                myAuthority = authority;
            }
        }

        public static void helperComposePublishableStateMessageBegin(IPublishableComposer composer, PublishableStateMessageHeader header)
        {
            composer.composeStructBegin();
            if (header.type == PublishableStateMessageHeader.MsgType.subscriptionResponse ||
                        header.type == PublishableStateMessageHeader.MsgType.stateUpdate ||
                        header.type == PublishableStateMessageHeader.MsgType.connectionMessage)
            {
                header.compose(composer, true);
                composer.composeKey("data");
                // next call would be generateXXXMessage()
            }
            else
                header.compose(composer, false);
        }

        public static void helperComposePublishableStateMessageEnd(IPublishableComposer composer)
        {
            composer.composeStructEnd();
        }
        public static void helperParsePublishableStateMessageBegin(IPublishableParser parser, ref PublishableStateMessageHeader header)
        {
            parser.parseStructBegin();
            header.parse(parser);
            if (header.type == PublishableStateMessageHeader.MsgType.subscriptionResponse ||
                header.type == PublishableStateMessageHeader.MsgType.stateUpdate ||
                header.type == PublishableStateMessageHeader.MsgType.connectionMessage)
            {
                parser.parseKey("data");
            }
        }

        public static void helperParsePublishableStateMessageEnd(IPublishableParser parser)
        {
            parser.parseStructEnd();
        }

        public static void helperParseAndUpdatePublishableStateMessage(IPlatformSupport ps, BufferT buffFrom, BufferT buffTo, PublishableStateMessageHeader.UpdatedData udata)
        {
            ReadIteratorT riter = buffFrom.getReadIterator();
            
            ReadIteratorT riter1 = buffFrom.getReadIterator();
            IPublishableParser parserCurrent = ps.makePublishableParser(riter1);
            parserCurrent.parseStructBegin();

            IPublishableComposer composer = ps.makePublishableComposer(buffTo);

            PublishableStateMessageHeader.parseAndUpdate(riter, parserCurrent, composer, udata);
        }

        public class MsgToBeSent
        {
            public InProcessMessagePostmanBase postman;
            public BufferT message;

            public MsgToBeSent(InProcessMessagePostmanBase postman, BufferT message)
            {
                this.postman = postman;
                this.message = message;
            }
        }


        public void postMessage(BufferT msg, UInt64 senderID, SlotIdx senderSlotIdx)
        {
            PublishableStateMessageHeader mh = new PublishableStateMessageHeader();
            ReadIteratorT riter = msg.getReadIterator();
            IPublishableParser parser = platformSupport.makePublishableParser(riter);
            helperParsePublishableStateMessageBegin(parser, ref mh);

            //std::unique_lock < std::mutex > lock (mx) ;
            List<MsgToBeSent> msgsToSend = new List<MsgToBeSent>();
            lock (mx)
            {

                SlotIdx senderIdx = senderIDToSlotIdx(senderID);
                Debug.Assert(senderIdx.idx == senderSlotIdx.idx);
                Debug.Assert(senderIdx.reincarnation == senderSlotIdx.reincarnation);

                switch (mh.type)
                {
                    case PublishableStateMessageHeader.MsgType.subscriptionRequest:
                        {
                            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
                            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
                            bool pathOK = GmqPathHelper.parse(mh.path, pc);
                            if (!pathOK)
                                throw new Exception(); // TODO: ... (bad path)

                            string addr = GmqPathHelper.localPart(pc);
                            if (isMyAuthority(pc.authority)) // local
                            {
                                Debug.Assert(pc.nodeName.Length != 0);

                                ValueTuple<ConcentratorWrapper, bool> findCr = findOrAddStateConcentrator(addr, mh.state_type_id_or_direction);
                                ConcentratorWrapper concentrator = findCr.Item1;
                                Debug.Assert(concentrator != null);

                                ConcentratorWrapper.SubscriberData sd;
                                sd.ref_id_at_subscriber = mh.ref_id_at_subscriber;
                                sd.ref_id_at_publisher = ++publisherAndItsConcentratorBase;
                                sd.senderSlotIdx = senderSlotIdx;
                                UInt64 sid = concentrator.addSubscriber(sd);
                                addConcentratorSubscriberPair(sd.ref_id_at_publisher, sid, concentrator.idInQueue);

                                if (findCr.Item2)
                                {
                                    if (concentrator.isSsubscriptionResponseReceived())
                                    {
                                        PublishableStateMessageHeader hdrBack = new PublishableStateMessageHeader();
                                        hdrBack.type = PublishableStateMessageHeader.MsgType.subscriptionResponse;
                                        hdrBack.priority = mh.priority;
                                        hdrBack.ref_id_at_subscriber = mh.ref_id_at_subscriber;
                                        hdrBack.ref_id_at_publisher = sd.ref_id_at_publisher;

                                        BufferT msgBack = platformSupport.makeBuffer();
                                        IPublishableComposer composer = platformSupport.makePublishableComposer(msgBack);
                                        helperComposePublishableStateMessageBegin(composer, hdrBack);
                                        concentrator.generateStateSyncMessage(composer);
                                        helperComposePublishableStateMessageEnd(composer);

                                        InProcessMessagePostmanBase postman = addressableLocations.getPostman(senderSlotIdx);
                                        //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                                        //postman.postMessage(msgBack);
                                        msgsToSend.Add(new MsgToBeSent(postman, msgBack));
                                    }
                                }
                                else
                                {
                                    // TODO: revise!
                                    SlotIdx targetIdx = locationNameToSlotIdx(pc.nodeName);
                                    if (!targetIdx.isInitialized())
                                        throw new Exception(); // TODO: post permanent error message to sender instead or in addition; remove concentrator

                                    PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                                    ud.ref_id_at_subscriber = concentrator.idInQueue;
                                    ud.update_ref_id_at_subscriber = true;

                                    BufferT msgForward = platformSupport.makeBuffer();
                                    helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                                    InProcessMessagePostmanBase postman = addressableLocations.getPostman(targetIdx);
                                    //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                                    //postman.postMessage(msgForward);
                                    msgsToSend.Add(new MsgToBeSent(postman, msgForward));
                                }
                            }
                            else
                            {
                                Debug.Assert(false); // not yet implemented (but up to Postman should go along the same lines)
                            }
                            break;
                        }
                    case PublishableStateMessageHeader.MsgType.subscriptionResponse:
                        {
                            ConcentratorWrapper concentrator = findStateConcentrator(mh.ref_id_at_publisher);
                            concentrator.onSubscriptionResponseMessage(parser, mh.ref_id_at_publisher);

                            // forward message to all concentrator's subscribers
                            PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                            ud.update_ref_id_at_publisher = true;
                            ud.update_ref_id_at_subscriber = true;
                            foreach (ConcentratorWrapper.SubscriberData subscriber in concentrator.subscribers)
                            {
                                ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
                                ud.ref_id_at_publisher = subscriber.ref_id_at_publisher;
                                BufferT msgForward = platformSupport.makeBuffer();
                                helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                                InProcessMessagePostmanBase postman = addressableLocations.getPostman(subscriber.senderSlotIdx);
                                //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                                //postman.postMessage(msgForward);
                                //lock.lock () ;
                                msgsToSend.Add(new MsgToBeSent(postman, msgForward));
                            }

                            break;
                        }
                    case PublishableStateMessageHeader.MsgType.stateUpdate: // so far we have the same processing
                        {
                            ConcentratorWrapper concentrator = findStateConcentrator(mh.ref_id_at_publisher);
                            if (concentrator == null)
                                throw new Exception(); // TODO: ?
                            concentrator.onStateUpdateMessage(parser);

                            // forward message to all concentrator's subscribers
                            PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                            ud.update_ref_id_at_publisher = true;
                            ud.update_ref_id_at_subscriber = true;
                            foreach (ConcentratorWrapper.SubscriberData subscriber in concentrator.subscribers)
                            {
                                ud.ref_id_at_subscriber = subscriber.ref_id_at_subscriber;
                                ud.ref_id_at_publisher = subscriber.ref_id_at_publisher;
                                BufferT msgForward = platformSupport.makeBuffer();
                                helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                                InProcessMessagePostmanBase postman = addressableLocations.getPostman(subscriber.senderSlotIdx);
                                //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                                //postman.postMessage(msgForward);
                                //lock.lock () ;
                                msgsToSend.Add(new MsgToBeSent(postman, msgForward));
                            }

                            break;
                        }
                    case PublishableStateMessageHeader.MsgType.connectionRequest:
                        {
                            GmqPathHelper.PathComponents pc = new GmqPathHelper.PathComponents();
                            pc.type = PublishableStateMessageHeader.MsgType.subscriptionRequest;
                            bool pathOK = GmqPathHelper.parse(mh.path, pc);
                            if (!pathOK)
                                throw new Exception(); // TODO: ... (bad path)

                            string addr = GmqPathHelper.localPart(pc);
                            if (isMyAuthority(pc.authority)) // local
                            {
                                Debug.Assert(pc.nodeName.Length != 0);

                                SlotIdx targetIdx = locationNameToSlotIdx(pc.nodeName);
                                if (!targetIdx.isInitialized())
                                    throw new Exception(); // TODO: post permanent error message to sender instead or in addition

                                Connections.FieldsForSending fields = connections.onConnRequest(mh.ref_id_at_subscriber, senderSlotIdx, targetIdx, mh.path);
                                PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                                ud.ref_id_at_subscriber = fields.idAtSource;
                                ud.update_ref_id_at_subscriber = true;

                                BufferT msgForward = platformSupport.makeBuffer();
                                helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                                InProcessMessagePostmanBase postman = addressableLocations.getPostman(targetIdx);
                                //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                                msgsToSend.Add(new MsgToBeSent(postman, msgForward));
                            }
                            else
                            {
                                Debug.Assert(false); // not yet implemented (but up to Postman should go along the same lines)
                            }
                            break;
                        }
                    case PublishableStateMessageHeader.MsgType.connectionAccepted:
                        {
                            Connections.FieldsForSending fields = connections.onConnAccepted(mh.ref_id_at_subscriber, mh.ref_id_at_publisher, senderSlotIdx);

                            PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                            ud.update_ref_id_at_publisher = true;
                            ud.update_ref_id_at_subscriber = true;
                            ud.ref_id_at_subscriber = fields.idAtTarget;
                            ud.ref_id_at_publisher = fields.idAtSource;

                            BufferT msgForward = platformSupport.makeBuffer();
                            helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                            InProcessMessagePostmanBase postman = addressableLocations.getPostman(fields.targetSlotIdx);
                            //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                            //postman.postMessage(msgForward);
                            msgsToSend.Add(new MsgToBeSent(postman, msgForward));

                            break;
                        }
                    case PublishableStateMessageHeader.MsgType.connectionMessage:
                        {
                            PublishableStateMessageHeader.UpdatedData ud = new PublishableStateMessageHeader.UpdatedData();
                            ud.update_ref_id_at_publisher = true;
                            ud.update_ref_id_at_subscriber = true;

                            Connections.FieldsForSending fields = new Connections.FieldsForSending();
                            if (mh.state_type_id_or_direction == (ulong)PublishableStateMessageHeader.ConnMsgDirection.toServer)
                            {
                                fields = connections.onConnMsg(mh.ref_id_at_publisher, mh.ref_id_at_subscriber, senderSlotIdx);
                                ud.ref_id_at_subscriber = fields.idAtSource;
                                ud.ref_id_at_publisher = fields.idAtTarget;
                            }
                            else if (mh.state_type_id_or_direction == (ulong)PublishableStateMessageHeader.ConnMsgDirection.toClient)
                            {
                                fields = connections.onConnMsg(mh.ref_id_at_subscriber, mh.ref_id_at_publisher, senderSlotIdx);
                                ud.ref_id_at_subscriber = fields.idAtTarget;
                                ud.ref_id_at_publisher = fields.idAtSource;
                            }
                            else
                                throw new Exception();


                            BufferT msgForward = platformSupport.makeBuffer();
                            helperParseAndUpdatePublishableStateMessage(platformSupport, msg, msgForward, ud);

                            InProcessMessagePostmanBase postman = addressableLocations.getPostman(fields.targetSlotIdx);
                            //lock.unlock(); // NOTE: this is correct as long as postans are not released; rework otherwise
                            //postman.postMessage(msgForward);
                            msgsToSend.Add(new MsgToBeSent(postman, msgForward));

                            break;
                        }
                    default:
                        throw new Exception(); // TODO: ... (unknown msg type)
                }
            }

            // mb: I am not sure this is ok
            // as this requires 'postMessage' to be thread safe,               
            foreach (MsgToBeSent current in msgsToSend)
            {
                current.postman.postMessage(current.message);
            }
        }

        public UInt64 add(string name, InProcessMessagePostmanBase postman, ref SlotIdx idx)
        {
            Debug.Assert(name.Length != 0);

            //std::unique_lock < std::mutex > lock (mx) ;
            lock (mx)
            {
                idx = addressableLocations.add(postman);
                addNamedLocation(name, idx);
                return addSender(idx);
            }
        }
        public UInt64 add(InProcessMessagePostmanBase postman, ref SlotIdx idx)
        {
            //std::unique_lock < std::mutex > lock (mx) ;
            lock (mx)
            {
                idx = addressableLocations.add(postman);
                return addSender(idx);
            }
        }
        public void remove(string name, SlotIdx idx)
        {
            //std::unique_lock < std::mutex > lock (mx) ;
            lock (mx)
            {
                if (name.Length != 0)
                    removeNamedLocation(name);
                addressableLocations.remove(idx);
                // TODO: revise and finalize implementation
            }
        }

    };

    public class InProcTransferrable
    {
        public string name;
        public UInt64 id;

        //void serialize(uint8_t* buff, size_t maxSz) // NOTE: temporary solution
        //{
        //    assert(name.size() + 1 + sizeof(id) <= maxSz);
        //    memcpy(buff, &id, sizeof(id));
        //    memcpy(buff + sizeof(id), name.c_str(), name.size());
        //    buff[sizeof(id) + name.size()] = 0;
        //}
        //void deserilaize(uint8_t* buff) // NOTE: temporary solution
        //{
        //    memcpy(&id, buff, sizeof(id));
        //    name = (char*)(buff + sizeof(id));
        //}
    };

    public class GMQTransportBase
    {
        GMQueue gmq;
        string name;
        SlotIdx idx = SlotIdx.Invalid;
        UInt64 id;
        GMQTransportBase(GMQueue queue, string name_, SlotIdx idx_, UInt64 id_)
        {
            this.gmq = queue;
            this.name = name_;
            this.idx = idx_;
            this.id = id_;
        }

        //GMQTransportBase(GMQueue<PlatformSupportT>& queue ) : gmq(queue ) { }
        public void destroy()
        {
            if (idx.isInitialized())
                gmq.remove(name, idx);

        }
        //virtual ~GMQTransportBase()
        //{
        //    if (idx.isInitialized())
        //        gmq.remove(name, idx);
        //}
        public GMQTransportBase(GMQueue queue, string name_, InProcessMessagePostmanBase postman)
        {

            Debug.Assert(name_.Length != 0);
            this.gmq = queue;
            this.name = name_;
            this.id = gmq.add(name, postman, ref idx);
        }
        public GMQTransportBase(GMQueue queue, InProcessMessagePostmanBase postman)
        {
            this.gmq = queue;

            this.id = gmq.add(postman, ref idx);
        }
        public void postMessage(BufferT msg)
        {
            Debug.Assert(idx.isInitialized());
            gmq.postMessage(msg, id, idx);
        }

        InProcTransferrable makeTransferrable()
        {
            InProcTransferrable ret = new InProcTransferrable();
            ret.name = name;
            ret.id = id;
            idx = SlotIdx.Invalid;
            return ret;
        }

        void restore(InProcTransferrable t, GMQueue queue_)
        {
            Debug.Assert(ReferenceEquals(gmq, queue_));
            Debug.Assert(!idx.isInitialized());
            if (t.name.Length == 0)
            {
                idx = gmq.senderIDToSlotIdx(t.id);
                Debug.Assert(idx.isInitialized());
                id = t.id;
            }
            else
            {
                idx = gmq.locationNameToSlotIdx(t.name);
                Debug.Assert(idx.isInitialized());
                SlotIdx idx2 = gmq.senderIDToSlotIdx(t.id);
                Debug.Assert(idx2.isInitialized());
                Debug.Assert(idx == idx2);
                name = t.name;
                id = t.id;
            }
        }
    };


} // namespace globalmq
