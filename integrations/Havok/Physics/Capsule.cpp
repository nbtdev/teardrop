/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Capsule.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
Capsule::Capsule()
{
}
//---------------------------------------------------------------------------
Capsule::~Capsule()
{
}
//---------------------------------------------------------------------------
bool Capsule::initialize(
	const Vector4& p0,
	const Vector4& p1,
	float radius)
{
	if (!ShapeHavok::initialize())
	{
		return false;
	}

	// make a hkpCapsuleShape
	m_pShape = new hkpCapsuleShape(
			(hkVector4&)p0,
			(hkVector4&)p1,
			radius
		);

	return true;
}
