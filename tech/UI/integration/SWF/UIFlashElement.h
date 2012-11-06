/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIFLASHELEMENT_INCLUDED)
#define UIFLASHELEMENT_INCLUDED

#include "UIElement.h"
#include "UIFlashInterfaces.h"
#include "Resource/ResourceHandle.h"

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
				HResource hTexture, 
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
