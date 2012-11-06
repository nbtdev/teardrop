/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Transform.h"
#include "Vector4.h"
#include "Quaternion.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
const Teardrop::Transform& Teardrop::Transform::IDENTITY = Teardrop::Transform(
	Teardrop::Vector4(0, 0, 0, 0), 
	Teardrop::Quaternion(1, 0, 0, 0),
	Teardrop::Vector4(1, 1, 1, 0)
	);
//---------------------------------------------------------------------------
Transform::~Transform()
{
}
//---------------------------------------------------------------------------
Transform::Transform(const Vector4& p, const Quaternion& r, const Vector4& s)
{
	trans = p;
	rot = r;
	scale = s;
}
//---------------------------------------------------------------------------
Transform& Transform::operator =(const Transform& other)
{
	trans = other.trans;
	rot = other.rot;
	scale = other.scale;

	return *this;
}
