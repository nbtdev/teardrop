/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

#if !defined(RENDERWINDOWD3D11_INCLUDED)
#define RENDERWINDOWD3D11_INCLUDED

#include "stdafx.h"
#include "RenderTargetD3D11.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Direct3D11
		{
			class Renderer;

			class RenderWindow : public RenderTarget
			{
			public:
				RenderWindow(Renderer* aRenderer, HWND hWnd);
				RenderWindow(Renderer* aRenderer, HWND hWnd, int flags);
				RenderWindow(Renderer* aRenderer, HWND hWnd, int aWidth, int aHeight);
				RenderWindow(Renderer* aRenderer, HWND hWnd, int aWidth, int aHeight, int aFlags);
				~RenderWindow();

				void present();
				void resize(int w, int h);

				HWND hWnd();

				TD_DECLARE_ALLOCATOR();

			protected:
				HWND mHwnd = 0;
				int mInitFlags = 0;
				ComPtr<IDXGISwapChain> mSwapChain;
				ComPtr<ID3D11Texture2D> mBackBuffer;
				D3D11_TEXTURE2D_DESC mDesc;
			};
		}
	}
}

#endif // RENDERWINDOWD3D11_INCLUDED
