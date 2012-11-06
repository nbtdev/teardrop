/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OVERLAY_INCLUDED)
#define OVERLAY_INCLUDED

namespace Teardrop
{
	/*
		Overlay is where UI or other 2D renderables
		meet the viewport. An Overlay consists of one
		UI::Composer reference, and one GfxViewport
		reference. 
	*/

	class GfxViewport;
	namespace UI
	{
		class Composer;
	}

	class Overlay
	{
		GfxViewport* m_pViewport;
		UI::Composer* m_pComposer;

	public:
		Overlay();
		~Overlay();

	private:
		Overlay(const Overlay&); // not implemented
		Overlay& operator=(const Overlay&); // not implemented
	};
}

#endif // OVERLAY_INCLUDED
