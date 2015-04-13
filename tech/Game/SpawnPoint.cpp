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
