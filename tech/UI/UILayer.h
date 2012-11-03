/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(UILAYER_INCLUDED)
#define UILAYER_INCLUDED

#include "Memory/Memory.h"
#include <map>

namespace CoS
{
	namespace UI
	{
		/*
			Layer is a group of UI elements, all rendered at the same 
			z-order within a composition. Within a given z-order, UI 
			elements are rendered in a FIFO fashion. This means that 
			the first element added to a layer will be on the "bottom"
			of the layer as the layer is rendered.

			A layer is always "full-viewport"; that is, its normalized
			position in a Composer is (0,0) and its normalized size is
			(1,1). This is independent of the actual size of the render
			target used for the UI render -- the viewport on that render
			target is owned and operated by the Composer, and may not
			be full-render-target size.
		*/

		class Renderer;
		class Element;

		class Layer
		{
			typedef std::multimap<int, Element*> Elements;
			Elements m_elements;

		public:
			Layer();
			~Layer();

			enum ElementRenderOrder
			{
				ERO_BOTTOMTOTOP=0, // default
				ERO_TOPTOBOTTOM,
			};

			/*! add an element to the layer (inc's refcount on the element)
				The default z-order for the element is "on top of all others"
			*/
			void addElement(Element* pElement, int zOrder=-1);

			/*! remove an element from the layer (decs refcount on the element,
				which will destroy/delete the element if this is the last 
				reference to the object, so if you intend for the Element to
				exist past this call, you must make sure that other references
				exist to it).
			*/
			void removeElement(Element* pElement);

			//! remove and dec-ref all elements in the layer
			void clear();

			//! get the render ordering for elements in this layer
			ElementRenderOrder getRenderOrder() const;

			//! set the render ordering for elements in this layer
			void setRenderOrder(ElementRenderOrder order);

			//! update the layer
			bool update(float deltaT);
			//! render the layer  
			bool render(Renderer* pRenderer);
			//! absolute (pixel) dimensions
			void resize(float width, float height);

			/*
				User input
			*/
			bool injectMouseMove(int x, int y);
			bool injectMouseDown(int button, int x, int y);
			bool injectMouseUp(int button, int x, int y);
			bool injectKeyDown(int keyCode, int keyChar);
			bool injectKeyUp(int keyCode, int keyChar);
			bool injectKeyEvent(size_t msg, size_t wParam, size_t lParam);

			COS_DECLARE_ALLOCATOR();

		protected:
			ElementRenderOrder m_renderOrder;

		private:
			Layer(const Layer&); // not implemented
			Layer& operator=(const Layer&); // not implemented
		};
	} // namespace UI
} // namespace CoS

#endif // UILAYER_INCLUDED
