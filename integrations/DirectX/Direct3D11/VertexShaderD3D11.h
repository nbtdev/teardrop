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

#if !defined(VERTEXSHADERD3D11_INCLUDED)
#define VERTEXSHADERD3D11_INCLUDED

#include "stdafx.h"
#include "Gfx/VertexShader.h"
#include "Util/_String.h"
#include <vector>

namespace Teardrop
{
	namespace Gfx
	{
		class ShaderConstant;
		class ShaderConstantTable;

		namespace Direct3D11
		{
			class VertexShader : public Gfx::VertexShader
			{
			public:
				VertexShader(ComPtr<ID3D11Device> aDevice, ShaderConstantTable* constants, Submesh* submesh);
				~VertexShader();

				void apply();

				void* bytecode();
				int bytecodeLength();

				TD_DECLARE_ALLOCATOR();

			protected:
				ComPtr<ID3D11Device> mDevice;
				ComPtr<ID3DBlob> mBytecode;
				ComPtr<ID3D11VertexShader> mVS;
				ComPtr<ID3D11ClassLinkage> mLinkage;

				// the constants declared in the shader
				//LPD3DXCONSTANTTABLE mConstantTable;
				// the constants managed by the engine
				ShaderConstantTable* mConstants;

				struct ConstantBinding {
					int mCurrentVersion;
					ShaderConstant* mConstant;
				};

				typedef std::vector<ConstantBinding> ConstantBindings;
				ConstantBindings mBindings;

				String mSource; // in case we need to recompile the shader
				String mFullpath; // in case someone wants to save the shader?
				String mErrs; // in case we need to recompile the shader
			};
		}
	}
}

#endif // VERTEXSHADERD3D11_INCLUDED
