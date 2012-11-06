/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHAPEHAVOK_INCLUDED)
#define SHAPEHAVOK_INCLUDED

#include "Memory/Allocators.h"
#include "Physics/Shape.h"

class hkpShape;

namespace Teardrop
{
	class ShapeHavok : public Shape
	{
	public:
		ShapeHavok();
		~ShapeHavok();

		bool initialize();
		bool release();
		bool update(float deltaT);

		hkpShape* getHavokShape();

		TD_DECLARE_ALLOCATOR();

	protected:
		hkpShape* m_pShape;
	};
}

#endif // SHAPEHAVOK_INCLUDED
