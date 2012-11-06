/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "SpawnPoint.h"
#include "Zone.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(SpawnPoint);
//---------------------------------------------------------------------------
SpawnPoint::SpawnPoint()
{
	m_valid = 0;
	m_spawnIndex = size_t(-1);
}
//---------------------------------------------------------------------------
SpawnPoint::~SpawnPoint()
{
}
//---------------------------------------------------------------------------
bool SpawnPoint::initialize()
{
	m_valid = 1;
	return true;
}
//---------------------------------------------------------------------------
bool SpawnPoint::isValid() const
{
	return (0 != m_valid);
}
//---------------------------------------------------------------------------
void SpawnPoint::setValid(bool valid)
{
	m_valid = valid ? 1 : 0;
}
//---------------------------------------------------------------------------
size_t SpawnPoint::getSpawnIndex() const
{
	return m_spawnIndex;
}
//---------------------------------------------------------------------------
void SpawnPoint::setSpawnIndex(size_t idx)
{
	m_spawnIndex = idx;
}
//---------------------------------------------------------------------------
bool SpawnPoint::operator==(const SpawnPoint& other)
{
	Transform xform(const_cast<SpawnPoint&>(other).getTransform());
	return (getTransform().trans == xform.trans);
}
//---------------------------------------------------------------------------
bool SpawnPoint::operator!=(const SpawnPoint& other)
{
	return !(*this == other);
}
//---------------------------------------------------------------------------
void SpawnPoint::addedToZone(Zone* pZone)
{
	pZone->addSpawnPoint(this);
}
//---------------------------------------------------------------------------
void SpawnPoint::removeFromZone(Zone* /*pZone*/)
{
}
