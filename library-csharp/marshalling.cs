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
        public static bool areEquivalent<T, U>(IList<T> left, IList<U> right) where T : IEquivalenceComparable<T> where U : T
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
                for(int i = 0; i != left.Count; ++i)
                {
                    if (!left[i].isEquivalent(right[i]))
                        return false;
                }


                return true;
            }
        }
    }

    public interface ICollectionCompose
    {
        public void composeGmq(GmqComposer composer);
        public void composeJson(JsonComposer composer);
    };

    public class CollectionWrapperForComposing : ICollectionCompose
    {
        SizeDelegate lsize_;
        NextDelegate lnext_;

        public delegate int SizeDelegate();
        public delegate void NextDelegate(ComposerBase composer, int ordinal);
        public CollectionWrapperForComposing(SizeDelegate lsize, NextDelegate lnext)
        {
            lsize_ = lsize;
            lnext_ = lnext;
        }
        public void composeGmq(GmqComposer composer)
        {
            int collSz = lsize_();
            composer.composeUnsignedInteger((uint)collSz);
            for (int i = 0; i < collSz; ++i)
                lnext_(composer, i);
        }
        public void composeJson(JsonComposer composer)
        {
            //composer.append('{');
            //composer.addNamePart("size");

            int collSz = lsize_();
            //composer.composeUnsignedInteger((ulong)collSz);
            //composer.append(", ");
            //composer.addNamePart("data");

            composer.append('[');
            for (int i = 0; i < collSz; ++i)
            {
                if (i != 0)
                    composer.append(", ");
                lnext_(composer, i);
            }
            composer.append(']');
            //composer.append('}');
        }

    };
    public interface ICollectionParse
    {
        public void parseGmq(GmqParser parser);
        public void parseJson(JsonParser parser);
    };
    public class CollectionWrapperForParsing : ICollectionParse
    {
        MakeDelegate lmake;
        NextDelegate lnext;

        public delegate void MakeDelegate();
        public delegate void NextDelegate(ParserBase parser, int ordinal);

        public CollectionWrapperForParsing(MakeDelegate lmake, NextDelegate lnext)
        {
            this.lmake = lmake;
            this.lnext = lnext;
        }
        public CollectionWrapperForParsing(NextDelegate lnext)
        {
            this.lnext = lnext;
        }
        public void parseGmq(GmqParser parser)
        {
            if (lmake != null)
                lmake();
            int sz;
            parser.parseUnsignedInteger(out sz);
            for (int i = 0; i < sz; ++i)
                lnext(parser, i);
        }
        public void parseJson(JsonParser parser)
        {
            if (lmake != null)
                lmake();
            parser.skipDelimiter('[');

            if (!parser.isDelimiter(']')) // there are some items there
            {
                for (int i = 0; /*!newFormat || i < sz*/; ++i)
                {
                    lnext(parser, i);
                    if (parser.isDelimiter(','))
                    {
                        parser.skipDelimiter(',');
                        continue;
                    }
                    if (parser.isDelimiter(']'))
                    {
                        parser.skipDelimiter(']');
                        break;
                    }
                }
            }
            else
                parser.skipDelimiter(']');

        }
    };

    public class JsonCollectionParser
    {
        NextDelegate lnext;

        public delegate void NextDelegate(JsonParser parser, int ordinal);

        public JsonCollectionParser(NextDelegate lnext)
        {
            this.lnext = lnext;
        }
        public void parseJson(JsonParser parser)
        {
            parser.skipDelimiter('[');

            if (!parser.isDelimiter(']')) // there are some items there
            {
                for (int i = 0; /*!newFormat || i < sz*/; ++i)
                {
                    lnext(parser, i);
                    if (parser.isDelimiter(','))
                    {
                        parser.skipDelimiter(',');
                        continue;
                    }
                    if (parser.isDelimiter(']'))
                    {
                        parser.skipDelimiter(']');
                        break;
                    }
                }
            }
            else
                parser.skipDelimiter(']');
        }
    };

    public class GmqCollectionParser
    {
        NextDelegate lnext;
        public delegate void NextDelegate(GmqParser parser, int ordinal);

        public GmqCollectionParser(NextDelegate lnext)
        {
            this.lnext = lnext;
        }
        public void parseGmq(GmqParser parser)
        {
            int sz;
            parser.parseUnsignedInteger(out sz);
            for (int i = 0; i < sz; ++i)
                lnext(parser, i);
        }
    };

    public class SimpleTypeCollection
    {
        public static CollectionWrapperForComposing makeComposer(IList<sbyte> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<short> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<int> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<long> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(sbyte[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(short[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(int[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(long[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<byte> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<ushort> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<uint> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<ulong> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(byte[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(ushort[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(uint[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(ulong[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }

        public static CollectionWrapperForComposing makeComposer(IList<float> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<double> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(float[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(double[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(IList<string> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (composer, ordinal) => { composer.composeString(coll[ordinal]); });
        }
        public static CollectionWrapperForComposing makeComposer(string[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (composer, ordinal) => { composer.composeString(coll[ordinal]); });
        }

    };

    public struct JsonCollectionComposer<T> 
    {
        IList<T> elements;
        Action<JsonComposer, T> composeFunction;

        public JsonCollectionComposer(IList<T> elements, Action<JsonComposer, T> composeFunction)
        {
            this.elements = elements;
            this.composeFunction = composeFunction;
        }

        public void composeJson(JsonComposer composer)
        {
            composer.append('[');
            for (int i = 0; i < elements.Count; ++i)
            {
                if (i != 0)
                    composer.append(", ");

                composeFunction(composer, elements[i]);
            }
            composer.append(']');
        }
    }

    public struct GmqCollectionComposer<T>
    {
        IList<T> elements;
        Action<GmqComposer, T> composeFunction;

        public GmqCollectionComposer(IList<T> elements, Action<GmqComposer, T> composeFunction)
        {
            this.elements = elements;
            this.composeFunction = composeFunction;
        }

        public void composeGmq(GmqComposer composer)
        {
            composer.composeUnsignedInteger((UInt64)elements.Count);
            for (int i = 0; i < elements.Count; ++i)
            {
                composeFunction(composer, elements[i]);
            }
        }
    }

    //TODO remove
    public struct CollectionComposer<T> where T : IMessageCompose
    {
        int size;
        Func<int, T> makeComposer;

        CollectionComposer(int size, Func<int, T> makeComposer)
        {
            this.size = size;
            this.makeComposer = makeComposer;
        }

        public void composeJson(JsonComposer composer)
        {
            composer.append('[');
            for (int i = 0; i < size; ++i)
            {
                if (i != 0)
                    composer.append(", ");

                T next = makeComposer(i);
                next.composeJson(composer);
            }
            composer.append(']');
        }
        public void composeGmq(GmqComposer composer)
        {
            composer.composeUnsignedInteger((UInt64)size);
            for (int i = 0; i < size; ++i)
            {
                T next = makeComposer(i);
                next.composeGmq(composer);
            }
        }
        public static CollectionComposer<T> make(int size, Func<int, T> makeComposer)
        {
            return new CollectionComposer<T>(size, makeComposer);
        }
    }
    public interface IMessageCompose
    {
        public void composeGmq(GmqComposer composer);
        public void composeJson(JsonComposer composer);

    };

    public class MessageWrapperForComposing : IMessageCompose
    {
        ComposeDelegate lcompose_;

        public delegate void ComposeDelegate(ComposerBase composer);

        public MessageWrapperForComposing(ComposeDelegate lcompose) { lcompose_ = lcompose; }

        public void composeGmq(GmqComposer composer)
        {
            lcompose_(composer);
        }
        public void composeJson(JsonComposer composer)
        {
            lcompose_(composer);
        }


    };

    public interface IMessageParse
    {
        public void parseGmq(GmqParser parser);
        public void parseJson(JsonParser parser);

    };
    public class MessageWrapperForParsing :IMessageParse
    {
        ParseDelegate lparse_;

        public delegate void ParseDelegate(ParserBase composer);


        public MessageWrapperForParsing(ParseDelegate lparse) { lparse_ = lparse; }
        public void parseGmq(GmqParser parser)
        {
            lparse_(parser);
        }
        public void parseJson(JsonParser parser)
        {
            lparse_(parser);
        }
    };

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