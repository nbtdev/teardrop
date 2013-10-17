/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CONNECTION_INCLUDED)
#define CONNECTION_INCLUDED

#include "Reflection/Reflection.h"
#include "Memory/Allocators.h"
#include "Util/_String.h"

namespace Teardrop
{
	namespace Gfx 
	{
		struct Attribute;
		class MaterialExpression;
		class Material;

		class Connection : public Reflection::Object 
		{
		public:
			TD_CLASS(Connection, Object);
			TD_POINTER_PROPERTY(FromExpression, "Source MaterialExpression", MaterialExpression, "Hidden");
			TD_PROPERTY(FromAttribute, "Source Attribute Name", String, "", "Hidden");
			TD_POINTER_PROPERTY(ToExpression, "Destination MaterialExpression", MaterialExpression, "Hidden");
			TD_PROPERTY(ToAttribute, "Destination Attribute Name", String, "", "Hidden");
			TD_POINTER_PROPERTY(Parent, "Parent Material", Material, "Hidden");

			Connection();
			~Connection();

			bool initialize();

			Attribute* input();
			Attribute* output();

			TD_DECLARE_ALLOCATOR();

		protected:
			Attribute* mInput;
			Attribute* mOutput;
		};
	}
}

#endif // CONNECTION_INCLUDED
