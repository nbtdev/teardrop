/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECTMODE_INCLUDED)
#define OBJECTMODE_INCLUDED

#include "Mode.h"
#include "Memory/Memory.h"
#include <vector>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	class TransformGizmo;
}

class Toolbar;

/*
	class ObjectMode

	Object-manipulation mode. This mode is responsible for the 
	standard editor features such as 
	
	select(pick)-object
	move-selected-object
	rotate-selected-object
	scale-selected-object
	place-selected-object-type-instance

	and so on.
*/

class ObjectMode
	: public Mode
{
	typedef std::vector<Tool*> Tools;
	Tools m_tools;

	size_t m_currentTool;
	size_t m_toolsetId;
	Teardrop::Reflection::Object* m_pSelectedObject;
	Teardrop::TransformGizmo* m_pGizmo;

public:
	ObjectMode();
	virtual ~ObjectMode();

	// Mode implementation
	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData);
	Tool* getActiveTool();
	void setActiveTool(Tool* pTool, size_t toolIdx);
	const Tool** getTools();
	size_t getNumTools();
	ContextMenu* getContextMenuDefinition();

	TD_DECLARE_ALLOCATOR();

protected:
	void _activeViewChanged();
	void selectedObjectChanged(void* sender, Teardrop::Reflection::Object* pObj);
	void handleContextMenu(void* sender, size_t id);
};

#endif // OBJECTMODE_INCLUDED
