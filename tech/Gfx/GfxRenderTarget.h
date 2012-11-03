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

#if !defined(GFXRENDERTARGET_INCLUDED)
#define GFXRENDERTARGET_INCLUDED

#include <map>

namespace CoS
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
