/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXMESH_INCLUDED)
#define TDGFXMESH_INCLUDED

#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include <vector>

namespace Teardrop
{
	class GfxVertexData;
	class GfxVertexFormat;
	class GfxSubMesh;
	class ResourceSerializer;
	class Vector4;

    class GfxMesh
	{
		const static int MAX_SHARED_VERTEX_STREAMS = 4;
		const static int MAX_SUBMESHES = 27;
		const static int MAX_MESH_NAME_LENGTH = 23;

		GfxVertexFormat m_sharedVertexFormat;					
		unsigned int m_dynamicSubmeshFlags;						
		unsigned int m_dynamicSharedVertexFlags;				
		char m_name[MAX_MESH_NAME_LENGTH+1];					
        std::vector<GfxSubMesh*> mSubMeshes;
        std::vector<GfxVertexData*> mSharedVertexData;

	public:
		//! normal c'tor (cannot fail)
		GfxMesh();
		//! placement c'tor (cannot fail and does not initialize)
		GfxMesh(int i);
		//! d'tor (cannot fail)
		~GfxMesh();

        GfxMesh(const GfxMesh& other) = delete;
        GfxMesh& operator=(const GfxMesh& other) = delete;

		//! initialize the mesh (anything that can fail)
		bool initialize();
		//! delete the mesh
		bool destroy();
		//! just release a static mesh's dynamic data
		bool release();
		//! create a submesh in this mesh; returns 0 if all slots full
		GfxSubMesh* createSubMesh(bool bUseSharedVertexData=false);
		//! create a shared vertex stream; returns 0 if all slots full
		GfxVertexData* createSharedVertexData(
			size_t& /*out*/streamIndex,
			Environment& env,
			GfxVertexData::CreationFlags flags=GfxVertexData::NONE,
			size_t elemSize=0,
			size_t numElems=0,
			void* pData=0);
		//! destroy a submesh on this mesh, compacts submesh array
		bool destroySubMesh(GfxSubMesh* pSubMesh);
		//! destroy a submesh on this mesh (by index), compacts submesh array
		bool destroySubMesh(size_t index);
		//! reset the mesh data to initial state
		bool clear();
		
		/** accessors
		*/
		//! get the submesh at specified index (returns 0 if invalid index)
		GfxSubMesh* getSubMesh(size_t index) const;
		//! get the number of submeshes in this mesh
		size_t getNumSubMeshes() const;
		//! get the number of shared vertex data streams
		size_t getNumSharedVertexData() const;
		//! get a const pointer to the shared vertex data
		const GfxVertexData* const getSharedVertexData(size_t index) const;
		//! get an editable pointer to the shared vertex data
		GfxVertexData* getSharedVertexData(size_t index);
		//! get the mesh name
		const char* getName() const;
		//! get the shared vertex data format
		const GfxVertexFormat& getSharedVertexFormat() const;
		//! get this mesh's local-space AABB in corners form
		void getAABB(Vector4& minCorner, Vector4& maxCorner) const;

		/** mutators
		*/
		//! set the mesh name
		void setName(const char* name);
		void setSharedVertexFormat(const GfxVertexFormat& fmt);

		DECLARE_GFX_ALLOCATOR();

	private:
	};
}

#endif // TDGFXMESH_INCLUDED
