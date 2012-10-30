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

#if !defined(_RENDERWINDOW_INCLUDED)
#define _RENDERWINDOW_INCLUDED

#include "wx/panel.h"
#include "View.h"
#include "Memory/include/Memory.h"

namespace CoS
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
	CoS::GfxRenderTarget* m_pWin;
	CoS::GfxCamera* m_pCam;
	CoS::GfxViewport* m_pVP;
	CameraControllerManager* m_pCamMgr;
	CoS::SceneRenderer* m_pSceneRenderer;
	ToolRenderStep* m_pToolRenderStep;
	
public:
	RenderWindow(wxWindow* pParent);
	virtual ~RenderWindow();

	// View implementation
	void update(float deltaT, Data* pData);
	bool handleInput(const CoS::InputEvent& evt);
	void activate(MessageBus* pMsgBus);
	void deactivate(MessageBus* pMsgBus);
	const CoS::GfxCamera* getCamera();
	void getViewSize(/*out*/CoS::Vector2& size);
	bool isInside(int x, int y);

	COS_DECLARE_ALLOCATOR();

private:
	DECLARE_EVENT_TABLE()
	void OnSize(wxSizeEvent& event);

	void createWindow(int w, int h);

	void onSelectedObjectChanged(void* pSender, CoS::Reflection::Object* pObj);
};

#endif // _RENDERWINDOW_INCLUDED
