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

namespace globalmq.marshalling
{
    public class Publishable
    {
        public static ulong[] makeAddress(ulong[] baseAddress, ulong last)
        {
            ulong[] address = new ulong[baseAddress.Length + 1];
            baseAddress.CopyTo(address, 0);
            address[baseAddress.Length] = last;
            return address;
        }
    }
    public interface IPublishableParser
    {
        bool parseAddress(ref UInt64[] addr);
        void parseAddressEnd();
        Int64 parseInteger(String expectedName);
        UInt64 parseUnsigned(String expectedName);
        Double parseReal(String expectedName);
        String parseString(String expectedName);
        void parsePublishableStructBegin(String expectedName);
        void parsePublishableStructEnd();
        void parseStructBegin();
        void parseStructEnd();
        void parseStateUpdateMessageBegin();
        void parseStateUpdateMessageEnd();
    }
    public class JsonPublishableParser : IPublishableParser
    {
        JsonParser p;

        public JsonPublishableParser(ReadIteratorT riter) { p = new JsonParser(riter); }

        private void parseKey(String expectedName)
        {
            String key;
            p.readKeyFromJson(out key);
            if (key != expectedName)
                throw new Exception(); // bad format
        }
        public bool parseAddress(ref UInt64[] addr)
        {
            //mb: I am commneting out code paths I don't
            // know when can happend. Will uncomment as
            // I learn their reasons to exist.
            //if (p.isDelimiter('{'))
            //{
                p.skipDelimiter('{');
                if (p.isDelimiter('}'))
                {
                    p.skipDelimiter('}');
                    //p.skipDelimiter(']');
                    //p.skipDelimiter('}');
                    return false;
                }
            //}
            //else if (p.isDelimiter(']'))
            //{
            //    p.skipDelimiter(']');
            //    p.skipDelimiter('}');
            //    return false;
            //}

            parseKey("addr");

            p.skipDelimiter('[');
            List<UInt64> tmpAddr = new List<ulong>();
            if (!p.isDelimiter(']')) // there are some items there
            {
                UInt64 tmp;
                for (; ; )
                {
                    p.parseUnsignedInteger(out tmp);
                    tmpAddr.Add(tmp);
                    if (p.isDelimiter(','))
                    {
                        p.skipDelimiter(',');
                        continue;
                    }
                    if (p.isDelimiter(']'))
                    {
                        p.skipDelimiter(']');
                        break;
                    }
                }
            }
            else
                p.skipDelimiter(']');

            //if (p.isDelimiter(','))
                p.skipDelimiter(',');
            //else
            //    throw new Exception(); // bad format

            addr = tmpAddr.ToArray();
            return true;

        }
        public void parseAddressEnd()
        {
            p.skipDelimiter('}');
            p.skipDelimiter(',');
        }

        public Int64 parseInteger(String expectedName)
        {
            parseKey(expectedName);

            Int64 val;
            p.parseSignedInteger (out val);
            if (p.isDelimiter(','))
                p.skipDelimiter(',');

            return val;
        }
        public UInt64 parseUnsigned(String expectedName)
        {
            parseKey(expectedName);

            UInt64 val;
            p.parseUnsignedInteger(out val);
            if (p.isDelimiter(','))
                p.skipDelimiter(',');

            return val;
        }
        public Double parseReal(String expectedName)
        {
            parseKey(expectedName);

            Double val;
            p.parseReal(out val);
            if (p.isDelimiter(','))
                p.skipDelimiter(',');

            return val;
        }
        public String parseString(String expectedName)
        {
            parseKey(expectedName);

            String val;
            p.parseString(out val);
            if (p.isDelimiter(','))
                p.skipDelimiter(',');

            return val;
        }
        public void parsePublishableStructBegin(String expectedName)
        {
            parseKey(expectedName);
            p.skipDelimiter('{');
        }
        public void parsePublishableStructEnd()
        {
            p.skipDelimiter('}');

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }

        public void parseStructBegin()
        {
            p.skipDelimiter('{');
        }
        public void parseStructEnd()
        {
            p.skipDelimiter('}');
        }

        public void parseStateUpdateMessageBegin()
        {
            p.skipDelimiter('{');
            parseKey("changes");
            p.skipDelimiter('[');
        }
        public void parseStateUpdateMessageEnd()
        {
            p.skipDelimiter(']');
            p.skipDelimiter('}');
        }
    }

    public class GmqPublishableParser : IPublishableParser
    {
        GmqParser p;

        public GmqPublishableParser(ReadIteratorT riter) { p = new GmqParser(riter); }

        public bool parseAddress(ref UInt64[] addr)
        {
            UInt64 cnt;
            p.parseUnsignedInteger(out cnt);
            if (cnt == 0)
                return false;
            addr = new UInt64[cnt];
            for (UInt64 i = 0; i < cnt; ++i)
            {
                p.parseUnsignedInteger(out addr[i]);
            }
            return true;
        }
        public void parseAddressEnd() { }
        public Int64 parseInteger(String expectedName)
        {
            Int64 val;
            p.parseSignedInteger(out val);
            return val;
        }
        public UInt64 parseUnsigned(String expectedName)
        {
            UInt64 val;
            p.parseUnsignedInteger(out val);
            return val;
        }
        public Double parseReal(String expectedName)
        {
            Double val;
            p.parseReal(out val);
            return val;
        }

        public String parseString(String expectedName)
        {
            String val;
            p.parseString(out val);
            return val;
        }

        public void parsePublishableStructBegin(String expectedName) { }
        public void parsePublishableStructEnd() { }
        public void parseStructBegin() { }
        public void parseStructEnd() { }
        public void parseStateUpdateMessageBegin() { }
        public void parseStateUpdateMessageEnd() { }
    }


    public interface IPublishableComposer
    {
        void startTick(BufferT buffer);
        BufferT endTick();
        void composeAddress(UInt64[] baseAddr, UInt64 last);
        void composeAddressEnd();
        void composeInteger(String name, Int64 value, bool separator);
        void composeUnsigned(String name, UInt64 value, bool separator);
        void composeReal(String name, Double value, bool separator);
        void composeString(String name, String value, bool separator);
        void composePublishableStructBegin(String name);
        void composePublishableStructEnd(bool separator);
        void composeStructBegin();
        void composeStructEnd();
        void composeStateUpdateMessageBegin();
        void composeStateUpdateMessageEnd();
    }

    public class JsonPublishableComposer : IPublishableComposer
    {
        JsonComposer composer;

        public JsonPublishableComposer() { composer = new JsonComposer(null); }

        public void startTick(BufferT buffer)
        {
            composer.startTick(buffer);
        }
        public BufferT endTick()
        {
            return composer.endTick();
        }

        public void composeAddress(ulong[] baseAddr, ulong last)
        {
            composer.append('{');
            composer.addNamePart("addr");
            composer.append('[');
            int collSz = baseAddr.Length;
            for (int i = 0; i < collSz; ++i)
            {
                composer.composeUnsignedInteger(baseAddr[i]);
                composer.append(", ");
            }
            composer.composeUnsignedInteger(last);
            composer.append(']');
            composer.append(',');
        }
        public void composeAddressEnd() { composePublishableStructEnd(true); }
        public void composeInteger(String name, Int64 value, bool separator)
        {
            composer.addNamePart(name);
            composer.composeSignedInteger(value);
            if (separator)
                composer.append(",");
        }
        public void composeUnsigned(String name, UInt64 value, bool separator)
        {
            composer.addNamePart(name);
            composer.composeUnsignedInteger(value);
            if (separator)
                composer.append(",");
        }
        public void composeReal(String name, Double value, bool separator)
        {
            composer.addNamePart(name);
            composer.composeReal(value);
            if (separator)
                composer.append(",");
        }
        public void composeString(String name, String value, bool separator)
        {
            composer.addNamePart(name);
            composer.composeString(value);
            if (separator)
                composer.append(",");
        }
        public void composePublishableStructBegin(String name)
        {
            composer.addNamePart(name);
            composer.append("{");
        }
        public void composePublishableStructEnd(bool separator)
        {
            composer.append("}");
            if (separator)
                composer.append(",");
        }
        public void composeStructBegin()
        {
            composer.append('{');
        }
        public void composeStructEnd()
        {
            composer.append('}');
        }
        public void composeStateUpdateMessageBegin()
        {
            composer.append("{\"changes\":[");
        }
        public void composeStateUpdateMessageEnd()
        {
            composer.append("{}]}");
        }

    }

    public class GmqPublishableComposer : IPublishableComposer
    {
        GmqComposer composer;

        public GmqPublishableComposer() { composer = new GmqComposer(null); }

        public void startTick(BufferT buffer)
        {
            composer.startTick(buffer);
        }
        public BufferT endTick()
        {
            return composer.endTick();
        }
        public void composeAddress(ulong[] baseAddr, ulong last)
        {
            int collSz = baseAddr.Length;
            composer.composeUnsignedInteger((ulong)collSz + 1);
            for (int i = 0; i < collSz; ++i)
                composer.composeUnsignedInteger(baseAddr[i]);
            composer.composeUnsignedInteger(last);
        }
        public void composeAddressEnd() { }

        public void composeInteger(String name, Int64 value, bool separator)
        {
            composer.composeSignedInteger(value);
        }
        public void composeUnsigned(String name, UInt64 value, bool separator)
        {
            composer.composeUnsignedInteger(value);
        }
        public void composeReal(String name, Double value, bool separator)
        {
            composer.composeReal(value);
        }
        public void composeString(String name, String value, bool separator)
        {
            composer.composeString(value);
        }
        public void composePublishableStructBegin(String name) { }
        public void composePublishableStructEnd(bool separator) { }

        public void composeStructBegin() { }
        public void composeStructEnd() { }
        public void composeStateUpdateMessageBegin() { }
        public void composeStateUpdateMessageEnd()
        {
            composer.composeUnsignedInteger(0);
        }
    }

} // namespace globalmq::marshalling
