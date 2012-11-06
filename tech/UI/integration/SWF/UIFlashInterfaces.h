/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(UIFLASHINTERFACES_INCLUDED)
#define UIFLASHINTERFACES_INCLUDED

namespace Teardrop
{
	namespace UI
	{
		/*
		*/

		class IFlashEventListener
		{
		public:
			virtual void onFSCommand(const char* cmd, const char* args) = 0;
			virtual void onFlashCall(const char* cmd) = 0;
		};

		class IFlashHost
		{
		public:
			virtual void onInvalidateRect(int x, int y, int w, int h) = 0;
		};

	} // namespace UI
} // namespace Teardrop

#endif // UIFLASHINTERFACES_INCLUDED
