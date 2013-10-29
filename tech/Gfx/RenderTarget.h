/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RENDERTARGET_INCLUDED)
#define RENDERTARGET_INCLUDED

namespace Teardrop
{
	namespace Gfx 
	{
		class Viewport;

		class RenderTarget
		{
		public:
			RenderTarget();
			virtual ~RenderTarget();

			virtual void clear(
				bool color = true,
				unsigned int clearColor = 0,
				bool depth = true,
				float depthValue = 1,
				bool stencil = true,
				unsigned int stencilValue = 0) = 0;
			virtual int width() = 0;
			virtual int height() = 0;
			virtual void setCurrent() = 0;
			virtual void present() = 0;
			virtual Viewport* addViewport(float x=0, float y=0, float w=1, float h=1, unsigned int zOrder=0) = 0;
			virtual void releaseViewport(Viewport* vp) = 0;
		};
	}
}

#endif // RENDERTARGET_INCLUDED
