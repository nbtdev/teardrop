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

#if !defined(INPUTEVENT_INCLUDED)
#define INPUTEVENT_INCLUDED

namespace Teardrop
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

		enum KeyCode {
			KC_NONE,
			KC_ESCAPE,
			KC_1,
			KC_2,
			KC_3,
			KC_4,
			KC_5,
			KC_6,
			KC_7,
			KC_8,
			KC_9,
			KC_0,
			KC_MINUS,
			KC_EQUALS,
			KC_BACK,
			KC_TAB,
			KC_Q,
			KC_W,
			KC_E,
			KC_R,
			KC_T,
			KC_Y,
			KC_U,
			KC_I,
			KC_O,
			KC_P,
			KC_LBRACKET,
			KC_RBRACKET,
			KC_RETURN,
			KC_LCONTROL,     
			KC_A,
			KC_S,
			KC_D,
			KC_F,
			KC_G,
			KC_H,
			KC_J,
			KC_K,
			KC_L,
			KC_SEMICOLON,
			KC_APOSTROPHE,
			KC_GRAVE,
			KC_LSHIFT,
			KC_BACKSLASH,
			KC_Z,
			KC_X,
			KC_C,
			KC_V,
			KC_B,
			KC_N,
			KC_M,
			KC_COMMA,
			KC_PERIOD,
			KC_SLASH,
			KC_RSHIFT,
			KC_MULTIPLY,
			KC_LMENU,
			KC_SPACE,
			KC_CAPITAL,
			KC_F1,
			KC_F2,
			KC_F3,
			KC_F4,
			KC_F5,
			KC_F6,
			KC_F7,
			KC_F8,
			KC_F9,
			KC_F10,
			KC_NUMLOCK,
			KC_SCROLL,
			KC_NUMPAD7,
			KC_NUMPAD8,
			KC_NUMPAD9,
			KC_SUBTRACT,
			KC_NUMPAD4,
			KC_NUMPAD5,
			KC_NUMPAD6,
			KC_ADD,
			KC_NUMPAD1,
			KC_NUMPAD2,
			KC_NUMPAD3,
			KC_NUMPAD0,
			KC_DECIMAL,
			KC_OEM_102,
			KC_F11,
			KC_F12,
			KC_F13,
			KC_F14,
			KC_F15,
			KC_KANA,
			KC_ABNT_C1,
			KC_CONVERT,
			KC_NOCONVERT,
			KC_YEN,
			KC_ABNT_C2,
			KC_NUMPADEQUALS,
			KC_PREVTRACK,
			KC_AT,
			KC_COLON,
			KC_UNDERLINE,
			KC_KANJI,
			KC_STOP,
			KC_AX,
			KC_UNLABELED,
			KC_NEXTTRACK,
			KC_NUMPADENTER,
			KC_RCONTROL,
			KC_MUTE,
			KC_CALCULATOR,
			KC_PLAYPAUSE,
			KC_MEDIASTOP,
			KC_VOLUMEDOWN,
			KC_VOLUMEUP,
			KC_WEBHOME,
			KC_NUMPADCOMMA,
			KC_DIVIDE,
			KC_SYSRQ,
			KC_RMENU,
			KC_PAUSE,
			KC_HOME,
			KC_UP,
			KC_PRIOR,
			KC_LEFT,
			KC_RIGHT,
			KC_END,
			KC_DOWN,
			KC_NEXT,
			KC_INSERT,
			KC_DELETE,
			KC_LWIN,
			KC_RWIN,
			KC_APPS,
			KC_POWER,
			KC_SLEEP,
			KC_WAKE,
			KC_WEBSEARCH,
			KC_WEBFAVORITES,
			KC_WEBREFRESH,
			KC_WEBSTOP,
			KC_WEBFORWARD,
			KC_WEBBACK,
			KC_MYCOMPUTER,
			KC_MAIL,
			KC_MEDIASELECT,
		};

		struct Key
		{
			int keyChar;
			KeyCode keyCode;
			bool bKeyDown;
			bool lCtrlDown;
			bool lAltDown;
			bool lShiftDown;
			bool rCtrlDown;
			bool rAltDown;
			bool rShiftDown;
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
