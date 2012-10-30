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

#if !defined(GFXRENDERTARGETD3D9_INCLUDED)
#define GFXRENDERTARGETD3D9_INCLUDED

struct IDirect3DSurface9;

namespace CoS
{
	class GfxRenderTarget;

	class GfxRenderTargetD3D9
	{
	public:
		static const unsigned __int64 IID;

		virtual IDirect3DSurface9* getRenderSurface() = 0;
		virtual IDirect3DSurface9* getDepthStencil() = 0;
		virtual void onDeviceReset() = 0;
		virtual void onDeviceRestored() = 0;
		virtual bool present() = 0;

	protected:
		GfxRenderTargetD3D9();
	private:
		GfxRenderTargetD3D9(const GfxRenderTargetD3D9& other);
		GfxRenderTargetD3D9& operator=(const GfxRenderTargetD3D9& other);
	};
}

#endif // GFXRENDERTARGETD3D9_INCLUDED
