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
