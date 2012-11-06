/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDMATHUTIL_INCLUDED)
#define TDMATHUTIL_INCLUDED

#include "Math/Vector4.h"

#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

namespace Teardrop
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

#endif // TDMATHUTIL_INCLUDED
