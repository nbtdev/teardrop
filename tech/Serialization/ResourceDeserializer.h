/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCEDESERIALIZER_INCLUDED)
#define RESOURCEDESERIALIZER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class Stream;

	class ResourceDeserializer
	{
	public:
		ResourceDeserializer();
		~ResourceDeserializer();

		//! get a pointer to the start of the image
		void* get(void* pMem);
		//! fix up an in-memory resource image
		void fix(void* pMem);
		//! fix up an external dependency in the image
		void fix(void* pMem, size_t index, void* pTarget);
		//! dump this resource's dependencies
		void dumpDependencies(void* pMem, Stream& strm);
		//! dump this resource's dependencies
		const char* getDependencies(void* pMem);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RESOURCEDESERIALIZER_INCLUDED
