/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(POINTLIGHT_INCLUDED)
#define POINTLIGHT_INCLUDED

#include "Game/DynamicLight.h"

namespace Teardrop
{
	class PointLight
		: public DynamicLight
	{

	public:
		TD_CLASS(PointLight, DynamicLight);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Range, "Effective radius (used for culling)", float, 1000, 0);
		TD_COMPLEX_PROPERTY(Attenuation, "Attenuation parameters (constant, linear, quadratic)", Vector4, "(1,0,0,0)", 0);

		PointLight();
		virtual ~PointLight();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // POINTLIGHT_INCLUDED
