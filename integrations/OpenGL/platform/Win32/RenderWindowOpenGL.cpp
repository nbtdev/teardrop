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
		case WM_CREATE: 
			reinterpret_cast<Teardrop::Gfx::OpenGL::RenderWindow*>(lParam)->initContext(hWnd);
			return 0;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
	}
} // namespace

namespace Teardrop {
namespace Gfx {
namespace OpenGL {

RenderWindow::RenderWindow(HWND aParent, int aFlags)
	: mParent(aParent)
	, mInitFlags(aFlags)
{
	// first make a Win32 window with CS_OWNDC style
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "wglContextWindow";
	wc.style = CS_OWNDC;
	RegisterClass(&wc);

	RECT rect = { 0 };
	GetClientRect(mParent, &rect);

	int w = rect.right - rect.left;
	int h = rect.bottom = rect.top;

	HWND tmp = CreateWindow(
		wc.lpszClassName,
		NULL,
		WS_CHILD,
		0, 0,
		w, h,
		mParent,
		(HMENU)0,
		(HINSTANCE)0,
		this
		);

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
	GetWindowRect(mWindow, &rect);
	MoveWindow(mWindow, rect.left, rect.top, w, h, TRUE);
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
