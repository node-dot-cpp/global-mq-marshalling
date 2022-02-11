/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the OLogN Technologies AG nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
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

#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <memory>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace System::IO;

namespace interopbufferdotnet {

	public value struct InteropBufferMemento
	{
		uint64_t size;
		uint64_t capacity;
		uint8_t* buffer;
	};

	static constexpr int MIN_BUFFER = 1024;

	public ref class InteropReadIter : globalmq::marshalling::ReadIteratorT
	{
		const uint8_t* begin = nullptr;
		const uint8_t* end = nullptr;
		int currentOffset = 0;

	public:
		//using BufferT = InteropBuffer;

	public:
		InteropReadIter(const uint8_t* begin, const uint8_t* end) :begin(begin), end(end) { }

		virtual bool isData() { return begin < end; }
		virtual int directlyAvailableSize() { assert(begin <= end); return end - begin; }
		virtual uint8_t get() { assert(begin < end); return *begin; }
		virtual wchar_t getChar() { assert(begin < end); return (wchar_t)*begin; }
		virtual void inc() { ++begin; ++currentOffset; }
		virtual ReadOnlySpan<uint8_t> read(int size)
		{
			assert(begin < end);
			size = std::min(size, (int)(end - begin));
			ReadOnlySpan<uint8_t> res((void*)begin, size);
			currentOffset += size;
			begin += size;
			return res;

		}
		virtual void skip(int size)
		{
			size = std::min(size, (int)(end - begin));
			begin += size;
			currentOffset += size;
		}
		virtual int offset() { return currentOffset; }
		virtual globalmq::marshalling::ReadIteratorT^ shallowClone()
		{
			InteropReadIter^ cl = gcnew InteropReadIter(begin, end);
			cl->currentOffset = currentOffset;
			return cl;
		}
	};

	public ref class InteropBuffer : globalmq::marshalling::BufferT {
	public:

		InteropReadIter^ getReadIter() { return gcnew InteropReadIter(_data, _data + _size); }

	private:
		int _size = 0;
		int _capacity = 0;
		uint8_t* _data = nullptr;

	private:
		void ensureCapacity(int sz) { // NOTE: may invalidate pointers
			if (_data == nullptr) {
				reserve(sz);
			}
			else if (sz > _capacity) {
				int cp = std::max(sz, MIN_BUFFER);
				std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);
				memcpy(tmp.get(), _data, _size);
				_capacity = cp;
				delete[] _data;
				_data = tmp.release();
			}
		}

		void reserve(int sz) {
			assert(_size == 0);
			assert(_capacity == 0);
			assert(_data == nullptr);

			size_t cp = std::min(sz, MIN_BUFFER);
			std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);

			_capacity = cp;
			delete[] _data;
			_data = tmp.release();
		}

		InteropBuffer(size_t _size, size_t _capacity, uint8_t* _data) : _size(_size), _capacity(_capacity), _data(_data) {}

	public:
		InteropBuffer() {}
		~InteropBuffer() { release(); }
		!InteropBuffer() { release(); }

		static InteropBuffer^ makeFromMemento(InteropBufferMemento memento)
		{
			return gcnew InteropBuffer(memento.size, memento.capacity, memento.buffer);
		}

		InteropBufferMemento releaseToMemento()
		{
			InteropBufferMemento memento{ _size ,_capacity, _data };
			_data = nullptr;
			_size = 0;
			_capacity = 0;

			return memento;
		}

		void release()
		{
			if (_data != nullptr)
			{
				delete[] _data;
				_data = nullptr;
				_size = 0;
				_capacity = 0;
			}
		}
		virtual int size() { return _size; }
		virtual bool empty() { return _size == 0; }
		virtual int capacity() { return _capacity; }
		void append(ReadOnlySpan<uint8_t> dt)
		{
			ensureCapacity(_size + dt.Length);
			dt.CopyTo(Span<uint8_t>((void*)(_data + _size), dt.Length));
			_size += dt.Length;
		}
		virtual void append(array<uint8_t>^ dt)
		{
			append(ReadOnlySpan<uint8_t>(dt));
		}

		virtual void append(globalmq::marshalling::ReadIteratorT^ it, int size)
		{
			while (size > 0 && it->isData())
			{
				int avail = it->directlyAvailableSize();
				int toRead = std::min(size, avail);
				ReadOnlySpan<uint8_t> sp = it->read(toRead);
				append(sp);
				size -= sp.Length;
			}
		}
		virtual void appendAscii(String^ str)
		{
			InteropBuffer::append(Encoding::ASCII->GetBytes(str));
		}
		virtual void appendUint8(uint8_t val)
		{
			ensureCapacity(_size + 1);
			_data[_size] = val;
			++_size;
		}
		virtual void appendUint8(wchar_t val)
		{
			ensureCapacity(_size + 1);
			_data[_size] = (uint8_t)val;
			++_size;
		}
		virtual globalmq::marshalling::ReadIteratorT^ getReadIterator()
		{
			return getReadIter();
		}

		void writeToFile(String^ path)
		{
			FileStream^ fs = File::OpenWrite(path);
			try
			{
				ReadOnlySpan<uint8_t> sp(_data, _size);
				fs->Write(sp);
			}
			finally
			{
				fs->Close();
			}
		}
		static InteropBuffer^ readFromFile(String^ path)
		{
			FileStream^ fs = File::OpenRead(path);
			try
			{
				InteropBuffer^ b = gcnew InteropBuffer();
				int sz = (int)fs->Length;
				b->reserve(sz);
				Span<uint8_t> sp(b->_data, sz);
				fs->Read(sp);
				b->_size = sz;
				return b;
			}
			finally
			{
				fs->Close();
			}
		}
	};

}




