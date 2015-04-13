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
