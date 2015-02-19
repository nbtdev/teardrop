/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXRENDERWINDOW_INCLUDED)
#define GFXRENDERWINDOW_INCLUDED

#include <cstdint>

namespace Teardrop
{
	class GfxRenderWindow
	{
	public:
        GfxRenderWindow(const GfxRenderWindow& other) = delete;
        GfxRenderWindow& operator=(const GfxRenderWindow& other) = delete;

        static const uint64_t IID;

		virtual void* getHwnd() const = 0;
		virtual void resizeWindow(int w=-1, int h=-1) = 0;

	protected:
		GfxRenderWindow();
	private:
	};
}

#endif // GFXRENDERWINDOW_INCLUDED
