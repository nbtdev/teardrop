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
