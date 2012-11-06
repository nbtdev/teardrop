/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ShapeHavok.h"
#include "Util/Environment.h"
#include "Util/Logger.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ShapeHavok::ShapeHavok()
{
	m_pShape = 0;
}
//---------------------------------------------------------------------------
ShapeHavok::~ShapeHavok()
{
}
//---------------------------------------------------------------------------
bool ShapeHavok::initialize()
{
	release();
	return Shape::initialize();
}
//---------------------------------------------------------------------------
bool ShapeHavok::release()
{
	if (m_pShape)
	{
		m_pShape->removeReference();
		m_pShape = 0;
	}

	return Shape::release();
}
//---------------------------------------------------------------------------
bool ShapeHavok::update(float deltaT)
{
	return Shape::update(deltaT);
}
//---------------------------------------------------------------------------
hkpShape* ShapeHavok::getHavokShape()
{
	return m_pShape;
}
