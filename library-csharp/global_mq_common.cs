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

namespace globalmq.marshalling
{

    public interface ReadIteratorT
    {
        bool isData();
        int directlyAvailableSize();
        byte get();
        char getChar();
        void inc();
        ReadOnlySpan<byte> read(int size);
        void skip(int size);
        int offset();
        ReadIteratorT shallowClone();
    }

    public interface BufferT
    {
        int size();
        bool empty();
        int capacity();
        void append(byte[] dt);
        //void append(ReadOnlySpan<byte> dt);
        void append(ReadIteratorT it, int size);
        void appendAscii(string str);
        void appendUint8(byte val);
        void appendUint8(char val);
        ReadIteratorT getReadIterator();

    }
    public class SimpleBuffer : BufferT
    {
        static readonly int MIN_BUFFER = 1024;

        int _size = 0;
        //int _capacity = 0;
        byte[] _data;

        public ReadIteratorT getReadIterator() { return new ReadIter(_data, _size); }

        void ensureCapacity(int sz) // NOTE: may invalidate pointers
        {
            if (_data == null)
            {
                reserve(sz);
            }
            else if (sz > _data.Length)
            {
                int cp = Math.Max(sz, MIN_BUFFER);
                byte[] tmp = new byte[cp];
                Buffer.BlockCopy(_data, 0, tmp, 0, _size);
                //_capacity = cp;
                _data = tmp;
            }
        }

        void reserve(int sz)
        {
            Debug.Assert(_size == 0);
            //Debug.Assert(_capacity == 0);
            Debug.Assert(_data == null);

            int cp = Math.Max(sz, MIN_BUFFER);
            byte[] tmp = new byte[cp];

            //_capacity = cp;
            _data = tmp;
        }
        public SimpleBuffer() { }
        public SimpleBuffer(byte[] _data, int _size)
        {
            this._data = _data;
            this._size = _size;
        }
        public void writeToFile(string path)
        {
            using (FileStream fs = File.OpenWrite(path))
            {
                fs.Write(_data, 0, _size);
            }
        }
        public static SimpleBuffer readFromFile(string path)
        {
            using (FileStream fs = File.OpenRead(path))
            {
                SimpleBuffer b = new SimpleBuffer();
                int sz = (int)fs.Length;
                b.reserve(sz);
                fs.Read(b._data, 0, sz);
                b._size = sz;
                return b;
            }
        }
        public int size() { return _size; }
        public bool empty() { return _size == 0; }
        public int capacity() { return _data.Length; }
        public void append(byte[] dt) // NOTE: may invalidate pointers
        {
            ensureCapacity(_size + dt.Length);
            Buffer.BlockCopy(dt, 0, _data, _size, dt.Length);
            _size += dt.Length;
        }
        void append(ReadOnlySpan<byte> dt)
        {
            ensureCapacity(_size + dt.Length);
            dt.CopyTo(new Span<byte>(_data, _size, dt.Length));
            _size += dt.Length;
        }

        public void append(ReadIteratorT it, int size)
        {
            while(size > 0 && it.isData())
            {
                int avail = it.directlyAvailableSize();
                int toRead = Math.Min(size, avail);
                ReadOnlySpan<byte> sp = it.read(toRead);
                append(sp);
                size -= sp.Length;
            }
        }

        public void appendAscii(string str)
        {
            append(Encoding.ASCII.GetBytes(str));
        }

        public void appendUint8(byte val)
        {
            ensureCapacity(size() + 1);
            _data[_size] = val;
            ++_size;
        }

        public void appendUint8(char val)
        {
            appendUint8((byte)val);
        }

        public override bool Equals(object obj)
        {
            if (obj is SimpleBuffer buffer && _size == buffer._size)
            {
                for (int i = 0; i < _size; ++i)
                {
                    if (_data[i] != buffer._data[i])
                        return false;
                }
                return true;
            }
            return false;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(_size, _data);
        }
        public string toDebugString()
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < _size; ++i)
            {
                sb.Append((char)_data[i]);
            }

            return sb.ToString();
        }

        public class ReadIter : ReadIteratorT
        {
            int _size = 0;
            int currentOffset = 0;
            byte[] _data;
            public ReadIter(byte[] _data, Int32 _size)
            {
                this._data = _data;
                this._size = _size;
                //if (_data == null)
                //    throw new ArgumentNullException();
                //if (_size < 0 || _size > _data.Length)
                //    throw new ArgumentOutOfRangeException();
            }
            public bool isData() { return _data != null && currentOffset < _size; }
            public int directlyAvailableSize() { return _data != null ? _size - currentOffset : 0; }
            public byte get()
            {
                return _data[currentOffset];
            }
            public char getChar()
            {
                return (char)get();
            }
            public void inc()
            {
                ++currentOffset;
            }
            public ReadOnlySpan<byte> read(int size)
            {
                Debug.Assert(isData());
                size = Math.Min(size, (_size - currentOffset));
                ReadOnlySpan<byte> res = new ReadOnlySpan<byte>(_data, currentOffset, size);
                currentOffset += size;
                return res;
            }

            public void skip(int size)
            {
                size = Math.Min(size, (_size - currentOffset));
                currentOffset += size;
            }
            public int offset() { return currentOffset; }
            public ReadIteratorT shallowClone()
            {
                return (ReadIteratorT)MemberwiseClone();
            }

            public string toDebugString()
            {
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < _size; ++i)
                {
                    sb.Append((char)_data[i]);
                }

                return sb.ToString();
            }
        }
    }
} // namespace globalmq
