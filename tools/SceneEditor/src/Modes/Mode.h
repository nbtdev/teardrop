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

#if !defined(MODE_INCLUDED)
#define MODE_INCLUDED

namespace CoS
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
	virtual bool handleInputEvent(const CoS::InputEvent& event, Data* pData) = 0;
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
