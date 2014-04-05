/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADER_CONSTANT_H)
#define SHADER_CONSTANT_H

#include "Memory/Allocators.h"
#include "Gfx/VertexElement.h"

namespace Teardrop {
	namespace Gfx {

		class ShaderConstant
		{
		public:
			ShaderConstant();
			~ShaderConstant();

			const void* data() const;
			int version() const;

		protected:
			// typically VET_FLOAT
			VertexElementType mType;
			// 1, 2, 3, or 4
			int mWidth;
			// for arrays of constant entries
			int mRank;
			// incremented every time the constant value is changed
			int mVersion;
			// constant storage
			unsigned char* mStorage;
		};
	}
}

#endif // SHADER_CONSTANT_H
