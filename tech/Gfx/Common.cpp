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


#include "Common.h"

namespace Teardrop {
namespace Gfx {

RendererRegistration::RendererRegistration()
	: mCreateFn(0)
	, mDisplayName(0)
	, mDescription(0)
	, mNext(0)
{
}

static RendererRegistration* sRegistrations = 0;

void registerRenderer(RendererRegistration* registration)
{
	// do our best to make sure the caller doesn't play games
	if (!registration)
		return;

	registration->mNext = 0;

	RendererRegistration** reg = &sRegistrations;

	while (*reg) {
		reg = &(*reg)->mNext;
	}

	*reg = registration;
}

const RendererRegistration* rendererRegistrations()
{
	return sRegistrations;
}

Renderer* RendererRegistration::create(int aFlags) const
{
	if (mCreateFn)
		return mCreateFn(aFlags);

	return 0;
}

} // namespace Gfx
} // namespace Teardrop
