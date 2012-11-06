/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Triangle.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Triangle::Triangle()
{
}
//---------------------------------------------------------------------------
Triangle::~Triangle()
{
}
//---------------------------------------------------------------------------
Triangle::Triangle(const Triangle& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
Triangle::Triangle(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	fromPoints(p0, p1, p2);
}
//---------------------------------------------------------------------------
Triangle& Triangle::operator=(const Triangle& other)
{
	p[0] = other.p[0];
	p[1] = other.p[1];
	p[2] = other.p[2];
	return *this;
}
//---------------------------------------------------------------------------
void Triangle::fromPoints(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
}
