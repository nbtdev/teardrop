/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDVECTOR4_INCLUDED)
#define TDVECTOR4_INCLUDED

namespace Teardrop
{
	class Quaternion;

	class Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4(const Vector4& other);
		Vector4(float x, float y, float z, float w);
		~Vector4();

		bool operator==(const Vector4& other) const;
		bool operator!=(const Vector4& other) const;

		Vector4& operator=(const Vector4& other);
		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(float factor);
		Vector4& operator/=(float factor);
		Vector4 operator-() const;

		float getLength() const;
		float getLength2() const; // squared
		float normalize();
		Vector4 getNormalized() const;
		Vector4& applyScale(const Vector4& scaleVec);
		void makeAbsolute();

		static const Vector4& ZERO;
		static const Vector4& UNIT_SCALE;
		static const Vector4& UNIT_X;
		static const Vector4& UNIT_Y;
		static const Vector4& UNIT_Z;
		static const Vector4& POSITIVE_INFINITY;
		static const Vector4& NEGATIVE_INFINITY;
	};

	inline Vector4::Vector4()
	{
	}

	inline Vector4::Vector4(const Vector4& other)
	{
		*this = other;
	}

	inline Vector4::Vector4(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	inline Vector4& Vector4::operator=(const Vector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	inline Vector4 Vector4::operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline Vector4 operator+(const Vector4& l, const Vector4& r)
	{
		return Vector4(l.x+r.x, l.y+r.y, l.z+r.z, l.w+r.w);
	}

	inline Vector4 operator-(const Vector4& l, const Vector4& r)
	{
		return Vector4(l.x-r.x, l.y-r.y, l.z-r.z, l.w-r.w);
	}

	inline Vector4 operator*(const Vector4& l, float factor)
	{
		return Vector4(l.x*factor, l.y*factor, l.z*factor, l.w*factor);
	}

	inline Vector4 operator*(float factor, const Vector4& l)
	{
		return Vector4(l.x*factor, l.y*factor, l.z*factor, l.w*factor);
	}

	inline Vector4 operator/(const Vector4& l, float factor)
	{
		return Vector4(l.x/factor, l.y/factor, l.z/factor, l.w/factor);
	}

	inline Vector4 operator/(float factor, const Vector4& l)
	{
		return Vector4(factor/l.x, factor/l.y, factor/l.z, factor/l.w);
	}

	inline float dot(const Vector4& l, const Vector4& r)
	{
		return l.x*r.x + l.y*r.y + l.z*r.z + l.w*r.w;
	}

	inline void cross(/*out*/Vector4& out, /*in*/const Vector4& u, /*in*/const Vector4& v)
	{
		out.x = u.y*v.z - u.z*v.y;
		out.y = u.z*v.x - u.x*v.z;
		out.z = u.x*v.y - u.y*v.x;
		out.w = 0;
	}

	inline Vector4 cross(const Vector4& u, const Vector4& v)
	{
		Vector4 out;
		cross(out, u, v);
		return out;
	}

	inline Vector4& Vector4::applyScale(const Vector4& scaleVec)
	{
		x *= scaleVec.x;
		y *= scaleVec.y;
		z *= scaleVec.z;
		w *= scaleVec.w;
		return *this;
	}
}

#endif // TDVECTOR4_INCLUDED
