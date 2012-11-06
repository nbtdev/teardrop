/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
