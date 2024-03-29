﻿/* -------------------------------------------------------------------------------
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
using System.Linq;

namespace globalmq.marshalling
{
    /// <summary>
    /// Allows to compare for equivalence Messages, Publishers and Subscribers
    /// </summary>
    public interface IEquivalenceComparable<T>
    {
        bool isEquivalent(T other);
    }

    public class EquivalenceComparer
    {
        public static bool areEquivalent<T>(T left, T right) where T : IEquivalenceComparable<T>
        {
            if (left == null && right == null)
                return true;
            else if (left == null || right == null)
                return false;
            else if (ReferenceEquals(left, right))
                return true;
            else
                return left.isEquivalent(right);
        }
        public static bool areEquivalent<T>(ICollection<T> left, ICollection<T> right) where T : IEquivalenceComparable<T>
        {
            if (left == null && right == null)
                return true;
            else if (left == null || right == null)
                return false;
            else if (ReferenceEquals(left, right))
                return true;
            else
            {
                if (left.Count != right.Count)
                    return false;

                IEnumerator<T> itl = left.GetEnumerator();
                IEnumerator<T> itr = right.GetEnumerator();

                while(itl.MoveNext())
                {
                    if (!itr.MoveNext())
                        return false;
                    if (!areEquivalent(itl.Current, itr.Current))
                        return false;
                }

                // if right still has more elements
                if (itr.MoveNext())
                    return false;

                return true;
            }
        }

        public static bool areEquivalent<K, V>(IDictionary<K, V> left, IDictionary<K, V> right) where V : IEquivalenceComparable<V>
        {
            if (left == null && right == null)
                return true;
            else if (left == null || right == null)
                return false;
            else if (ReferenceEquals(left, right))
                return true;
            else
            {
                if (left.Count != right.Count)
                    return false;

                IEnumerator<KeyValuePair<K, V>> itl = left.GetEnumerator();
                IEnumerator<KeyValuePair<K, V>> itr = right.GetEnumerator();

                while (itl.MoveNext())
                {
                    if (!itr.MoveNext())
                        return false;
                    if (!itl.Current.Key.Equals(itr.Current.Key))
                        return false;
                    if (!areEquivalent(itl.Current.Value, itr.Current.Value))
                        return false;
                }

                // if right still has more elements
                if (itr.MoveNext())
                    return false;

                return true;
            }
        }
    }

    public class JsonMessageHandler
    {
        public static ulong DefaultHandler = ulong.MaxValue;
        ulong msgID { get; set; }
        HandlerDelegate _delegate;

        public delegate void HandlerDelegate(JsonParser parser, ulong msgID);
        public JsonMessageHandler(ulong msgID, HandlerDelegate lhandler)
        {
            this.msgID = msgID;
            this._delegate = lhandler;
        }

        static JsonMessageHandler find_handler(ulong msgID, JsonMessageHandler[] handlers)
        {
            // TODO improve
            JsonMessageHandler defaultHandler = null;

            for (int i = 0; i != handlers.Length; ++i)
            {
                if (msgID == handlers[i].msgID)
                {
                    return handlers[i];
                }
                else if (handlers[i].msgID == JsonMessageHandler.DefaultHandler)
                {
                    defaultHandler = handlers[i];
                }
            }

            return defaultHandler;
        }
        public static void handle(JsonParser parser, JsonMessageHandler[] handlers)
        {
            parser.skipDelimiter('{');
            string key;
            parser.readKeyFromJson(out key);
            if (key != "msgid")
                throw new Exception(); // bad format
            ulong msgID;
            parser.parseUnsignedInteger(out msgID);
            parser.skipSpacesEtc();
            parser.skipDelimiter(',');
            parser.readKeyFromJson(out key);
            if (key != "msgbody")
                throw new Exception(); // bad format

            JsonMessageHandler handler = find_handler(msgID, handlers);

            if (handler != null)
            {
                handler._delegate(parser, msgID);
                //parser.skipDelimiter('}');
            }
            else
                throw new Exception();
        }

    }

    public class GmqMessageHandler
    {
        public static ulong DefaultHandler = ulong.MaxValue;
        ulong msgID { get; set; }
        HandlerDelegate _delegate;

        public delegate void HandlerDelegate(GmqParser parser, ulong msgID);
        public GmqMessageHandler(ulong msgID, HandlerDelegate lhandler)
        {
            this.msgID = msgID;
            _delegate = lhandler;
        }
        void handle(GmqParser parser, ulong msgID)
        {
            _delegate(parser, msgID);
        }

        static GmqMessageHandler find_handler(ulong msgID, GmqMessageHandler[] handlers)
        {
            // TODO improve
            GmqMessageHandler defaultHandler = null;

            for (int i = 0; i != handlers.Length; ++i)
            {
                if (msgID == handlers[i].msgID)
                {
                    return handlers[i];
                }
                else if (handlers[i].msgID == GmqMessageHandler.DefaultHandler)
                {
                    defaultHandler = handlers[i];
                }
            }

            return defaultHandler;
        }
        public static void handle(GmqParser parser, GmqMessageHandler[] handlers)
        {
            ulong msgID;
            parser.parseUnsignedInteger(out msgID);
            GmqMessageHandler handler = find_handler(msgID, handlers);

            if (handler != null)
                handler._delegate(parser, msgID);
            else
                throw new Exception();
        }

    }


    public class MessageHandlerCommon
    {
        public static ulong parseMsgBegin(JsonParser parser)
        {
            parser.skipDelimiter('{');
            string key;
            parser.readKeyFromJson(out key);
            if (key != "msgid")
                throw new Exception(); // bad format
            ulong msgID;
            parser.parseUnsignedInteger(out msgID);
            parser.skipSpacesEtc();
            parser.skipDelimiter(',');
            parser.readKeyFromJson(out key);
            if (key != "msgbody")
                throw new Exception(); // bad format

            return msgID;
        }

        public static void parseMsgEnd(JsonParser parser)
        {
            parser.skipDelimiter('}');
        }

        public static ulong parseMsgBegin(GmqParser parser)
        {
            ulong msgID;
            parser.parseUnsignedInteger(out msgID);

            return msgID;
        }

        public static void parseMsgEnd(GmqParser parser) { }

    }
}