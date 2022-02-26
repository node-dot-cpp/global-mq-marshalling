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
                if(!Enumerable.SequenceEqual(left.Keys, right.Keys))
                    return false;

                return areEquivalent(left.Values, right.Values);
            }
        }
        //public static bool areEquivalent<T, U>(IList<T> left, IList<U> right) where T : IEquivalenceComparable<T> where U : T
        //{
        //    if (left == null && right == null)
        //        return true;
        //    else if (left == null || right == null)
        //        return false;
        //    else if (ReferenceEquals(left, right))
        //        return true;
        //    else
        //    {
        //        if (left.Count != right.Count)
        //            return false;
        //        for(int i = 0; i != left.Count; ++i)
        //        {
        //            if (!left[i].isEquivalent(right[i]))
        //                return false;
        //        }


        //        return true;
        //    }
        //}
    }
    public class MessageHandler
    {
        public static ulong DefaultHandler = ulong.MaxValue;
        public ulong msgID { get; set; }
        HandlerDelegate lhandler_;

        public delegate void HandlerDelegate(ParserBase parser);
        public MessageHandler(ulong msgID, HandlerDelegate lhandler)
        {
            this.msgID = msgID;
            lhandler_ = lhandler;
        }

        public void handle(ParserBase parser)
        {
            lhandler_(parser);
        }

        public static MessageHandler find_handler(ulong msgID, MessageHandler[] handlers)
        {
            // TODO improve
            MessageHandler defaultHandler = null;

            for (int i = 0; i != handlers.Length; ++i)
            {
                if (msgID == handlers[i].msgID)
                {
                    return handlers[i];
                }
                else if (handlers[i].msgID == MessageHandler.DefaultHandler)
                {
                    defaultHandler = handlers[i];
                }
            }

            return defaultHandler;
        }
        public static void gmq_handle(GmqParser parser, MessageHandler[] handlers)
        {
            ulong msgID;
            parser.parseUnsignedInteger(out msgID);
            MessageHandler handler = find_handler(msgID, handlers);

            if(handler != null)
                handler.handle(parser);
            else
                throw new Exception();
        }
        public static void json_handle(JsonParser parser, MessageHandler[] handlers)
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

            MessageHandler handler = find_handler(msgID, handlers);

            if(handler != null)
            {
                handler.handle(parser);
                parser.skipDelimiter('}');
            }
            else
                throw new Exception();
        }
    }
}