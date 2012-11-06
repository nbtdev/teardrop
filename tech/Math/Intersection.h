/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDINTERSECTION_INCLUDED)
#define TDINTERSECTION_INCLUDED

namespace Teardrop
{
	class AABB;
	class Plane;
	class Vector4;

	class Intersection
	{
	public:
		bool AABB_Plane(const AABB& aabb, const Plane& plane);
	};
}

#endif // TDINTERSECTION_INCLUDED
