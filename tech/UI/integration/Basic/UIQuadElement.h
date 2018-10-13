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

#if !defined(UIQUADELEMENT_INCLUDED)
#define UIQUADELEMENT_INCLUDED

#include "UIElement.h"
#include "UIDefs.h"
//#include "Resource/ResourceHandle.h"

namespace Teardrop
{
	class GfxMaterial;

	namespace UI
	{
		/*
			Implements a basic quad UI element, that describes a rectangular
			area in 2D "screen" space, and usually has a single texture
			associated with it. This functionality is provided via a GfxMesh
			and a GfxMaterial for the quad -- the mesh is a normal resource
			obtained the normal way, through the resource manager.
		*/

		class QuadElement : public Element
		{
//			HResource m_hQuad;
			GfxMaterial* m_pMtl;

		public:
			QuadElement();
			~QuadElement();

            bool initialize(/*HResource hTexture, */const ElementParams& params);
			void destroy();

			bool render(Renderer* pRenderer);
//			HResource getMeshHandle();

			// invoke a script method
			void call(const char* function, const _variant_t* argv=0, int argc=0);

			/*
				User input
			*/
			bool injectMouseMove(int x, int y);
			bool injectMouseDown(int button, int x, int y);
			bool injectMouseUp(int button, int x, int y);
			bool injectKeyDown(int keyCode, int keyChar);
			bool injectKeyUp(int keyCode, int keyChar);
			bool injectKeyEvent(size_t msg, size_t wParam, size_t lParam);

		private:
			QuadElement(const QuadElement&); // not implemented
			QuadElement& operator=(const QuadElement&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIQUADELEMENT_INCLUDED
