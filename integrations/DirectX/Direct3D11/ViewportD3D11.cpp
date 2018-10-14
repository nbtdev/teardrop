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

#include "ViewportD3D11.h"

namespace Teardrop {
namespace Gfx {
namespace Direct3D11 {

Viewport::Viewport(Gfx::RenderTarget* rt)
	: Gfx::Viewport(rt)
{
	mVP.MinDepth = 0.f;
	mVP.MaxDepth = 1.f;
}

Viewport::~Viewport()
{
}

D3D11_VIEWPORT& Viewport::viewport()
{
	updateDimensions();

	mVP.TopLeftX = mPos.x;
	mVP.TopLeftY = mPos.y;
	mVP.Width = mSize.x;
	mVP.Height = mSize.y;

	return mVP;
}

} // namespace Direct3D9
} // Gfx
} // Teardrop
