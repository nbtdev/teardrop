/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BONE_INCLUDED)
#define BONE_INCLUDED

#include "Serialization/Serialization.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class Bone
	{
	public:
		Bone();
		virtual ~Bone();

		virtual bool initialize();
		virtual bool destroy();

		virtual const char* getName() const;
		virtual Bone* getParent(const char* name) const;
		virtual size_t getNumChildren() const;
		virtual const Bone* getChild(size_t index) const;
		virtual Bone* getChild(size_t index);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // BONE_INCLUDED
