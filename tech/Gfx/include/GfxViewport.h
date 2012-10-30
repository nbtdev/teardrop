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

#if !defined(GFXVIEWPORT_INCLUDED)
#define GFXVIEWPORT_INCLUDED

#include "Gfx/include/GfxCommon.h"
#include "Math/include/Vector2.h"
#include "Math/include/Matrix44.h"

namespace CoS
{
	class GfxRenderTarget;
	class GfxMeshInstance;

	class GfxViewport
	{
	public:
		GfxViewport();
		virtual ~GfxViewport();

		virtual bool initialize(GfxRenderTarget* pRT);
		virtual bool destroy();

		virtual void updateDimensions();

		/*
			Accessors
		*/
		const Vector2& getPosition(bool normalized=false) const;
		const Vector2& getSize(bool normalized=false) const;
		const GfxRenderTarget* getRenderTarget() const;
		bool isEnabled() const;
		bool getClearEachFrame() const;
		const Matrix44& getViewMatrix() const;
		const Matrix44& getProjectionMatrix() const;
		const GfxMeshInstance* getVisibleMeshes() const;
		size_t getNumVisibleMeshes() const;

		/*
			Mutators
		*/
		void setPosition(const Vector2& pos, bool normalized);
		void setSize(const Vector2& size, bool normalized);
		void translate(const Vector2& pos, bool normalized);
		void enable(bool enabled);
		void setClearEachFrame(bool clear);
		void setViewMatrix(const Matrix44& view);
		void setProjectionMatrix(const Matrix44& proj);
		void setVisibleMeshes(const GfxMeshInstance* pMeshes, size_t count);
		
		DECLARE_GFX_ALLOCATOR();

	protected:
		Vector2 m_pos;
		Vector2 m_size;
		Vector2 m_posNorm;
		Vector2 m_sizeNorm;
		GfxRenderTarget* m_pRT;
		Matrix44 m_view;
		Matrix44 m_proj;
		GfxMeshInstance* m_pVisibleMeshes;
		size_t m_numVisibleMeshes;
		bool m_bEnabled;
		bool m_bClearEachRender;
	};
}

#endif // GFXVIEWPORT_INCLUDED
