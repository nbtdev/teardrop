/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXVIEWPORT_INCLUDED)
#define GFXVIEWPORT_INCLUDED

#include "Gfx/GfxCommon.h"
#include "Math/Vector2.h"
#include "Math/Matrix44.h"

namespace Teardrop
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
