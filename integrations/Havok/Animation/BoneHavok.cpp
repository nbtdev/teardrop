/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "BoneHavok.h"
#include "Animation/Animation/Rig/hkaBone.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
BoneHavok::BoneHavok()
{
	m_pBone = 0;
	m_pParent = 0;
}
//---------------------------------------------------------------------------
BoneHavok::~BoneHavok()
{
}
//---------------------------------------------------------------------------
bool BoneHavok::initialize(hkaSkeleton* pParent, hkaBone* pBone)
{
	m_pBone = pBone;
	m_pParent = pParent;

	return true;
}
//---------------------------------------------------------------------------
bool BoneHavok::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
const char* BoneHavok::getName() const
{
	if (!m_pBone)
	{
		return 0;
	}

	return m_pBone->m_name;
}
//---------------------------------------------------------------------------
Bone* BoneHavok::getParent(const char* name) const
{
	return 0;
}
//---------------------------------------------------------------------------
size_t BoneHavok::getNumChildren() const
{
	return 0;
}
//---------------------------------------------------------------------------
const Bone* BoneHavok::getChild(size_t index) const
{
	return 0;
}
//---------------------------------------------------------------------------
Bone* BoneHavok::getChild(size_t index)
{
	return 0;
}
