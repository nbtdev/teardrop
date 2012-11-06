/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDGFXRENDERCONFIG_INCLUDED)
#define TDGFXRENDERCONFIG_INCLUDED
#include "Gfx/GfxCommon.h"

namespace Teardrop
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

#endif // TDGFXRENDERCONFIG_INCLUDED
