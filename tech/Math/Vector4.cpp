/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Vector4.h"
#include "MathUtil.h"
#include <limits>

using namespace Teardrop;
//---------------------------------------------------------------------------
const Teardrop::Vector4& Teardrop::Vector4::ZERO = Teardrop::Vector4(0, 0, 0, 0);
const Teardrop::Vector4& Teardrop::Vector4::UNIT_SCALE = Teardrop::Vector4(1, 1, 1, 0);
const Teardrop::Vector4& Teardrop::Vector4::UNIT_X = Teardrop::Vector4(1, 0, 0, 0);
const Teardrop::Vector4& Teardrop::Vector4::UNIT_Y = Teardrop::Vector4(0, 1, 0, 0);
const Teardrop::Vector4& Teardrop::Vector4::UNIT_Z = Teardrop::Vector4(0, 0, 1, 0);
const Teardrop::Vector4& Teardrop::Vector4::POSITIVE_INFINITY = Teardrop::Vector4(
	std::numeric_limits<float>::infinity(),
	std::numeric_limits<float>::infinity(),
	std::numeric_limits<float>::infinity(),
	std::numeric_limits<float>::infinity());
const Teardrop::Vector4& Teardrop::Vector4::NEGATIVE_INFINITY = Teardrop::Vector4(
	-std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity(),
	-std::numeric_limits<float>::infinity());
//---------------------------------------------------------------------------
Vector4::~Vector4()
{
}
//---------------------------------------------------------------------------
bool Vector4::operator!=(const Vector4& other) const
{
	return !(*this == other);
}
//---------------------------------------------------------------------------
bool Vector4::operator==(const Vector4& other) const
{
	return (
			(MathUtil::abs(x - other.x) < MathUtil::EPSILON || x == other.x)
		&&	(MathUtil::abs(y - other.y) < MathUtil::EPSILON || y == other.y)
		&&	(MathUtil::abs(z - other.z) < MathUtil::EPSILON || z == other.z)
		&&	(MathUtil::abs(w - other.w) < MathUtil::EPSILON || w == other.w)
		);
}
//---------------------------------------------------------------------------
Vector4& Vector4::operator+=(const Vector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}
//---------------------------------------------------------------------------
Vector4& Vector4::operator-=(const Vector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}
//---------------------------------------------------------------------------
float Vector4::getLength() const
{
	// w doesn't enter innit
	return MathUtil::sqrt(x*x + y*y + z*z);
}
//---------------------------------------------------------------------------
float Vector4::getLength2() const
{
	// w doesn't enter innit
	return x*x + y*y + z*z;
}
//---------------------------------------------------------------------------
Vector4& Vector4::operator*=(float factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
	w *= factor;
	return *this;
}
//---------------------------------------------------------------------------
Vector4& Vector4::operator/=(float factor)
{
	x /= factor;
	y /= factor;
	z /= factor;
	w /= factor;
	return *this;
}
//---------------------------------------------------------------------------
float Vector4::normalize()
{
	// normalizes this vector
	float l = getLength();
	*this *= (1.0f / l);
	return l;
}
//---------------------------------------------------------------------------
Vector4 Vector4::getNormalized() const
{
	// normalizes this vector
	float l = getLength();
	Vector4 rtn(*this);
	rtn *= (1.0f / l);
	return rtn;
}
//---------------------------------------------------------------------------
void Vector4::makeAbsolute()
{
	// make all components absolute values
	x = MathUtil::abs(x);
	y = MathUtil::abs(y);
	z = MathUtil::abs(z);
	w = MathUtil::abs(w);
}
