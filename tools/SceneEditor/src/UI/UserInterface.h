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

#if !defined(USERINTERFACE_INCLUDED)
#define USERINTERFACE_INCLUDED

#include "FastDelegate.h"

/*
	Facade class for the various non-View elements of the user
	interface for the editor (treeview(s), property grid(s), 
	toolbar(s), status bar(s), etc)
*/

namespace CoS
{
	namespace Reflection
	{
		class Object;
	}

	union InputEvent;
}

class SceneEditorFrame;
class StatusBar;
class Toolbar;
class PropertyGridControl;
class ObjectTreeView;
struct ContextMenu;

class UserInterface
{
	SceneEditorFrame* m_pShell;

public:
	UserInterface(SceneEditorFrame* pShell);
	~UserInterface();

	StatusBar* getStatusBar();
	Toolbar* getToolBar();
	PropertyGridControl* getPropGrid();
	ObjectTreeView* getObjectTreeView();
	void showContextMenu(ContextMenu* pMenu, int x, int y);
	void showTemplateList(bool bShow);
	void showMaterialEditor(bool bShow);
	CoS::Reflection::Object* getSelectedTemplate();
	bool handleInputEvent(const CoS::InputEvent& evt);

	// alert that the user interface is shutting down
	func::delegate0<> UserInterfaceClosing;
	func::delegate2<const CoS::InputEvent&, bool&> OnInputEvent;

	COS_DECLARE_ALLOCATOR();

private:
	void onShellClosing();
};

#endif // USERINTERFACE_INCLUDED
