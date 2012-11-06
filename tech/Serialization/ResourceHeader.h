/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCEHEADER_INCLUDED)
#define RESOURCEHEADER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	struct ResourceHeader
	{
		unsigned int version;
		unsigned int size;
		unsigned int stringTableOffset;
		unsigned int fixupTableOffset;
		unsigned __int64 resrved;
		unsigned __int64 resourceId;

		ResourceHeader()
		{
			version = 0;
			size = 0;
			stringTableOffset = 0;
			fixupTableOffset = 0;
			resrved = 0;
			resourceId = 0;
		}

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RESOURCEHEADER_INCLUDED
