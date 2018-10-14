/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(RENDERWINDOWD3D9_INCLUDED)
#define RENDERWINDOWD3D9_INCLUDED

#include "RenderTargetD3D9.h"

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class RenderWindow : public RenderTarget
			{
			public:
				RenderWindow(IDirect3DDevice9* device, HWND hWnd);
				RenderWindow(IDirect3DDevice9* device, HWND hWnd, int flags);
				~RenderWindow();

				void present();
				void resize(int w, int h);

				HWND hWnd();
				D3DPRESENT_PARAMETERS& presentParams();

				TD_DECLARE_ALLOCATOR();

			protected:
				HWND mHwnd;
				int mInitFlags;
				D3DPRESENT_PARAMETERS mPParams;
				IDirect3DSwapChain9* mSwapChain;
			};
		}
	}
}

#endif // RENDERWINDOWD3D9_INCLUDED
