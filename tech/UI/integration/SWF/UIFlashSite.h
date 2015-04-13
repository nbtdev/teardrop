/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(UIFLASHSITE_INCLUDED)
#define UIFLASHSITE_INCLUDED

#include "Memory/Memory.h"

namespace Teardrop
{
	namespace UI
	{
		/*
			Offscreen OLE/OCX site for rendering the Flash SWF 
			in preparation for blit to texture
		*/

		class IFlashHost;

		class FlashSite 
			: 
			public IOleInPlaceSiteWindowless, 
			public IOleClientSite
		{
			IFlashHost* m_pHost;
			int	m_refCount;

		public:
			FlashSite(IFlashHost* pHost);
			~FlashSite();

			STDMETHOD(QueryInterface(REFIID riid, LPVOID* ppv));
			STDMETHOD_(ULONG, AddRef());
			STDMETHOD_(ULONG, Release());
			STDMETHOD(SaveObject(void));
			STDMETHOD(GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk));
			STDMETHOD(GetContainer(IOleContainer ** theContainerP));
			STDMETHOD(ShowObject(void));
			STDMETHOD(OnShowWindow(BOOL));
			STDMETHOD(RequestNewObjectLayout(void));
			STDMETHOD(ContextSensitiveHelp(BOOL fEnterMode));
			STDMETHOD(GetWindow(HWND __RPC_FAR* theWnndow));
			STDMETHOD(CanInPlaceActivate(void));
			STDMETHOD(OnInPlaceActivate(void));
			STDMETHOD(OnUIActivate(void));
			STDMETHOD(GetWindowContext(IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame, IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc, 
				LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo));
			STDMETHOD(Scroll(SIZE scrollExtant));
			STDMETHOD(OnUIDeactivate(BOOL fUndoable));
			STDMETHOD(OnInPlaceDeactivate(void));
			STDMETHOD(DiscardUndoState(void));
			STDMETHOD(DeactivateAndUndo(void));
			STDMETHOD(OnPosRectChange(LPCRECT lprcPosRect));
			STDMETHOD(OnInPlaceActivateEx(BOOL __RPC_FAR *pfNoRedraw, DWORD dwFlags));
			STDMETHOD(OnInPlaceDeactivateEx(BOOL fNoRedraw));
			STDMETHOD(RequestUIActivate(void));
			STDMETHOD(CanWindowlessActivate(void));
			STDMETHOD(GetCapture(void));
			STDMETHOD(SetCapture(BOOL fCapture));
			STDMETHOD(GetFocus(void));
			STDMETHOD(SetFocus(BOOL fFocus));
			STDMETHOD(GetDC(LPCRECT pRect, DWORD grfFlags, HDC __RPC_FAR *phDC));
			STDMETHOD(ReleaseDC(HDC hDC));
			STDMETHOD(InvalidateRect(LPCRECT pRect, BOOL fErase));
			STDMETHOD(InvalidateRgn(HRGN hRGN, BOOL fErase));
			STDMETHOD(ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip));
			STDMETHOD(AdjustRect(LPRECT prc));
			STDMETHOD(OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT __RPC_FAR *plResult));

			TD_DECLARE_ALLOCATOR();

		private:
			FlashSite(const FlashSite&); // not implemented
			FlashSite& operator=(const FlashSite&); // not implemented
		};
	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHSITE_INCLUDED
