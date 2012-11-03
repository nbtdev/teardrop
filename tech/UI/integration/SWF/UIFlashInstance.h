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

#if !defined(UIFLASHINSTANCE_INCLUDED)
#define UIFLASHINSTANCE_INCLUDED

#include "UIFlashInterfaces.h"
#include "Memory/Memory.h"
#include "Resource/ResourceHandle.h"

struct IOleObject;
struct IOleInPlaceObjectWindowless;

namespace CoS
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

			COS_DECLARE_ALLOCATOR();

		private:
			bool _init();
		};
	} // namespace UI
} // namespace CoS

#endif // UIFLASHINSTANCE_INCLUDED
