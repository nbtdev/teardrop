/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERERD3D9_INCLUDED)
#define RENDERERD3D9_INCLUDED

#include "Gfx/Renderer.h"
#include "Math/Matrix44.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		namespace Direct3D9
		{
			class Renderer : public Gfx::Renderer
			{
			public:
				Renderer();
				~Renderer();

				RenderTarget* initialize(uintptr_t windowHandle, int flags);
				void shutdown();
				RenderTarget* createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags);
				RenderTarget* createRenderTexture(int w, int h, SurfaceFormat fmt, int flags);
				void setRenderTarget(RenderTarget* rt);
				void beginFrame(bool color = true,
					unsigned int clearColor = 0,
					bool depth = true,
					float depthValue = 1,
					bool stencil = true,
					unsigned int stencilValue = 0);
				void beginScene(Camera* camera, Viewport* vp);
				void endScene();
				void endFrame();
				void apply(Material* material);
				void render(Submesh* submesh);

				IDirect3DDevice9* device();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDirect3D9* mD3D9;
				IDirect3DDevice9* mDevice;
				IDirect3DSurface9* mDepthStencil;
				IDirect3DSwapChain9* mSwapChain;
				D3DPRESENT_PARAMETERS mPParams;

				typedef std::vector<RenderTarget*> RenderTargets;
				RenderTargets mRenderTargets;
				RenderTarget* mCurrentRT;

				Camera* mCurrentCamera;
				Viewport* mCurrentVP;
			};
		}
	}
}

#endif // RENDERERD3D9_INCLUDED
