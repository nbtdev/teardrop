/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if !defined(TDMEMORYREGION_INCLUDED)
#define TDMEMORYREGION_INCLUDED

namespace Teardrop
{
	class MemoryRegion
	{
	public:
		MemoryRegion(const char* name, size_t size, void* pBase = 0);
		~MemoryRegion();

		void* Allocate(size_t size);
		void* AllocAligned(size_t size, size_t alignment);
		void* Reallocate(void* pMem, size_t newSize);
		void  Deallocate(void* pMem);
		size_t GetChunkSize(void* pMem);
		void Close();

		const char* GetName() { return m_name; }
		size_t GetSize() { return m_size; }

		size_t GetHighWaterMark() { return m_maxBytes; }
		size_t GetMemoryUsedInBytes() { return m_totalBytes; }

	private:
		void* m_pData;
		size_t m_size;
		char m_name[32];
		void* m_pBase;

		//! for allocation statistics
		size_t m_totalBytes;
		size_t m_maxBytes;
	};
}

#endif // TDMEMORYREGION_INCLUDED
