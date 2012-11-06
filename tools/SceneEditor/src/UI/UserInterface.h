/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(USERINTERFACE_INCLUDED)
#define USERINTERFACE_INCLUDED

#include "FastDelegate.h"

/*
	Facade class for the various non-View elements of the user
	interface for the editor (treeview(s), property grid(s), 
	toolbar(s), status bar(s), etc)
*/

namespace Teardrop
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
	Teardrop::Reflection::Object* getSelectedTemplate();
	bool handleInputEvent(const Teardrop::InputEvent& evt);

	// alert that the user interface is shutting down
	func::delegate0<> UserInterfaceClosing;
	func::delegate2<const Teardrop::InputEvent&, bool&> OnInputEvent;

	TD_DECLARE_ALLOCATOR();

private:
	void onShellClosing();
};

#endif // USERINTERFACE_INCLUDED
