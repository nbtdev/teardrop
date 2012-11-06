/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MODE_INCLUDED)
#define MODE_INCLUDED

namespace Teardrop
{
	union InputEvent;
}

class Tool;
class Data;
class View;
class Toolbar;
struct ContextMenu;

/*
	class Mode

	Base class for editor modes. 

	An editor mode is any high-level state in which the editor can be at any
	given time. For example, "Object" mode would be a mode where the user
	can manipulate objects (move, rotate, place, etc), and "Terrain" mode
	would be a mode where the user can manipulate terrain (sculpting, resizing, 
	etc). Each mode might user zero or more tools (see Tool class) to manage the
	different types of work they can do on a scene. 
*/

class Mode
{
public:
	Mode();
	virtual ~Mode();

	virtual bool update(float deltaT, Data* pData) = 0;
	virtual bool handleInputEvent(const Teardrop::InputEvent& event, Data* pData) = 0;
	virtual Tool* getActiveTool() = 0;
	virtual void setActiveTool(Tool* pTool, size_t toolIdx) = 0;
	virtual const Tool** getTools() = 0;
	virtual size_t getNumTools() = 0;
	virtual void activate();
	virtual void deactivate();
	virtual ContextMenu* getContextMenuDefinition();
	void setActiveView(View*);

protected:
	View* m_pActiveView;

	virtual void _activeViewChanged();
};

#endif // MODE_INCLUDED
