/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COSMATHUTIL_INCLUDED)
#define COSMATHUTIL_INCLUDED

#include "Math/include/Vector4.h"

#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

namespace CoS
{
	class Vector4;
	class Transform;
	class Matrix44;

	struct MathUtil
	{
		const static float PI;
		const static float TWO_PI;
		const static float HALF_PI;
		const static float EPSILON;
		const static float POSITIVE_INFINITY;
		const static float NEGATIVE_INFINITY;
		const static float SMALLEST;

		static float sin(float radians);
		static float cos(float radians);
		static float tan(float radians);
		static float asin(float radians);
		static float acos(float radians);
		static float atan(float radians);
		static float atan2(float r1, float r2);
		static float sqrt(float val);
		static float abs(float val);
		static float min(float a, float b);
		static float max(float a, float b);
		static int min(int a, int b);
		static int max(int a, int b);
		static size_t min(size_t a, size_t b);
		static size_t max(size_t a, size_t b);
		static void seed(size_t seed);
		static int rand(int min, int max);
		static void mt_init();
		static unsigned long mt_rand();
		static float clamp(float val, float minimum, float maximum);
		static float floor(float val);
		static float ceil(float val);
		static void transformToMatrix44(/*in*/const Transform& xform, /*out*/Matrix44& mat);
		static void matrix44ToTransform(/*in*/const Matrix44& mat, /*out*/Transform& xform);

		static Vector4 makeMax(const Vector4& l, const Vector4& r);
		static Vector4 makeMin(const Vector4& l, const Vector4& r);
	};
}

#endif // COSMATHUTIL_INCLUDED
