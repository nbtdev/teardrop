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

#if !defined(TDALLOCATIONTRACKER_INCLUDED)
#define TDALLOCATIONTRACKER_INCLUDED

#if !defined(TDCONFIG_INCLUDED)
#include "Config.h"
#endif

#if defined(TD_OPTION_MEMPROFILE)
#include <set>
#include <map>

namespace Teardrop
{
	class Stream;

	struct Allocation
	{
		Allocation(
			void* pMem, 
			size_t size, 
			const char* pAllocatorName, 
			const char* pFilename, 
			size_t line)
			: m_pMem(pMem), m_size(size), 
				m_pAllocatorName(pAllocatorName), m_pFilename(pFilename), m_line(line) {}

		void* m_pMem;
		size_t m_size;
		const char* m_pFilename;
		const char* m_pAllocatorName;
		size_t m_line;

		void* operator new(size_t s) { return malloc(s); }
		void operator delete(void* p) { free(p); }
	};

	class AllocationTracker
	{
		typedef std::set<Allocation*> AllocationSet;
		typedef std::map<const char*, AllocationSet> AllocationLUT;
		typedef std::map<const char*, AllocationSet> AllocationByAllocatorLUT;
		typedef std::map<void*, Allocation*> AllocationAddrLUT;
		AllocationLUT s_lutByFilename;
		AllocationByAllocatorLUT s_lutByAllocator;
		AllocationAddrLUT s_lutByBlockAddress;
		static bool s_bTracking; // hack to prevent deallocation tracking when d'tor ourselves

	public:
		AllocationTracker();
		~AllocationTracker();
		void close();

		void addAllocation(Allocation* pAlloc);
		void removeAllocation(void* pMem);
		static AllocationTracker* getInstance();

		//! print out usage by file
		void PrintUsageByFile(Stream& outStream);
		void PrintUsageByAllocator(Stream& outStream);
	};
}
#endif // TD_OPTION_MEMPROFILE

#if defined(TD_OPTION_MEMPROFILE)

#	define TD_TRACK_ALLOCATION(p, sz, name) \
	Teardrop::AllocationTracker::getInstance()->addAllocation( \
		new Teardrop::Allocation(p, sz, name, pFilename, line)); \

#	define TD_TRACK_DEALLOCATION(pMem) \
	Teardrop::AllocationTracker::getInstance()->removeAllocation(pMem); \

#	define TD_TRACK_ALLOCATION_WITH_OBJINST(pMem, size) \

#	define TD_TRACK_DEALLOCATION_WITH_OBJINST(pMem) \

#else

#	define TD_TRACK_ALLOCATION(pMem, size, name) \

#	define TD_TRACK_DEALLOCATION(pMem) \

#	define TD_TRACK_ALLOCATION_WITH_OBJINST(pMem, size) \

#	define TD_TRACK_DEALLOCATION_WITH_OBJINST(pMem) \

#endif // TD_OPTION_MEMPROFILE


#endif // TDALLOCATIONTRACKER_INCLUDED
