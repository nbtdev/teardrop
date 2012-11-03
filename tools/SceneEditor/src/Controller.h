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

#if !defined(CONTROLLER_INCLUDED)
#define CONTROLLER_INCLUDED

#include "MessageBus.h"
#include "Memory/Memory.h"

namespace CoS
{
	class Input;
}

class ModeManager;
class ViewManager;
class UserInterface;
class View;
class Data;

class Controller
	: public MessageBus
{
	ModeManager* m_pModeMgr;
	ViewManager* m_pViewMgr;
	UserInterface* m_pUI;
	CoS::Input* m_pInput;
	View* m_pActiveView;
	bool m_bActive; // tracks the "active" state of the app/top-level-window

public:
	Controller(
		CoS::Input* pInput,		// central HID input interface
		View* pDefault,			// default view (usually app main window)
		UserInterface* pUI		// the windowing system shell/frame
		);
	~Controller();

	bool update(float deltaT, Data* pData);
	void activate(bool bActive);

	COS_DECLARE_ALLOCATOR();

private:
	void onActiveViewChanged(View*);
	void onUserInterfaceClosing();
};

#endif // CONTROLLER_INCLUDED
