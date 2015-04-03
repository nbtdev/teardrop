/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERERD3D11_INCLUDED)
#define RENDERERD3D11_INCLUDED

#include "Gfx/Renderer.h"
#include "Math/Matrix44.h"
#include "Memory/Allocators.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstant;

		namespace Direct3D11
		{
			class Renderer : public Gfx::Renderer
			{
			public:
				Renderer(int flags);
				~Renderer();

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
				void beginScene(Camera* camera, Gfx::Viewport* vp = 0);
				void beginObject(const Matrix44& worldXf);
				void endObject();
				void endScene();
				void endFrame();
				void apply(Material* material);
				void render(Submesh* submesh);

				ID3D11Device* device();
				ID3D11DeviceContext* context();
				IDXGIFactory* factory();

				TD_DECLARE_ALLOCATOR();

			protected:
				IDXGIAdapter* mAdapter = nullptr;
				IDXGIFactory* mFactory = nullptr;
				ID3D11Device* mDevice = nullptr;
				ID3D11DeviceContext* mDeviceContext = nullptr;

				typedef std::vector<Gfx::RenderTarget*> RenderTargets;
				RenderTargets mRenderTargets;
				Gfx::RenderTarget* mCurrentRT = nullptr;

				Camera* mCurrentCamera = nullptr;
				Viewport* mCurrentVP = nullptr;

				ShaderConstant* mWorldITXf = nullptr;
				ShaderConstant* mWvpXf = nullptr;
				ShaderConstant* mWorldXf = nullptr;
				ShaderConstant* mWorldInv = nullptr;
				ShaderConstant* mViewIXf = nullptr;
				ShaderConstant* mViewProj = nullptr;
				ShaderConstant* mBones = nullptr;
			};
		}
	}
}

#endif // RENDERERD3D11_INCLUDED
