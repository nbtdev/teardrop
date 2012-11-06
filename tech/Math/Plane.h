/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDPLANE_INCLUDED)
#define TDPLANE_INCLUDED

#include "Math/Vector4.h"

namespace Teardrop
{
	class Plane
	{
	public:
		Vector4 n;	// plane normal
		float d;	// d = N dot P, for P on the plane

		Plane();
		Plane(const Plane& other);
		Plane(const Vector4& origin, const Vector4& normal);
		~Plane();

		Plane& operator=(const Plane& other);
		//! CCW order
		void fromTriangle(const Vector4& v1, const Vector4& v2, const Vector4& v3);
		void fromPointNormal(const Vector4& origin, const Vector4& normal);
	};
}

#endif // TDPLANE_INCLUDED
