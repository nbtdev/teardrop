/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(OBJECTMODE_INCLUDED)
#define OBJECTMODE_INCLUDED

#include "Mode.h"
#include "Memory/Memory.h"
#include <vector>

namespace CoS
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
	CoS::Reflection::Object* m_pSelectedObject;
	CoS::TransformGizmo* m_pGizmo;

public:
	ObjectMode();
	virtual ~ObjectMode();

	// Mode implementation
	bool update(float deltaT, Data* pData);
	bool handleInputEvent(const CoS::InputEvent& event, Data* pData);
	Tool* getActiveTool();
	void setActiveTool(Tool* pTool, size_t toolIdx);
	const Tool** getTools();
	size_t getNumTools();
	ContextMenu* getContextMenuDefinition();

	COS_DECLARE_ALLOCATOR();

protected:
	void _activeViewChanged();
	void selectedObjectChanged(void* sender, CoS::Reflection::Object* pObj);
	void handleContextMenu(void* sender, size_t id);
};

#endif // OBJECTMODE_INCLUDED
