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

#if !defined(RENDERERD3D11_INCLUDED)
#define RENDERERD3D11_INCLUDED

#include "stdafx.h"
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

				// aMatrixPalette must already be in 3x4 transposed form prior to calling this method
				void updateMatrixPalette(const Matrix44* aMatrixPalette, int nMatrices);

				ComPtr<ID3D11Device> device();
				ComPtr<ID3D11DeviceContext> context();
				ComPtr<IDXGIFactory> factory();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<IDXGIAdapter> mAdapter;
				ComPtr<IDXGIFactory> mFactory;
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3D11DeviceContext> mDeviceContext;
				ComPtr<ID3D11Buffer> mXformConstantBuffer;
				ComPtr<ID3D11Buffer> mMatrixPalette;

				typedef std::vector<std::shared_ptr<Gfx::RenderTarget>> RenderTargets;
				RenderTargets mRenderTargets;

				Camera* mCurrentCamera = nullptr;

				// if you change this struct you must also change the cbuffer decl in VertexShaderD3D11.cpp!
				struct XformConstants {
					Matrix44 mWorldITXf;
					Matrix44 mWvpXf;
					Matrix44 mWorldXf;
					Matrix44 mWorldInv;
					Matrix44 mViewIXf;
					Matrix44 mViewProj;
				};

				XformConstants mXformConstants;
			};
		}
	}
}

#endif // RENDERERD3D11_INCLUDED
