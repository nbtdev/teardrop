/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ROTATEOBJECTTOOL_INCLUDED)
#define ROTATEOBJECTTOOL_INCLUDED

#include "Tool.h"
#include "Math/Quaternion.h"

namespace Teardrop
{
	class TransformGizmo;
}

/*
	class RotateObjectTool

	Tool used to rotate an object in the scene, usually with a visible
	and co-located active transform gizmo.
*/

class RotateObjectTool
	: public Tool
{
	Teardrop::TransformGizmo& m_gizmo;
	Teardrop::Quaternion m_rotateStart;
	float m_rotateStartAngle;
	bool m_bLMB;
	bool m_bRotating;

public:
	RotateObjectTool(Teardrop::TransformGizmo& pGizmo);
	virtual ~RotateObjectTool();

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
	void rotate(int rx, int ry);
};

#endif // ROTATEOBJECTTOOL_INCLUDED
