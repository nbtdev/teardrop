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

#if !defined(UIFLASHELEMENT_INCLUDED)
#define UIFLASHELEMENT_INCLUDED

#include "UIElement.h"
#include "UIFlashInterfaces.h"
//#include "Resource/ResourceHandle.h"

namespace Teardrop
{
	class GfxMaterial;

	namespace UI
	{
		/*
			Implements a Flash (SWF) UI element; this element lives
			in a normal on-screen quad, but its data comes from a Flash
			SWF that is updated and rendered off-screen and copied
			to a texture resource.
		*/

		class FlashInstance;

		class FlashElement 
			: public Element,
			public IFlashEventListener
		{
			GfxMaterial* m_pMtl;
			FlashInstance* m_pFlash;
			void* m_pData;

		public:
			FlashElement();
			~FlashElement();

			bool initialize(
				const char* resourceName,
//				HResource hTexture,
				const ElementParams& params);

			void destroy();

			bool render(Renderer* pRenderer);
			bool update(float deltaT);

			// invoke a method in the swf
			void call(const char* function, const _variant_t* argv=0, int argc=0);

			//! IFlashEventListener implementation
			void onFSCommand(const char* cmd, const char* args);
			void onFlashCall(const char* cmd);

			/*
				User input
			*/
			bool injectMouseMove(int x, int y);
			bool injectMouseDown(int button, int x, int y);
			bool injectMouseUp(int button, int x, int y);
			bool injectKeyDown(int keyCode, int keyChar);
			bool injectKeyUp(int keyCode, int keyChar);
			bool injectKeyEvent(size_t msg, size_t wParam, size_t lParam);

			TD_DECLARE_ALLOCATOR();

		private:
			FlashElement(const FlashElement&); // not implemented
			FlashElement& operator=(const FlashElement&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHELEMENT_INCLUDED
