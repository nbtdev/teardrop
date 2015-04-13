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

#if !defined(SHADER_CONSTANT_H)
#define SHADER_CONSTANT_H

#include "Memory/Allocators.h"
#include "Gfx/VertexElement.h"

namespace Teardrop {
	namespace Gfx {

		class ShaderConstant
		{
		public:
			ShaderConstant(VertexElementType type, int width, int rows=1);
			~ShaderConstant();

			int width() const;
			int rows() const;
			int version() const;
			const void* data() const;

			void set(const void* data);

		protected:
			// typically VET_FLOAT
			VertexElementType mType;
			// 1, 2, 3, or 4
			int mWidth;
			// for arrays of constant entries
			int mRows;
			// incremented every time the constant value is changed
			int mVersion;
			// constant storage
			unsigned char* mStorage;
			// length (bytes) of storage
			int mLen;
		};
	}
}

#endif // SHADER_CONSTANT_H
