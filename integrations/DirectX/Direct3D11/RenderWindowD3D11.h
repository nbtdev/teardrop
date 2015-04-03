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
			class RenderWindow : public RenderTarget
			{
			public:
				RenderWindow(HWND hWnd);
				RenderWindow(HWND hWnd, int flags);
				~RenderWindow();

				void present();
				void resize(int w, int h);

				HWND hWnd();
//				D3DPRESENT_PARAMETERS& presentParams();

				TD_DECLARE_ALLOCATOR();

			protected:
				HWND mHwnd = 0;
				int mInitFlags = 0;
//				D3DPRESENT_PARAMETERS mPParams;
//				IDirect3DSwapChain9* mSwapChain;
			};
		}
	}
}

#endif // RENDERWINDOWD3D11_INCLUDED
