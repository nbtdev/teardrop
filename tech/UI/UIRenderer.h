/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIRENDERER_INCLUDED)
#define UIRENDERER_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	class GfxRenderer;
	class GfxViewport;

	namespace UI
	{
		/*
			The UI Renderer is a convenience class used by Composers to render
			the layers and elements that they contain. The UI Renderer typically
			is responsible for altering the view and projection settings and
			rendering them into a graphics viewport.
		*/

		class Element;

		class Renderer
		{
		public:
			Renderer();
			~Renderer();

			virtual bool initialize(GfxRenderer* pRenderer);
			virtual bool destroy();

			virtual void setViewport(GfxViewport* pVP);
			virtual bool render(Element* pElement);

			TD_DECLARE_ALLOCATOR();

		protected:
			GfxViewport* m_pViewport;
			GfxRenderer* m_pRenderer;
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIRENDERER_INCLUDED
