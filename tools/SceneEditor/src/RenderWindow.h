/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(_RENDERWINDOW_INCLUDED)
#define _RENDERWINDOW_INCLUDED

#include "wx/panel.h"
#include "View.h"
#include "Memory/Memory.h"

namespace Teardrop
{
	class GfxRenderWindow;
	class GfxCamera;
	class SceneRenderer;
	class GfxViewport;
	union InputEvent;

	namespace Reflection
	{
		class Object;
	}
}

class CameraControllerManager;
class MessageBus;
class ToolRenderStep;

class RenderWindow
	: public wxPanel,
	public View
{
	Teardrop::GfxRenderTarget* m_pWin;
	Teardrop::GfxCamera* m_pCam;
	Teardrop::GfxViewport* m_pVP;
	CameraControllerManager* m_pCamMgr;
	Teardrop::SceneRenderer* m_pSceneRenderer;
	ToolRenderStep* m_pToolRenderStep;
	
public:
	RenderWindow(wxWindow* pParent);
	virtual ~RenderWindow();

	// View implementation
	void update(float deltaT, Data* pData);
	bool handleInput(const Teardrop::InputEvent& evt);
	void activate(MessageBus* pMsgBus);
	void deactivate(MessageBus* pMsgBus);
	const Teardrop::GfxCamera* getCamera();
	void getViewSize(/*out*/Teardrop::Vector2& size);
	bool isInside(int x, int y);

	TD_DECLARE_ALLOCATOR();

private:
	DECLARE_EVENT_TABLE()
	void OnSize(wxSizeEvent& event);

	void createWindow(int w, int h);

	void onSelectedObjectChanged(void* pSender, Teardrop::Reflection::Object* pObj);
};

#endif // _RENDERWINDOW_INCLUDED
