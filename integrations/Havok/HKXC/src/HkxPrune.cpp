/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
