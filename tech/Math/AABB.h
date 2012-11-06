/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDAABB_INCLUDED)
#define TDAABB_INCLUDED

#include "Math/Vector4.h"

namespace Teardrop
{
	class AABB
	{
	public:
		Vector4 center;
		Vector4 extents;

		AABB();
		AABB(const AABB& other);
		AABB(const Vector4& center, const Vector4& extents);
		~AABB();

		AABB& operator=(const AABB& other);

		void fromCorners(const Vector4& minCorner, const Vector4& maxCorner);
		void toCorners(Vector4& minCorner, Vector4& maxCorner) const;
		void getCorners(Vector4* corners);
		void merge(const AABB& other);
		float getRadius() const;
		float getRadius2() const; // squared
	};
}

#endif // TDAABB_INCLUDED
