/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "CCMState.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
CCMState::CCMState(CCMState* pNextState)
{
	m_pNextState = pNextState;
}
//---------------------------------------------------------------------------
CCMState::~CCMState()
{
}
