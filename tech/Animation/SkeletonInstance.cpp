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

#include "Animation.h"
#include "Rig.h"
#include "SkeletonInstance.h"
#include "Math/Matrix44.h"
#include "Util/Environment.h"
#include "Util/SystemManager.h"
//#include "Resource/ResourceManager.h"
#include "Memory/Allocators.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
SkeletonInstance::SkeletonInstance(Rig* pRig)
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	m_boneCount = pRig->getBoneCount();
	m_pBoneCache = static_cast<Matrix44*>(
		pAnimSys->getAllocator()->AllocateAligned(
			sizeof(Matrix44) * m_boneCount,
			16 TD_ALLOC_SITE)
	);
}
//---------------------------------------------------------------------------
SkeletonInstance::~SkeletonInstance()
{
	Teardrop::System* pAnimSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_ANIMATION);

	pAnimSys->getAllocator()->DeallocateAligned(m_pBoneCache);
}
//---------------------------------------------------------------------------
const Matrix44* SkeletonInstance::getBoneCache() const
{
	return m_pBoneCache;
}
//---------------------------------------------------------------------------
Matrix44* SkeletonInstance::getBoneCache()
{
	return m_pBoneCache;
}
//---------------------------------------------------------------------------
size_t SkeletonInstance::getBoneCount() const
{
	return m_boneCount;
}
//---------------------------------------------------------------------------
size_t SkeletonInstance::getBoneIndexByName(const String& /*name*/) const
{
	return size_t(-1);
}
