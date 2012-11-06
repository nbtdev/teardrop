/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SPHERE_INCLUDED)
#define SPHERE_INCLUDED

#include "Math/Vector4.h"

namespace Teardrop
{
	class Sphere
	{
	public:
		Vector4 m_center;
		float m_radius;

		Sphere();
		Sphere(const Sphere& other);
		Sphere(const Vector4& center, float radius);
		~Sphere();
		Sphere& operator=(const Sphere& other);

		Sphere& fromCenterRadius(const Vector4& center, float radius);
	};
}

#endif // SPHERE_INCLUDED
