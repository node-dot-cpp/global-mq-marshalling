/* -------------------------------------------------------------------------------
* Copyright (c) 2020, OLogN Technologies AG
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

#ifndef GLOBAL_MQ_COMMON_H
#define GLOBAL_MQ_COMMON_H

#include <fmt/format.h>

/////////////////////////////////////////
// means for replacing std:: by nodecpp::
#ifdef GMQ_USE_EXTERNAL_DEFINITIONS
#include GMQ_USE_EXTERNAL_DEFINITIONS
#endif // GMQ_USE_EXTERNAL_DEFINITIONS

#ifndef GMQ_ASSERT
#include <assert.h>
#define GMQ_ASSERT( condition, ... ) assert( condition )
#endif // GMQ_ASSERT

#ifndef GMQ_COLL
#include <string>
#include <unordered_map>
#include <vector>
#define GMQ_COLL std::
#endif // GMQ_COLL
/////////////////////////////////////////


namespace globalmq::marshalling {

	static constexpr size_t MIN_BUFFER = 1024;

	struct BufferMemento
	{
		uint64_t size;
		uint64_t capacity;
		uint8_t* buffer;
	};

	class Buffer {
	public:
		class ReadIter
		{
			const uint8_t* begin = nullptr;
			const uint8_t* end = nullptr;
			size_t currentOffset = 0;

		public:
			using BufferT = Buffer;
			using CharT = char;

		public:
			ReadIter() {}
			ReadIter( const Buffer& buff ) {
				begin = buff.begin();
				end = buff.begin() + buff.size();
			}
			bool isData() { return end > begin; }
			size_t directlyAvailableSize() { return begin != nullptr ? end - begin : 0; }
			const uint8_t* directRead( size_t sz ) { 
				if ( begin != nullptr ) { 
					assert( end >= begin && sz <= static_cast<size_t>(end - begin) ); 
					auto ret = begin; 
					begin += sz; 
					currentOffset += sz;
					return ret; 
				}
				else
					return nullptr;
			}
			uint8_t operator * ()
			{
				GMQ_ASSERT( begin != end ); 
				return *begin;
			}
			void operator ++ () 
			{
				begin++;
				++currentOffset;
			}
			size_t read( void* buff, size_t size )
			{
				if ( begin )
				{
					assert( end >= begin ); 
					size = size <= static_cast<size_t>(end - begin) ? size : static_cast<size_t>(end - begin);
					memcpy( buff, begin, size );
					begin += size;
					currentOffset += size;
				}
				else
					size = 0;
				return size; 
			}
			size_t skip( size_t size )
			{
				if ( begin )
				{
					assert( end >= begin ); 
					size = size <= static_cast<size_t>(end - begin) ? size : static_cast<size_t>(end - begin);
					begin += size;
					currentOffset += size;
				}
				else
					size = 0;
				return size;
			}
			size_t offset() const { return currentOffset; }
			CharT readChar()
			{
				GMQ_ASSERT( begin != end ); 
				++currentOffset;
				return *begin++;
			}
		};

		using ReadIteratorT = ReadIter;

		ReadIter getReadIter() { return ReadIter( *this ); }

	private:
		size_t _size = 0;
		size_t _capacity = 0;
		std::unique_ptr<uint8_t[]> _data;

	private:
		void ensureCapacity(size_t sz) { // NOTE: may invalidate pointers
			if (_data == nullptr) {
				reserve(sz);
			}
			else if (sz > _capacity) {
				size_t cp = std::max(sz, MIN_BUFFER);
				std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);
				memcpy(tmp.get(), _data.get(), _size);
				_capacity = cp;
				_data = std::move(tmp);
			}
		}

	protected:
		void reserve(size_t sz) {
			GMQ_ASSERT( _size == 0 ); 
			GMQ_ASSERT( _capacity == 0 );
			GMQ_ASSERT( _data == nullptr );

			size_t cp = std::max(sz, MIN_BUFFER);
			std::unique_ptr<uint8_t[]> tmp(new uint8_t[cp]);

			_capacity = cp;
			_data = std::move(tmp);
		}

	public:
		Buffer() {}
//		Buffer(size_t res) { reserve(res); }
		Buffer(Buffer&& p) noexcept {
			std::swap(_size, p._size);
			std::swap(_capacity, p._capacity);
			std::swap(_data, p._data);
		}
		Buffer& operator = (Buffer&& p) noexcept {
			std::swap(_size, p._size);
			std::swap(_capacity, p._capacity);
			std::swap(_data, p._data);
			return *this;
		}
		Buffer(const Buffer&) = delete;
		Buffer& operator = (const Buffer& p) = delete;

		static Buffer makeFromMemento(BufferMemento memento)
		{
			Buffer tmp;
			tmp._size = memento.size;
			tmp._capacity = memento.capacity;
			tmp._data.reset(memento.buffer);

			return tmp;
		}

		BufferMemento releaseToMemento()
		{
			BufferMemento memento{ _size ,_capacity, _data.release() };
			// _data = nullptr;
			_size = 0;
			_capacity = 0;

			return memento;
		}

		size_t size() const { return _size; }
		bool empty() const { return _size == 0; }
//		size_t capacity() const { return _capacity; }
		uint8_t* begin() { return _data.get(); }
		const uint8_t* begin() const { return _data.get(); }
		uint8_t* end() { return _data.get() + _size; }
		const uint8_t* end() const { return _data.get() + _size; }

		void append(const void* dt, size_t sz) { // NOTE: may invalidate pointers
			ensureCapacity(_size + sz);
			memcpy(end(), dt, sz);
			_size += sz;
		}

		void append(ReadIteratorT it, size_t count)
		{
			ensureCapacity(_size + count);
			while(count != 0 && it.isData())
            {
                size_t avail = it.directlyAvailableSize();
                size_t toRead = std::min(count, avail);
                size_t actualyRead = it.read(end(), toRead);
				_size += actualyRead;
                count -= actualyRead;
            }

		}

		size_t appendUint8( int8_t val ) {
			ensureCapacity(size() + 1);
			*reinterpret_cast<uint8_t*>(begin() + size() ) = val;
			return ++_size;
		}

		void set_size(size_t sz) { // NOTE: keeps pointers
			GMQ_ASSERT( sz <= _capacity );
			GMQ_ASSERT( _data != nullptr );
			_size = sz;
		}

		void read_file(FILE* input)
		{
			constexpr size_t size = 4000;
			std::unique_ptr<uint8_t[]> tmp(new uint8_t[size]);

			size_t r = 0;
			do
			{
				r = fread(tmp.get(), 1, size, input);
				append(tmp.get(), r);

			} while (r == size);
		}

		void write_file(FILE* output)
		{
			fwrite( _data.get(), 1, _size, output );
		}		
	};

	class FileReadBuffer : public Buffer // rather a temporary solution
	{
	public:
		FileReadBuffer(size_t res) { Buffer::reserve(res); }
	};

	template<class InputIterT, class OutputIterT>
	OutputIterT& copy( InputIterT& ii, OutputIterT& oi, size_t count = SIZE_MAX )
	{
		size_t copiedCount = 0;
		while( copiedCount < count && ii.isData() )
		{
			oi.appendUint8( *ii );
			++ii;
			++copiedCount;
		}
		return oi;
	}
	
} // namespace globalmq::marshalling


#endif // GLOBAL_MQ_COMMON_H
