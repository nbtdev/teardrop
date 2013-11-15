/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/


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

Renderer* RendererRegistration::create() const
{
	if (mCreateFn)
		return mCreateFn();

	return 0;
}

} // namespace Gfx
} // namespace Teardrop
