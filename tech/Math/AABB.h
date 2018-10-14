/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
