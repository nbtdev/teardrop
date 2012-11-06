/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERTARGET_INCLUDED)
#define GFXRENDERTARGET_INCLUDED

#include <map>

namespace Teardrop
{
	class GfxViewport;
	struct GfxRenderConfig;

	class GfxRenderTarget
	{
	public:
		const static unsigned __int64 IID;

		GfxRenderTarget();
		virtual ~GfxRenderTarget();

		virtual bool initialize(
			GfxRenderConfig& cfg,
			SurfaceFormat fmt = FMT_R8G8B8);
		virtual bool destroy();

		/*
			Accessors
		*/
		virtual size_t getWidth() const;
		virtual size_t getHeight() const;
		size_t getDepth() const;
		SurfaceFormat getFormat() const;
		bool isEnabled() const;

		typedef std::multimap<size_t, GfxViewport*> Viewports;
		const Viewports& getViewports() const;

		//! allow the render target to handle 'resized' situations
		virtual bool handlePendingResize();

		/*
			Mutators
		*/
		//! x, y, w, h in normalized coords; z ordered from back to front
		GfxViewport* addViewport(float x, float y, float w, float h, size_t zorder=0);
		void removeViewport(GfxViewport* pVP);
		void removeAllViewports();
		void setEnabled(bool enabled);

		// "QueryInterface" because there's not a better way to pull off the multiple
		// things that can be a GfxRenderTarget
		virtual bool queryInterface(unsigned __int64 IID, void** ppInterface);
		
		DECLARE_GFX_ALLOCATOR();

	protected:
		SurfaceFormat m_fmt;
		size_t m_width;
		size_t m_height;
		size_t m_depth;
		Viewports m_viewports;
		bool m_enabled;
		bool m_bResized; // flag that a resize happened
	};
}

#endif // GFXRENDERTARGET_INCLUDED
