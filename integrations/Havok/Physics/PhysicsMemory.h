/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PHYSICSMEMORY_INCLUDED)
#define PHYSICSMEMORY_INCLUDED

#include <list>
#include <map>

namespace Teardrop
{
	struct PhysicsMemory : public hkMemoryAllocator
	{
		PhysicsMemory();
		~PhysicsMemory();

		void* blockAlloc(int numBytes);
		void blockFree(void* p, int numBytes);
		void getMemoryStatistics(MemoryStatistics& u);
		int getAllocatedSize(const void* obj, int nbytes);
			
		TD_DECLARE_ALLOCATOR();
	};
}

#endif // PHYSICSMEMORY_INCLUDED
