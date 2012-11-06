/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TOOLRENDERSTEP_INCLUDED)
#define TOOLRENDERSTEP_INCLUDED

#include "Game/RenderStep.h"

namespace Teardrop
{
	class Scene;
}

class Tool;

/*
	Render step for rendering Tool renderables
*/

class ToolRenderStep
	: public Teardrop::RenderStep
{
	Tool* m_pActiveTool;

public:
	ToolRenderStep();
	~ToolRenderStep();

	void setActiveTool(Tool* pTool);

	// RenderStep implementation
	void render(const VisibleObjects& objects, Teardrop::GfxRenderer* pRenderer, Teardrop::Scene* pScene);

	TD_DECLARE_ALLOCATOR();
};

#endif // TOOLRENDERSTEP_INCLUDED
