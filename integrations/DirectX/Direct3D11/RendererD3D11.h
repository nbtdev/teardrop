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

				std::shared_ptr<Gfx::RenderTarget> createRenderWindow(uintptr_t hWnd, SurfaceFormat fmt, int flags);
				std::shared_ptr<Gfx::RenderTarget> createRenderTexture(int w, int h, SurfaceFormat fmt, int flags);
				void beginFrame();
				void beginScene(Camera* camera, Gfx::Viewport* vp = 0);
				void beginObject(const Matrix44& worldXf);
				void endObject();
				void endScene();
				void endFrame();
				void apply(Material* material);
				void render(Submesh* submesh);

				ComPtr<ID3D11Device> device();
				ComPtr<ID3D11DeviceContext> context();
				ComPtr<IDXGIFactory> factory();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<IDXGIAdapter> mAdapter = nullptr;
				ComPtr<IDXGIFactory> mFactory = nullptr;
				ComPtr<ID3D11Device> mDevice = nullptr;
				ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;

				typedef std::vector<std::shared_ptr<Gfx::RenderTarget>> RenderTargets;
				RenderTargets mRenderTargets;

				Camera* mCurrentCamera = nullptr;

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
