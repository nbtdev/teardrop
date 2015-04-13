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

#if !defined(TDMATRIX44_INCLUDED)
#define TDMATRIX44_INCLUDED

#include <cstddef>

namespace Teardrop
{
	class Quaternion;
	class Vector4;

	class Matrix44
	{
	public:
		union
		{
			float _m[16];
			float m[4][4];
		};

		Matrix44();
		Matrix44(const Matrix44& other);
		Matrix44(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			);
		~Matrix44();

		Matrix44& operator=(const Matrix44& other);

		Matrix44& fromLookAtLH(
			const Vector4& position,
			const Vector4& lookAt,
			const Vector4& up);
		Matrix44& fromLookAtRH(
			const Vector4& position,
			const Vector4& lookAt,
			const Vector4& up);
		Matrix44& makeOrthoProjectionLH(
			float width, float height,
			float near, float far);
		Matrix44& makePerspectiveProjectionLH(
			float near, float far,
			float fov, float aspect);
		Matrix44& makePerspectiveProjectionRH(
			float near, float far,
			float fov, float aspect);
		void transpose(/*out*/Matrix44& out) const;
		float determinant() const;
		bool invert(/*out*/Matrix44& out) const;
		Vector4 operator*(const Vector4& v) const;
		void setRow(size_t row, const Vector4& v);

		static const Matrix44& IDENTITY;
		static const Matrix44& ZERO;
	};

	inline Matrix44::Matrix44()
	{
	}

	inline Matrix44::Matrix44(const Matrix44& other)
	{
		*this = other;
	}

	Matrix44 operator-(const Matrix44& mat);
	Matrix44 operator+(const Matrix44& l, const Matrix44& r);
	Matrix44 operator-(const Matrix44& l, const Matrix44& r);
	Matrix44 operator*(const Matrix44& l, const Matrix44& r);
	void add(/*out*/Matrix44& out, /*in*/const Matrix44& l, /*in*/const Matrix44& r);
	void diff(/*out*/Matrix44& out, /*in*/const Matrix44& l, /*in*/const Matrix44& r);

	void mul(/*out*/Vector4& out, const Matrix44& l, const Vector4& r);
	void mul(/*out*/Matrix44& out, const Matrix44& l, const Matrix44& r);

	//! for multiplying only the rotation part
	void mul3(/*out*/Vector4& out, const Matrix44& l, const Vector4& r);
	void mul3(/*out*/Matrix44& out, const Matrix44& l, const Matrix44& r);
}

#endif // TDMATRIX44_INCLUDED
