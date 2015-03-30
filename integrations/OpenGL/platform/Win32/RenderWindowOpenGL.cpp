/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "RenderWindowOpenGL.h"
#include "Gfx/Common.h"
#include <assert.h>
#include <iostream>

namespace {
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_CREATE: {
			CREATESTRUCT* s = (CREATESTRUCT*)lParam;
			reinterpret_cast<Teardrop::Gfx::OpenGL::RenderWindow*>(s->lpCreateParams)->initContext(hWnd);
			return 0;
		}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
	}

	struct ClassReg
	{
		static const char* className;
		ClassReg() {
			WNDCLASS wc = { 0 };
			wc.lpfnWndProc = WndProc;
			wc.hbrBackground = (HBRUSH)WHITE_BRUSH;
			wc.lpszClassName = className;
			wc.style = CS_OWNDC;
			RegisterClass(&wc);
		}

		~ClassReg() {
			UnregisterClass(className, (HINSTANCE)0);
		}
	};

	const char* ClassReg::className = "wglContextWindow";
} // namespace

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

RenderWindow::RenderWindow(HWND aParent, int aFlags)
	: mParent(aParent)
	, mInitFlags(aFlags)
{
	static ClassReg cr;

	// first make a Win32 window with CS_OWNDC style
	RECT rect = { 0 };
	GetClientRect(mParent, &rect);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	HWND tmp = CreateWindow(
		cr.className,
		NULL,
		WS_CHILD|WS_VISIBLE,
		0, 0,
		w, h,
		mParent,
		(HMENU)0,
		(HINSTANCE)0,
		this
		);

	mWidth = w;
	mHeight = h;

	// force this WM_CREATE through before we return
	MSG msg = { 0 };
	BOOL ret;
	while ((ret = PeekMessage(&msg, tmp, 0, 0, TRUE)) != 0) {
		if (ret != -1) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

RenderWindow::RenderWindow(HWND aParent)
: RenderWindow(aParent, 0)
{
}

RenderWindow::~RenderWindow()
{
	wglDeleteContext(mContext);
}

void RenderWindow::resize(int w, int h)
{
	RECT rect;

	if (mParent)
		GetClientRect(mParent, &rect);
	else
		GetWindowRect(mWindow, &rect);

	MoveWindow(mWindow, rect.left, rect.top, w, h, TRUE);

	mWidth = w;
	mHeight = h;
}

void RenderWindow::present()
{
	wglSwapLayerBuffers(mDC, WGL_SWAP_MAIN_PLANE);
}

void
RenderWindow::setCurrent()
{
	wglMakeCurrent(mDC, mContext);
}

void
RenderWindow::unsetCurrent()
{
	wglMakeCurrent(mDC, nullptr);
}

void RenderWindow::initContext(HWND aDummy)
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32; // color buffer depth
	pfd.cDepthBits = 24; // depth buffer bits
	pfd.cStencilBits = 8; // stencil buffer bits
	pfd.iLayerType = PFD_MAIN_PLANE;

	mWindow = aDummy;
	mDC = GetDC(mWindow);

	int pixelFormat = ChoosePixelFormat(mDC, &pfd);
	SetPixelFormat(mDC, pixelFormat, &pfd);

	mContext = wglCreateContext(mDC);
	wglMakeCurrent(mDC, mContext);
}

int RenderWindow::initFlags()
{
	return mInitFlags;
}

} // namespace OpenGL
} // namespace Gfx
} // namespace Teardrop
