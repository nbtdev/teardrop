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

#include "Plane.h"

using namespace CoS;
//---------------------------------------------------------------------------
Plane::Plane()
{
}
//---------------------------------------------------------------------------
Plane::~Plane()
{
}
//---------------------------------------------------------------------------
Plane::Plane(const Plane& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
Plane::Plane(const Vector4& origin, const Vector4& normal)
{
	fromPointNormal(origin, normal);
}
//---------------------------------------------------------------------------
Plane& Plane::operator=(const Plane& other)
{
	n = other.n;
	d = other.d;
	return *this;
}
//---------------------------------------------------------------------------
void Plane::fromPointNormal(const Vector4& origin, const Vector4& normal)
{
	n = normal;
	d = dot(n, origin);
}
//---------------------------------------------------------------------------
void Plane::fromTriangle(
	const Vector4& p1,
	const Vector4& p2,
	const Vector4& p3
	)
{
	n = cross(p2-p1, p3-p1).getNormalized();
	d = dot(n, p1);
}
