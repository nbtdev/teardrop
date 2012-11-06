/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXSUBMESH_INCLUDED)
#define TDGFXSUBMESH_INCLUDED

#include "Gfx/GfxVertexData.h"
#include "Gfx/GfxVertexFormat.h"
#include "Serialization/SerialPointerArray.h"

namespace Teardrop
{
	class GfxVertexFormat;
	class GfxIndexData;
	class GfxMaterial;
	class Matrix44;
	class ResourceSerializer;

	// 64 bytes each
	class GfxSubMesh
	{
		DECLARE_SERIALIZABLE(GfxSubMesh);

	public:
		//! max number of vertex streams supported by a submesh
		const static size_t MAX_VERTEX_STREAMS = 6;

		DECLARE_GFX_ALLOCATOR();

	private:
		//! pointers to up to MAX_VERTEX_STREAMS
		SerialPointerArray<GfxVertexData> m_pVertexData; 
		//! pointers to bind pose matrices (for skinning, if submesh is animated)
		SerialPointerArray<Matrix44> m_pBindPoseData; 
		//! pointer to the single index stream for this submesh
		SerialPointer<GfxIndexData> m_pIndexData;
		//! pointer to the material used for this submesh
		SerialPointer<GfxMaterial> m_pMaterial;
		//! vertex element layout (38 bytes)
		GfxVertexFormat m_format;
		//! 32-bit hash code for this submesh (used for shader lookup)
		unsigned int m_hashCode;
		//! whether we are using our own vertex data or that of our parent mesh
		bool m_bUseSharedVertexData;
		//! tracks dynamic buffers
		unsigned char m_dynamicFlags;
		//! submesh primitive type
		unsigned char m_primType;
		//! padding 
		char m_unused[1];

	public:
		//! normal c'tor (cannot fail)
		GfxSubMesh();
		//! placement c'tor (cannot fail and does not initialize)
		GfxSubMesh(int);
		//! d'tor (cannot fail)
		~GfxSubMesh();

		//! initialize the submesh (anything that can fail)
		bool initialize();
		//! destroy the submesh
		bool destroy();
		//! release a static submesh's dynamic data
		bool release();
		//! create a new vertex stream in the next available slot; returns 0 if no more available
		GfxVertexData* createVertexData(
			size_t& /*out*/streamIndex,
			Environment& /*in*/env,
			size_t vertexSize=0, 
			size_t vertexCount=0, 
			GfxVertexData::CreationFlags flags = GfxVertexData::NONE,
			void* pData = 0);
		//! create a new index stream, replacing the existing one if it exists; indexSize is in bytes
		GfxIndexData* createIndexData(
			Environment& env,
			size_t indexSize, 
			size_t indexCount, 
			void* pData = 0);
		
		/** accessors
		*/
		//! get a const pointer to the vertex data
		const GfxVertexData* const getVertexData(size_t stream) const;
		//! get an editable pointer to the vertex data
		GfxVertexData* getVertexData(size_t stream);
		//! get an editable pointer to the vertex data
		size_t getNumVertexData();
		//! get a const pointer to the index data
		const GfxIndexData* const getIndexData() const;
		//! get an editable pointer to the index data
		GfxIndexData* getIndexData();
		//! whether this submesh uses dedicated or shared vertex data
		bool usesSharedVertexData() const;
		//! get this submesh's material definition
		GfxMaterial* getMaterial();
		//! get this SubMesh's vertex format
		GfxVertexFormat& getVertexFormat() { return m_format; }
		//! get this submesh's hash value
		unsigned int getHashCode() const { return m_hashCode; }
		//! access the bind pose array
		const SerialPointerArray<Matrix44>& getBindPoses() const;
		//! access the submesh primitive type
		PrimitiveType getPrimitiveType() const;

		/** mutators
		*/
		//! set the vertex data to use the provided instance
		bool setVertexData(const GfxVertexData* vertexData, size_t stream);
		//! set this SubMesh's vertex format
		bool setVertexFormat(Environment& env, const GfxVertexFormat& /*in*/vertexFormat);
		//! set the index data to use the provided instance
		bool setIndexData(const GfxIndexData* indexData);
		//! set whether or not to use shared vertex data 
		void setUseSharedVertexData(bool bUseSharedData);
		//! set this submesh's material definition; autoDelete=true takes ownership of the material
		void setMaterial(GfxMaterial* pMaterial, bool autoDelete=false);
		//! access the bind pose array
		SerialPointerArray<Matrix44>& getBindPoses();
		//! set this submesh's primitive type
		void setPrimitiveType(PrimitiveType type);

		/**
			Serialization
		*/
		//! package for storage
		bool serialize(ResourceSerializer& serializer);

	private:
		// recalculate hash code
		void recalcHashCode(); 

		//! NOT IMPLEMENTED
		GfxSubMesh(const GfxSubMesh& other);
		GfxSubMesh& operator=(const GfxSubMesh& other);
	};
}

#endif // TDGFXSUBMESH_INCLUDED
