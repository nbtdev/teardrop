/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDQUATERNION_INCLUDED)
#define TDQUATERNION_INCLUDED

namespace Teardrop
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
        Quaternion conjugate() const;

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

#endif // TDQUATERNION_INCLUDED
