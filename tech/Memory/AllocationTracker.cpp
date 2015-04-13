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

#include "Config.h"
#include "Memory.h"
#include "AllocationTracker.h"
#include "MemoryRegion.h"
#include "Stream/Stream.h"
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif // _WIN32, _WIN64

using namespace Teardrop;

#if defined(TD_OPTION_MEMPROFILE)
//-----------------------------------------------------------------------------
bool AllocationTracker::s_bTracking = false;
//-----------------------------------------------------------------------------
void AllocationTracker::PrintUsageByFile(Stream& outStream)
{
#if defined(TD_OPTION_MEMPROFILE)
	std::stringstream ss;
	ss << "Allocations:" << std::endl;

	size_t total = 0;
	for (AllocationLUT::iterator it = s_lutByFilename.begin();
		it != s_lutByFilename.end(); ++it)
	{
		size_t usage = 0;
		size_t  lNum = 0;
		for (AllocationSet::iterator setIt = it->second.begin();
			setIt != it->second.end(); ++setIt)
		{
			usage += (*setIt)->m_size;
			lNum = (*setIt)->m_line;
		}

		if (usage)
		{
			total += usage;
			if (it->first)
			{
				ss << it->first;
			}
			else
			{
				ss << "(unknown)"; 
			}

			ss << " (" << lNum << ") : " << usage << " bytes" << std::endl;
		}
	}

	size_t untracked = GetDEFAULTRegion()->GetMemoryUsedInBytes() - total;

	ss << "Total bytes: " << total  << " (" <<
		untracked << " bytes untracked)" << std::endl;

	//OutputDebugString(ss.str().c_str());
	//outStream << ss.str() << std::endl;
	Serial::Util::write(ss.str().c_str(), outStream);
#endif
}
//-----------------------------------------------------------------------------
void AllocationTracker::PrintUsageByAllocator(Stream& outStream)
{
#if defined(TD_OPTION_MEMPROFILE)
	std::stringstream ss;
	ss << "Allocations:" << std::endl;

	size_t total = 0;
	for (AllocationByAllocatorLUT::iterator it = s_lutByAllocator.begin();
		it != s_lutByAllocator.end(); ++it)
	{
		size_t usage = 0;
		for (AllocationSet::iterator setIt = it->second.begin();
			setIt != it->second.end(); ++setIt)
		{
			usage += (*setIt)->m_size;
		}

		total += usage;
		ss << it->first << ": " << usage << " bytes" << std::endl;
	}

	size_t untracked = GetDEFAULTRegion()->GetMemoryUsedInBytes() - total;

	ss << "Total bytes: " << total  << " (" <<
		untracked << " bytes untracked)" << std::endl;

	//OutputDebugString(ss.str().c_str());
	//outStream << ss.str() << std::endl;
	Serial::Util::write(ss.str().c_str(), outStream);
#endif
}
//-----------------------------------------------------------------------------
AllocationTracker::AllocationTracker()
{ 
	s_bTracking = true; 
}
//-----------------------------------------------------------------------------
AllocationTracker::~AllocationTracker()
{
	close();
}
//-----------------------------------------------------------------------------
void AllocationTracker::close()
{
	s_bTracking = false;

	// clean up allocation objects on our way out
	for (AllocationLUT::iterator it = s_lutByFilename.begin(); 
		it != s_lutByFilename.end(); ++it)
	{
		for (AllocationSet::iterator setIt = it->second.begin();
			setIt != it->second.end(); ++setIt)
		{
			delete *setIt;
		}
	}

	s_lutByFilename.clear();

	for (AllocationByAllocatorLUT::iterator it = s_lutByAllocator.begin(); 
		it != s_lutByAllocator.end(); ++it)
	{
		for (AllocationSet::iterator setIt = it->second.begin();
			setIt != it->second.end(); ++setIt)
		{
			//delete *setIt;
		}
	}

	s_lutByAllocator.clear();

	for (AllocationAddrLUT::iterator it = s_lutByBlockAddress.begin(); 
		it != s_lutByBlockAddress.end(); ++it)
	{
		//delete it->second;
	}

	s_lutByBlockAddress.clear();
}
//-----------------------------------------------------------------------------
void AllocationTracker::addAllocation(Allocation* pAlloc)
{
	if (!s_bTracking)
		return;

	//assert(pAlloc);
	//assert(s_lutByBlockAddress.find(pAlloc->m_pMem) == 
	//	s_lutByBlockAddress.end());
	//assert(s_lutByFilename.find(pAlloc->m_pFilename) == 
	//	s_lutByFilename.end());

	s_lutByBlockAddress[pAlloc->m_pMem] = pAlloc;
	AllocationSet::value_type val(pAlloc);

	// insert into by-filename lut
	if (s_lutByFilename.find(pAlloc->m_pFilename) == s_lutByFilename.end())
	{
		s_lutByFilename[pAlloc->m_pFilename] = AllocationSet();
	}
	s_lutByFilename[pAlloc->m_pFilename].insert(val);

	// insert into by-allocator-name lut
	if (s_lutByAllocator.find(pAlloc->m_pAllocatorName) == s_lutByAllocator.end())
	{
		s_lutByAllocator[pAlloc->m_pAllocatorName] = AllocationSet();
	}
	s_lutByAllocator[pAlloc->m_pAllocatorName].insert(val);
}
//-----------------------------------------------------------------------------
void AllocationTracker::removeAllocation(void* pMem)
{
	if (!s_bTracking)
		return;

	AllocationAddrLUT::iterator it = 
		s_lutByBlockAddress.find(pMem);
	if (it != s_lutByBlockAddress.end())
	{
		// remove it from the other map(s) first
		AllocationLUT::iterator fnLutEntry = 
			s_lutByFilename.find(it->second->m_pFilename);
		if (fnLutEntry != s_lutByFilename.end())
		{
			AllocationSet::iterator setIt = fnLutEntry->second.find(it->second);
			if (setIt != fnLutEntry->second.end())
			{
				fnLutEntry->second.erase(setIt);
			}
		}

		AllocationByAllocatorLUT::iterator allocLutEntry = 
			s_lutByAllocator.find(it->second->m_pAllocatorName);
		if (allocLutEntry != s_lutByAllocator.end())
		{
			AllocationSet::iterator setIt = allocLutEntry->second.find(it->second);
			if (setIt != allocLutEntry->second.end())
			{
				allocLutEntry->second.erase(setIt);
			}
		}

		// then from the address LUT
		delete it->second;
		s_lutByBlockAddress.erase(it);
	}
}
//-----------------------------------------------------------------------------
AllocationTracker* AllocationTracker::getInstance()
{
	static AllocationTracker s_tracker;
	return &s_tracker;
}
#endif // TD_OPTION_MEMPROFILE
