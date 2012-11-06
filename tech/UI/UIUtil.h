/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIUTIL_INCLUDED)
#define UIUTIL_INCLUDED

class TiXmlElement;

namespace Teardrop
{
	class GfxRenderer;

	namespace UI
	{
		class Renderer;
		class Element;
		class LayoutTarget;
		struct ElementParams;

		class Util
		{
		public:
			/*
				create/destroy a concrete instance of the UI renderer
			*/
			static Renderer* createQuadRenderer(GfxRenderer*);
			static Renderer* createFlashRenderer(GfxRenderer*);

			/*
				Create a UI element using the supplied parameters
			*/
			static Element* createElement(const ElementParams& params);

			static void destroyUIRenderer(Renderer*);
			static void destroyElement(Element*);

			static bool layout(TiXmlElement& pRoot, LayoutTarget& target, const char* path);
		};

	} // namespace UI
} // namespace Teardrop

#endif // UIUTIL_INCLUDED
