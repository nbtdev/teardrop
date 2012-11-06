/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(COREINTERFACE_INCLUDED)
#define COREINTERFACE_INCLUDED

namespace Teardrop
{
	class Input;
}

class ModeManager;
class ViewManager;
class UserInterface;
class View;
class Mode;
class Tool;
class MessageBus;
class Controller;

// it's the non-singleton place we store all of our singletons...
struct CoreInterface
{
	UserInterface* pUI;
	MessageBus* pMsgBus;
	ViewManager* pViewMgr;
	ModeManager* pModeMgr;
	Teardrop::Input* pInput;
	View* pCurrentView;
	Controller* pController;

	CoreInterface();
	static CoreInterface& get();
};

#endif // COREINTERFACE_INCLUDED
