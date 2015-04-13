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

#include "HkxPrune.h"
#include "Common/Base/hkBase.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"
#include "Common/SceneData/Scene/hkxScene.h"
#include "Common/SceneData/Environment/hkxEnvironment.h"
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
HkxPrune::HkxPrune()
{
	m_pHkx = 0;
}
//---------------------------------------------------------------------------
HkxPrune::~HkxPrune()
{
}
//---------------------------------------------------------------------------
bool HkxPrune::initialize(hkRootLevelContainer* pHkx)
{
	m_pHkx = pHkx;
	return true;
}
//---------------------------------------------------------------------------
bool HkxPrune::destroy()
{
	m_pHkx = 0;
	return true;
}
//---------------------------------------------------------------------------
bool HkxPrune::prune(hkRootLevelContainer& dest, size_t mask)
{
	if (!m_pHkx)
	{
		// output warning?
		return false;
	}
#if 0 // we don't really need this anymore...
	for (int i=0; i<m_pHkx->m_namedVariants.getSize(); ++i)
	{
		bool bCopy = (mask == NONE 
			|| ((mask & ENVIRONMENTDATA) && 
				hkString::strCmp(m_pHkx->m_namedVariants[i].getTypeName(), 
					hkxEnvironmentClass.getName())!=0) 
			|| ((mask & SCENEDATA) && 
				hkString::strCmp(m_pHkx->m_namedVariants[i].getTypeName(), 
					hkxSceneClass.getName())!=0)
			);

		if (bCopy)
		{
			dest.m_namedVariants[dest.m_numNamedVariants++] = 
				m_pHkx->m_namedVariants[i];
		}
	}
#endif
	return true;
}
