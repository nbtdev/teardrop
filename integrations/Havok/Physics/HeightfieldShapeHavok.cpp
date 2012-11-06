/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "HeightfieldShapeHavok.h"
#include "Util/SystemManager.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Math/MathUtil.h"
#include "Memory/Allocators.h"
#include "stdio.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
HeightfieldShapeHavok::HeightfieldShapeHavok(const hkpSampledHeightFieldBaseCinfo& ci)
: hkpSampledHeightFieldShape(ci)
{
	m_pData = 0;
	m_bpp = 1;
	m_ooy = 1;
}
//---------------------------------------------------------------------------
HeightfieldShapeHavok::HeightfieldShapeHavok()
: hkpSampledHeightFieldShape()
{
	m_pData = 0;
	m_bpp = 1;
	m_ooy = 1;
}
//---------------------------------------------------------------------------
HeightfieldShapeHavok::~HeightfieldShapeHavok()
{
}
//---------------------------------------------------------------------------
bool HeightfieldShapeHavok::initialize()
{
	release();

	if (!m_userData)
	{
		return false;
	}

	Teardrop::System* pSys = 
		Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_PHYSICS);

	m_width = m_xRes;
	m_height = m_zRes;
	m_ooy = m_intToFloatScale(1) / (float)(m_bpp==1?256:65536);
	m_pShape = this;

	size_t bufSize = m_width * m_height * m_bpp;
	m_pData = pSys->getAllocator()->AllocateAligned(bufSize, 16 TD_ALLOC_SITE);
	memcpy(m_pData, (void*)m_userData, bufSize);

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShapeHavok::release()
{
	if (m_pData)	
	{
		Teardrop::System* pSys = 
			Environment::get().pSystemMgr->getActiveSystem(Teardrop::System::SYSTEM_PHYSICS);

		pSys->getAllocator()->DeallocateAligned(m_pData);
		m_pData = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShapeHavok::update(float deltaT)
{
	return true;
}
