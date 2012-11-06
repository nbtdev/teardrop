/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "GfxRenderWindowD3D9.h"
#include "GfxRendererD3D9.h"
#include "GfxViewportD3D9.h"
#include "Util/Environment.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
static GfxRenderWindowD3D9* s_pWindow = 0; // todo: support more than one window
//---------------------------------------------------------------------------
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		s_pWindow = (GfxRenderWindowD3D9*)((CREATESTRUCT*)lParam)->lpCreateParams;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		if (s_pWindow)
			s_pWindow->resizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;

	//case WM_PAINT:
	//	ValidateRect(hWnd, NULL);
	//	break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}
//---------------------------------------------------------------------------
HWND GfxRenderWindowD3D9::createWindow(
	const char* /*name*/,
	int x,
	int y,
	int w, 
	int h,
	GfxRenderWindowD3D9* pWindow)
{
	// Register the window class.
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = 
	wc.lpszMenuName = "Katana";

	RegisterClass( &wc );

	// Create the application's window.
	HWND hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		wc.lpszClassName, 
		wc.lpszClassName, 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		x, y, w, h,
		NULL, NULL, 
		wc.hInstance, (void*)pWindow);

	// resize the window because the supplied size doesn't take into account
	// the window decorations
	RECT rect;
	GetClientRect(hWnd, &rect);
	w += (w - (rect.right - rect.left));
	h += (h - (rect.bottom - rect.top));
	SetWindowPos(hWnd, 0, CW_USEDEFAULT, CW_USEDEFAULT, w, h, SWP_NOMOVE);

	UpdateWindow(hWnd);
	
	return hWnd;
}
//---------------------------------------------------------------------------
GfxRenderWindowD3D9::GfxRenderWindowD3D9()
{
	m_pSurface = 0;
	m_pSwapChain = 0;
	m_pDepthStencil = 0;
	m_bResized = false;
}
//---------------------------------------------------------------------------
GfxRenderWindowD3D9::~GfxRenderWindowD3D9()
{
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::queryInterface(unsigned __int64 IID, void **ppI)
{
	if (IID == GfxRenderTarget::IID)
	{
		*ppI = static_cast<GfxRenderTarget*>(this);
		return true;
	}
	else if (IID == GfxRenderWindow::IID)
	{
		*ppI = static_cast<GfxRenderWindow*>(this);
		return true;
	}
	else if (IID == GfxRenderTargetD3D9::IID)
	{
		*ppI = static_cast<GfxRenderTargetD3D9*>(this);
		return true;
	}

	*ppI = 0;
	return false;
}
//---------------------------------------------------------------------------
void GfxRenderWindowD3D9::setAsMainWindow()
{
	s_pWindow = this;
}
//---------------------------------------------------------------------------
void* GfxRenderWindowD3D9::getHwnd() const
{
	return (void*)m_hWnd;
}
//---------------------------------------------------------------------------
size_t GfxRenderWindowD3D9::getWidth() const
{
	return m_width;
}
//---------------------------------------------------------------------------
size_t GfxRenderWindowD3D9::getHeight() const
{
	return m_height;
}
//---------------------------------------------------------------------------
void GfxRenderWindowD3D9::onDeviceReset()
{
}
//---------------------------------------------------------------------------
void GfxRenderWindowD3D9::onDeviceRestored()
{
}
//---------------------------------------------------------------------------
void GfxRenderWindowD3D9::resizeWindow(int w, int h)
{
	if (w == -1 && h == -1)
	{
		// reread window size from the handle
		RECT rect;
		GetClientRect((HWND)m_hWnd, &rect);
		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;
	}
	else if (w > 0 && h > 0)
	{
		// set from the params
		m_width = w;
		m_height = h;
	}

	// update attached viewports
	for (Viewports::iterator it = m_viewports.begin();
		it != m_viewports.end(); ++it)
	{
		GfxViewportD3D9* pVP = static_cast<GfxViewportD3D9*>(it->second);
		pVP->updateDimensions();
	}

	// notify that we want a reset of this window
	m_bResized = true;
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::initialize(
	GfxRenderConfig& cfg,
	SurfaceFormat fmt)
{
	if (!GfxRenderTarget::initialize(cfg, fmt))
	{
		return false;
	}

	if (!cfg.hWnd)
	{
		cfg.hWnd = createWindow("Katana", 
			CW_USEDEFAULT, CW_USEDEFAULT, 
			(int)cfg.width, (int)cfg.height,
			this);

		if (!cfg.hWnd)
		{
			return false;
		}
	}

	m_hWnd = (HWND)cfg.hWnd;
	GfxRendererD3D9* pRend = 
		static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);

	if (!pRend->pDevice)
	{
		createDevice(cfg, fmt);

		// hack -- there have been resize events by this point, and 
		// there is no need to reset a device we just created
		m_bResized = false;
	}

	if (!pRend->m_pMainWindow)
	{
		// then this is the primary device window, use the implicit render target
		pRend->getDevice()->GetRenderTarget(0, &m_pSurface);
		pRend->getDevice()->GetDepthStencilSurface(&m_pDepthStencil);
		m_pparams = pRend->getPresentParams();

		pRend->m_pMainWindow = this;
	}
	else
	{
		// any other window is a swap chain, and also gets its 
		// render target differently
		ZeroMemory(&m_pparams, sizeof(m_pparams));
		m_pparams.Windowed = TRUE;
		m_pparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_pparams.BackBufferFormat = D3DFMT_UNKNOWN;
		m_pparams.hDeviceWindow = (HWND)m_hWnd;
		m_pparams.BackBufferWidth = cfg.width;
		m_pparams.BackBufferHeight = cfg.height;
		m_pparams.BackBufferCount = 1;
		m_pparams.EnableAutoDepthStencil = TRUE;
		m_pparams.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_pparams.PresentationInterval = 
			cfg.vsync ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

		HRESULT hr = pRend->getDevice()->CreateAdditionalSwapChain(&m_pparams, &m_pSwapChain);
		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
			return false;
		}

		// get render target surface ref
		m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pSurface);

		// create depth buffer for additional swap chain
		hr = pRend->getDevice()->CreateDepthStencilSurface(
			m_pparams.BackBufferWidth,
			m_pparams.BackBufferHeight,
			m_pparams.AutoDepthStencilFormat,
			D3DMULTISAMPLE_NONE, // todo: support AA at some point
			0,
			FALSE,
			&m_pDepthStencil,
			NULL);

		if (FAILED(hr))
		{
			pRend->logD3DError(hr);
			return false;
		}

		return true;
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::destroy()
{
	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = 0;
	}

	if (m_pDepthStencil)
	{
		m_pDepthStencil->Release();
		m_pDepthStencil = 0;
	}

	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
IDirect3DSurface9* GfxRenderWindowD3D9::getRenderSurface()
{
	return m_pSurface;
}
//---------------------------------------------------------------------------
IDirect3DSurface9* GfxRenderWindowD3D9::getDepthStencil()
{
	return m_pDepthStencil;
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::present()
{
	HRESULT hr;
	GfxRendererD3D9* pRend = 
		static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);

	if (m_pSwapChain)
	{
		hr = m_pSwapChain->Present(0, 0, 0, 0, 0);
	}
	else
	{
		hr = pRend->getDevice()->Present(0, 0, 0, 0);
	}

	if (FAILED(hr))
		pRend->logD3DError(hr);

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::handlePendingResize()
{
	if (!m_bResized)
		return false;

	HRESULT hr;
	GfxRendererD3D9* pRend = 
		static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);

	m_pparams.BackBufferWidth = m_width;
	m_pparams.BackBufferHeight = m_height;

	pRend->releaseDefaultPoolResources();

	// also need to release the stencil and depth surfaces we have
	m_pSurface->Release();
	m_pDepthStencil->Release();
	m_pSurface = 0;
	m_pDepthStencil = 0;

	hr = pRend->getDevice()->Reset(&m_pparams);
	
	if (FAILED(hr))
		pRend->logD3DError(hr);

	m_bResized = false;

	hr = pRend->getDevice()->TestCooperativeLevel();
	while (hr != D3D_OK)
	{
		hr = pRend->getDevice()->TestCooperativeLevel();
		if (hr == D3DERR_DEVICENOTRESET)
		{
			hr = pRend->getDevice()->Reset(&m_pparams);
		}
	}

	// in case we had to release these surfaces, we want them back now
	if (!m_pSurface)
	{
		pRend->getDevice()->GetRenderTarget(0, &m_pSurface);
	}

	if (!m_pDepthStencil)
	{
		pRend->getDevice()->GetDepthStencilSurface(&m_pDepthStencil);
	}

	return true;
}
//---------------------------------------------------------------------------
bool GfxRenderWindowD3D9::createDevice(
	GfxRenderConfig& cfg, SurfaceFormat /*fmt*/)
{
	GfxRendererD3D9* pRend = 
		static_cast<GfxRendererD3D9*>(Environment::get().pRenderer);
	IDirect3DDevice9* pDevice = pRend->getDevice();

	if (!pDevice)
	{
		// create D3D9
		IDirect3D9* pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (!pD3D9)
		{
			return false;
		}

		if (!cfg.hWnd)
		{
			cfg.hWnd = createWindow(
				"RenderWindow",
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				(UINT)cfg.width,
				(UINT)cfg.height);
		}
		else
		{
			RECT rect;
			GetClientRect((HWND)cfg.hWnd, &rect);
			cfg.fullscreen = false;
			cfg.width = rect.right - rect.left;
			cfg.height = rect.bottom - rect.top;
		}

		// create a device on D3D9 object
		ZeroMemory(&m_pparams, sizeof(m_pparams));
		m_pparams.BackBufferCount = 1;
		m_pparams.Windowed = (cfg.fullscreen && !cfg.hWnd) ? FALSE : TRUE;
		m_pparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_pparams.BackBufferFormat = D3DFMT_UNKNOWN;
		m_pparams.hDeviceWindow = (HWND)cfg.hWnd;
		m_pparams.BackBufferWidth = (UINT)cfg.width;
		m_pparams.BackBufferHeight = (UINT)cfg.height;
		m_pparams.EnableAutoDepthStencil = TRUE;
		m_pparams.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_pparams.PresentationInterval = 
			cfg.vsync ? (D3DPRESENT_DONOTWAIT | D3DPRESENT_INTERVAL_ONE) : D3DPRESENT_INTERVAL_IMMEDIATE;

		// find the adapter to use 
		UINT adapterIndex = D3DADAPTER_DEFAULT;
		D3DDEVTYPE devType = D3DDEVTYPE_HAL;
		if (cfg.bNVPerfHUDAdapter)
		{
			for (UINT a=0; a<pD3D9->GetAdapterCount(); ++a)
			{
				D3DADAPTER_IDENTIFIER9 id;
				HRESULT hr = pD3D9->GetAdapterIdentifier(a, 0, &id);
				//LOG_DEBUG(id.Description)
				if (SUCCEEDED(hr) && strstr(id.Description, "PerfHUD") != 0)
				{
					//LOG_DEBUG("Using NVPerfHUD Adapter")
					adapterIndex = a;
					devType = D3DDEVTYPE_REF;
					break;
				}
			}
		}

		HRESULT hr = S_OK;
		if (FAILED(hr=pD3D9->CreateDevice(
			adapterIndex,
			devType,
			(HWND)cfg.hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_pparams,
			&pDevice)))
		{
			pRend->logD3DError(hr);
			return false;
		}

		pRend->pD3D9 = pD3D9;
		pRend->pDevice = pDevice;
		pRend->paramsPresent = m_pparams;
	}

	return true;
}