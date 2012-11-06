/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEW_INCLUDED)
#define VIEW_INCLUDED

#include "FastDelegate.h"

namespace Teardrop
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
	virtual bool handleInput(const Teardrop::InputEvent& event);
	virtual void setActiveTool(Tool* pTool);
	virtual void activate(MessageBus* pMsgBus);
	virtual void deactivate(MessageBus* pMsgBus);

	// views that involve a render window might implement these
	virtual const Teardrop::GfxCamera* getCamera(); 
	virtual void getViewSize(/*out*/Teardrop::Vector2& size);
	virtual bool isInside(int x, int y);

protected:
	Tool* m_pActiveTool;
};

#endif // VIEW_INCLUDED
