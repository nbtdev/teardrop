/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UILAYOUTTARGET_INCLUDED)
#define UILAYOUTTARGET_INCLUDED

#include "UI/UIDefs.h"

namespace Teardrop
{
	class String;

	namespace UI
	{
		/*
		*/

		class LayoutTarget
		{
		public:
			//! callback when a new layer is encountered
			virtual void addLayer(int zOrder=-1) = 0;

			//! callback when a new element is encountered
			virtual void addElement(const ElementParams& params) = 0;
		};
	} // namespace UI
} // namespace Teardrop

#endif // UILAYOUTTARGET_INCLUDED
