/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
				RenderWindow(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS& pparams, HWND hWnd);
				~RenderWindow();

				void present();

				HWND hWnd();
				D3DPRESENT_PARAMETERS& presentParams();

				void setPresentParams(D3DPRESENT_PARAMETERS& pparams);

				TD_DECLARE_ALLOCATOR();

			protected:
				HWND mHwnd;
				D3DPRESENT_PARAMETERS mPParams;
				IDirect3DSwapChain9* mSwapChain;
			};
		}
	}
}

#endif // RENDERWINDOWD3D9_INCLUDED
