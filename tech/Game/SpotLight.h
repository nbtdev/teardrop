/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SPOTLIGHT_INCLUDED)
#define SPOTLIGHT_INCLUDED

#include "Game/DynamicLight.h"

namespace Teardrop
{
	class SpotLight
		: public DynamicLight
	{

	public:
		TD_CLASS(SpotLight, DynamicLight);
		TD_CLASS_CREATABLE();
		TD_PROPERTY(Range, "Effective radius (used for culling)", float, 1000, 0);
		TD_COMPLEX_PROPERTY(Attenuation, "Attenuation parameters (constant, linear, quadratic)", Vector4, "(1,0,0,0)", 0);
		TD_PROPERTY(InnerAngle, "Inner cone angle (degrees)", float, 30, 0);
		TD_PROPERTY(OuterAngle, "Outer cone angle (degrees)", float, 60, 0);

		SpotLight();
		virtual ~SpotLight();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // SPOTLIGHT_INCLUDED
