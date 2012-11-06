/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CAPSULE_INCLUDED)
#define CAPSULE_INCLUDED

#include "Memory/Allocators.h"
#include "ShapeHavok.h"

namespace Teardrop
{
	class Vector4;

	class Capsule : public ShapeHavok
	{
	public:
		Capsule();
		~Capsule();

		bool initialize(
			const Vector4& p0,
			const Vector4& p1,
			float radius
			);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // CAPSULE_INCLUDED
