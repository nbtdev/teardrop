/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Plane.h"

using namespace Teardrop;
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
