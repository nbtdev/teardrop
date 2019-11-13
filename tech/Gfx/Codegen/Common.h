/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(TEARDROP_GFX_CODEGEN_COMMON_INCLUDED)
#define TEARDROP_GFX_CODEGEN_COMMON_INCLUDED

#include "Gfx/Codegen/Expression.h"

namespace Teardrop
{
	namespace Gfx
	{
		namespace Codegen
		{
			enum DataType {
				TYPE_VOID,
				TYPE_FLOAT,
				TYPE_FLOAT2,
				TYPE_FLOAT3,
				TYPE_FLOAT4,
				TYPE_SAMPLER,
				TYPE_UNKNOWN,
			};

			enum BinaryOperation {
				BINARY_OP_ASSIGN,
                BINARY_OP_ADD,
                BINARY_OP_SUB,
                BINARY_OP_MUL,
                BINARY_OP_DIV,
            };

			enum Direction {
				DIRECTION_IN,
				DIRECTION_OUT,
				DIRECTION_INOUT
			};

		} // namespace Codegen
	} // namespace Gfx
} // namespace Teardrop

#endif // TEARDROP_GFX_CODEGEN_COMMON_INCLUDED