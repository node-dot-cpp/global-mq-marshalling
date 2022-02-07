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
using System.Collections;
using System.Collections.Generic;

namespace globalmq.marshalling
{
    public class Publishable
    {
        public enum ActionOnVector { update_at = 1, insert_single_before = 2, remove_at = 3 };

        public static UInt64[] makeAddress(UInt64[] baseAddress, ulong last)
        {
            UInt64[] address = new UInt64[baseAddress.Length + 1];
            baseAddress.CopyTo(address, 0);
            address[baseAddress.Length] = last;
            return address;
        }
    }
    public interface IPublishableParser
    {
        void parseKey(String expectedName); //TODO remove, make private
        public ReadIteratorT getIterator();
        bool parseAddress(ref UInt64[] addr);
        void parseAddressEnd();
        Int64 parseInteger(String expectedName);
        UInt64 parseUnsigned(String expectedName);
        Double parseReal(String expectedName);
        String parseString(String expectedName);
        void parsePublishableStructBegin(String expectedName);
        void parsePublishableStructEnd();
        void parseVector(String expectedName, Action<IPublishableParser, int> parseDelegate);
        void parseSimpleVector(String expectedName, IList<Int64> collection);
        void parseSimpleVector(String expectedName, IList<UInt64> collection);
        void parseSimpleVector(String expectedName, IList<Double> collection);
        void parseSimpleVector(String expectedName, IList<String> collection);
        void parseStructBegin();
        void parseStructEnd();
        void parseStateUpdateMessageBegin();
        void parseStateUpdateMessageEnd();
        UInt64 parseActionInPublishable();

    }
    public class JsonPublishableParser : IPublishableParser
    {
        JsonParser p;

        public JsonPublishableParser(ReadIteratorT riter) { p = new JsonParser(riter); }

        public ReadIteratorT getIterator()
        {
            return p.getIterator();
        }

        public void parseKey(String expectedName)
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
        public void parseVector(String expectedName, Action<IPublishableParser, int> parseDelegate)
        {
            parseKey(expectedName);
            p.skipDelimiter('[');

            if (!p.isDelimiter(']')) // there are some items there
            {
                for (int i = 0; /*!newFormat || i < sz*/; ++i)
                {
                    parseDelegate(this, i);
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

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }
        public void parseSimpleVector(String expectedName, IList<Int64> collection)
        {

            parseKey(expectedName);
            JsonCollectionParser simple = new JsonCollectionParser(
                    (JsonParser parser, int index) =>
                    {
                        Int64 val;
                        p.parseSignedInteger(out val);
                        collection.Add(val);
                    }
            );
            simple.parseJson(p);

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }

        public void parseSimpleVector(String expectedName, IList<UInt64> collection)
        {

            parseKey(expectedName);
            JsonCollectionParser simple = new JsonCollectionParser(
                    (JsonParser parser, int index) =>
                    {
                        UInt64 val;
                        p.parseUnsignedInteger(out val);
                        collection.Add(val);
                    }
            );
            simple.parseJson(p);

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }

        public void parseSimpleVector(String expectedName, IList<Double> collection)
        {

            parseKey(expectedName);
            JsonCollectionParser simple = new JsonCollectionParser(
                    (JsonParser parser, int index) =>
                    {
                        Double val;
                        p.parseReal(out val);
                        collection.Add(val);
                    }
            );
            simple.parseJson(p);

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }
        public void parseSimpleVector(String expectedName, IList<String> collection)
        {

            parseKey(expectedName);
            JsonCollectionParser simple = new JsonCollectionParser(
                    (JsonParser parser, int index) =>
                    {
                        String val;
                        p.parseString(out val);
                        collection.Add(val);
                    }
            );
            simple.parseJson(p);

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
        public UInt64 parseActionInPublishable()
        {
            return parseUnsigned("action");
        }

    }

    public class GmqPublishableParser : IPublishableParser
    {
        GmqParser p;

        public GmqPublishableParser(ReadIteratorT riter) { p = new GmqParser(riter); }

        public void parseKey(String expectedName) { }
        public ReadIteratorT getIterator()
        {
            return p.getIterator();
        }
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

        public void parseVector(String expectedName, Action<IPublishableParser, int> parseDelegate)
        {
            int sz;
            p.parseUnsignedInteger(out sz);
            for (int i = 0; i < sz; ++i)
                parseDelegate(this, i);

        }
        public void parseSimpleVector(String expectedName, IList<Int64> collection)
        {

            GmqCollectionParser simple = new GmqCollectionParser(
                    (GmqParser parser, int index) =>
                    {
                        Int64 val;
                        p.parseSignedInteger(out val);
                        collection.Add(val);
                    }
            );
            simple.parseGmq(p);
        }

        public void parseSimpleVector(String expectedName, IList<UInt64> collection)
        {

            GmqCollectionParser simple = new GmqCollectionParser(
                    (GmqParser parser, int index) =>
                    {
                        UInt64 val;
                        p.parseUnsignedInteger(out val);
                        collection.Add(val);
                    }
            );
            simple.parseGmq(p);
        }

        public void parseSimpleVector(String expectedName, IList<Double> collection)
        {
            GmqCollectionParser simple = new GmqCollectionParser(
                    (GmqParser parser, int index) =>
                    {
                        Double val;
                        p.parseReal(out val);
                        collection.Add(val);
                    }
            );
            simple.parseGmq(p);
        }
        public void parseSimpleVector(String expectedName, IList<String> collection)
        {
            GmqCollectionParser simple = new GmqCollectionParser(
                    (GmqParser parser, int index) =>
                    {
                        String val;
                        p.parseString(out val);
                        collection.Add(val);
                    }
            );
            simple.parseGmq(p);
        }

        public void parseStructBegin() { }
        public void parseStructEnd() { }
        public void parseStateUpdateMessageBegin() { }
        public void parseStateUpdateMessageEnd() { }

        public UInt64 parseActionInPublishable()
        {
            return parseUnsigned("action");
        }
    }


    public interface IPublishableComposer
    {
        void composeKey(String name); //TODO remove
        void appendRaw(ReadIteratorT it, int size = int.MaxValue);
        BufferT getBuffer();
        void composeAddress(UInt64[] baseAddr, UInt64 last);
        void composeAddressEnd();
        void composeAction(UInt64 actionId, bool emptyData);
        void composeInteger(String name, Int64 value, bool separator);
        void composeUnsigned(String name, UInt64 value, bool separator);
        void composeReal(String name, Double value, bool separator);
        void composeString(String name, String value, bool separator);
        void composePublishableStructBegin(String name);
        void composePublishableStructEnd(bool separator);
        void composeVector(String name, int size, Action<IPublishableComposer, int> composeDelegate, bool separator);
        void composeSimpleVector(String name, IList<Int64> collection, bool separator);
        void composeSimpleVector(String name, IList<UInt64> collection, bool separator);
        void composeSimpleVector(String name, IList<Double> collection, bool separator);
        void composeSimpleVector(String name, IList<String> collection, bool separator);

        void composeStructBegin();
        void composeStructEnd();
        void composeStateUpdateMessageBegin();
        void composeStateUpdateMessageEnd();
    }

    public class JsonPublishableComposer : IPublishableComposer
    {
        JsonComposer composer;

        public JsonPublishableComposer(BufferT buffer) { composer = new JsonComposer(buffer); }
        public void composeKey(String name) //TODO remove
        {
            composer.addNamePart(name);
        }
        public void appendRaw(ReadIteratorT it, int size = int.MaxValue)
        {
            composer.getBuffer().append(it, size);
        }

        public BufferT getBuffer()
        {
            return composer.getBuffer();
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
        public void composeAction(UInt64 actionId, bool emptyData)
        {
            composer.addNamePart("action");
            composer.composeUnsignedInteger(actionId);
            if (!emptyData)
                composer.append(',');
        }

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
        public void composeVector(String name, int size, Action<IPublishableComposer, int> composeDelegate, bool separator)
        {
            composer.addNamePart(name);
            composer.append('[');
            for (int i = 0; i < size; ++i)
            {
                if (i != 0)
                    composer.append(", ");

                composer.append("{");
                composeDelegate(this, i);
                composer.append("}");
            }
            composer.append(']');
            if (separator)
                composer.append(",");
        }
        public void composeSimpleVector(String name, IList<Int64> collection, bool separator)
        {
            composer.addNamePart(name);
            JsonCollectionComposer<Int64> simple = new JsonCollectionComposer<Int64>(collection,
                (JsonComposer c, Int64 val) => { composer.composeSignedInteger(val); }
            );
            simple.composeJson(composer);
            if (separator)
                composer.append(",");
        }
        public void composeSimpleVector(String name, IList<UInt64> collection, bool separator)
        {
            composer.addNamePart(name);
            JsonCollectionComposer<UInt64> simple = new JsonCollectionComposer<UInt64>(collection,
                (JsonComposer c, UInt64 val) => { composer.composeUnsignedInteger(val); }
            );
            simple.composeJson(composer);
            if (separator)
                composer.append(",");
        }
        public void composeSimpleVector(String name, IList<Double> collection, bool separator)
        {
            composer.addNamePart(name);
            JsonCollectionComposer<Double> simple = new JsonCollectionComposer<Double>(collection,
                (JsonComposer c, Double val) => { composer.composeReal(val); }
            );
            simple.composeJson(composer);
            if (separator)
                composer.append(",");
        }
        public void composeSimpleVector(String name, IList<String> collection, bool separator)
        {
            composer.addNamePart(name);
            JsonCollectionComposer<String> simple = new JsonCollectionComposer<String>(collection,
                (JsonComposer c, String val) => { composer.composeString(val); }
            );
            simple.composeJson(composer);
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

        public GmqPublishableComposer(BufferT buffer) { composer = new GmqComposer(buffer); }

        public void composeKey(String name) { }
        public void appendRaw(ReadIteratorT it, int size = int.MaxValue)
        {
            composer.getBuffer().append(it, size);
        }
        public BufferT getBuffer()
        {
            return composer.getBuffer();
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
        public void composeAction(UInt64 actionId, bool emptyData)
        {
            composer.composeUnsignedInteger(actionId);
        }

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
        public void composeVector(String name, int size, Action<IPublishableComposer, int> composeDelegate, bool separator)
        {
            composer.composeUnsignedInteger((UInt64)size);
            for (int i = 0; i < size; ++i)
                composeDelegate(this, i);
        }
        public void composeSimpleVector(String name, IList<Int64> collection, bool separator)
        {
            GmqCollectionComposer<Int64> simple = new GmqCollectionComposer<Int64>(collection,
                (GmqComposer c, Int64 val) => { composer.composeSignedInteger(val); }
            );
            simple.composeGmq(composer);
        }
        public void composeSimpleVector(String name, IList<UInt64> collection, bool separator)
        {
            GmqCollectionComposer<UInt64> simple = new GmqCollectionComposer<UInt64>(collection,
                (GmqComposer c, UInt64 val) => { composer.composeUnsignedInteger(val); }
            );
            simple.composeGmq(composer);
        }
        public void composeSimpleVector(String name, IList<Double> collection, bool separator)
        {
            GmqCollectionComposer<Double> simple = new GmqCollectionComposer<Double>(collection,
                (GmqComposer c, Double val) => { composer.composeReal(val); }
            );
            simple.composeGmq(composer);
        }
        public void composeSimpleVector(String name, IList<String> collection, bool separator)
        {
            GmqCollectionComposer<String> simple = new GmqCollectionComposer<String>(collection,
                (GmqComposer c, String val) => { composer.composeString(val); }
            );
            simple.composeGmq(composer);
        }

        public void composeStructBegin() { }
        public void composeStructEnd() { }
        public void composeStateUpdateMessageBegin() { }
        public void composeStateUpdateMessageEnd()
        {
            composer.composeUnsignedInteger(0);
        }
    }

    public class PublisherVectorWrapper<T> : IList<T>
    {
        //public delegate void ComposeDelegate(IPublishableComposer composer, T val);
        //public delegate T ElementWrapperDelegate(T val, IPublishableComposer composer, UInt64[] address);

        IList<T> t;
        IPublishableComposer composer;
        UInt64[] address;
        Action<IPublishableComposer, T> composeDelegate;
        Func<T, IPublishableComposer, UInt64[], T> elementWrapperDelegate;


        public PublisherVectorWrapper(IList<T> t, IPublishableComposer composer, UInt64[] address,
            Action<IPublishableComposer, T> composeDelegate, Func<T, IPublishableComposer, UInt64[], T> elementWrapperDelegate)
        {
            this.t = t;
            this.composer = composer;
            this.address = address;
            this.composeDelegate = composeDelegate;
            this.elementWrapperDelegate = elementWrapperDelegate;
        }
        public T this[int index]
        {
            get
            {
                if (elementWrapperDelegate != null)
                    return elementWrapperDelegate(t[index], composer, Publishable.makeAddress(address, (UInt64)index));
                else
                    return t[index];
            }
            set
            {
                t[index] = value;
                composer.composeAddress(address, (UInt64)index);
                composer.composeAction((UInt64)Publishable.ActionOnVector.update_at, false);
                composeDelegate(composer, value);
                composer.composeAddressEnd();
            }
        }
        public int Count => t.Count;

        public bool IsReadOnly => false;

        public void Add(T item)
        {
            throw new NotImplementedException();
        }

        public void Clear()
        {
            throw new NotImplementedException();
        }

        public bool Contains(T item)
        {
            return t.Contains(item);
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            t.CopyTo(array, arrayIndex);
        }

        public IEnumerator<T> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        public int IndexOf(T item)
        {
            return t.IndexOf(item);
        }

        public void Insert(int index, T item)
        {
            t.Insert(index, item);
            composer.composeAddress(address, (UInt64)index);
            composer.composeAction((UInt64)Publishable.ActionOnVector.insert_single_before, false);
            composeDelegate(composer, item);
            composer.composeAddressEnd();
        }

        public bool Remove(T item)
        {
            throw new NotImplementedException();
        }

        public void RemoveAt(int index)
        {
            t.RemoveAt(index);
            composer.composeAddress(address, (UInt64)index);
            composer.composeAction((UInt64)Publishable.ActionOnVector.remove_at, true);
            composer.composeAddressEnd();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }

    }

    public class SubscriberVectorWrapper<T, U> : IList<T> where U : T
    {
        IList<U> t;

        public SubscriberVectorWrapper(IList<U> t)
        {
            this.t = t;
        }
        //public SubscriberVectorWrapper()
        //{
        //    this.t = new List<T>();
        //}
        public T this[int index] { get => t[index]; set => throw new InvalidOperationException(); }

        public int Count => t.Count;

        public bool IsReadOnly => true;

        public void Add(T item)
        {
            throw new InvalidOperationException();
        }

        public void Clear()
        {
            throw new InvalidOperationException();
        }

        public bool Contains(T item)
        {
            return item is U ? t.Contains((U)item) : false;
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            throw new NotImplementedException();
        }

        public IEnumerator<T> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        public int IndexOf(T item)
        {
            return item is U ? t.IndexOf((U)item) : -1;
        }

        public void Insert(int index, T item)
        {
            throw new InvalidOperationException();
        }

        public bool Remove(T item)
        {
            throw new InvalidOperationException();
        }

        public void RemoveAt(int index)
        {
            throw new InvalidOperationException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }

    public class SubscriberVectorHelper
    {

        public static bool parseVectorPrimitive<T>(IPublishableParser parser, IList<T> data, int index,
    Func<IPublishableParser, T> elementParser, Action<int, T> notifyUpdate,
    Action<int> notifyInsert, Action<int> notifyRemove)
        {
            bool changed = false;
            Publishable.ActionOnVector action = (Publishable.ActionOnVector)parser.parseActionInPublishable();
            switch (action)
            {
                case Publishable.ActionOnVector.update_at:
                    {
                        T newVal = elementParser(parser);
                        if (!data[index].Equals(newVal))
                        {
                            T oldVal = data[index];
                            data[index] = newVal;
                            changed = true;
                            notifyUpdate(index, oldVal);
                        }
                        break;
                    }
                case Publishable.ActionOnVector.insert_single_before:
                    {
                        T newVal = elementParser(parser);
                        data.Insert(index, newVal);
                        changed = true;
                        notifyInsert(index);
                        break;
                    }
                case Publishable.ActionOnVector.remove_at:
                    {
                        data.RemoveAt(index);
                        changed = true;
                        notifyRemove(index);
                        break;
                    }
                default:
                    throw new Exception();
            }

            return changed;
        }

    }
} // namespace globalmq::marshalling
