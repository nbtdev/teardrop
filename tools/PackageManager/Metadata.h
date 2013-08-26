/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(METADATA_INCLUDED)
#define METADATA_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	namespace Tools
	{
		class Metadata : public Reflection::Object
		{
		public:
			TD_CLASS(Metadata, Object);
			TD_PROPERTY(Name, "Object Name", String, "(undefined)", 0);
			TD_PROPERTY(ID, "Object ID", String, "(undefined)", "ReadOnly");

			Metadata();
			~Metadata();

			TD_DECLARE_ALLOCATOR();
		};
	}
}

#endif // METADATA_INCLUDED
