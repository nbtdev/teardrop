/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "MathUtil.h"
#include "Transform.h"
#include "Matrix44.h"
#include <math.h>
#include <algorithm>
#include <limits>

using namespace Teardrop;
//---------------------------------------------------------------------------
const float MathUtil::PI = (float)acos(-1.0);
const float MathUtil::HALF_PI = MathUtil::PI / 2.0f;
const float MathUtil::TWO_PI = MathUtil::PI * 2.0f;
const float MathUtil::EPSILON = std::numeric_limits<float>::epsilon();
const float MathUtil::POSITIVE_INFINITY = std::numeric_limits<float>::max();
const float MathUtil::NEGATIVE_INFINITY = -std::numeric_limits<float>::max();
const float MathUtil::SMALLEST = std::numeric_limits<float>::min();
//---------------------------------------------------------------------------
float MathUtil::sin(float radians)
{
	return ::sin(radians);
}
//---------------------------------------------------------------------------
float MathUtil::cos(float radians)
{
	return ::cos(radians);
}
//---------------------------------------------------------------------------
float MathUtil::tan(float radians)
{
	return ::tan(radians);
}
//---------------------------------------------------------------------------
float MathUtil::asin(float radians)
{
	return ::asin(radians);
}
//---------------------------------------------------------------------------
float MathUtil::acos(float radians)
{
	return ::acos(radians);
}
//---------------------------------------------------------------------------
float MathUtil::atan(float radians)
{
	return ::atan(radians);
}
//---------------------------------------------------------------------------
float MathUtil::atan2(float r1, float r2)
{
	return ::atan2(r1, r2);
}
//---------------------------------------------------------------------------
float MathUtil::sqrt(float val)
{
	return ::sqrtf(val);
}
//---------------------------------------------------------------------------
int MathUtil::abs(int val)
{
	return ::abs(val);
}
//---------------------------------------------------------------------------
float MathUtil::abs(float val)
{
	return ::fabs(val);
}
//---------------------------------------------------------------------------
double MathUtil::abs(double val)
{
	return ::fabs(val);
}
//---------------------------------------------------------------------------
float MathUtil::min(float a, float b)
{
	return std::min(a, b);
}
//---------------------------------------------------------------------------
float MathUtil::max(float a, float b)
{
	return std::max(a, b);
}
//---------------------------------------------------------------------------
int MathUtil::min(int a, int b)
{
	return std::min(a, b);
}
//---------------------------------------------------------------------------
int MathUtil::max(int a, int b)
{
	return std::max(a, b);
}
//---------------------------------------------------------------------------
size_t MathUtil::min(size_t a, size_t b)
{
	return std::min(a, b);
}
//---------------------------------------------------------------------------
size_t MathUtil::max(size_t a, size_t b)
{
	return std::max(a, b);
}
//---------------------------------------------------------------------------
float MathUtil::clamp(float val, float fmin, float fmax)
{
	return min(max(val, fmin), fmax);
}
//---------------------------------------------------------------------------
void MathUtil::seed(size_t seed)
{
	::srand((unsigned int)seed);
}
//---------------------------------------------------------------------------
int MathUtil::rand(int min, int max)
{
	float r = (float)::rand() / (float)RAND_MAX;
	float range = ((float)max - MathUtil::EPSILON + 1) - (float)min;
	return (int)(min + r * range);
}
//---------------------------------------------------------------------------
Vector4 MathUtil::makeMax(const Vector4& l, const Vector4& r)
{
	return Vector4(
		std::max(l.x, r.x),
		std::max(l.y, r.y),
		std::max(l.z, r.z),
		std::max(l.w, r.w)
		);
}
//---------------------------------------------------------------------------
Vector4 MathUtil::makeMin(const Vector4& l, const Vector4& r)
{
	return Vector4(
		std::min(l.x, r.x),
		std::min(l.y, r.y),
		std::min(l.z, r.z),
		std::min(l.w, r.w)
		);
}
//---------------------------------------------------------------------------
float MathUtil::floor(float val)
{
	return ::floor(val);
}
//---------------------------------------------------------------------------
float MathUtil::ceil(float val)
{
	return ::ceil(val);
}
//---------------------------------------------------------------------------
extern void mt_init();
extern unsigned long mt_random();
unsigned long MathUtil::mt_rand()
{
	return ::mt_random();
}
//---------------------------------------------------------------------------
void MathUtil::mt_init()
{
	return ::mt_init();
}
//---------------------------------------------------------------------------
void MathUtil::transformToMatrix44(const Transform& xform, Matrix44& mat)
{
	Matrix44 rot;
	xform.rot.toRotationMatrix(rot);
	Matrix44 scale(Matrix44::IDENTITY);
	scale.m[0][0] = xform.scale.x;
	scale.m[1][1] = xform.scale.y;
	scale.m[2][2] = xform.scale.z;

	mul3(mat, rot, scale);
	mat.m[3][0] = xform.trans.x;
	mat.m[3][1] = xform.trans.y;
	mat.m[3][2] = xform.trans.z;
}
//---------------------------------------------------------------------------
void MathUtil::matrix44ToTransform(const Matrix44& mat, Transform& xform)
{
	Matrix44 rot(mat);

	// extract scale from rotation matrix
	Vector4 x(rot.m[0][0], rot.m[1][0], rot.m[2][0], 0);
	xform.scale.x = x.normalize();
	rot.m[0][0] = x.x; rot.m[0][0] = x.y; rot.m[0][0] = x.z;
	if (fabs(xform.scale.x - 1) < MathUtil::EPSILON)
		xform.scale.x = 1;

	Vector4 y(rot.m[0][1], rot.m[1][1], rot.m[2][1], 0);
	xform.scale.y = y.normalize();
	rot.m[0][1] = y.x; rot.m[0][1] = y.y; rot.m[0][1] = y.z;
	if (fabs(xform.scale.y - 1) < MathUtil::EPSILON)
		xform.scale.y = 1;

	Vector4 z(rot.m[0][2], rot.m[1][2], rot.m[2][2], 0);
	xform.scale.z = z.normalize();
	rot.m[0][2] = z.x; rot.m[0][2] = z.y; rot.m[0][2] = z.z;
	if (fabs(xform.scale.z - 1) < MathUtil::EPSILON)
		xform.scale.z = 1;

	xform.rot.fromRotationMatrix(rot);

	xform.trans = (Vector4&)*mat.m[0];
}