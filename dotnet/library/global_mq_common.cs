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
using System.Diagnostics;
using System.Text;

namespace globalmq.marshalling
{

    public interface ReadIterT
    {
        bool isData();
        int directlyAvailableSize();
        byte get();
        char getChar();
        void inc();
        ReadOnlySpan<byte> read(int size);
        void skip(int size);
        int offset();
    }

    public interface BufferT
    {
        int size();
        bool empty();
        int capacity();
        void append(byte[] dt);
        void appendAscii(string str);
        void appendUint8(byte val);
        void appendUint8(char val);
    }
    public class SimpleBuffer : BufferT
    {
        static readonly int MIN_BUFFER = 1024;

        int _size = 0;
        int _capacity = 0;
        byte[] _data;

        public ReadIter getReadIterator() { return new ReadIter(this); }

        void ensureCapacity(int sz) // NOTE: may invalidate pointers
        {
            if (_data == null)
            {
                reserve(sz);
            }
            else if (sz > _capacity)
            {
                int cp = Math.Max(sz, MIN_BUFFER);
                byte[] tmp = new byte[cp];
                Buffer.BlockCopy(_data, 0, tmp, 0, _size);
                _capacity = cp;
                _data = tmp;
            }
        }

        void reserve(int sz)
        {
            Debug.Assert(_size == 0);
            Debug.Assert(_capacity == 0);
            Debug.Assert(_data == null);

            int cp = Math.Max(sz, MIN_BUFFER);
            byte[] tmp = new byte[cp];

            _capacity = cp;
            _data = tmp;
        }
        public SimpleBuffer() { }
        public int size() { return _size; }
        public bool empty() { return _size == 0; }
        public int capacity() { return _capacity; }
        public void append(byte[] dt) // NOTE: may invalidate pointers
        {
            ensureCapacity(_size + dt.Length);
            Buffer.BlockCopy(dt, 0, _data, _size, dt.Length);
            _size += dt.Length;
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

        public class ReadIter : ReadIterT
        {
            int _size = 0;
            int currentOffset = 0;
            byte[] _data;
            public ReadIter() { }
            public ReadIter(SimpleBuffer buff)
            {
                _data = buff._data;
                _size = buff._size;
            }
            public bool isData() { return currentOffset < _size; }
            public int directlyAvailableSize() { return _data != null ? _size - currentOffset : 0; }
            public byte get()
            {
                Debug.Assert(currentOffset != _size);
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
                if (_data != null)
                {
                    size = size <= (_size - currentOffset) ? size : _size - currentOffset;
                    ReadOnlySpan<byte> res = new ReadOnlySpan<byte>(_data, currentOffset, size);
                    currentOffset += size;
                    return res;
                }
                else
                    return ReadOnlySpan<byte>.Empty;
            }
            public void skip(int size)
            {
                if (_data != null)
                {
                    size = size <= (_size - currentOffset) ? size : _size - currentOffset;
                    currentOffset += size;
                }
            }
            public int offset() { return currentOffset; }
            public string debugToString()
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
