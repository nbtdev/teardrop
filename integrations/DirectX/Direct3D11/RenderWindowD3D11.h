/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERWINDOWD3D11_INCLUDED)
#define RENDERWINDOWD3D11_INCLUDED

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
				~RenderWindow();

				void present();
				void resize(int w, int h);

				HWND hWnd();

				TD_DECLARE_ALLOCATOR();

			protected:
				HWND mHwnd = 0;
				int mInitFlags = 0;
				ID3D11Device* mDevice = nullptr;
				IDXGISwapChain* mSwapChain = nullptr;
			};
		}
	}
}

#endif // RENDERWINDOWD3D11_INCLUDED
