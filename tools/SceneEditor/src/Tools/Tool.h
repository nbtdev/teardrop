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

#if !defined(TOOL_INCLUDED)
#define TOOL_INCLUDED

#include "Base/AttributeContainer.h"
#include <list>

namespace CoS
{
	namespace Reflection
	{
		class Object;
	}

	class GfxMeshInstance;
}

class Data;
class MessageBus;
class View;

/*
	class Tool

	Base class for editor tools. 

	An editor tool is something that the user manipulates to effect
	a change on the scene configuration. For example, a translation 
	tool would move the selected object (if any, and if it can be 
	moved) in response to user mouse movements. Likewise, a terrain
	sculpting "raise" tool might raise the affected terrain by 
	whatever configured amount is currently set. Tools generally are
	used by derived Mode classes.
*/

class Tool
	: public AttributeContainer
{
public:
	Tool();
	virtual ~Tool();

	virtual bool update(float deltaT, Data* pData) = 0;
	virtual bool handleInputEvent(const CoS::InputEvent& event, Data* pData) = 0;
	virtual size_t getNumRenderables() = 0;
	virtual bool getRenderable(/*out*/CoS::GfxMeshInstance& inst, /*in*/size_t index) = 0;
	virtual void activate();
	virtual void deactivate();
	
	void setActiveView(View*);
	void setSelectedObject(CoS::Reflection::Object* pObj);

protected:
	View* m_pView;

	typedef std::list<CoS::Reflection::Object*> SelectedObjects;
	SelectedObjects m_selectedObjects;

	virtual void _onViewChanged();
	virtual void selectedObjectChanged(void* sender, CoS::Reflection::Object* pObj);
};

#endif // TOOL_INCLUDED
