/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(SHADER_CONSTANT_TABLE_H)
#define SHADER_CONSTANT_TABLE_H

#include "Memory/Allocators.h"
#include "Gfx/VertexElement.h"
#include "Util/_String.h"
#include <map>

namespace Teardrop {
	namespace Gfx {

		class ShaderConstant;

		class ShaderConstantTable
		{
		public:
			struct Listener {
				virtual ~Listener();
				virtual void onConstantAdded(const String& name, ShaderConstant* constant) = 0;
			};

			ShaderConstantTable(Listener* listener = 0);
			~ShaderConstantTable();

			ShaderConstant* addNew(const String& name, VertexElementType type, int width, int rows=1);
			ShaderConstant* find(const String& name);

		protected:
			typedef std::map<String, ShaderConstant*> ShaderConstants;
			ShaderConstants mConstants;

			Listener* mListener;
		};
	}
}

#endif // SHADER_CONSTANT_TABLE_H
