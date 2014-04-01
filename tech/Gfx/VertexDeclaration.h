/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VERTEXDECLARATION_INCLUDED)
#define VERTEXDECLARATION_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		class VertexDeclaration
		{
		public:
			VertexDeclaration(Submesh* parent);
			virtual ~VertexDeclaration();

			virtual void rebuild() = 0;
			virtual int hash();

			TD_DECLARE_ALLOCATOR();

		protected:
			Submesh* mParent;
			int mHash;
		};
	}
}

#endif // VERTEXDECLARATION_INCLUDED
