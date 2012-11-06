/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RAY_INCLUDED)
#define RAY_INCLUDED

#include "Math/Vector4.h"

namespace Teardrop
{
	class Ray
	{
	public:
		Vector4 end;	// ray endpoint
		Vector4 dir;	// ray direction

		Ray();
		Ray(const Ray& other);
		Ray(const Vector4& end, const Vector4& dir);
		~Ray();

		Ray& operator=(const Ray& other);
		void fromEndpointDirection(const Vector4& end, const Vector4& dir);
	};
}

#endif // RAY_INCLUDED
