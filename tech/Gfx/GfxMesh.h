/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(COSGFXMESH_INCLUDED)
#define COSGFXMESH_INCLUDED

#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Serialization/Serialization.h"
#include "Resource/Resource.h"

namespace CoS
{
	class GfxVertexData;
	class GfxVertexFormat;
	class GfxSubMesh;
	class ResourceSerializer;
	struct FourCC;
	class Vector4;

	class GfxMesh : public Resource
	{
		DECLARE_SERIALIZABLE(GfxMesh);
		DECLARE_SERIALIZABLE_VTABLE;

		const static int MAX_SHARED_VERTEX_STREAMS = 4;
		const static int MAX_SUBMESHES = 27;
		const static int MAX_MESH_NAME_LENGTH = 23;

		GfxVertexFormat m_sharedVertexFormat;					
		unsigned int m_dynamicSubmeshFlags;						
		unsigned int m_dynamicSharedVertexFlags;				
		char m_name[MAX_MESH_NAME_LENGTH+1];					
		SerialPointerArray<GfxSubMesh> m_pSubMeshes;			
		SerialPointerArray<GfxVertexData> m_pSharedVertexData;	

	public:
		const static FourCC& RESOURCE_TYPE;

		//! normal c'tor (cannot fail)
		GfxMesh();
		//! placement c'tor (cannot fail and does not initialize)
		GfxMesh(int i);
		//! d'tor (cannot fail)
		~GfxMesh();

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

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

		DECLARE_GFX_ALLOCATOR();

	private:
		//! NOT IMPLEMENTED
		GfxMesh(const GfxMesh& other);
		GfxMesh& operator=(const GfxMesh& other);
	};
}

#endif // COSGFXMESH_INCLUDED
