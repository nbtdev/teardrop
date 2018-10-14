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

#if !defined(MATERIAL_INCLUDED)
#define MATERIAL_INCLUDED

#include "Gfx/MaterialOutput.h"
#include "Memory/Allocators.h"
#include "Reflection/Reflection.h"
#include <map>
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class MaterialExpression;
		class FragmentShader;
		class Connection;
		struct Attribute;

		class Material : public Reflection::Object
		{
		public:
			TD_CLASS(Material, Object);
			TD_CLASS_CREATABLE();
			TD_POINTER_PROPERTY(Output, "Material output expression", MaterialOutput, Hidden);

			Material();
			~Material();

			bool initialize();
			bool destroy();
			void apply();

			FragmentShader* shader();
			void addConnection(Connection* conn);
			int connections(Connection** connections, int nConnections);

			void sortExpressions();
			int expressionCount();
			int connectionCount();
			MaterialExpression** sortedExpressions();

			TD_DECLARE_ALLOCATOR();

		protected:
			// will hold pointer to platform/renderer-specific shader instance of this material
			FragmentShader* mShader;

			// map of connections from input to instance; this works because inputs support only
			// one connection
			typedef std::map<const Attribute*, Connection*> Connections;
			Connections mConnections;

			typedef std::vector<MaterialExpression*> Expressions;
			Expressions mSortedExpressions;
		};
	}
}

#endif // MATERIAL_INCLUDED
