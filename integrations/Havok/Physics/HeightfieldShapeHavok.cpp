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
#include "HeightfieldShapeHavok.h"
#include "Util/SystemManager.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Math/MathUtil.h"
#include "Memory/Allocators.h"
#include "stdio.h"

using namespace CoS;
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
	m_pData = pSys->getAllocator()->AllocateAligned(bufSize, 16 COS_ALLOC_SITE);
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
