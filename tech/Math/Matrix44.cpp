/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "Matrix44.h"
#include "Vector4.h"
#include "MathUtil.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
const Teardrop::Matrix44& Teardrop::Matrix44::IDENTITY = 
	Teardrop::Matrix44(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
//---------------------------------------------------------------------------
const Teardrop::Matrix44& Teardrop::Matrix44::ZERO = 
	Teardrop::Matrix44(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);
//---------------------------------------------------------------------------
Matrix44::~Matrix44()
{
}
//---------------------------------------------------------------------------
Matrix44::Matrix44(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
)
{
	m[0][0] = m00;	m[0][1] = m01;	m[0][2] = m02;	m[0][3] = m03;
	m[1][0] = m10;	m[1][1] = m11;	m[1][2] = m12;	m[1][3] = m13;
	m[2][0] = m20;	m[2][1] = m21;	m[2][2] = m22;	m[2][3] = m23;
	m[3][0] = m30;	m[3][1] = m31;	m[3][2] = m32;	m[3][3] = m33;
}
//---------------------------------------------------------------------------
Matrix44& Matrix44::operator=(const Matrix44& other)
{
	m[0][0] = other.m[0][0];	m[0][1] = other.m[0][1];	m[0][2] = other.m[0][2];	m[0][3] = other.m[0][3];	
	m[1][0] = other.m[1][0];	m[1][1] = other.m[1][1];	m[1][2] = other.m[1][2];	m[1][3] = other.m[1][3];	
	m[2][0] = other.m[2][0];	m[2][1] = other.m[2][1];	m[2][2] = other.m[2][2];	m[2][3] = other.m[2][3];	
	m[3][0] = other.m[3][0];	m[3][1] = other.m[3][1];	m[3][2] = other.m[3][2];	m[3][3] = other.m[3][3];	

	return *this;
}
//---------------------------------------------------------------------------
void Teardrop::mul(Matrix44& out, const Matrix44& a, const Matrix44& b)
{
	Matrix44 tmp;
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			tmp.m[i][j] = 
				a.m[i][0] * b.m[0][j] +
				a.m[i][1] * b.m[1][j] +
				a.m[i][2] * b.m[2][j] +
				a.m[i][3] * b.m[3][j];
		}
	}

	out = tmp;
}
//---------------------------------------------------------------------------
void Teardrop::mul3(Matrix44& out, const Matrix44& a, const Matrix44& b)
{
	for (int i=0; i<3; ++i)
	{
		for (int j=0; j<3; ++j)
		{
			out.m[i][j] = 
				a.m[i][0] * b.m[0][j] +
				a.m[i][1] * b.m[1][j] +
				a.m[i][2] * b.m[2][j];
		}
	}

	out.m[0][3] =
	out.m[1][3] =
	out.m[2][3] =
	out.m[3][0] = 
	out.m[3][1] = 
	out.m[3][2] = 0;	
	out.m[3][3] = 1;
}
//---------------------------------------------------------------------------
// from DX SDK docs, D3DXMatrixLookAtLH
Matrix44& Matrix44::fromLookAtLH(
	const Vector4& position, 
	const Vector4& lookAt, 
	const Vector4& up)
{
	Vector4 z(lookAt - position);
	z.normalize();
	Vector4 x(cross(up, z));
	x.normalize();
	Vector4 y(cross(z, x));

	m[0][0] = x.x;
	m[0][1] = y.x;
	m[0][2] = z.x;
	m[0][3] = 0;

	m[1][0] = x.y;
	m[1][1] = y.y;
	m[1][2] = z.y;
	m[1][3] = 0;

	m[2][0] = x.z;
	m[2][1] = y.z;
	m[2][2] = z.z;
	m[2][3] = 0;

	m[3][0] = -dot(x, position);
	m[3][1] = -dot(y, position);
	m[3][2] = -dot(z, position);
	m[3][3] = 1;

	return *this;
}
//---------------------------------------------------------------------------
// from DX SDK docs, D3DXMatrixLookAtRH
Matrix44& Matrix44::fromLookAtRH(
	const Vector4& position, 
	const Vector4& lookAt, 
	const Vector4& up)
{
	Vector4 z(position - lookAt);
	z.normalize();
	Vector4 x(cross(up, z));
	x.normalize();
	Vector4 y(cross(z, x));

	m[0][0] = x.x;
	m[0][1] = y.x;
	m[0][2] = z.x;
	m[0][3] = 0;

	m[1][0] = x.y;
	m[1][1] = y.y;
	m[1][2] = z.y;
	m[1][3] = 0;

	m[2][0] = x.z;
	m[2][1] = y.z;
	m[2][2] = z.z;
	m[2][3] = 0;

	m[3][0] = -dot(x, position);
	m[3][1] = -dot(y, position);
	m[3][2] = -dot(z, position);
	m[3][3] = 1;

	return *this;
}
//---------------------------------------------------------------------------
// make this into a perspective projection matrix using the supplied params
// maps objects in frustum to [-1..1] depth range, right-handed
// [ 2/w 0   0        0   ]
// [ 0   2/h 0        0   ]
// [ 0   0   1/(f-n) -n/(f-n) ]
// [ 0   0   0        1   ]
//
Matrix44& Matrix44::makeOrthoProjectionLH(
	float width, float height, 
	float near, float far 
	)
{
	*this = IDENTITY;

	// from DX SDK docs, for D3DXMatrixOrthoLH
	m[0][0] = 2/width;
	m[1][1] = 2/height;
	m[2][2] = 1/(far-near);
	m[3][2] = -near/(far-near);

	return *this;
}
//---------------------------------------------------------------------------
// make this into a perspective projection matrix using the supplied params
// maps objects in frustum to [-1..1] depth range, right-handed
// [ a   0   c   0  ]
// [ 0   b   d   0  ]
// [ 0   0   q   qn ]
// [ 0   0  -1   0  ]
//
Matrix44& Matrix44::makePerspectiveProjectionRH(
	float near, float far, 
	float fov, float aspect
	)
{
	*this = ZERO;

	// from DX SDK docs, for D3DXMatrixPerspectiveFovRH
	float yScale = 1 / MathUtil::tan(fov / 2);
	float xScale = yScale / aspect;
	float zScale = far / (near - far);
	float q = near * zScale;

	m[0][0] = xScale;
	m[1][1] = yScale;
	m[2][2] = zScale;
	m[3][2] = q;
	m[2][3] = -1;

	return *this;
}
//---------------------------------------------------------------------------
// make this into a perspective projection matrix using the supplied params
// maps objects in frustum to [-1..1] depth range, left-handed
// [ a   0   c   0  ]
// [ 0   b   d   0  ]
// [ 0   0   q  -qn ]
// [ 0   0   1   0  ]
//
Matrix44& Matrix44::makePerspectiveProjectionLH(
	float near, float far, 
	float fov, float aspect
)
{
	*this = ZERO;

	// from DX SDK docs, for D3DXMatrixPerspectiveFovLH
	float yScale = 1 / MathUtil::tan(fov / 2);
	float xScale = yScale / aspect;
	float zScale = far / (far - near);
	float q = -near * zScale;

	m[0][0] = xScale;
	m[1][1] = yScale;
	m[2][2] = zScale;
	m[3][2] = q;
	m[2][3] = 1;

	return *this;
}
//---------------------------------------------------------------------------
void Matrix44::transpose(Matrix44& out) const
{
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			out.m[i][j] = m[j][i];
		}
	}
}
//---------------------------------------------------------------------------
// from http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
float Matrix44::determinant() const
{
	// brute-force way, 72 mults, 24 adds
	
	// put our matrix elements in registers (or at least let the compiler do 
	// that) to eliminate dereferencing during these operations...
	float m00 = m[0][0];
	float m01 = m[0][1];
	float m02 = m[0][2];
	float m03 = m[0][3];
	float m10 = m[1][0];
	float m11 = m[1][1];
	float m12 = m[1][2];
	float m13 = m[1][3];
	float m20 = m[2][0];
	float m21 = m[2][1];
	float m22 = m[2][2];
	float m23 = m[2][3];
	float m30 = m[3][0];
	float m31 = m[3][1];
	float m32 = m[3][2];
	float m33 = m[3][3];

	return 
		+ m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30
		- m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30
		+ m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30
		- m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31
		+ m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31
		- m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31
		+ m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32
		- m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32
		+ m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32
		- m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33
		+ m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33
		- m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33
		;
}
//---------------------------------------------------------------------------
bool Matrix44::invert(Matrix44& out) const
{
#if 0
	float det = determinant();
	if (MathUtil::abs(det) < MathUtil::EPSILON)
	{
		// not invertible
		out = Matrix44::IDENTITY;
		return false;
	}

	float oodet = 1.f / det;
	

	for (size_t i=0; i<4; ++i)
	{
		for (size_t j=0; j<4; ++j)
		{
			out.m[i][j] = m[i][j] * oodet;
		}
	}
#endif
    float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
    float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
    float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
    float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;

    float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

    float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;

    float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    out.m[0][0] = d00; out.m[0][1] = d01; out.m[0][2] =  d02; out.m[0][3] =  d03;
    out.m[1][0] = d10; out.m[1][1] = d11; out.m[1][2] =  d12; out.m[1][3] =  d13;
    out.m[2][0] = d20; out.m[2][1] = d21; out.m[2][2] =  d22; out.m[2][3] =  d23;
    out.m[3][0] = d30; out.m[3][1] = d31; out.m[3][2] =  d32; out.m[3][3] =  d33;

	return true;
}
//---------------------------------------------------------------------------
Vector4 Matrix44::operator*(const Vector4& v) const
{
	Vector4 rtn;
	mul(rtn, *this, v);
	return rtn;
}
//---------------------------------------------------------------------------
void Matrix44::setRow(size_t row, const Vector4& v)
{
	assert(row >= 0 && row < 4);
	if (row >= 0 && row < 4)
	{
		m[row][0] = v.x;
		m[row][1] = v.y;
		m[row][2] = v.z;
		m[row][3] = v.w;
	}
}
//---------------------------------------------------------------------------
Matrix44 Teardrop::operator-(const Matrix44& mat)
{
	Matrix44 rtn;
	
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			rtn.m[i][j] = -mat.m[j][i];
		}
	}

	return rtn;
}
//---------------------------------------------------------------------------
void Teardrop::mul(/*out*/Vector4& out, /*in*/const Matrix44& l, /*in*/const Vector4& r)
{
	out.x = l.m[0][0]*r.x + l.m[0][1]*r.y + l.m[0][2]*r.z + l.m[0][3]*r.w;
	out.y = l.m[1][0]*r.x + l.m[1][1]*r.y + l.m[1][2]*r.z + l.m[1][3]*r.w;
	out.z = l.m[2][0]*r.x + l.m[2][1]*r.y + l.m[2][2]*r.z + l.m[2][3]*r.w;
	out.w = l.m[3][0]*r.x + l.m[3][1]*r.y + l.m[3][2]*r.z + l.m[3][3]*r.w;
}
//---------------------------------------------------------------------------
void Teardrop::mul3(/*out*/Vector4& out, /*in*/const Matrix44& l, /*in*/const Vector4& r)
{
	out.x = l.m[0][0]*r.x + l.m[0][1]*r.y + l.m[0][2]*r.z;
	out.y = l.m[1][0]*r.x + l.m[1][1]*r.y + l.m[1][2]*r.z;
	out.z = l.m[2][0]*r.x + l.m[2][1]*r.y + l.m[2][2]*r.z;
	out.w = 0;
}
//---------------------------------------------------------------------------
Matrix44 Teardrop::operator*(const Matrix44& l, /*in*/const Matrix44& r)
{
	Matrix44 out;
	mul(out, l, r);
	return out;
}
//---------------------------------------------------------------------------
void Teardrop::add(/*out*/Matrix44& out, /*in*/const Matrix44& l, /*in*/const Matrix44& r)
{
	for(size_t i=0; i<4; ++i)
	{
		for (size_t j=0; j<4; ++j)
		{
			out.m[i][j] = l.m[i][j] + r.m[i][j];
		}
	}
}
//---------------------------------------------------------------------------
Matrix44 Teardrop::operator+(/*in*/const Matrix44& l, /*in*/const Matrix44& r)
{
	Matrix44 out;
	add(out, l, r);
	return out;
}
//---------------------------------------------------------------------------
void Teardrop::diff(/*out*/Matrix44& out, /*in*/const Matrix44& l, /*in*/const Matrix44& r)
{
	for(size_t i=0; i<4; ++i)
	{
		for (size_t j=0; j<4; ++j)
		{
			out.m[i][j] = l.m[i][j] - r.m[i][j];
		}
	}
}
//---------------------------------------------------------------------------
Matrix44 Teardrop::operator-(/*in*/const Matrix44& l, /*in*/const Matrix44& r)
{
	Matrix44 out;
	diff(out, l, r);
	return out;
}
