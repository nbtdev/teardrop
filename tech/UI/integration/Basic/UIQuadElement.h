/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIQUADELEMENT_INCLUDED)
#define UIQUADELEMENT_INCLUDED

#include "UIElement.h"
#include "UIDefs.h"
#include "Resource/ResourceHandle.h"

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
			HResource m_hQuad;
			GfxMaterial* m_pMtl;

		public:
			QuadElement();
			~QuadElement();

			bool initialize(HResource hTexture, const ElementParams& params);
			void destroy();

			bool render(Renderer* pRenderer);
			HResource getMeshHandle();

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
