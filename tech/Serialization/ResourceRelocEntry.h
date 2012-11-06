/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCERELOCENTRY_INCLUDED)
#define RESOURCERELOCENTRY_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	struct ResourceRelocEntry
	{
		enum Type
		{
			INVALID=0,
			INTERNAL,
			EXTERNAL,
			VTABLE,

			TYPE_FORCE_32_BIT=0xFFFFFFFF,
		};

		ResourceRelocEntry()
		{
			at = 0;
			to = 0;
			id = 0;
			type = INVALID;
			reserved[0] = 0;
			reserved[1] = 0;
			reserved[2] = 0;
		}

		unsigned __int64 id; // classId for vtable types, resource ID for external types
		unsigned int at;
		unsigned int to;
		Type type; 
		unsigned int reserved[3];

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RESOURCERELOCENTRY_INCLUDED
