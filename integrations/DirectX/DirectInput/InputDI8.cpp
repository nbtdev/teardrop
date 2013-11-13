/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "InputDI8.h"
#include "Core/InputEvent.h"
#include "tbb/concurrent_queue.h"

static char s_keys[256];
static const int MICE_SUPPORTED = 1;

using Teardrop::InputEvent;

typedef tbb::concurrent_queue<InputEvent> Win32InputEvents;
static Win32InputEvents s_win32Events;
static HHOOK s_hKeyHook = 0;

LRESULT CALLBACK Win32HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		MSG *msg = (MSG*)lParam;
		if(wParam & PM_REMOVE)
		{
			switch(msg->message)
			{
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			case WM_DEADCHAR:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_SYSDEADCHAR:
			case WM_SYSCHAR:
			case WM_IME_CHAR:
			case WM_IME_COMPOSITION:
			case WM_IME_COMPOSITIONFULL:
			case WM_IME_CONTROL:
			case WM_IME_ENDCOMPOSITION:
			case WM_IME_KEYDOWN:
			case WM_IME_KEYUP:
			case WM_IME_NOTIFY:
			case WM_IME_REQUEST:
			case WM_IME_SELECT:
			case WM_IME_SETCONTEXT:
			case WM_IME_STARTCOMPOSITION:
			case WM_HELP:
			case WM_CANCELMODE: 
				{
					InputEvent evt;
					evt.evtType.type = InputEvent::RAW_WIN32;
					evt.raw.msg = msg->message;
					evt.raw.lParam = msg->lParam;
					evt.raw.wParam = msg->wParam;
					s_win32Events.push(evt);
					break;
				}
			}
		}
	}

	return CallNextHookEx(s_hKeyHook, nCode, wParam, lParam);
}

namespace Teardrop {
namespace DirectInput {

Input::Input()
	: mDI(0)
	, mKeyboard(0)
	, mStick(0)
	, mHwnd(0)
{
	mLCtrl = false;
	mRCtrl = false;
	mLAlt = false;
	mRAlt = false;
	mLShift = false;
	mRShift = false;
	mExtentX = mExtentY = 0;

	mMouse.resize(MICE_SUPPORTED);
	mMousePos.resize(MICE_SUPPORTED);
}

Input::~Input()
{
}

bool Input::initialize(void* hW)
{
	HWND hWnd = (HWND)hW;

	// this will be zero, not INVALID_HANDLE_VALUE, until it's available
	if (!hWnd)
		return false;

	// initialize DInput8
	HRESULT hr;
	hr = DirectInput8Create(
		GetModuleHandle(0),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&mDI,
		0);

	if (FAILED(hr))
		return false;

	mHwnd = hWnd;

	// create the mouse device
	for (int i=0; i<int(mMouse.size()); ++i)
	{
		hr = mDI->CreateDevice(
			GUID_SysMouse,
			&mMouse[i],
			0);

		if (FAILED(hr))
			return false;

		// set mouse data format and coop level
		IDirectInputDevice8* mouse = mMouse[i];
		hr = mouse->SetDataFormat(&c_dfDIMouse2);

		if (FAILED(hr))
			return false;

		hr = mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);

		// set up for buffered mouse input
		DIPROPDWORD  dipdw; 
		dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		dipdw.diph.dwObj = 0; 
		dipdw.diph.dwHow = DIPH_DEVICE; 
		dipdw.dwData = 1024; 
		mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		// aqcuire the mouse device
		mouse->Acquire();
	}

	// create keyboard device
	hr = mDI->CreateDevice(
		GUID_SysKeyboard,
		&mKeyboard,
		0);

	if (FAILED(hr))
		return false;

	// set the keyboard data format and co-op level
	IDirectInputDevice8* keyboard = mKeyboard;
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(hr))
		return false;

	hr = keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);

	// set up for buffered keyboard input
	DIPROPDWORD  dipdw; 
	dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj = 0; 
	dipdw.diph.dwHow = DIPH_DEVICE; 
	dipdw.dwData = 32; 
	keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	// acquire the keyboard device
	keyboard->Acquire();

	// init stick device
	hr = mDI->CreateDevice(
		GUID_Joystick,
		&mStick,
		0);

	if (FAILED(hr))
		return false;

	IDirectInputDevice8* stick = mStick;
	dipdw.dwData = 1024; 
	stick->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = stick->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
		return false;

	hr = stick->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);

	// set initial extents from window size
	RECT rect;
	GetWindowRect(hWnd, &rect);
	setExtents(rect.right - rect.left, rect.bottom - rect.top);

	// hook into the win32 keyboard events
	s_hKeyHook = SetWindowsHookEx(
		WH_GETMESSAGE,
		Win32HookProc,
		GetModuleHandle(0),
		GetCurrentThreadId());

	return true;
}

bool Input::destroy()
{
	UnhookWindowsHookEx(s_hKeyHook);
	s_hKeyHook = 0;

	// shut down DInput8
	mKeyboard->Unacquire();
	mKeyboard->Release();

	for (int i=0; i<int(mMouse.size()); ++i)
	{
		mMouse[i]->Unacquire();
		mMouse[i]->Release();
	}

	mDI->Release();

	return true;
}

static bool pollMouse(IDirectInputDevice8* mouse, InputEvent& event)
{
	if (!mouse)
		return false;

	DIDEVICEOBJECTDATA data[1024];
	DWORD dwNumElems = 1024;

	HRESULT hr = mouse->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	if (FAILED(mouse->Poll()))
	{
		return false;
	}

	// we want to get only the events in the same sequence (for 
	// example, mouse-move events that should be put together)
	hr = mouse->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA),
		data,
		&dwNumElems,
		DIGDD_PEEK);

	if (hr == DIERR_NOTACQUIRED)
	{
		return false;
	}

	if (hr == DI_BUFFEROVERFLOW)
	{
		return false;
	}

	if (FAILED(hr))
	{
		return false;
	}

	if (dwNumElems == 0)
	{
		return false;
	}

	// go through the data and figure out how many events
	// belong to the same sequence
	DWORD dwSeq = data[0].dwSequence;
	DWORD numInSeq = 1;
	for (DWORD i=1; i<dwNumElems; ++i)
	{
		if (data[i].dwSequence != dwSeq)
		{
			break;
		}

		++numInSeq;
	}

	// now we know how many we want to get, so get that many (note that 
	// there is no need to check error codes this time, because we already
	// did that above)
	hr = mouse->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA),
		data,
		&numInSeq,
		0);

	for (DWORD i=0; i<numInSeq; ++i)
	{
		// translate the DI event data to our InputEvent union
		switch (data[i].dwOfs)
		{
		case DIMOFS_X:
			event.mouse.relX = (char)data[i].dwData;
			break;

		case DIMOFS_Y:
			event.mouse.relY = (char)data[i].dwData;
			break;

		case DIMOFS_Z:
			event.mouse.relZ = (short)data[i].dwData;
			break;

		case DIMOFS_BUTTON0:
			event.mouse.btnsDown |= ((data[i].dwData & 0x80) ? 0x01 : 0);
			event.mouse.btnsReleased |= ((data[i].dwData & 0x80) ? 0 : 0x01);
			break;

		case DIMOFS_BUTTON1:
			event.mouse.btnsDown |= ((data[i].dwData & 0x80) ? 0x02 : 0);
			event.mouse.btnsReleased |= ((data[i].dwData & 0x80) ? 0 : 0x02);
			break;

		case DIMOFS_BUTTON2:
			event.mouse.btnsDown |= ((data[i].dwData & 0x80) ? 0x04 : 0);
			event.mouse.btnsReleased |= ((data[i].dwData & 0x80) ? 0 : 0x04);
			break;

		case DIMOFS_BUTTON3:
			event.mouse.btnsDown |= ((data[i].dwData & 0x80) ? 0x08 : 0);
			event.mouse.btnsReleased |= ((data[i].dwData & 0x80) ? 0 : 0x08);
			break;
		}
	}

	return true;
}

static int getKeyChar(int keyCode)
{
	// snippet stolen from OIS
	BYTE keyState[256];
	HKL  layout = GetKeyboardLayout(0);
	if( GetKeyboardState(keyState) == 0 )
		return 0;

	unsigned int vk = MapVirtualKeyEx(keyCode, 3, layout);
	if( vk == 0 )
		return 0;

	unsigned char buff[3] = {0,0,0};
	int ascii = ToAsciiEx(vk, keyCode, keyState, (LPWORD) buff, 0, layout);

	if (ascii == 1)
	{
		return buff[0];
	}

	return 0;
}

// the mappings are 1:1 and contiguous from DI8 to TD 
// keycodes, at least through 0x53
static InputEvent::KeyCode sKeyCodes[] = {
	InputEvent::KC_NONE,
	InputEvent::KC_ESCAPE,
	InputEvent::KC_1,
	InputEvent::KC_2,
	InputEvent::KC_3,
	InputEvent::KC_4,
	InputEvent::KC_5,
	InputEvent::KC_6,
	InputEvent::KC_7,
	InputEvent::KC_8,
	InputEvent::KC_9,
	InputEvent::KC_0,
	InputEvent::KC_MINUS,
	InputEvent::KC_EQUALS,
	InputEvent::KC_BACK,
	InputEvent::KC_TAB,
	InputEvent::KC_Q,
	InputEvent::KC_W,
	InputEvent::KC_E,
	InputEvent::KC_R,
	InputEvent::KC_T,
	InputEvent::KC_Y,
	InputEvent::KC_U,
	InputEvent::KC_I,
	InputEvent::KC_O,
	InputEvent::KC_P,
	InputEvent::KC_LBRACKET,
	InputEvent::KC_RBRACKET,
	InputEvent::KC_RETURN,
	InputEvent::KC_LCONTROL,     
	InputEvent::KC_A,
	InputEvent::KC_S,
	InputEvent::KC_D,
	InputEvent::KC_F,
	InputEvent::KC_G,
	InputEvent::KC_H,
	InputEvent::KC_J,
	InputEvent::KC_K,
	InputEvent::KC_L,
	InputEvent::KC_SEMICOLON,
	InputEvent::KC_APOSTROPHE,
	InputEvent::KC_GRAVE,
	InputEvent::KC_LSHIFT,
	InputEvent::KC_BACKSLASH,
	InputEvent::KC_Z,
	InputEvent::KC_X,
	InputEvent::KC_C,
	InputEvent::KC_V,
	InputEvent::KC_B,
	InputEvent::KC_N,
	InputEvent::KC_M,
	InputEvent::KC_COMMA,
	InputEvent::KC_PERIOD,
	InputEvent::KC_SLASH,
	InputEvent::KC_RSHIFT,
	InputEvent::KC_MULTIPLY,
	InputEvent::KC_LMENU,
	InputEvent::KC_SPACE,
	InputEvent::KC_CAPITAL,
	InputEvent::KC_F1,
	InputEvent::KC_F2,
	InputEvent::KC_F3,
	InputEvent::KC_F4,
	InputEvent::KC_F5,
	InputEvent::KC_F6,
	InputEvent::KC_F7,
	InputEvent::KC_F8,
	InputEvent::KC_F9,
	InputEvent::KC_F10,
	InputEvent::KC_NUMLOCK,
	InputEvent::KC_SCROLL,
	InputEvent::KC_NUMPAD7,
	InputEvent::KC_NUMPAD8,
	InputEvent::KC_NUMPAD9,
	InputEvent::KC_SUBTRACT,
	InputEvent::KC_NUMPAD4,
	InputEvent::KC_NUMPAD5,
	InputEvent::KC_NUMPAD6,
	InputEvent::KC_ADD,
	InputEvent::KC_NUMPAD1,
	InputEvent::KC_NUMPAD2,
	InputEvent::KC_NUMPAD3,
	InputEvent::KC_NUMPAD0,
	InputEvent::KC_DECIMAL,
	InputEvent::KC_NONE,
	InputEvent::KC_NONE,
	InputEvent::KC_OEM_102,
	InputEvent::KC_F11,
	InputEvent::KC_F12,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_F13,
	InputEvent::KC_F14,
	InputEvent::KC_F15,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_KANA,
	InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_ABNT_C1,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_CONVERT,
	InputEvent::KC_NONE,
	InputEvent::KC_NOCONVERT,
	InputEvent::KC_NONE,
	InputEvent::KC_YEN,
	InputEvent::KC_ABNT_C2,
	InputEvent::KC_NONE,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_NUMPADEQUALS,
	InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_PREVTRACK,
	InputEvent::KC_AT,
	InputEvent::KC_COLON,
	InputEvent::KC_UNDERLINE,
	InputEvent::KC_KANJI,
	InputEvent::KC_STOP,
	InputEvent::KC_AX,
	InputEvent::KC_UNLABELED,
	InputEvent::KC_NONE, 
	InputEvent::KC_NEXTTRACK,
	InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_NUMPADENTER,
	InputEvent::KC_RCONTROL,
	InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_MUTE,
	InputEvent::KC_CALCULATOR,
	InputEvent::KC_PLAYPAUSE,
	InputEvent::KC_NONE, 
	InputEvent::KC_MEDIASTOP,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_VOLUMEDOWN,
	InputEvent::KC_NONE, 
	InputEvent::KC_VOLUMEUP,
	InputEvent::KC_NONE, 
	InputEvent::KC_WEBHOME,
	InputEvent::KC_NUMPADCOMMA,
	InputEvent::KC_NONE, 
	InputEvent::KC_DIVIDE,
	InputEvent::KC_NONE, 
	InputEvent::KC_SYSRQ,
	InputEvent::KC_RMENU,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_PAUSE,
	InputEvent::KC_NONE, 
	InputEvent::KC_HOME,
	InputEvent::KC_UP,
	InputEvent::KC_PRIOR,
	InputEvent::KC_NONE, 
	InputEvent::KC_LEFT,
	InputEvent::KC_NONE, 
	InputEvent::KC_RIGHT,
	InputEvent::KC_NONE, 
	InputEvent::KC_END,
	InputEvent::KC_DOWN,
	InputEvent::KC_NEXT,
	InputEvent::KC_INSERT,
	InputEvent::KC_DELETE,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE, 
	InputEvent::KC_LWIN,
	InputEvent::KC_RWIN,
	InputEvent::KC_APPS,
	InputEvent::KC_POWER,
	InputEvent::KC_SLEEP,
	InputEvent::KC_NONE, InputEvent::KC_NONE, InputEvent::KC_NONE,
	InputEvent::KC_WAKE,
	InputEvent::KC_NONE, 
	InputEvent::KC_WEBSEARCH,
	InputEvent::KC_WEBFAVORITES,
	InputEvent::KC_WEBREFRESH,
	InputEvent::KC_WEBSTOP,
	InputEvent::KC_WEBFORWARD,
	InputEvent::KC_WEBBACK,
	InputEvent::KC_MYCOMPUTER,
	InputEvent::KC_MAIL,
	InputEvent::KC_MEDIASELECT,
};

static bool pollKeyboard(IDirectInputDevice8* keyboard, InputEvent& event)
{
	if (!keyboard)
		return false;

	DIDEVICEOBJECTDATA data[32];
	DWORD dwNumElems = 32;
	HRESULT hr;

	hr = keyboard->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	hr = keyboard->GetDeviceData( 
		sizeof(DIDEVICEOBJECTDATA), 
		data, 
		&dwNumElems, 
		0);

	if(hr != DI_OK)
	{
		hr = keyboard->Acquire();

		if (FAILED(hr))
		{
			if (hr == E_ACCESSDENIED)
			{
				HRESULT  hr = keyboard->GetDeviceState(sizeof(s_keys), &s_keys);

				if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
				{
					hr = keyboard->Acquire();
					if (hr != DIERR_OTHERAPPHASPRIO)
						keyboard->GetDeviceState(sizeof(s_keys), &s_keys);
				}
			}

			while(hr == DIERR_INPUTLOST)
				hr = keyboard->Acquire();

			return false;
		}

		// try again...
		hr = keyboard->GetDeviceData( 
			sizeof(DIDEVICEOBJECTDATA), 
			data, 
			&dwNumElems, 
			0);

		if (FAILED(hr))
		{
			return false;
		}
	}

	// if there was no keyboard input...
	if (!dwNumElems)
	{
		return false;
	}

	// the event is cleared to all zeros/false's prior to calling this,
	// so we can act only on the things that need to be set true or non-zero
	// in this method
	for(unsigned int i = 0; i < dwNumElems; ++i )
	{
		int kc = (int)data[i].dwOfs;
		event.key.keyCode = sKeyCodes[kc];
		event.key.keyChar = getKeyChar(kc);

		if(data[i].dwData & 0x80)
		{
			event.key.bKeyDown = true;

			//Turn on modifiers
			if(kc == DIK_LCONTROL)
				event.key.lCtrlDown = true;
			else if (kc == DIK_RCONTROL)
				event.key.rCtrlDown = true;
			else if(kc == DIK_LSHIFT)
				event.key.lShiftDown = true;
			else if (kc == DIK_RSHIFT)
				event.key.rShiftDown = true;
			else if(kc == DIK_LMENU)
				event.key.lAltDown = true;
			else if (kc == DIK_RMENU)
				event.key.rAltDown = true;
		}
	}

	return true;
}
//---------------------------------------------------------------------------
static bool pollJoystick(IDirectInputDevice8* stick, InputEvent& event)
{
	if (!stick)
		return false;

	DIDEVICEOBJECTDATA data[1024];
	DWORD dwNumElems = 1024;

	HRESULT hr;
	if (SUCCEEDED(stick->Poll()))
	{
		hr = stick->GetDeviceData(
			sizeof(DIDEVICEOBJECTDATA),
			data,
			&dwNumElems,
			DIGDD_PEEK);
	}
	else
	{
		hr = stick->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = stick->Acquire();

		if (SUCCEEDED(stick->Poll()))
		{
			hr = stick->GetDeviceData(
				sizeof(DIDEVICEOBJECTDATA),
				data,
				&dwNumElems,
				DIGDD_PEEK);
		}
		else
		{
			// give up for now
			return false;
		}
	}

	if (!dwNumElems)
		return false;

	// go through the data and figure out how many events
	// belong to the same sequence
	DWORD dwSeq = data[0].dwSequence;
	DWORD numInSeq = 1;
	for (DWORD i=1; i<dwNumElems; ++i)
	{
		if (data[i].dwSequence != dwSeq)
		{
			break;
		}

		++numInSeq;
	}

	// now we know how many we want to get, so get that many (note that 
	// there is no need to check error codes this time, because we already
	// did that above)
	hr = stick->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA),
		data,
		&numInSeq,
		0);


	for (DWORD i=0; i<numInSeq; ++i)
	{
		if (data[i].dwOfs >= DIJOFS_BUTTON(0) && data[i].dwOfs <= DIJOFS_BUTTON(15)) 
		{
			event.stick.buttons = 1 << (data[i].dwOfs-DIJOFS_BUTTON(0));
			event.stick.btnDown = ((data[i].dwData & 0x80) == 0x80);
		}
		else
		{
			switch (data[i].dwOfs)
			{
			case DIJOFS_X: // stick X position, 0 (full left) to 65535 (full right)
				event.stick.absX = unsigned short(data[i].dwData);
				break;
			case DIJOFS_Y: // stick Y position, 0 (full fwd) to 65535 (full bkwd)
				event.stick.absY = unsigned short(data[i].dwData);
				break;
			case DIJOFS_Z:
				break;
			case DIJOFS_RX:
				break;
			case DIJOFS_RY:
				break;
			case DIJOFS_RZ: // rudder position, 0 (full left) to 65535 (full right)
				event.stick.absZ = unsigned short(data[i].dwData);
				break;
			case DIJOFS_SLIDER(0): // throttle position, 0 (full fwd) to 65536 (full bkwd)
				event.stick.absU = unsigned short(data[i].dwData);
				break;
			case DIJOFS_SLIDER(1):
				event.stick.absV = unsigned short(data[i].dwData);
				break;
			case DIJOFS_POV(0): // degrees * 100 (CW, 0 at top), or -1 if released to center
				event.stick.pov = unsigned short(data[i].dwData);
				break;
			case DIJOFS_POV(1):
				break;
			case DIJOFS_POV(2):
				break;
			case DIJOFS_POV(3):
				break;
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
static bool pollRawWin32(InputEvent& event)
{
	if (s_win32Events.empty())
		return false;

	// otherwise get one and fill in the event
	s_win32Events.try_pop(event);
	return true;
}
//---------------------------------------------------------------------------
// Application is expected to call this method until no more events are pending
bool Input::getNextEvent(InputEvent& /*out*/ event)
{
	// clear the event (sanity check)
	ZeroMemory(&event, sizeof(event));

	// keyboard and at least one mouse have to be present
	if (!mMouse[0] || !mKeyboard)
		return false;

	if (pollJoystick(mStick, event)) {
		event.evtType.type = InputEvent::STICK;
		return true;
	}

	if (pollKeyboard(mKeyboard, event)) {
		event.evtType.type = InputEvent::KEYBOARD;

		if (event.key.keyCode == DIK_LCONTROL) mLCtrl = event.key.bKeyDown;
		if (event.key.keyCode == DIK_LALT) mLAlt = event.key.bKeyDown;
		if (event.key.keyCode == DIK_LSHIFT) mLShift = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RCONTROL) mRCtrl = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RALT) mRAlt = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RSHIFT) mRShift = event.key.bKeyDown;

		event.key.lCtrlDown = mLCtrl;
		event.key.lShiftDown = mLShift;
		event.key.lAltDown = mLAlt;
		event.key.rCtrlDown = mRCtrl;
		event.key.rShiftDown = mRShift;
		event.key.rAltDown = mRAlt;

		return true;
	}

	// poll all active mice
	for (int i=0; i<int(mMouse.size()); ++i) {
		if (pollMouse(mMouse[i], event)) {
			AbsPos& mousePos = mMousePos[i];

			event.evtType.type = InputEvent::MOUSE;
			event.mouse.index = (unsigned char)i;
			mousePos.x += event.mouse.relX;
			mousePos.y += event.mouse.relY;
			mousePos.z += event.mouse.relZ;

			// DirectX rel values are actually meaningless in non-exclusive mode, 
			// so we can't accumulate them directly
			//event.mouse.absX = mousePos.x;
			//event.mouse.absY = mousePos.y;

			POINT point;
			GetCursorPos(&point);
			ScreenToClient(mHwnd, &point);
			event.mouse.absX = (short)point.x;
			event.mouse.absY = (short)point.y;
			event.mouse.absZ = (short)mousePos.z;
			event.mouse.isInside = (
				point.x >= 0 && point.x < mExtentX
				&& point.y >= 0 && point.y < mExtentY
				);
			return true;
		}
	}

	// lowest priority since it's typically UI-bound keyboard events
	if (pollRawWin32(event)) {
		event.evtType.type = InputEvent::RAW_WIN32;
		return true;
	}

	return false;
}

void Input::getExtents(int& x, int& y)
{
	x = mExtentX;
	y = mExtentY;
}

void Input::setExtents(int x, int y)
{
	mExtentX = x;
	mExtentY = y;
}

void Input::suspend()
{
	for (int i=0; i<int(mMouse.size()); ++i)
	{
		if (mMouse[i])
			mMouse[i]->Unacquire();
	}

	if (mKeyboard)
		mKeyboard->Unacquire();
}

void Input::resume()
{
	for (int i=0; i<int(mMouse.size()); ++i)
	{
		if (mMouse[i])
			mMouse[i]->Acquire();
	}

	if (mKeyboard)
		mKeyboard->Acquire();
}

} // namespace DirectInput
} // namespace Teardrop
