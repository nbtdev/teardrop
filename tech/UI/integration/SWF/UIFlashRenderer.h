/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIFLASHRENDERER_INCLUDED)
#define UIFLASHRENDERER_INCLUDED

#include "UIRenderer.h"

namespace Teardrop
{
	class GfxRenderer;

	namespace UI
	{
		/*
		*/

		class Element;

		class FlashRenderer : public Renderer
		{
		public:
			FlashRenderer();
			~FlashRenderer();

			//! overrides
			bool initialize(GfxRenderer* pRenderer);
			bool destroy();
			bool render(Element* pElement);

			TD_DECLARE_ALLOCATOR();
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHRENDERER_INCLUDED
