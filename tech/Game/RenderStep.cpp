/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderStep.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
RenderStep::RenderStep()
{
	m_pCamera = 0;
}
//---------------------------------------------------------------------------
RenderStep::~RenderStep()
{
}
//---------------------------------------------------------------------------
void RenderStep::setCamera(Gfx::Camera* pCamera)
{
	m_pCamera = pCamera;
}
