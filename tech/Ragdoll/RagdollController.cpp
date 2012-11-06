/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RagdollController.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
RagdollController::RagdollController()
{
	m_hRagdoll = INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
RagdollController::~RagdollController()
{
}
//---------------------------------------------------------------------------
bool RagdollController::initialize(HResource hRagdoll)
{
	m_hRagdoll = hRagdoll;
	return true;
}
//---------------------------------------------------------------------------
bool RagdollController::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
bool RagdollController::drive(
	  float timeStep, AnimationBlender* pBlender, const Transform& world)
{
	return false;
}
//---------------------------------------------------------------------------
bool RagdollController::addToWorld(World* pWorld)
{
	return false;
}
//---------------------------------------------------------------------------
bool RagdollController::removeFromWorld(World* pWorld)
{
	return false;
}
//---------------------------------------------------------------------------
void RagdollController::setUserData(void*)
{
}
