/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SELECTOBJECTTOOL_INCLUDED)
#define SELECTOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Gfx/GfxMeshInstance.h"
#include <vector>

/*
	class SelectObjectTool

	Tool used to select an object in the scene. Mouse-picking,
	in other words.
*/

class SelectObjectTool
	: public Tool
{
	typedef std::vector<Teardrop::GfxMeshInstance> SelectedInstances;
	SelectedInstances m_selectedInst;

public:
	SelectObjectTool();
	virtual ~SelectObjectTool();

	// Tool implementation
	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	size_t getNumRenderables();
	bool getRenderable(/*out*/Teardrop::GfxMeshInstance& inst, /*in*/size_t index);
	void activate();
	void deactivate();

	TD_DECLARE_ALLOCATOR();

protected:
	void _onViewChanged();
	virtual void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);
};

#endif // SELECTOBJECTTOOL_INCLUDED
