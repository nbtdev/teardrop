/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TRIANGLE_INCLUDED)
#define TRIANGLE_INCLUDED

#include "Math/Vector4.h"

namespace Teardrop
{
	class Triangle
	{
	public:
		Vector4 p[3];	// triangle points, in CCW order

		Triangle();
		Triangle(const Triangle& other);
		Triangle(const Vector4& p0, const Vector4& p1, const Vector4& p2);
		~Triangle();

		Triangle& operator=(const Triangle& other);
		void fromPoints(const Vector4& p0, const Vector4& p1, const Vector4& p2);
	};
}

#endif // TRIANGLE_INCLUDED
