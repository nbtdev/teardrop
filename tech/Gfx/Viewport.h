/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(VIEWPORT_INCLUDED)
#define VIEWPORT_INCLUDED

#include "Gfx/GfxCommon.h"
#include "Math/Vector2.h"
#include "Math/Matrix44.h"

namespace Teardrop
{
	namespace Gfx 
	{
		class RenderTarget;

		class Viewport
		{
		public:
			Viewport();
			virtual ~Viewport();

			virtual bool initialize(RenderTarget* rt);
			virtual bool destroy();

			virtual void updateDimensions();

			/*
				Accessors
			*/
			const Vector2& getPosition(bool normalized=false) const;
			const Vector2& getSize(bool normalized=false) const;
			const RenderTarget* getRenderTarget() const;
			bool isEnabled() const;
			bool getClearEachFrame() const;
			const Matrix44& getViewMatrix() const;
			const Matrix44& getProjectionMatrix() const;
			//const GfxMeshInstance* getVisibleMeshes() const;
			//size_t getNumVisibleMeshes() const;

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
			//void setVisibleMeshes(const GfxMeshInstance* pMeshes, size_t count);
			
			TD_DECLARE_ALLOCATOR();

		protected:
			Vector2 mPos;
			Vector2 mSize;
			Vector2 mPosNorm;
			Vector2 mSizeNorm;
			RenderTarget* mRT;
			Matrix44 mView;
			Matrix44 mProj;
			//GfxMeshInstance* m_pVisibleMeshes;
			//size_t m_numVisibleMeshes;
			bool mEnabled;
			bool mClearEachRender;
		};
	}
}

#endif // VIEWPORT_INCLUDED
