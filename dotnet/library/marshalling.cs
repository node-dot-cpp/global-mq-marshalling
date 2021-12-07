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
    public interface ICompose
    {
    };

    public interface IParse
    {
    };

    interface AnyCollectionWrapperBase
    {
        public static readonly int unknown_size = -1;
    };

    interface CollectionWrapperBase : AnyCollectionWrapperBase { }

    interface SimpleTypeCollectionWrapperBase : AnyCollectionWrapperBase { }

    public class CollectionWrapperForComposing : CollectionWrapperBase, ICompose
    {
        SizeDelegate lsize_;
        NextDelegate lnext_;

        public delegate int SizeDelegate();
        public delegate void NextDelegate(ComposerBase composer, int ordinal);
        public CollectionWrapperForComposing(SizeDelegate lsize, NextDelegate lnext)
        {
            this.lsize_ = lsize;
            this.lnext_ = lnext;
        }
        public int size() { return lsize_(); }
        public void compose_next(ComposerBase composer, int ordinal)
        {
            lnext_(composer, ordinal);
        }
    };

    public class CollectionWrapperForParsing : CollectionWrapperBase, IParse
    {
        SizeDelegate lsize_;
        NextDelegate lnext_;
        public delegate void SizeDelegate(int size);
        public delegate void NextDelegate(ParserBase parser, int ordinal);

        public CollectionWrapperForParsing(SizeDelegate lsize, NextDelegate lnext)

        {
            this.lsize_ = lsize;
            this.lnext_ = lnext;
        }
        public void size_hint(int sz)
        {
            if (lsize_ != null)
                lsize_(sz);
        }
        public void parse_next(ParserBase parser, int ordinal)
        {
            lnext_(parser, ordinal);
        }
    };

    public class SimpleTypeCollection
    {
        public static ICompose makeComposer(IList<SByte> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<Int16> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<Int32> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<Int64> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(SByte[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(Int16[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(Int32[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(Int64[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeSignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<Byte> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<UInt16> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<UInt32> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<UInt64> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(Byte[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(UInt16[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(UInt32[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }
        public static ICompose makeComposer(UInt64[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeUnsignedInteger(coll[ordinal]); });
        }

        public static ICompose makeComposer(IList<Single> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<Double> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static ICompose makeComposer(Single[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static ICompose makeComposer(Double[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeReal(coll[ordinal]); });
        }
        public static ICompose makeComposer(IList<String> coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Count; }, (ComposerBase composer, int ordinal) => { composer.composeString(coll[ordinal]); });
        }
        public static ICompose makeComposer(String[] coll)
        {
            return new CollectionWrapperForComposing(() => { return coll.Length; }, (ComposerBase composer, int ordinal) => { composer.composeString(coll[ordinal]); });
        }




        public static IParse makeParser(IList<SByte> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { SByte value; parser.parseSignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Int16> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Int16 value; parser.parseSignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Int32> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Int32 value; parser.parseSignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Int64> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Int64 value; parser.parseSignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Byte> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Byte value; parser.parseUnsignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<UInt16> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { UInt16 value; parser.parseUnsignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<UInt32> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { UInt32 value; parser.parseUnsignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<UInt64> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { UInt64 value; parser.parseUnsignedInteger(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Single> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Single value; parser.parseReal(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<Double> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { Double value; parser.parseReal(out value); coll.Add(value); });
        }
        public static IParse makeParser(IList<String> coll)
        {
            return new CollectionWrapperForParsing(null, (ParserBase parser, int ordinal) => { String value; parser.parseString(out value); coll.Add(value); });
        }
    };

    interface MessageWrapperBase
    {
    };


    public class MessageWrapperForComposing : MessageWrapperBase, ICompose
    {
        ComposeDelegate lcompose_;

        public delegate void ComposeDelegate(ComposerBase composer);

        public MessageWrapperForComposing(ComposeDelegate lcompose) { this.lcompose_ = lcompose; }

        public void compose(ComposerBase composer)
        {
            lcompose_(composer);
        }
    };

    public class MessageWrapperForParsing : MessageWrapperBase, IParse
    {
        ParseDelegate lparse_;

        public delegate void ParseDelegate(ParserBase composer);


        public MessageWrapperForParsing(ParseDelegate lparse) { this.lparse_ = lparse; }
        public void parse(ParserBase parser)
        {
            lparse_(parser);
        }
    };



    public class gmq
    {

        public static void parseGmqParam(GmqParser parser, IParse o)
        {
            if (o is CollectionWrapperForParsing coll)
            {
                int sz;
				parser.parseUnsignedInteger(out sz);
                coll.size_hint(sz);
                for (int i = 0; i < sz; ++i)
                    coll.parse_next(parser, i);

            }
            else if (o is MessageWrapperForParsing msg)
                msg.parse(parser);
            else
                throw new Exception(); //TODO
        }


        ///////////////////////////////////////////

        public static void composeParamToGmq(GmqComposer composer, ICompose o)
        {

            if (o is CollectionWrapperForComposing coll)
            {
                int collSz = coll.size();
                composer.composeUnsignedInteger((uint)collSz);
                for (int i = 0; i < collSz; ++i)
                    coll.compose_next(composer, i);
            }
            else if(o is MessageWrapperForComposing msg)
                msg.compose(composer);
            else
                throw new Exception(); //TODO
        }

    } // class gmq

    public class json
    {
        public static void parseJson(JsonParser parser, CollectionWrapperForParsing coll)
        {
            bool newFormat = false;
            if (parser.isDelimiter('{'))
            {
                newFormat = true;
                parser.skipDelimiter('{');

                string key;
                parser.readKeyFromJson(out key);
                if (key != "size")
                    throw new Exception();

                int sz;
                parser.parseUnsignedInteger(out sz);
                coll.size_hint(sz);

                parser.skipSpacesEtc();
                parser.skipDelimiter(',');

                parser.readKeyFromJson(out key);
                if (key != "data")
                    throw new Exception();
            }

            parser.skipDelimiter('[');

            if (!parser.isDelimiter(']')) // there are some items there
            {
                for (int i = 0; ; ++i)
                {
                    coll.parse_next(parser, i);
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

            if (newFormat)
                parser.skipDelimiter('}');

        }

        public static void composeJson(JsonComposer composer, CollectionWrapperForComposing coll)
        {

            //mb: new format writes 'size' and 'data' for collections
            // that do have size on GMQ format.
            // So client perceived behaviour can be replicated at parsing

            composer.append('{');
            composer.addNamePart("size");

            int collSz = coll.size();
            composer.composeUnsignedInteger((UInt64)collSz);
            composer.append(", ");
            composer.addNamePart("data");

            composer.append('[');
            for (int i = 0; i < collSz; ++i)
            {
                if (i != 0)
                    composer.append(", ");
                coll.compose_next(composer, i);
            }
            composer.append(']');
            composer.append('}');
        }

        public static void parseJsonEntry(JsonParser parser, IParse o)
        {
            if (o is CollectionWrapperForParsing coll)
                parseJson(parser, coll);
            else if (o is MessageWrapperForParsing msg)
                msg.parse(parser);
            else
                throw new Exception(); //TODO
        }

        public static void composeParamToJson(JsonComposer composer, ICompose o)
        {

            if (o is CollectionWrapperForComposing coll)
                composeJson(composer, coll);
            else if (o is MessageWrapperForComposing msg)
                msg.compose(composer);
            else
                throw new Exception(); //TODO
        }
    }

    public class MessageHandler
    {
        public static UInt64 DefaultHandler = UInt64.MaxValue;
        public UInt64 msgID { get; set; }
        HandlerDelegate lhandler_;

        public delegate void HandlerDelegate(ParserBase parser);
        public MessageHandler(UInt64 msgID, HandlerDelegate lhandler)
        {
            this.msgID = msgID;
            this.lhandler_ = lhandler;
        }

        public void handle(ParserBase parser )
        {
            lhandler_(parser);
        }
    }



} // namespace globalmq::marshalling
