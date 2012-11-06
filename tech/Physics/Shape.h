/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHAPE_INCLUDED)
#define SHAPE_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class Shape
	{
	public:
		Shape();
		virtual ~Shape();

		virtual bool initialize();
		virtual bool release();
		virtual bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	protected:
	};
}

#endif // SHAPE_INCLUDED
