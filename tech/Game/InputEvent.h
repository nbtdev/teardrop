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

#if !defined(INPUTEVENT_INCLUDED)
#define INPUTEVENT_INCLUDED

#include "Memory/Allocators.h"

namespace CoS
{
	union InputEvent
	{
		typedef enum
		{
			NONE = 0xFF, // to assure 1-byte size
			MOUSE,
			KEYBOARD,
			STICK,
			RAW_WIN32,
		} Type;

		struct TypeInfo
		{
			char pad[15];
			Type type;
		} evtType;

		struct Key
		{
			int keyCode;
			int keyChar;
			bool bKeyDown;
			bool lCtrlDown;
			bool lAltDown;
			bool lShiftDown;
			bool rCtrlDown;
			bool rAltDown;
			bool rShiftDown;
			char pad[1];
		} key;

		struct Mouse
		{
			short absX;
			short absY;
			short absZ;
			short relZ;
			char relX;
			char relY;
			unsigned char btnsPressed;
			unsigned char btnsReleased;
			unsigned char btnsDown;
			unsigned char index;		 // support for N mice (default is 1)
			bool isInside;				// did the event happen inside the associated window's bounds?
			char pad[1];
		} mouse;

		enum MouseButton
		{
			BUTTON_NONE		= 0x00,
			BUTTON_LEFT		= 0x01,
			BUTTON_MIDDLE	= 0x04,
			BUTTON_RIGHT	= 0x02,
			BUTTON_4		= 0x08,
			BUTTON_5		= 0x10,
			BUTTON_6		= 0x20,
			BUTTON_7		= 0x40,
			BUTTON_8		= 0x80,
		};

		enum StickButton
		{
			JS_BUTTON_1  	= 0x0001,
			JS_BUTTON_2		= 0x0002,
			JS_BUTTON_3		= 0x0004,
			JS_BUTTON_4		= 0x0008,
			JS_BUTTON_5		= 0x0010,
			JS_BUTTON_6		= 0x0020,
			JS_BUTTON_7		= 0x0040,
			JS_BUTTON_8		= 0x0080,
			JS_BUTTON_9  	= 0x0100,
			JS_BUTTON_10	= 0x0200,
			JS_BUTTON_11	= 0x0400,
			JS_BUTTON_12	= 0x0800,
			JS_BUTTON_13	= 0x1000,
			JS_BUTTON_14	= 0x2000,
			JS_BUTTON_15	= 0x4000,
			JS_BUTTON_16	= 0x8000,
		};

		struct Joystick
		{
			unsigned short absX;
			unsigned short absY;
			unsigned short absZ; // rudder
			unsigned short absU; // throttle
			unsigned short absV; // other throttle?
			unsigned short pov;
			unsigned short buttons; // bitmask - if nonzero, then this is a button event
			bool btnDown; // true means "button pressed", false means "button released"
			char pad[1];
		} stick;

		// we hook into the Win32 message stream as a workaround
		// for the needs of some UI solutions there
		struct RawWin32Message
		{
			unsigned int msg;
			unsigned int wParam;
			unsigned int lParam;
			char pad[4];
		} raw;
	};
}

#endif // INPUTEVENT_INCLUDED
