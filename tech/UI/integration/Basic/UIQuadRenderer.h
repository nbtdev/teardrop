/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIQUADRENDERER_INCLUDED)
#define UIQUADRENDERER_INCLUDED

#include "UIRenderer.h"

namespace Teardrop
{
	class GfxRenderer;

	namespace UI
	{
		/*
		*/

		class Element;

		class QuadRenderer : public Renderer
		{
		public:
			QuadRenderer();
			~QuadRenderer();

			//! overrides
			bool initialize(GfxRenderer* pRenderer);
			bool destroy();
			bool render(Element* pElement);
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIQUADRENDERER_INCLUDED
