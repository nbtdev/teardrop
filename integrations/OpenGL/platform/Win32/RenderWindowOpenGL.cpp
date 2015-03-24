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
				MSG msg = { 0 };
				WNDCLASS wc = { 0 };
				wc.lpfnWndProc = WndProc;
				wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
				wc.lpszClassName = "wglContextWindow";
				wc.style = CS_OWNDC;

				RECT rect = { 0 };
				GetClientRect(mParent, &rect);

				int w = rect.right - rect.left;
				int h = rect.bottom = rect.top;

				HWND dummy = CreateWindow(
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
#if 0
				XResizeWindow(mDisplay, mWindow, w, h);

				assert(mHwnd);

				if (mSurface) {
					mSurface->Release();
					mSurface = 0;
				}

				if (mDepthStencil) {
					mDepthStencil->Release();
					mDepthStencil = 0;
				}

				if (mSwapChain) {
					mSwapChain->Release();
					mSwapChain = 0;
				}

				mWidth = w;
				mHeight = h;

				ZeroMemory(&mPParams, sizeof(mPParams));
				mPParams.Windowed = TRUE;
				mPParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
				mPParams.BackBufferFormat = D3DFMT_UNKNOWN;
				mPParams.hDeviceWindow = mHwnd;
				mPParams.BackBufferWidth = mWidth;
				mPParams.BackBufferHeight = mHeight;
				mPParams.BackBufferCount = 1;
				mPParams.EnableAutoDepthStencil = TRUE;

				D3DFORMAT dsFmt = D3DFMT_UNKNOWN;

				if (mInitFlags & (INIT_ENABLE_DEPTH_BUFFER | INIT_ENABLE_STENCIL_BUFFER))
					dsFmt = D3DFMT_D24S8;

				mPParams.AutoDepthStencilFormat = dsFmt;

				mPParams.PresentationInterval =
					(mInitFlags & INIT_ENABLE_VSYNC) ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

				HRESULT hr = mDevice->CreateAdditionalSwapChain(&mPParams, &mSwapChain);

				if (SUCCEEDED(hr)) {
					// get render target surface ref
					mSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mSurface);

					// create depth buffer for additional swap chain
					hr = mDevice->CreateDepthStencilSurface(
						mPParams.BackBufferWidth,
						mPParams.BackBufferHeight,
						mPParams.AutoDepthStencilFormat,
						D3DMULTISAMPLE_NONE, // todo: support AA at some point
						0,
						FALSE,
						&mDepthStencil,
						NULL);
				}

				assert(mSurface);
				assert(mDepthStencil);
				assert(mSwapChain);
#endif
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
