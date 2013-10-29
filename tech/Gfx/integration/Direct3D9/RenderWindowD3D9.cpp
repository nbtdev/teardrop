/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "RenderWindowD3D9.h"
#include <assert.h>

namespace Teardrop {
namespace Gfx {
namespace Direct3D9 {

RenderWindow::RenderWindow(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS& pparams, HWND hWnd)
	: RenderTarget(device)
	, mHwnd(hWnd)
	, mSwapChain(0)
{
	mPParams = pparams;

	RECT rect;
	GetClientRect(hWnd, &rect);

	mWidth = rect.right - rect.left;
	mHeight = rect.bottom - rect.top;
}

RenderWindow::~RenderWindow()
{
}

HWND RenderWindow::hWnd()
{
	return mHwnd;
}

D3DPRESENT_PARAMETERS& RenderWindow::presentParams()
{
	return mPParams;
}

void RenderWindow::present()
{
	assert(mDevice);
	if (mDevice) {
		if (mSwapChain) {
			mSwapChain->Present(0, 0, 0, 0, 0);
		}
		else {
			mDevice->Present(0, 0, 0, 0);
		}
	}
}

} // namespace Direct3D9
} // namespace Gfx
} // namespace Teardrop
