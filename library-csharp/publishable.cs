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
using System.Diagnostics;

namespace globalmq.marshalling
{
    public class Publishable
    {
        public enum ActionOnVector { update_at = 1, insert_single_before = 2, remove_at = 3 };
        public enum ActionOnDictionary { update_value = 1, insert = 2, remove = 3 };
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
        void parseKey(String expectedName);
        ReadIteratorT getIterator();
        bool parseAddress(ref UInt64[] addr);
        void parseAddressEnd();
        Int64 parseInteger(String expectedName);
        UInt64 parseUnsigned(String expectedName);
        Double parseReal(String expectedName);
        String parseString(String expectedName);
        void parseDictionary<K, V>(String expectedName, IDictionary<K, V> collection, Func<IPublishableParser, K> parseKeyDelegate, Func<IPublishableParser, V> parseValueDelegate);
        void parseVector2<T>(String expectedName, IList<T> collection, Func<IPublishableParser, T> parseDelegate);

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
            if(!String.IsNullOrEmpty(expectedName))
            {
                String key;
                p.readKeyFromJson(out key);
                if (key != expectedName)
                    throw new Exception(); // bad format
            }
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
        public void parseDictionary<K, V>(String expectedName, IDictionary<K, V> collection, Func<IPublishableParser, K> parseKeyDelegate, Func<IPublishableParser, V> parseValueDelegate)
        {
            Debug.Assert(collection.Count == 0);
            parseKey(expectedName);
            p.skipDelimiter('{');

            if (!p.isDelimiter('}')) // there are some items there
            {
                for (int i = 0; /*!newFormat || i < sz*/; ++i)
                {
                    p.skipDelimiter('{');
                    K k = parseKeyDelegate(this);
    
                    if(p.isDelimiter(',')) // usually eaten by parseKeyDelegate
                        p.skipDelimiter(',');

                    V v = parseValueDelegate(this);
                    p.skipDelimiter('}');

                    collection.Add(k, v);
                    if (p.isDelimiter(','))
                    {
                        p.skipDelimiter(',');
                        continue;
                    }
                    if (p.isDelimiter('}'))
                    {
                        p.skipDelimiter('}');
                        break;
                    }
                }
            }
            else
                p.skipDelimiter('}');

            if (p.isDelimiter(','))
                p.skipDelimiter(',');
        }

        public void parseVector2<T>(String expectedName, IList<T> collection, Func<IPublishableParser, T> parseDelegate)
        {
            Debug.Assert(collection.Count == 0);
            parseKey(expectedName);
            p.skipDelimiter('[');

            if (!p.isDelimiter(']')) // there are some items there
            {
                for (int i = 0; /*!newFormat || i < sz*/; ++i)
                {
                    T t = parseDelegate(this);
                    collection.Add(t);
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

        public void parseStructBegin()
        {
            p.skipDelimiter('{');
        }
        public void parseStructEnd()
        {
            p.skipDelimiter('}');

            if (p.isData() && p.isDelimiter(','))
                p.skipDelimiter(',');
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
        public void parseDictionary<K, V>(String expectedName, IDictionary<K, V> collection, Func<IPublishableParser, K> parseKeyDelegate, Func<IPublishableParser, V> parseValueDelegate)
        {
            Debug.Assert(collection.Count == 0);
            int sz;
            p.parseUnsignedInteger(out sz);
            for (int i = 0; i < sz; ++i)
            {
                K k = parseKeyDelegate(this);
                V v = parseValueDelegate(this);
                collection.Add(k, v);
            }
        }
        public void parseVector2<T>(String expectedName, IList<T> collection, Func<IPublishableParser, T> parseDelegate)
        {
            Debug.Assert(collection.Count == 0);
            int sz;
            p.parseUnsignedInteger(out sz);
            for (int i = 0; i < sz; ++i)
            {
                T t = parseDelegate(this);
                collection.Add(t);
            }

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
        void composeKey(String name);
        void addSeparator(bool separator);
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
        void composeVector2<T>(String name, IList<T> collection, Action<IPublishableComposer, T> composeDelegate, bool separator);
        void composeDictionary<K, V>(String name, IDictionary<K, V> collection, Action<IPublishableComposer, K> composeKeyDelegate, Action<IPublishableComposer, V> composeValueDelegate, bool separator);

        void composeStructBegin();
        void composeStructEnd();
        void composeStateUpdateMessageBegin();
        void composeStateUpdateMessageEnd();
    }

    public class JsonPublishableComposer : IPublishableComposer
    {
        JsonComposer composer;

        public JsonPublishableComposer(BufferT buffer) { composer = new JsonComposer(buffer); }
        public void composeKey(String name)
        {
            composer.addNamePart(name);
        }
        public void addSeparator(bool separator)
        {
            if (separator)
                composer.append(",");

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
        public void composeVector2<T>(String name, IList<T> collection, Action<IPublishableComposer, T> composeDelegate, bool separator)
        {
            composer.addNamePart(name);
            composer.append('[');
            int i = 0;
            foreach (T t in collection)
            {
                if (i != 0)
                    composer.append(", ");

                composeDelegate(this, t);
                ++i;
            }
            composer.append(']');
            if (separator)
                composer.append(",");
        }
        public void composeDictionary<K, V>(String name, IDictionary<K, V> collection, Action<IPublishableComposer, K> composeKeyDelegate, Action<IPublishableComposer, V> composeValueDelegate, bool separator)
        {
            composer.addNamePart(name);
            composer.append('{');

            int i = 0;
            foreach (KeyValuePair<K, V> each in collection)
            {
                if (i != 0)
                    composer.append(", ");

                composer.append('{');
                composeKeyDelegate(this, each.Key);
                composer.append(", ");
                composeValueDelegate(this, each.Value);
                composer.append('}');
                ++i;
            }
            composer.append('}');
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
        public void addSeparator(bool separator) { }

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
        public void composeVector2<T>(String name, IList<T> collection, Action<IPublishableComposer, T> composeDelegate, bool separator)
        {
            composer.composeUnsignedInteger((UInt64)collection.Count);
            for (int i = 0; i < collection.Count; ++i)
                composeDelegate(this, collection[i]);
        }
        public void composeDictionary<K, V>(String name, IDictionary<K, V> collection, Action<IPublishableComposer, K> composeKeyDelegate, Action<IPublishableComposer, V> composeValueDelegate, bool separator)
        {
            composer.composeUnsignedInteger((UInt64)collection.Count);
            foreach(KeyValuePair<K, V> each in collection)
            {
                composeKeyDelegate(this, each.Key);
                composeValueDelegate(this, each.Value);
            }
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

        public bool IsReadOnly => t.IsReadOnly;

        public void Add(T item)
        {
            this.Insert(Count, item);
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
            return t.GetEnumerator();
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
            int ix = t.IndexOf(item);
            if (ix == -1)
                return false;
            else
            {
                this.RemoveAt(ix);
                return true;
            }
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
            return ((IEnumerable)t).GetEnumerator();
        }

    }

    public class VectorWrapper<T> : IList<T>// where U : T
    {
        IList<T> t;
        Func<T, T> elementWrapperDelegate;


        public VectorWrapper(IList<T> t, Func<T, T> elementWrapperDelegate)
        {
            this.t = t;
            this.elementWrapperDelegate = elementWrapperDelegate;
        }
        public T this[int index]
        {
            get => elementWrapperDelegate != null ? elementWrapperDelegate(t[index]) : t[index];
            set => throw new NotSupportedException();
        }

        public int Count => t.Count;

        public bool IsReadOnly => true;

        public void Add(T item)
        {
            throw new NotSupportedException();
        }

        public void Clear()
        {
            throw new NotSupportedException();
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
            return t.GetEnumerator();
        }

        public int IndexOf(T item)
        {
            return t.IndexOf(item);
        }

        public void Insert(int index, T item)
        {
            throw new NotSupportedException();
        }

        public bool Remove(T item)
        {
            throw new NotSupportedException();
        }

        public void RemoveAt(int index)
        {
            throw new NotSupportedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)t).GetEnumerator();
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

    public class PublisherDictionaryWrapper<K, V> : IDictionary<K, V>
    {
        IDictionary<K, V> t;
        IPublishableComposer composer;
        UInt64[] address;
        Action<IPublishableComposer, K, bool> composeKeyDelegate;
        Action<IPublishableComposer, V> composeValueDelegate;
        Func<K, V, IPublishableComposer, UInt64[], V> valueWrapperDelegate;


        public PublisherDictionaryWrapper(IDictionary<K, V> t, IPublishableComposer composer, UInt64[] address,
            Action<IPublishableComposer, K, bool> composeKeyDelegate, Action<IPublishableComposer, V> composeValueDelegate,
            Func<K, V, IPublishableComposer, UInt64[], V> valueWrapperDelegate)
        {
            this.t = t;
            this.composer = composer;
            this.address = address;
            this.composeKeyDelegate = composeKeyDelegate;
            this.composeValueDelegate = composeValueDelegate;
            this.valueWrapperDelegate = valueWrapperDelegate;
        }
        public V this[K key]
        {
            get
            {
                if (valueWrapperDelegate != null)
                    return valueWrapperDelegate(key, t[key], composer, address);
                else
                    return t[key];
            }
            set
            {
                if (t.ContainsKey(key))
                    updateValue(key, value);
                else
                    Add(key, value);
            }
        }
        public int Count => t.Count;
        public ICollection<K> Keys => t.Keys;
        public ICollection<V> Values => t.Values;
        bool ICollection<KeyValuePair<K, V>>.IsReadOnly => ((ICollection<KeyValuePair<K, V>>)t).IsReadOnly;
        void updateValue(K key, V value)
        {
            Debug.Assert(t.ContainsKey(key));
            t[key] = value;
            composer.composeAddress(address, (UInt64)Publishable.ActionOnDictionary.update_value);
            //composer.composeAction((UInt64)Publishable.ActionOnDictionary.update_value, false);
            composeKeyDelegate(composer, key, true);
            composeValueDelegate(composer, value);
            composer.composeAddressEnd();
        }
        public void Add(K key, V value)
        {
            t.Add(key, value);
            composer.composeAddress(address, (UInt64)Publishable.ActionOnDictionary.insert);
            //composer.composeAction((UInt64)Publishable.ActionOnDictionary.insert, false);
            composeKeyDelegate(composer, key, true);
            composeValueDelegate(composer, value);
            composer.composeAddressEnd();
        }
        public void Add(KeyValuePair<K, V> item)
        {
            throw new NotImplementedException();
        }
        public void Clear()
        {
            throw new NotImplementedException();
        }

        public bool ContainsKey(K key)
        {
            return t.ContainsKey(key);
        }

        public IEnumerator<KeyValuePair<K, V>> GetEnumerator()
        {
            return t.GetEnumerator();
        }

        public bool Remove(K key)
        {
            bool res = t.Remove(key);
            if (res)
            {
                composer.composeAddress(address, (UInt64)Publishable.ActionOnDictionary.remove);
                //composer.composeAction((UInt64)Publishable.ActionOnDictionary.remove, false);
                composeKeyDelegate(composer, key, false);
                composer.composeAddressEnd();
            }
            return res;
        }

        public bool Remove(KeyValuePair<K, V> item)
        {
            throw new NotImplementedException();
        }

        public bool TryGetValue(K key, out V value)
        {
            return t.TryGetValue(key, out value);
        }

        bool ICollection<KeyValuePair<K, V>>.Contains(KeyValuePair<K, V> item)
        {
            return ((ICollection<KeyValuePair<K, V>>)t).Contains(item);
        }

        void ICollection<KeyValuePair<K, V>>.CopyTo(KeyValuePair<K, V>[] array, int arrayIndex)
        {
            ((ICollection<KeyValuePair<K, V>>)t).CopyTo(array, arrayIndex);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)t).GetEnumerator();
        }
    }

    public class DictionaryWrapper<K, V> : IDictionary<K, V>
    {
        IDictionary<K, V> t;
        Func<V, V> valueWrapperDelegate;

        public DictionaryWrapper(IDictionary<K, V> t, Func<V, V> valueWrapperDelegate)
        {
            this.t = t;
            this.valueWrapperDelegate = valueWrapperDelegate;
        }
        public V this[K key]
        {
            get => valueWrapperDelegate != null ? valueWrapperDelegate(t[key]) : t[key];
            set => throw new NotSupportedException();
        }
        public int Count => t.Count;
        public ICollection<K> Keys => t.Keys;
        public ICollection<V> Values => t.Values;
        bool ICollection<KeyValuePair<K, V>>.IsReadOnly => ((ICollection<KeyValuePair<K, V>>)t).IsReadOnly;
        public void Add(K key, V value)
        {
            throw new NotSupportedException();
        }
        public void Add(KeyValuePair<K, V> item)
        {
            throw new NotSupportedException();
        }
        public void Clear()
        {
            throw new NotSupportedException();
        }

        public bool ContainsKey(K key)
        {
            return t.ContainsKey(key);
        }

        public IEnumerator<KeyValuePair<K, V>> GetEnumerator()
        {
            return t.GetEnumerator();
        }

        public bool Remove(K key)
        {
            throw new NotSupportedException();
        }

        public bool Remove(KeyValuePair<K, V> item)
        {
            throw new NotSupportedException();
        }

        public bool TryGetValue(K key, out V value)
        {
            return t.TryGetValue(key, out value);
        }

        bool ICollection<KeyValuePair<K, V>>.Contains(KeyValuePair<K, V> item)
        {
            return ((ICollection<KeyValuePair<K, V>>)t).Contains(item);
        }

        void ICollection<KeyValuePair<K, V>>.CopyTo(KeyValuePair<K, V>[] array, int arrayIndex)
        {
            ((ICollection<KeyValuePair<K, V>>)t).CopyTo(array, arrayIndex);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable)t).GetEnumerator();
        }

        public bool UpdateValue(K key, V value)
        {
            throw new NotSupportedException();
        }
    }

} // namespace globalmq::marshalling
