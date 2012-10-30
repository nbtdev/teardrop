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

#if !defined(COSGFXMESHINSTANCE_INCLUDED)
#define COSGFXMESHINSTANCE_INCLUDED

#include "Gfx/include/GfxCommon.h"
#include "Math/include/Transform.h"
#include "Math/include/AABB.h"
#include "FastDelegate.h"

namespace CoS
{
	class GfxMesh;
	class GfxMaterial;
	class GfxRenderer;
	class GfxLight;
	class Matrix44;
	class GfxShaderConstantTable;
	class GfxVertexFormat;
	class GfxVertexData;

	class GfxMeshInstance
	{
	public:
		GfxMeshInstance();
		~GfxMeshInstance();

		// fired right before an instance is rendered
		func::delegate1<GfxRenderer*> PreRender;
		// fired right after an instance is rendered
		func::delegate1<GfxRenderer*> PostRender;

		bool initialize();
		bool destroy();
		bool clear();

		/** Accessors
		*/
		HResource getMeshHandle() const { return m_meshHandle; }
		size_t getMaterialCount() const { return m_numMaterials; }
		GfxMaterial* getMaterialByIndex(size_t index) const;
		const Transform& getTransform() const { return m_transform; }
		float getDistToCam() const { return m_distToCamera; }
		const AABB& getAABB() const { return m_aabb; }
		bool showAABB() const { return (0 != m_bShowAABB); }

		/** Mutators
		*/
		bool setMeshHandle(HResource handle);
		bool setMaterialByIndex(size_t index, GfxMaterial* pMaterial);
		bool setTransform(const Transform& xform);
		void setDistToCam(float dist) { m_distToCamera = dist; }
		void createMaterialOverrides(size_t numOverrides);
		void setAABB(const AABB& aabb, bool show);

		/** operators
		*/
		GfxMeshInstance& operator=(const GfxMeshInstance& other);
		bool operator<(const GfxMeshInstance& other);

		// renderer calls this before rendering the instance
		void preRender(GfxRenderer*) const;
		// renderer calls this after rendering the instance
		void postRender(GfxRenderer*) const;

		/*
			List of lights that affect this instance; managed and set by 
			RenderComponent, etc; we just carry it around
		*/
		const GfxLight** m_pLights;
		size_t m_numLights;

		/*
			Used for skeletal animation, if present
		*/
		const Matrix44* m_pMatrixPalette;
		size_t m_numMatrices;

		/*
			Pointer to the source shader constants for this instance
		*/
		GfxShaderConstantTable* m_pShaderConstants;

		/*
			Shader override (applies to all submeshes in this mesh instance)
		*/
		const char* m_pShader;

		/*
			Does this instance cast shadows? (default is yes)
		*/
		size_t m_bCastShadows;

		/*
			Does this instance receive shadows? (default is yes)
		*/
		size_t m_bReceiveShadows;

		/*
			In order to support graphics objects such as billboards 
			and terrain data, that are not actually "resources" (in 
			that they are not loaded from pre-cooked asset files), as
			well as other objects that are created procedurally instead 
			of created by artists and exported to asset (resource) files,
			we need to provide a way to hang an actual GfxMesh* on a 
			GfxMeshInstance object. However, DO NOT ABUSE THIS FEATURE --
			all resources still need to be created and managed through 
			the ResourceManager. Note that when you step outside of the
			resource management system, you are responsible for the lifetime
			of ALL graphics objects you create/(materials, textures, etc).
		*/
		GfxMesh* m_pProceduralMesh; 

		/*
			In order to support instanced rendering, we need to allow the
			user to provide an instance count (of either the handle-referenced
			mesh or the procedural mesh), a custom GfxVertexFormat and an 
			array of instance data (GfxVertexData). 
		*/

		// (used with instanced rendering) - number of geometry instances
		size_t m_instanceCount;
		// (used with instanced rendering) - instance data stream
		GfxVertexData* m_instanceVertexData;

		DECLARE_GFX_ALLOCATOR();

	private:
		/**
			Handle to the mesh we use as base for this instance
		*/
		HResource m_meshHandle;

		/**
			Transform for this mesh (relative to its parent transform, if any)
		*/
		Transform m_transform;

		/**
			AABB for this mesh, if AABB rendering is on (determined by setting of 
			the aabb flag)
		*/
		AABB m_aabb;
		int m_bShowAABB;

		/**
			Pointers to material overrides. The ordinal of material instance
			in this list corresponds to the ordinal of the *SubMesh* in the 
			mesh referenced by the handle above. In this way, you can override,
			if you like, only a single material in a mesh with multiple submeshes,
			and allow the material definitions embedded in the mesh still
			to be used.
		*/
		GfxMaterial** m_pMaterialOverrides;
		size_t m_numMaterials;

		/*
			Used by alpha-sorting process, to avoid having to calculate
			the distance-to-camera more than once
		*/
		float m_distToCamera;
	};

	inline GfxMeshInstance& GfxMeshInstance::operator=(const GfxMeshInstance& other)
	{
		m_meshHandle = other.m_meshHandle;
		m_transform = other.m_transform;
		m_pMatrixPalette = other.m_pMatrixPalette;
		m_numMatrices = other.m_numMatrices;
		m_distToCamera = other.m_distToCamera;

		m_numMaterials = other.m_numMaterials;
		m_pMaterialOverrides = other.m_pMaterialOverrides;

		m_pLights = other.m_pLights;
		m_pShaderConstants = other.m_pShaderConstants;

		m_pShader = other.m_pShader;

		m_bCastShadows = other.m_bCastShadows;
		m_bReceiveShadows = other.m_bReceiveShadows;

		m_pProceduralMesh = other.m_pProceduralMesh;
		m_instanceCount = other.m_instanceCount;
		m_instanceVertexData = other.m_instanceVertexData;

		PreRender = other.PreRender;
		PostRender = other.PostRender;

		return *this;
	}

	inline bool GfxMeshInstance::operator<(const GfxMeshInstance& other)
	{
		return m_distToCamera <= other.m_distToCamera;
	}
}

#endif // COSGFXMESHINSTANCE_INCLUDED
