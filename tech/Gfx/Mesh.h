/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MESH_INCLUDED)
#define MESH_INCLUDED

#include <vector>

namespace Teardrop
{
	namespace Gfx 
	{
		class Submesh;

		class Mesh
		{
		public:
			Mesh();
			~Mesh();

			Submesh* createSubmesh();
			void createSubmeshes(int nSubmeshes);
			void removeSubmesh(Submesh* submesh);
			void removeSubmesh(int index);
			int submeshCount();
			Submesh* submesh(int index);

			TD_DECLARE_ALLOCATOR();

		protected:
			typedef std::vector<Submesh*> Submeshes;
			Submeshes mSubmeshes;
		};
	}
}

#endif // MESH_INCLUDED
