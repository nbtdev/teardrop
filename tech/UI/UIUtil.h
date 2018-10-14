/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
