/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIAL_INCLUDED)
#define MATERIAL_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"
#include <map>
#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class MaterialOutput;
		class MaterialExpression;
		class Shader;
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

			Shader* shader();
			void addConnection(Connection* conn);
			int connections(Connection** connections, int nConnections);

			void sortExpressions();
			int expressionCount();
			MaterialExpression** sortedExpressions();

			TD_DECLARE_ALLOCATOR();

		protected:
			// will hold pointer to platform/renderer-specific shader instance of this material
			Shader* mShader;

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
