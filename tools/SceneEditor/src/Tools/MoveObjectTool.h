/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MOVEOBJECTTOOL_INCLUDED)
#define MOVEOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Math/Vector4.h"

namespace Teardrop
{
	class TransformGizmo;
}

/*
	class MoveObjectTool

	Tool used to move an object in the scene, usually with a visible
	and co-located active transform gizmo.
*/

class MoveObjectTool
	: public Tool
{
	Teardrop::TransformGizmo& m_gizmo;
	Teardrop::Vector4 m_transScaleStart;
	Teardrop::Vector4 m_transOriginal;
	bool m_bLMB;
	bool m_bMoving;

public:
	MoveObjectTool(Teardrop::TransformGizmo& pGizmo);
	virtual ~MoveObjectTool();

	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	size_t getNumRenderables();
	bool getRenderable(/*out*/Teardrop::GfxMeshInstance& inst, /*in*/size_t index);
	void activate();
	void deactivate();

	TD_DECLARE_ALLOCATOR();

protected:
	void _onViewChanged();
	void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);
	void translate(int rx, int ry);
};

#endif // MOVEOBJECTTOOL_INCLUDED
