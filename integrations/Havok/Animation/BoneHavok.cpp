/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
