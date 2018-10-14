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

#include "stdafx.h"
#include "UIFlashSite.h"
#include "UIFlashInterfaces.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
FlashSite::FlashSite(IFlashHost* pHost)
{
	m_refCount = 0;
	m_pHost = pHost;
}
//---------------------------------------------------------------------------
FlashSite::~FlashSite()
{
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::QueryInterface(REFIID riid, LPVOID* ppv)
{
	*ppv = 0;

	if(riid == IID_IUnknown)
	{
		*ppv = (IUnknown*)(IOleWindow*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IOleWindow)
	{
		*ppv = (IOleWindow*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IOleInPlaceSite)
	{
		*ppv = (IOleInPlaceSite*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IOleInPlaceSiteEx)
	{
		*ppv = (IOleInPlaceSiteEx*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IOleInPlaceSiteWindowless)
	{
		*ppv = (IOleInPlaceSiteWindowless*)this;
		AddRef();
		return S_OK;
	}
	else if (riid == IID_IOleClientSite)
	{
		*ppv = (IOleClientSite*)this;
		AddRef();
		return S_OK;
	}
	else
	{   
		return E_NOTIMPL;
	}
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) FlashSite::AddRef()
{  
	return ++m_refCount;
}
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) FlashSite::Release()
{  
	--m_refCount;
	ULONG refCount = m_refCount;

	if(!m_refCount)
		delete this;

	return refCount;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::SaveObject(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
{
	*ppmk = 0;
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetContainer(IOleContainer ** theContainerP)
{			
	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::ShowObject(void)
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnShowWindow(BOOL)
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::RequestNewObjectLayout(void)
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::ContextSensitiveHelp(BOOL fEnterMode)
{
    return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetWindow(HWND __RPC_FAR* theWnndow)
{
	return E_FAIL;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::CanInPlaceActivate(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnInPlaceActivate(void)
{		
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnUIActivate(void)
{		
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetWindowContext(
	IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame, 
	IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc, 
	LPRECT lprcPosRect, 
	LPRECT lprcClipRect, 
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
#if 0
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = owner->width;
	rect.bottom = owner->height;

	*lprcPosRect = rect;
	*lprcClipRect = rect;
	
	*ppFrame = 0;
	QueryInterface(__uuidof(IOleInPlaceFrame), (void**)ppFrame);		
	*ppDoc = 0;

	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = 0;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;
#endif
	assert("I am being called now");
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::Scroll(SIZE scrollExtant)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnUIDeactivate(BOOL fUndoable)
{		
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnInPlaceDeactivate(void)
{	
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::DiscardUndoState(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::DeactivateAndUndo(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnPosRectChange(LPCRECT lprcPosRect)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnInPlaceActivateEx(BOOL __RPC_FAR *pfNoRedraw, DWORD dwFlags)
{		
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnInPlaceDeactivateEx(BOOL fNoRedraw)
{		
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::RequestUIActivate(void)
{
	return S_FALSE;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::CanWindowlessActivate(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetCapture(void)
{
	return S_FALSE;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::SetCapture(BOOL fCapture)
{
	return S_FALSE;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetFocus(void)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::SetFocus(BOOL fFocus)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::GetDC(LPCRECT pRect, DWORD grfFlags, HDC __RPC_FAR *phDC)
{		
	return E_INVALIDARG;		
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::ReleaseDC(HDC hDC)
{
	return E_INVALIDARG;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::InvalidateRect(LPCRECT pRect, BOOL fErase)
{	
/*
	if(owner->isTotallyDirty)
		return S_OK;

	if(pRect)
	{
		if(owner->isClean)
		{
			owner->dirtyBounds.left = pRect->left;
			owner->dirtyBounds.right = pRect->right;
			owner->dirtyBounds.top = pRect->top;
			owner->dirtyBounds.bottom = pRect->bottom;
			owner->isClean = false;
		}
		else
		{
			owner->dirtyBounds.left = min(owner->dirtyBounds.left, pRect->left);
			owner->dirtyBounds.right = max(owner->dirtyBounds.right, pRect->right);
			owner->dirtyBounds.top = min(owner->dirtyBounds.top, pRect->top);
			owner->dirtyBounds.bottom = max(owner->dirtyBounds.bottom, pRect->bottom);
		}
	}

	if(owner->dirtyBounds.left < 0 || owner->dirtyBounds.top < 0 || 
		owner->dirtyBounds.right > owner->width || owner->dirtyBounds.bottom > owner->height || !pRect)
	{
		owner->invalidateTotally();
	}
*/
	if (pRect)
	{
		m_pHost->onInvalidateRect(pRect->left, pRect->top, 
			pRect->right - pRect->left, pRect->bottom - pRect->top);
	}

	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::InvalidateRgn(HRGN hRGN, BOOL fErase)
{	
//	owner->invalidateTotally();

	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
{
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::AdjustRect(LPRECT prc)
{
	if(!prc)
		return E_INVALIDARG;
	
	return S_OK;
}
//---------------------------------------------------------------------------
STDMETHODIMP FlashSite::OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT __RPC_FAR *plResult)
{
	return S_FALSE;
}
