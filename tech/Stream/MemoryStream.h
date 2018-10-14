/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#if !defined(TDMEMORYSTREAM_INCLUDED)
#define TDMEMORYSTREAM_INCLUDED

#include "Stream.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class MemoryStream : public Stream
	{
		unsigned char* mData;
		int mLen;
		int mPos;
		int mCapacity;
		bool mOwnData;

	public:
		MemoryStream();
		MemoryStream(size_t len);
		MemoryStream(void* data, size_t len);
		~MemoryStream();

		// Stream implementation - note that async flags are ignored for MemoryStream
		//! read data from this stream
		int read(void* pData, size_t len, bool async=false);
		//! write data to this stream
		int write(const void* pData, size_t len, bool async=false);
		//! return stream length in bytes
		size_t length();
		//! return stream position
		size_t getPosition();
		//! seek to another part of this stream
		bool seek(int offset, SeekType seekType = CURRENT, bool async=false);
		//! check to see if we are at the end of the stream
		bool isEnd();

		// MemoryStream-specific
		// return a pointer to the start of the data
		void* data();

		TD_DECLARE_ALLOCATOR();

	protected:
		void expand();
	};
}

#endif // TDMEMORYSTREAM_INCLUDED
