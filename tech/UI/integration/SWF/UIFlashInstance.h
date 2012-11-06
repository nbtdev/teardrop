/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIFLASHINSTANCE_INCLUDED)
#define UIFLASHINSTANCE_INCLUDED

#include "UIFlashInterfaces.h"
#include "Memory/Memory.h"
#include "Resource/ResourceHandle.h"

struct IOleObject;
struct IOleInPlaceObjectWindowless;

namespace Teardrop
{
	namespace UI
	{
		/*
		*/

		class FlashResource;
		class FlashSite;
		class FlashEvents;

		class FlashInstance
			: public IFlashEventListener,
			public IFlashHost
		{
			HResource m_hFlash;
			FlashResource* m_pFlash;
			FlashSite* m_pSite;
			IOleObject* m_pFlashPlayerControl;
			IOleInPlaceObjectWindowless* m_pSurface;
			ShockwaveFlashObjects::IShockwaveFlash* m_pFlashPlayer;
			FlashEvents* m_pFlashEvents;
			IFlashEventListener* m_pListener;
			HDC m_hDC;
			HDC m_hAltDC; // for alpha hacking
			HBITMAP m_hBitmap;
			HBITMAP m_hAltBitmap; // for alpha hacking
			void* m_pBits;
			void* m_pAltBits; // for alpha hack needs
			unsigned int m_width, m_height;
			size_t m_bNeedsUpdate;

		public:
			FlashInstance();
			~FlashInstance();

			bool initialize(
				const char* swfPath, IFlashEventListener* pListener = 0);
			void destroy();
			bool update(void* pDest, int pitch, float deltaT);

			void setSize(unsigned int w, unsigned int h);
			unsigned int getWidth() const;
			unsigned int getHeight() const;
			bool isDirty() const { return (m_bNeedsUpdate != 0); }

			// call a function in the SWF with variable number of args
			void call(const char* function, const _variant_t* args=0, int numArgs=0);
			
			//! IFlashEventListener implementation
			void onFSCommand(const char* cmd, const char* args);
			void onFlashCall(const char* cmd);

			//! IFlashHost implementation
			void onInvalidateRect(int x, int y, int w, int h);

			/*
				User input
			*/
			void injectMouseMove(int x, int y);
			void injectMouseDown(int button, int x, int y);
			void injectMouseUp(int button, int x, int y);
			void injectKeyDown(int keyCode, int keyChar);
			void injectKeyUp(int keyCode, int keyChar);
			void injectKeyEvent(size_t msg, size_t wParam, size_t lParam);

			TD_DECLARE_ALLOCATOR();

		private:
			bool _init();
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHINSTANCE_INCLUDED
