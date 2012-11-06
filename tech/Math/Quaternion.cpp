/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Quaternion.h"
#include "Vector4.h"
#include "Matrix44.h"
#include "MathUtil.h"
#include <math.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
const Teardrop::Quaternion& Teardrop::Quaternion::IDENTITY = Teardrop::Quaternion(1, 0, 0, 0);
//---------------------------------------------------------------------------
Quaternion::~Quaternion()
{
}
//---------------------------------------------------------------------------
void Quaternion::fromAngleAxis(float angle /*radians*/, const Vector4& axis)
{
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    float half = angle * 0.5f;
    float s = sin(half);
    w = cos(half);
    x = s * axis.x;
    y = s * axis.y;
    z = s * axis.z;
}
//---------------------------------------------------------------------------
void Quaternion::toAngleAxis(float& angle /*radians*/, Vector4& axis)
{
	// assumes quaternion is normalized already
	angle = MathUtil::acos(w) * 2;
	float s = MathUtil::sqrt(1 - w*w);
	if (MathUtil::abs(s) < MathUtil::EPSILON)
	{
		s = 1;
	}

	axis.x = x/s;
	axis.y = y/s;
	axis.z = z/s;
}
//---------------------------------------------------------------------------
Quaternion Quaternion::operator*(const Quaternion& q) const
{
	return Quaternion(
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y + y * q.w + z * q.x - x * q.z,
		w * q.z + z * q.w + x * q.y - y * q.x
		);
}
//---------------------------------------------------------------------------
// from http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q54
void Quaternion::toRotationMatrix(Matrix44& mat) const
{
	float xx      = x * x;
	float xy      = x * y;
	float xz      = x * z;
	float xw      = x * w;
	float yy      = y * y;
	float yz      = y * z;
	float yw      = y * w;
	float zz      = z * z;
	float zw      = z * w;
	mat.m[0][0]  = 1 - 2 * ( yy + zz );
	mat.m[1][0]  =     2 * ( xy - zw );
	mat.m[2][0]  =     2 * ( xz + yw );
	mat.m[0][1]  =     2 * ( xy + zw );
	mat.m[1][1]  = 1 - 2 * ( xx + zz );
	mat.m[2][1]  =     2 * ( yz - xw );
	mat.m[0][2]  =     2 * ( xz - yw );
	mat.m[1][2]  =     2 * ( yz + xw );
	mat.m[2][2]  = 1 - 2 * ( xx + yy );

	mat.m[0][3]  = mat.m[1][3] = mat.m[2][3] 
		= mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;
	mat.m[3][3] = 1;
}
//---------------------------------------------------------------------------
// from NVIDIA SDK
Vector4 Teardrop::operator*(const Quaternion& q, const Vector4& v)
{
	Vector4 uv, uuv;
	Vector4 qvec(q.x, q.y, q.z, 0);
	cross(uv, qvec, v);
	cross(uuv, qvec, uv);
	uv *= (2.0f * q.w);
	uuv *= 2.0f;

	return v + uv + uuv;
}
//---------------------------------------------------------------------------
// invert in place, this assumes unit quaternion!
void Teardrop::invert(/*out*/Quaternion& q)
{
	q.x = -q.x;
	q.y = -q.y;
	q.z = -q.z;
}
//---------------------------------------------------------------------------
// returns normal term
float Quaternion::normalize()
{
	float n = MathUtil::sqrt(w*w + x*x + y*y + z*z);

	if (n > 0.f)
	{
		float oon = 1.0f / n;

		w *= oon;
		x *= oon;
		y *= oon;
		z *= oon;
	}

	return n;
}
//---------------------------------------------------------------------------
// puts a normalized version of this quaternion in 'out', returns normal term
float Quaternion::getNormalized(/*out*/Quaternion& out) const
{
	out = *this;
	return out.normalize();
}
//---------------------------------------------------------------------------
// convert the quaternion to spherical (azimuth, elevation) angles
// from http://web.archive.org/web/20041029003853/http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q59
void Quaternion::toAzimuthElevation(float& azimuth, float& elevation) const
{
	float cosA = w;
	float sinA = MathUtil::sqrt(1 - cosA * cosA);
	float angle = MathUtil::acos(cosA) * 2;

	if (MathUtil::abs(MathUtil::sin(angle)) < MathUtil::EPSILON)
		sinA = 1;

	float tx = x / sinA;
	float ty = y / sinA;
	float tz = z / sinA;

	elevation = -MathUtil::asin(ty);

	if ((tx*tx + tz*tz) < MathUtil::EPSILON)
		azimuth = 0;
	else
		azimuth = MathUtil::atan2(tx, tz);

	if (azimuth < 0)
		azimuth += MathUtil::TWO_PI;
}
//---------------------------------------------------------------------------
bool Quaternion::operator==(const Quaternion& other)
{
	return (
			MathUtil::abs(x - other.x) < MathUtil::EPSILON 
		&&	MathUtil::abs(y - other.y) < MathUtil::EPSILON 
		&&	MathUtil::abs(z - other.z) < MathUtil::EPSILON 
		&&	MathUtil::abs(w - other.w) < MathUtil::EPSILON 
		);
}
//---------------------------------------------------------------------------
bool Quaternion::operator!=(const Quaternion& other)
{
	return !(*this == other);
}
//---------------------------------------------------------------------------
// the classic 1987 Shoemake algorithm
void Quaternion::fromRotationMatrix(const Matrix44& mat)
{
	float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2];

	// if trace > 0 "instant" calc
	if (trace > 0)
	{
		float root = MathUtil::sqrt(trace + 1);
		w = -0.5f * root;
		root = 0.5f / root;

		x = (mat.m[2][1] - mat.m[1][2]) * root;
		y = (mat.m[0][2] - mat.m[2][0]) * root;
		z = (mat.m[1][0] - mat.m[0][1]) * root;
	}
	else
	{
		static size_t next[3] = {1, 2, 0};
		size_t i = 0;

		if (mat.m[1][1] > mat.m[0][0])
			 i = 1;
		if (mat.m[2][2] > mat.m[i][i])
			 i = 2;

		size_t j = next[i];
		size_t k = next[j];

		float root = MathUtil::sqrt(
			mat.m[i][i] - mat.m[j][j] - mat.m[k][k] + 1);
		float *tmp[3] = { &x, &y, &z };
		*tmp[i] = 0.5f * root;
		root = 0.5f / root;
		w = (mat.m[j][k] - mat.m[k][j]) * root;
		*tmp[j] = (mat.m[j][i] + mat.m[i][j]) * root;
		*tmp[k] = (mat.m[k][i] + mat.m[i][k]) * root;
	}
}
