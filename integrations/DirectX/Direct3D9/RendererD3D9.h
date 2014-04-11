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
#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class ShaderConstant;

		namespace Direct3D9
		{
			class Renderer : public Gfx::Renderer
			{
			public:
				Renderer();
				~Renderer();

				Gfx::RenderTarget* initialize(uintptr_t windowHandle, int flags);
				void shutdown();
				Gfx::RenderTarget* createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags);
				Gfx::RenderTarget* createRenderTexture(int w, int h, SurfaceFormat fmt, int flags);
				void releaseRenderTarget(Gfx::RenderTarget* rt);
				void setRenderTarget(Gfx::RenderTarget* rt);
				void beginFrame(bool color = true,
					unsigned int clearColor = 0,
					bool depth = true,
					float depthValue = 1,
					bool stencil = true,
					unsigned int stencilValue = 0);
				void beginScene(Camera* camera, Gfx::Viewport* vp=0);
				void beginObject(const Matrix44& worldXf);
				void endObject();
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

				typedef std::vector<Gfx::RenderTarget*> RenderTargets;
				RenderTargets mRenderTargets;
				Gfx::RenderTarget* mCurrentRT;

				Camera* mCurrentCamera;
				Viewport* mCurrentVP;

				ShaderConstant* mWorldITXf;
				ShaderConstant* mWvpXf;
				ShaderConstant* mWorldXf;
				ShaderConstant* mWorldInv;
				ShaderConstant* mViewIXf;
				ShaderConstant* mViewProj;
				ShaderConstant* mBones;
			};
		}
	}
}

#endif // RENDERERD3D9_INCLUDED
