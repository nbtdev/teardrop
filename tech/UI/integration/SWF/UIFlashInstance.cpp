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
#include "UIFlashInstance.h"
#include "UIFlashResource.h"
#include "UIFlashSite.h"
#include "UIFlashEvents.h"
#include "Resource/ResourceManager.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Util/StringUtil.h"
#include "Util/_String.h"
#include <assert.h>

using namespace Teardrop;
using namespace UI;
//---------------------------------------------------------------------------
FlashInstance::FlashInstance()
{
	m_hFlash = INVALID_RESOURCE_HANDLE;
	m_pFlash = 0;
	m_pSite = 0;
	m_pFlashPlayerControl = 0;
	m_pFlashPlayer = 0;
	m_pSurface = 0;
	m_pFlashEvents = 0;
	m_width = m_height = 0;
	m_hDC = 0;
	m_hBitmap = 0;
	m_pBits = 0;
	m_pListener = 0;
}
//---------------------------------------------------------------------------
FlashInstance::~FlashInstance()
{
}
//---------------------------------------------------------------------------
void FlashInstance::setSize(unsigned int w, unsigned int h)
{
	m_width = w;
	m_height = h;
}
//---------------------------------------------------------------------------
bool FlashInstance::initialize(
	const char* swfPath, IFlashEventListener* pListener)
{
	//m_hFlash = Environment::get().pResourceMgr->acquire(
	//	FlashResource::RESOURCE_TYPE, swfPath);

	m_pListener = pListener;

	// create an OLE site for rendering this instance
	m_pSite = TD_NEW FlashSite(this);
	m_pSite->AddRef();

	// create the OLE Flash Player object using the COM infrastructure
	HRESULT hr = CoCreateInstance(
		ShockwaveFlashObjects::CLSID_ShockwaveFlash,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IOleObject,
		(void**)&m_pFlashPlayerControl);

	if (FAILED(hr))
	{
		// todo: fallback to local copy?
		return false;
	}

	IOleClientSite* pSite = 0;
	m_pSite->QueryInterface(__uuidof(IOleClientSite), (void**)&pSite);
	m_pFlashPlayerControl->SetClientSite(pSite);
	pSite->Release();

	// get a pointer to the player's interface
	m_pFlashPlayerControl->QueryInterface(
		__uuidof(ShockwaveFlashObjects::IShockwaveFlash), 
		(void**)&m_pFlashPlayer);

	m_pFlashPlayer->WMode = _bstr_t("transparent");
	m_pFlashPlayerControl->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, pSite, 0, 0, 0);

	m_pFlashPlayerControl->QueryInterface(
		__uuidof(IOleInPlaceObjectWindowless), 
		(void**)&m_pSurface);

	m_pFlashEvents = TD_NEW FlashEvents;
	m_pFlashEvents->AddRef();
	m_pFlashEvents->initialize(m_pFlashPlayer, this);

	char fullName[MAX_PATH];
	GetFullPathName(swfPath, MAX_PATH, fullName, 0);
	hr = m_pFlashPlayer->LoadMovie(0, fullName);

	IOleInPlaceObject* pObj=0;
	m_pFlashPlayerControl->QueryInterface(__uuidof(IOleInPlaceObject), (void**)&pObj);
	if (pObj)
	{
		RECT rect = {0, 0, m_width, m_height};
		pObj->SetObjectRects(&rect, &rect);
		pObj->Release();
	}

	// this always returns S_OK -- simple way to tell if the
	// movie actually loaded is to try to get a property; getWidth()
	// does just that and returns 0 on failure
	if (!getWidth())
	{
		String msg("Could not load movie from ");
		msg += swfPath;
		Environment::get().pLogger->logMessage(msg);
		return false;
	}

	// testing -- start it playing
	m_pFlashPlayer->Play();

	// set up the target rendering DC and bitmap
	HDC hDC = GetDC(0);
	BITMAPINFOHEADER bmi = {0};
	bmi.biSize = sizeof(bmi);
	bmi.biBitCount = 32;
	bmi.biCompression = BI_RGB;
	bmi.biPlanes = 1;
	bmi.biWidth = (LONG)getWidth();
	bmi.biHeight = (LONG)getHeight();

	// make our main buffer
	m_hDC = CreateCompatibleDC(hDC);
	m_hBitmap = CreateDIBSection(hDC, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, &m_pBits, 0, 0);
	SelectObject(m_hDC, m_hBitmap);
	ZeroMemory(m_pBits, bmi.biWidth * bmi.biHeight * (bmi.biBitCount >> 3));

	// and one for alpha hacking
	m_hAltDC = CreateCompatibleDC(hDC);
	m_hAltBitmap = CreateDIBSection(hDC, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, &m_pAltBits, 0, 0);
	SelectObject(m_hAltDC, m_hAltBitmap);

	ReleaseDC(0, hDC);

	return true;
}
//---------------------------------------------------------------------------
void FlashInstance::destroy()
{
	m_pFlashEvents->destroy();
	m_pFlashEvents->Release();
	m_pFlashEvents = 0;

	m_pSurface->Release();
	m_pSurface = 0;

	m_pFlashPlayer->Release();
	m_pFlashPlayer = 0;

	IOleClientSite* pSite;
	m_pFlashPlayerControl->GetClientSite(&pSite);
	m_pFlashPlayerControl->DoVerb(OLEIVERB_HIDE, 0, pSite, 0, 0, 0);
	pSite->Release();
	m_pFlashPlayerControl->Release();
	m_pFlashPlayerControl = 0;

	m_pSite->Release();
	m_pSite = 0;

	DeleteDC(m_hAltDC);
	DeleteObject(m_hAltBitmap);
	DeleteDC(m_hDC);
	DeleteObject(m_hBitmap);
}
//---------------------------------------------------------------------------
// assumption: pData points to a buffer that is the same width and height
// as the swf stage, and is 32-bit depth (RGBA)
bool FlashInstance::update(void* pData, int pitch, float deltaT)
{
	// deferred initialization -- flash asset data is not necessarily 
	// available right away
	//if (!m_pFlash)
	//{
	//	m_pFlash = (FlashResource*)Environment::get()
	//		.pResourceMgr->deref(m_hFlash);

	//	if (m_pFlash)
	//	{
	//		_init();
	//	}

	//	return true;
	//}

	if (!m_bNeedsUpdate)
		return false;

	// render the OCX into a memory DIB and then blit that to pData
	IViewObject* pView = 0;
	m_pFlashPlayer->QueryInterface(IID_IViewObject, (void**)&pView);

	RECT r = {0, 0, getWidth(), getHeight()};
	FillRect(m_hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
	FillRect(m_hAltDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

	//HRESULT hr = OleDraw(pView, DVASPECT_CONTENT, m_hDC, &r);
	RECTL rect = {0, 0, getWidth(), getHeight()};
	memset(m_pBits, 0, m_width * m_height * 4);
	memset(m_pAltBits, 255, m_width * m_height * 4);
	//HRESULT hr = pView->Draw(DVASPECT_CONTENT, 1, 0, 0, 0, m_hDC, &rect, 0, 0, 0);
	//hr = pView->Draw(DVASPECT_CONTENT, 1, 0, 0, 0, m_hAltDC, &rect, 0, 0, 0);
	HRESULT hr = pView->Draw(DVASPECT_TRANSPARENT, 1, 0, 0, 0, m_hDC, &rect, 0, 0, 0);
	assert(SUCCEEDED(hr) && "Cannot draw into DC");
	hr = pView->Draw(DVASPECT_TRANSPARENT, 1, 0, 0, 0, m_hAltDC, &rect, 0, 0, 0);

	/* 
		Artists notoriously don't pay attention to power-of-two texture
		sizing, and Flash artists are no different. DirectX, however, will
		create a power-of-two pitch when it makes the texture data storage, 
		even if it reports the texture size as the original non-power-of-two
		size. We need to take this into account during copying, which means
		doing it row-by-row.
	*/
	unsigned char* pDest = (unsigned char*)pData;			// the target texture
	unsigned char* pSrc = (unsigned char*)m_pBits;			// the render we just did 
	unsigned char* pAltSrc = (unsigned char*)m_pAltBits;	// the second render we just did 

	union T
	{
		unsigned int ui32;
		unsigned char c[4];
	};

	for (unsigned int j=0; j<m_height; ++j)
	{
		int srcOffset = j * m_width * 4;
		int destOffset = j * pitch;

		unsigned char* pMain = pSrc + srcOffset;
		unsigned char* pAlt = pAltSrc + srcOffset;
		unsigned char* pOut = pDest + destOffset;

		// copy the row the fast way
		memcpy(pDest+destOffset, pSrc+srcOffset, m_width * 4);

		// and then go over it and fix the alpha values
		for (size_t i=0; i<m_width*4; i+=4)
		{
			pOut[i+3] = 255 - (pAlt[i] - pMain[i]);
		}
	}
		//memcpy(pData, m_pBits, 
		//	(rect.right - rect.left) * (rect.bottom - rect.top) * 4);

	pView->Release();

	m_bNeedsUpdate = 0;
	return true;
}
//---------------------------------------------------------------------------
bool FlashInstance::_init()
{
	return true;
}
//---------------------------------------------------------------------------
void FlashInstance::onFSCommand(const char* cmd, const char* args)
{
	if (m_pListener)
		m_pListener->onFSCommand(cmd, args);
}
//---------------------------------------------------------------------------
void FlashInstance::onFlashCall(const char* cmd)
{
}
//---------------------------------------------------------------------------
void FlashInstance::onInvalidateRect(int x, int y, int w, int h)
{
	m_bNeedsUpdate = 1;
}
//---------------------------------------------------------------------------
// http://www.adobe.com/support/flash/publishexport/scriptingwithflash/scriptingwithflash_04.html#69558
unsigned int FlashInstance::getWidth() const
{
	return m_width;

	unsigned int rtn = 0;
	
	try
	{	
		_bstr_t s = m_pFlashPlayer->TGetProperty(_bstr_t("/"), 8);
		StringUtil::fromString(s, rtn);
	}
	catch(_com_error e)
	{
		const TCHAR* msg = e.ErrorMessage();
		if (msg)
			Environment::get().pLogger->logMessage(msg);
	}

	return  rtn;
}
//---------------------------------------------------------------------------
unsigned int FlashInstance::getHeight() const
{
	return m_height;

	unsigned int rtn = 0;
	
	try
	{	
		_bstr_t s = m_pFlashPlayer->TGetProperty(_bstr_t("/"), 9);
		StringUtil::fromString(s, rtn);
	}
	catch(_com_error e)
	{
		const TCHAR* msg = e.ErrorMessage();
		if (msg)
			Environment::get().pLogger->logMessage(msg);
	}

	return  rtn;
}
//---------------------------------------------------------------------------
void FlashInstance::injectMouseMove(int x, int y)
{
	LRESULT res;
	m_pSurface->OnWindowMessage(WM_MOUSEMOVE, 0, MAKELPARAM(x,y), &res);
}
//---------------------------------------------------------------------------
void FlashInstance::injectMouseDown(int button, int x, int y)
{
	LRESULT res;
	HRESULT hr = 
		m_pSurface->OnWindowMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(x,y), &res);
}
//---------------------------------------------------------------------------
void FlashInstance::injectMouseUp(int button, int x, int y)
{
	LRESULT res;
	m_pSurface->OnWindowMessage(WM_LBUTTONUP, 0, MAKELPARAM(x,y), &res);
}
//---------------------------------------------------------------------------
void FlashInstance::injectKeyDown(int keyCode, int keyChar)
{
	//LRESULT res;
	//m_pSurface->OnWindowMessage(WM_KEYDOWN, keyCode, 0, &res);
	//m_pSurface->OnWindowMessage(WM_CHAR, keyChar, 0, &res);
}
//---------------------------------------------------------------------------
void FlashInstance::injectKeyUp(int keyCode, int keyChar)
{
	//LRESULT res;
	//m_pSurface->OnWindowMessage(WM_KEYUP, keyCode, 0, &res);
}
//---------------------------------------------------------------------------
void FlashInstance::injectKeyEvent(size_t msg, size_t wParam, size_t lParam)
{
	LRESULT res;
	m_pSurface->OnWindowMessage(
		(UINT)msg, (WPARAM)wParam, (LPARAM)lParam, &res);
}
//---------------------------------------------------------------------------
void FlashInstance::call(const char* func, const _variant_t* args, int argc)
{
	_bstr_t call("<invoke name=\"");
	call += func;
	call += "\" returntype=\"xml\"><arguments>";

	for (int i=0; i<argc; ++i)
	{
		switch (args[i].vt)
		{
		case VT_BOOL:
			call += (args[i].boolVal == VARIANT_TRUE) ? "<true/>" : "<false/>";
			break;
		case VT_BSTR:
			call += "<string>";
			call += _bstr_t(args[i].bstrVal);
			call += "</string>";
			break;
		case VT_NULL:
			call += "<null/>";
			break;
		case VT_I8:
		case VT_I4:
		case VT_I1:
		case VT_I2:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_UI8:
		case VT_UINT:
		case VT_INT:
			call += "<number>";
			{
				char buf[16];
				StringUtil::toString(args[i].intVal, buf, 16);
				call += buf;
			}
			call += "</number>";
			break;
		default: // treat it as floating point
			call += "<number>";
			{
				char buf[16];
				StringUtil::toString(args[i].fltVal, buf, 16);
				call += buf;
			}
			call += "</number>";
			break;
		}
	}

	call += "</arguments></invoke>";

	// don't call the wrapper version because failure shouldn't crash us
	BSTR res;
	HRESULT hr = m_pFlashPlayer->raw_CallFunction(call, &res);
}
