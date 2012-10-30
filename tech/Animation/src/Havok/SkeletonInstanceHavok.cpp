/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "RigHavok.h"
#include "SkeletonInstanceHavok.h"
#include "Util/include/_String.h"
#include "Memory/include/Memory.h"

using namespace CoS;
//---------------------------------------------------------------------------
SkeletonInstanceHavok::SkeletonInstanceHavok(Rig* pRig)
: m_pInstance(0), SkeletonInstance(pRig)
{
	RigHavok* pRigH = static_cast<RigHavok*>(pRig);
	m_pInstance = new hkaAnimatedSkeleton(pRigH->getSkeleton());
}
//---------------------------------------------------------------------------
SkeletonInstanceHavok::~SkeletonInstanceHavok()
{
	delete m_pInstance;
}
//---------------------------------------------------------------------------
hkaAnimatedSkeleton* SkeletonInstanceHavok::getAnimatedSkeleton()
{
	return m_pInstance;
}
//---------------------------------------------------------------------------
size_t SkeletonInstanceHavok::getBoneIndexByName(const String& name) const
{
	// find the named bone in the skeleton, or return size_t(-1)
	if (m_pInstance)
	{
		const hkaSkeleton* pSkel = m_pInstance->getSkeleton();
		hkInt32 numBones = pSkel->m_bones.getSize();
		for (hkInt32 i=0; i<numBones; ++i)
		{
			if (name == pSkel->m_bones[i].m_name)
				return size_t(i);
		}
	}

	return size_t(-1);
}
