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

#if !defined(COSGFXRENDERER_INCLUDED)
#define COSGFXRENDERER_INCLUDED

#include "Gfx/include/GfxRenderConfig.h"
#include "Gfx/include/GfxCommon.h"

namespace CoS
{
	class GfxCommandBuffer;
	struct Environment;
	class GfxMeshInstance;
	class GfxMesh;
	class GfxSubMesh;
	class GfxVertexData;
	class GfxIndexData;
	class GfxTexture;
	class GfxMaterial;
	class GfxCamera;
	class GfxResourceFactory;
	class GfxMatrixPaletteCache;
	class GfxRenderWindow;
	class GfxRenderTarget;
	class Matrix44;
	class Vector4;
	struct GfxRenderStats;
	class GfxViewport;
	class GfxViewportData;
	class GfxShader;

	class GfxRenderer
	{
		size_t m_frameNumber;
		GfxResourceFactory* m_pResourceFactory;

	public:
		GfxRenderer(Environment& env);
		virtual ~GfxRenderer();

		enum RenderTargetType
		{
			RT_WINDOW,	// render window
			RT_TEXTURE,	// render texture
		};

		enum RenderMode
		{
			RENDER_DEFAULT,
			RENDER_SHADOW_CASTERS,
		};

		//! create renderer instance
		static GfxRenderer* allocate(Environment& env, Allocator* pAlloc=0);
		//! destroy renderer instance
		static void deallocate(GfxRenderer*);

		//! initialize the renderer
		virtual bool initialize(const GfxRenderConfig& cfg, bool createMainRenderWindow=true);
		//! shutdown the renderer
		virtual bool destroy();
		//! create a render target on the renderer
		virtual GfxRenderTarget* createRenderTarget(GfxRenderConfig& cfg, SurfaceFormat fmt, RenderTargetType type);
		//! destroy a previously-created render target
		virtual void destroyRenderTarget(GfxRenderTarget* pRT);
		// enqueue a mesh instance for deferred rendering
		virtual void queueForRendering(const GfxMeshInstance& rInst);
		//! set up the camera for this frame
		//! render current frame and wait until done
		bool swap(GfxRenderStats& stats);
		//! get main window
		GfxRenderTarget* getMainWindow() const;
		//! set render mode
		virtual void setRenderMode(RenderMode mode);

		//! begin frame (clears buffers, etc)
		virtual void beginFrame(
			bool bClearColor = true,			// whether or not to clear the color buffer (default is yes)
			unsigned int clearColor = 0,		// the clear color to use when clearing the color buffer (default is black)
			bool bClearDepth = true,			// whether or not to clear the depth buffer (default is yes)
			float depthValue = 1,				// the depth value to use when clearing the depth buffer
			bool bClearStencil = true,			// whether or not to clear the stencil buffer (default is yes)
			unsigned int stencilValue = 0		// the stencil value to use when clearing the stencil buffer
			) = 0;
		//! end frame (presents buffers, etc)
		virtual void endFrame() = 0;
		//! initialize render queue 
		virtual bool beginScene(GfxCamera* pCam, GfxViewport* pVP) = 0;
		//! flush the render queue to the device (implemented by derived classes)
		virtual bool endScene(bool renderOpaque = true, bool renderTransparent = true) = 0;
		//! create vertex data (implementation-dependent) -- caller owns the resource
		virtual GfxVertexData* createVertexData() = 0;
		//! create index data (implementation-dependent) -- caller owns the resource
		virtual GfxIndexData* createIndexData() = 0;
		//! create texture (implementation-dependent) -- caller owns the resource
		virtual GfxTexture* createTexture() = 0;
		//! implemented by derived classes
		virtual bool setRenderTarget(GfxRenderTarget* pRT) = 0;
		//! implemented by derived classes
		virtual void clearRenderTarget(
			bool bColor = true, unsigned int clearColor = 0,
			bool bDepth = true, float depthValue = 1,
			bool bStencil = true, unsigned int stencilValue = 0
			) = 0;
		//! implemented by derived classes
		virtual bool setViewport(GfxViewport* pVP) = 0;
		//! implemented by derived classes
		virtual bool setCamera(GfxCamera* pCam) = 0;
		//! implemented by derived classes -- used to set identity view and proj
		virtual bool setScreenSpaceMatrices(GfxViewport* pVP) = 0;
		//! implemented by derived classes
		virtual bool updateAllRenderTargets(GfxRenderStats& stats) = 0;
		//! implemented by derived classes
		virtual bool writeVertexFloatRegister(float*, int startReg=0, int numReg=1) = 0;
		//! implemented by derived classes
		virtual bool writeFragmentFloatRegister(float*, int startReg=0, int numReg=1) = 0;
		//! set the ambient scene lighting (not sure we want this in this interface?)
		virtual void setAmbientLight(const Vector4& ambient) = 0;
		//! turn on or off writing to the color buffer; returns previous setting
		virtual bool setColorWrite(bool bWrite) = 0;
		// find a shader by a renderable's material and submesh
		virtual GfxShader* getShader(GfxSubMesh* pSubMesh, GfxMaterial* pMtl, const char* pFilename) = 0;

		DECLARE_GFX_ALLOCATOR();

	protected:
		Environment& m_env;
		GfxRenderConfig m_cfg;
		GfxRenderTarget* m_pMainWindow;
		bool m_createMainWindow;
		RenderMode m_renderMode;

		virtual bool _swapImpl(GfxRenderStats& stats);

	private:
		// NOT IMPLEMENTED
		GfxRenderer(const GfxRenderer& other);
		GfxRenderer& operator=(const GfxRenderer& other);

		bool update();
	};
}

#endif // COSGFXRENDERER_INCLUDED
