/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Object.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(SceneObject);
//---------------------------------------------------------------------------
SceneObject::SceneObject()
{
	m_pObject = 0;
}
//---------------------------------------------------------------------------
SceneObject::~SceneObject()
{
}
//---------------------------------------------------------------------------
const char* SceneObject::getName() const
{
	return m_name.c_str();
}
//---------------------------------------------------------------------------
const char* SceneObject::getCategory() const
{
	return m_category.c_str();
}
//---------------------------------------------------------------------------
ZoneObject* SceneObject::getObject()
{
	return m_pObject;
}
//---------------------------------------------------------------------------
void SceneObject::setName(const char* name)
{
	if (name)
		m_name = name;
	else
		m_name.clear();
}
//---------------------------------------------------------------------------
void SceneObject::setCategory(const char* category)
{
	if (category)
		m_category = category;
	else
		m_category.clear();
}
//---------------------------------------------------------------------------
void SceneObject::setObject(ZoneObject* pObj)
{
	m_pObject = pObj;
}
