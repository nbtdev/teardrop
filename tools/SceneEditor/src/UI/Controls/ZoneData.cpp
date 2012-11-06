/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "ZoneData.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(ZoneData);
//---------------------------------------------------------------------------
ZoneData::ZoneData()
{
	m_pZone = 0;
}
//---------------------------------------------------------------------------
ZoneData::~ZoneData()
{
}
//---------------------------------------------------------------------------
Zone* ZoneData::getZone()
{
	return m_pZone;
}
//---------------------------------------------------------------------------
void ZoneData::setZone(Zone* pZone)
{
	m_pZone = pZone;
}
