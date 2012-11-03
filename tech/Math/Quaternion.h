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

#if !defined(COSQUATERNION_INCLUDED)
#define COSQUATERNION_INCLUDED

namespace CoS
{
	class Vector4;
	class Matrix44;

	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Quaternion();
		Quaternion(const Quaternion& other);
		Quaternion(float w, float x, float y, float z);
		Quaternion(float radians, const Vector4& axis);
		~Quaternion();
		Quaternion& operator=(const Quaternion& other);

		void fromAngleAxis(float angle /*radians*/, const Vector4& axis);
		void toAngleAxis(float& angle /*radians*/, Vector4& axis);
		void toAzimuthElevation(float& azimuth, float& elevation) const;
		Quaternion operator*(const Quaternion& q) const;
		void toRotationMatrix(/*out*/Matrix44& out) const;
		void fromRotationMatrix(/*in*/const Matrix44& mat);
		Quaternion operator-() const;
		float normalize();
		float getNormalized(/*out*/Quaternion& q) const;
		Quaternion Quaternion::conjugate() const;

		bool operator==(const Quaternion& other);
		bool operator!=(const Quaternion& other);

		static const Quaternion& IDENTITY;
	};

	inline Quaternion::Quaternion()
	{
	}

	inline Quaternion::Quaternion(float W, float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	inline Quaternion::Quaternion(const Quaternion& other)
	{
		*this = other;
	}

	inline Quaternion::Quaternion(float radians, const Vector4& axis)
	{
		fromAngleAxis(radians, axis);
	}

	inline Quaternion& Quaternion::operator=(const Quaternion& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	inline Quaternion Quaternion::operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}

	inline Quaternion Quaternion::conjugate() const
	{
		return Quaternion(-w, x, y, z);
	}

	Vector4 operator*(const Quaternion& q, const Vector4& v);
	void invert(/*out*/Quaternion& q); // inverts in place, assumes unit quaternion!!!
}

#endif // COSQUATERNION_INCLUDED
