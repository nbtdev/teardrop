/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
			virtual float aspect() = 0;
			virtual int width() = 0;
			virtual int height() = 0;
			virtual void resize(int w, int h) = 0;
			virtual void setCurrent() = 0;
			virtual void unsetCurrent() = 0;
			virtual void present() = 0;
			virtual Viewport* addViewport(float x=0, float y=0, float w=1, float h=1, unsigned int zOrder=0) = 0;
			virtual void releaseViewport(Viewport* vp) = 0;
		};
	}
}

#endif // RENDERTARGET_INCLUDED
