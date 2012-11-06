/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CONTROLLER_INCLUDED)
#define CONTROLLER_INCLUDED

#include "MessageBus.h"
#include "Memory/Memory.h"

namespace Teardrop
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
	Teardrop::Input* m_pInput;
	View* m_pActiveView;
	bool m_bActive; // tracks the "active" state of the app/top-level-window

public:
	Controller(
		Teardrop::Input* pInput,		// central HID input interface
		View* pDefault,			// default view (usually app main window)
		UserInterface* pUI		// the windowing system shell/frame
		);
	~Controller();

	bool update(float deltaT, Data* pData);
	void activate(bool bActive);

	TD_DECLARE_ALLOCATOR();

private:
	void onActiveViewChanged(View*);
	void onUserInterfaceClosing();
};

#endif // CONTROLLER_INCLUDED
