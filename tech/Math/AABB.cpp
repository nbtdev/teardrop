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

#include "AABB.h"
#include "MathUtil.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
AABB::AABB()
{
	center = Vector4(0,0,0,0);
	extents = Vector4(0,0,0,0);
}
//---------------------------------------------------------------------------
AABB::~AABB()
{
}
//---------------------------------------------------------------------------
AABB::AABB(const AABB& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
AABB::AABB(const Vector4& center, const Vector4& extents)
{
	this->center = center;
	this->extents = extents;
}
//---------------------------------------------------------------------------
AABB& AABB::operator=(const AABB& other)
{
	center = other.center;
	extents = other.extents;
	return *this;
}
//---------------------------------------------------------------------------
void AABB::fromCorners(const Vector4& minCorner, const Vector4& maxCorner)
{
	center = (minCorner + maxCorner) / 2.0f;
	extents = maxCorner - center;
}
//---------------------------------------------------------------------------
void AABB::toCorners(Vector4& minCorner, Vector4& maxCorner) const
{
	minCorner = center - extents;
	maxCorner = center + extents;
}
//---------------------------------------------------------------------------
void AABB::merge(const AABB& other)
{
	if (extents == Vector4::POSITIVE_INFINITY)
	{
		*this = other;
		return;
	}

	Vector4 maxCorner(MathUtil::makeMax(
		center + extents,
		other.center + other.extents)
		);
	Vector4 minCorner(MathUtil::makeMin(
		center - extents,
		other.center - other.extents)
		);

	extents = (maxCorner - minCorner) / 2.0f;
	center = maxCorner - extents;
}
//---------------------------------------------------------------------------
float AABB::getRadius() const
{
	return extents.getLength();
}
//---------------------------------------------------------------------------
float AABB::getRadius2() const
{
	return extents.getLength2();
}
//---------------------------------------------------------------------------
void AABB::getCorners(Vector4* corners)
{
	corners[0] = center - extents;
	corners[1] = center - Vector4(extents.x, extents.y, -extents.z, 0);
	corners[2] = center - Vector4(extents.x, -extents.y, extents.z, 0);
	corners[3] = center - Vector4(-extents.x, extents.y, extents.z, 0);
	corners[4] = center + extents;
	corners[5] = center + Vector4(extents.x, extents.y, -extents.z, 0);
	corners[6] = center + Vector4(extents.x, -extents.y, extents.z, 0);
	corners[7] = center + Vector4(-extents.x, extents.y, extents.z, 0);
}
