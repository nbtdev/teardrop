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

#if !defined(COSGFXRENDERCONFIG_INCLUDED)
#define COSGFXRENDERCONFIG_INCLUDED
#include "Gfx/GfxCommon.h"

namespace CoS
{
	class Logger;

	struct GfxRenderConfig
	{
		GfxRenderConfig()
		{
			hWnd = 0;
			pDevice = 0;
			pPresentParams = 0;
			pLogger = 0;
			width = 0;
			height = 0;
			fullscreen = false;
			vsync = false;
			bNVPerfHUDAdapter = false;
		}

		//! handle to the parent window for the render context
		void* hWnd;
		//! pointer to the actual device if we are using another
		void* pDevice;
		//! pointer to the device presentation params if we're leeching
		void* pPresentParams;
		//! horizontal resolution
		size_t width;
		//! vertical resolution
		size_t height;
		//! set to true to run in fullscreen mode
		bool fullscreen;
		//! set to true to run with vsync on
		bool vsync;
		//! set to true to run under the NVPerfHUD adapter
		bool bNVPerfHUDAdapter;
		//! logger to use for message logging
		Logger* pLogger;

		//-------------------------------------------------------------------
		//! convenience methods
		GfxRenderConfig(const GfxRenderConfig& other)
		{
			*this = other;
		}
		//-------------------------------------------------------------------
		GfxRenderConfig& operator=(const GfxRenderConfig& other)
		{
			hWnd = other.hWnd;
			pDevice = other.pDevice;
			pPresentParams = other.pPresentParams;
			vsync = other.vsync;
			fullscreen = other.fullscreen;
			width = other.width;
			height = other.height;
			pLogger = other.pLogger;
			bNVPerfHUDAdapter = other.bNVPerfHUDAdapter;
			return *this;
		}

		DECLARE_GFX_ALLOCATOR();
	};
}

#endif // COSGFXRENDERCONFIG_INCLUDED
