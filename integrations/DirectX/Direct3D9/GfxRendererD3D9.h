/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXRENDERERIMPLD3D9_INCLUDED)
#define TDGFXRENDERERIMPLD3D9_INCLUDED

#include <map>
#include <set>
#include "GfxRenderer.h"
#include "GfxRenderState.h"

namespace Teardrop
{
	class GfxMesh;
	class GfxSubMesh;
	struct Environment;
	class GfxMeshInstance;
	class GfxTexture;
	class GfxTextureD3D9;
	class GfxVertexDataD3D9;
	class GfxIndexDataD3D9;
	class GfxMaterial;
	class GfxShaderLib;
	class GfxShaderD3D9;
	class GfxRenderWindowD3D9;
	struct _RenderTargets;
	class GfxViewport;

	class GfxRendererD3D9 : public GfxRenderer
	{
		friend class GfxRenderWindowD3D9;

		IDirect3D9* pD3D9;
		IDirect3DDevice9* pDevice;
		D3DPRESENT_PARAMETERS paramsPresent;
		GfxShaderLib* pShaderLib;
		_RenderTargets* m_pRenderTargets;
		GfxRenderState state;

		struct VisibleObjects : public std::list<GfxMeshInstance>{};
		VisibleObjects solidObjects;
		VisibleObjects animatedObjects;
		struct DistanceSortedObjects : public std::map<float, GfxMeshInstance>{}; 
		DistanceSortedObjects transparentObjects;

		typedef std::set<GfxTextureD3D9*> TextureSet;
		TextureSet m_textures;
		typedef std::set<GfxVertexDataD3D9*> VertexDataSet;
		VertexDataSet m_vertexData;
		typedef std::set<GfxIndexDataD3D9*> IndexDataSet;
		IndexDataSet m_indexData;
		typedef std::set<GfxShaderD3D9*> ShaderSet;
		ShaderSet m_shaders;

		// utility objects (AABB mesh, etc)
		GfxMesh* m_pAABBMesh;

	public:
		GfxRendererD3D9(Environment& env);
		~GfxRendererD3D9();

		bool initialize(const GfxRenderConfig& cfg, bool createMainWindow=true);
		bool threadInit();
		bool destroy();
		GfxRenderTarget* createRenderTarget(
			GfxRenderConfig& cfg,
			SurfaceFormat fmt,
			RenderTargetType type);
		void destroyRenderTarget(GfxRenderTarget* pRT);

		//! begin frame (clears buffers, etc)
		void beginFrame(
			bool bClearColor = true,			// whether or not to clear the color buffer (default is yes)
			unsigned int clearColor = 0,		// the clear color to use when clearing the color buffer (default is black)
			bool bClearDepth = true,			// whether or not to clear the depth buffer (default is yes)
			float depthValue = 1,				// the depth value to use when clearing the depth buffer
			bool bClearStencil = true,			// whether or not to clear the stencil buffer (default is yes)
			unsigned int stencilValue = 0		// the stencil value to use when clearing the stencil buffer
			);
		//! end frame (presents buffers, etc)
		void endFrame();
		//! queue a mesh instance for rendering
		void queueForRendering(const GfxMeshInstance& rInst);
		//! update all active render targets (window, MRT, RTT, etc)
		bool updateAllRenderTargets(GfxRenderStats& stats);
		//! set the current render target to the indicated RT
		bool setRenderTarget(GfxRenderTarget* pRT);
		//! clear the current render target
		void clearRenderTarget(
			bool bColor = true, unsigned int clearColor = 0,
			bool bDepth = true, float depthValue = 1,
			bool bStencil = true, unsigned int stencilValue = 0
			);
		//! set the current viewport
		bool setViewport(GfxViewport* pVP);
		//! set the current camera
		bool setCamera(GfxCamera* pCam);
		//! used to set identity view and proj
		bool setScreenSpaceMatrices(GfxViewport* pVP);
		//! set up the viewport to use for a given render
		bool setupViewport(GfxViewport* pVP);
		//! render a mesh instance using current render state
		bool renderMeshInstance(const GfxMeshInstance& rInst, bool trans = false);
		//! sets the shader for rendering based on material 
		bool setupShader(GfxSubMesh& subMesh, GfxMaterial& mtl);
		//! implemented by derived classes
		bool writeVertexFloatRegister(float*, int startReg=0, int numReg=1);
		//! implemented by derived classes
		bool writeFragmentFloatRegister(float*, int startReg=0, int numReg=1);
		//! init queue for a new render
		bool beginScene(GfxCamera* pCam, GfxViewport* pVP);
		//! flush the render queue to the device
		bool endScene(bool renderOpaque = true, bool renderTransparent = true);
		//! set the ambient scene lighting (not sure we want this in this interface?)
		void setAmbientLight(const Vector4& ambient);
		//! turn the colow buffer writing on or off; returns previous setting
		bool setColorWrite(bool bWrite);
		// find a shader by a renderable's material and submesh
		GfxShader* getShader(GfxSubMesh* pSubMesh, GfxMaterial* pMtl, const char* pFilename);

		IDirect3DDevice9* getDevice() const { return pDevice; }
		const D3DPRESENT_PARAMETERS& getPresentParams() const { return paramsPresent; }

		void logD3DError(HRESULT hr);

		// methods declared by superclass
		bool _swapImpl(GfxRenderStats& stats);

		GfxVertexData* createVertexData();
		GfxIndexData* createIndexData();
		GfxTexture* createTexture();
		void releaseDefaultPoolResources();
		GfxRenderState* getState() { return &state; }

		// The following methods allow resources to populate the above three sets,
		// so that the renderer can centrally manage default-pool resources during
		// device-lost conditions

		//! add/remove (via track flag) texture to the tracked set
		void trackTexture(GfxTextureD3D9* pTex, bool track);
		void trackVertexData(GfxVertexDataD3D9* pTex, bool track);
		void trackIndexData(GfxIndexDataD3D9* pTex, bool track);
		void trackShader(GfxShaderD3D9* pShader, bool track);

		DECLARE_GFX_ALLOCATOR();

	private:
		GfxRenderTargetD3D9* m_pCurrentRT;

		void renderSolids();
		void renderTransparents();
		bool renderSubMesh(
			const GfxMeshInstance& rInst,
			GfxSubMesh* pSM, 
			size_t i=0, /* submesh index */
			bool trans=false /* transparent queue flag */);
		bool renderNormalGeometry(GfxSubMesh* pSubMesh);
		bool renderInstancedGeometry(const GfxMeshInstance& rInst, GfxSubMesh* pSubMesh);
	};
}

#endif // TDGFXRENDERERIMPLD3D9_INCLUDED
