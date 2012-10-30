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

#if !defined(VIEW_INCLUDED)
#define VIEW_INCLUDED

#include "FastDelegate.h"

namespace CoS
{
	union InputEvent;
	class Vector2;
	class GfxCamera;
}

class Data;
class Tool;
class MessageBus;

class View
{
public:
	View();
	virtual ~View();

	// when this window is activated (brought to front, etc)
	func::delegate1<View*> Activated(View* pView);

	virtual void update(float deltaT, Data* pData);
	virtual bool handleInput(const CoS::InputEvent& event);
	virtual void setActiveTool(Tool* pTool);
	virtual void activate(MessageBus* pMsgBus);
	virtual void deactivate(MessageBus* pMsgBus);

	// views that involve a render window might implement these
	virtual const CoS::GfxCamera* getCamera(); 
	virtual void getViewSize(/*out*/CoS::Vector2& size);
	virtual bool isInside(int x, int y);

protected:
	Tool* m_pActiveTool;
};

#endif // VIEW_INCLUDED
