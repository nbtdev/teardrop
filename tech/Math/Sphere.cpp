/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Sphere.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Sphere::Sphere()
{
}
//---------------------------------------------------------------------------
Sphere::~Sphere()
{
}
//---------------------------------------------------------------------------
Sphere::Sphere(const Sphere& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
Sphere::Sphere(const Vector4& center, float radius)
{
	fromCenterRadius(center, radius);
}
//---------------------------------------------------------------------------
Sphere& Sphere::operator=(const Sphere& other)
{
	m_center = other.m_center;
	m_radius = other.m_radius;
	return *this;
}
//---------------------------------------------------------------------------
Sphere& Sphere::fromCenterRadius(const Vector4& c, float r)
{
	m_center = c;
	m_radius = r;
	return *this;
}
