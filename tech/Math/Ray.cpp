/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Ray.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Ray::Ray()
{
}
//---------------------------------------------------------------------------
Ray::~Ray()
{
}
//---------------------------------------------------------------------------
Ray::Ray(const Ray& other)
{
	*this = other;
}
//---------------------------------------------------------------------------
Ray::Ray(const Vector4& end, const Vector4& dir)
{
	fromEndpointDirection(end, dir);
}
//---------------------------------------------------------------------------
Ray& Ray::operator=(const Ray& other)
{
	end = other.end;
	dir = other.dir;
	return *this;
}
//---------------------------------------------------------------------------
void Ray::fromEndpointDirection(const Vector4& e, const Vector4& d)
{
	end = e;
	dir = d;
}
