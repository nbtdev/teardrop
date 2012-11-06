/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "Tools/Tool.h"
#include "ToolRenderStep.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
ToolRenderStep::ToolRenderStep()
{
	m_pActiveTool = 0;
}
//---------------------------------------------------------------------------
ToolRenderStep::~ToolRenderStep()
{
}
//---------------------------------------------------------------------------
void ToolRenderStep::setActiveTool(Tool* pTool)
{
	m_pActiveTool = pTool;
}
//---------------------------------------------------------------------------
void ToolRenderStep::render(
	const VisibleObjects& /*objects*/, GfxRenderer* pRend, Scene* /*pScene*/)
{
	// if the active tool has anything that needs rendering, get it now
	if (m_pActiveTool)
	{
		for (size_t i=0; i<m_pActiveTool->getNumRenderables(); ++i)
		{
			GfxMeshInstance inst;
			if (m_pActiveTool->getRenderable(inst, i))
				pRend->queueForRendering(inst);
		}

		pRend->endScene();
	}
}
