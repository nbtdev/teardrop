/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CollisionFilterHavok.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
CollisionFilterHavok::CollisionFilterHavok()
{
	m_pFilter = new hkpGroupFilter;

	// enable all collisions by default
	m_pFilter->enableCollisionsUsingBitfield(unsigned int(-1), unsigned int(-1));
}
//---------------------------------------------------------------------------
CollisionFilterHavok::~CollisionFilterHavok()
{
	m_pFilter->removeReference();
}
//---------------------------------------------------------------------------
void CollisionFilterHavok::enableCollisionBetween(
	CollisionGroup /*group1*/, CollisionGroup /*group2*/)
{
}
//---------------------------------------------------------------------------
void CollisionFilterHavok::enableCollisionBetween(
	CollisionLayer layer1, CollisionLayer layer2)
{
	m_pFilter->enableCollisionsBetween(layer1, layer2);
}
//---------------------------------------------------------------------------
void CollisionFilterHavok::disableCollisionBetween(
	CollisionGroup /*group1*/, CollisionGroup /*group2*/)
{
}
//---------------------------------------------------------------------------
void CollisionFilterHavok::disableCollisionBetween(
	CollisionLayer layer1, CollisionLayer layer2)
{
	m_pFilter->disableCollisionsBetween(layer1, layer2);
}
