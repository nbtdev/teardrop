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

#include "Vector2.h"
#include <math.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
const Vector2& Vector2::ZERO = Vector2(0,0);
//---------------------------------------------------------------------------
Vector2::~Vector2()
{
}
//---------------------------------------------------------------------------
Vector2::Vector2(const Vector2& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
Vector2::Vector2(float ax, float ay)
{
	x = ax;
	y = ay;
}
//---------------------------------------------------------------------------
Vector2& Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;
	return *this;
}
//---------------------------------------------------------------------------
Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
//---------------------------------------------------------------------------
Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}
//---------------------------------------------------------------------------
Vector2& Vector2::add(const Vector2& a, const Vector2& b)
{
	x = a.x + b.x;
	y = a.y + b.y;
	return *this;
}
//---------------------------------------------------------------------------
Vector2& Vector2::diff(const Vector2& a, const Vector2& b)
{
	x = a.x - b.x;
	y = a.y - b.y;
	return *this;
}
//---------------------------------------------------------------------------
float Vector2::length() const
{
	return sqrt(x*x + y*y);
}
//---------------------------------------------------------------------------
float Vector2::length2() const
{
	return x*x + y*y;
}
//---------------------------------------------------------------------------
float Vector2::distance(const Vector2& other) const
{
	float dx = other.x - x;
	float dy = other.y - y;

	return sqrt(dx*dx + dy*dy);
}
//---------------------------------------------------------------------------
float Vector2::distance2(const Vector2& other) const
{
	float dx = other.x - x;
	float dy = other.y - y;

	return dx*dx + dy*dy;
}
//---------------------------------------------------------------------------
float Vector2::normalize()
{
	float len = length();
	if (len > 1e-08)
	{
		float f = 1.0f / len;
		x *= f;
		y *= f;
	}

	return len;
}
//---------------------------------------------------------------------------
Vector2 Teardrop::operator-(const Vector2& a, const Vector2& b)
{
	Vector2 rtn;

	rtn.x = a.x - b.x;
	rtn.y = a.y - b.y;

	return rtn;
}
//---------------------------------------------------------------------------
Vector2 Teardrop::operator+(const Vector2& a, const Vector2& b)
{
	Vector2 rtn;

	rtn.x = a.x + b.x;
	rtn.y = a.y + b.y;

	return rtn;
}
//---------------------------------------------------------------------------
void Teardrop::diff(Vector2& result, const Vector2& a, const Vector2& b)
{
	result.x = a.x - b.x;
	result.y = a.y - b.y;
}
//---------------------------------------------------------------------------
void Teardrop::add(Vector2& result, const Vector2& a, const Vector2& b)
{
	result.x = a.x + b.x;
	result.y = a.y + b.y;
}
