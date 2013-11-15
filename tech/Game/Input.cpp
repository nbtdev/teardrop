/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Input.h"
#include "InputEvent.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Memory/Memory.h"
#include "Gfx/GfxRenderer.h"
#include "Gfx/GfxRenderWindow.h"
#include "tbb/concurrent_queue.h"

#include <stdio.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
static char s_keys[256];

typedef tbb::concurrent_queue<InputEvent> Win32InputEvents;
static Win32InputEvents s_win32Events;
static HHOOK s_hKeyHook = 0;
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
struct Input::Impl
{
	static const int MICE_SUPPORTED = 1;
	
	struct AbsPos
	{
		int x, y, z;
	};

	IDirectInput8*			pDI;
	IDirectInputDevice8*	pMouse[MICE_SUPPORTED];
	IDirectInputDevice8*	pKeyboard;
	IDirectInputDevice8*	pStick;
	int						numMice;		// support for more than one 
	AbsPos					mousePos[MICE_SUPPORTED];
	HWND					hWnd;
	bool					bLCtrl;
	bool					bRCtrl;
	bool					bLAlt;
	bool					bRAlt;
	bool					bLShift;
	bool					bRShift;

	TD_DECLARE_ALLOCATOR();

	Impl()
		: pDI(0),
		pKeyboard(0),
		pStick(0),
		hWnd(0),
		numMice(MICE_SUPPORTED)
	{
		ZeroMemory(pMouse, sizeof(pMouse));
		ZeroMemory(mousePos, sizeof(mousePos));
		bLCtrl = false;
		bRCtrl = false;
		bLAlt = false;
		bRAlt = false;
		bLShift = false;
		bRShift = false;
	}
};
//---------------------------------------------------------------------------
Input::Input()
{
	m_pImpl = 0;
	m_extentX = m_extentY = 0;
}
//---------------------------------------------------------------------------
Input::~Input()
{
	delete m_pImpl;
}
//---------------------------------------------------------------------------
bool Input::initialize(Environment& env, void* hW)
{
	HWND hWnd = (HWND)hW;
	// this will be zero, not INVALID_HANDLE_VALUE, until it's available
	if (!hWnd)
	{
		return false;
	}

	HRESULT hr;
	if (!m_pImpl)
	{
		m_pImpl = TD_NEW Impl();

		// initialize DInput8
		hr = DirectInput8Create(
			GetModuleHandle(0),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_pImpl->pDI,
			0);

		if (FAILED(hr))
		{
			env.pLogger->logMessage("Could not create DirectInput");
			return false;
		}
	}

	m_pImpl->hWnd = hWnd;

	// create the mouse device
	for (int i=0; i<m_pImpl->numMice; ++i)
	{
		hr = m_pImpl->pDI->CreateDevice(
			GUID_SysMouse,
			&m_pImpl->pMouse[i],
			0);
		if (FAILED(hr))
		{
			env.pLogger->logMessage("Could not create mouse device");
			return false;
		}

		// set mouse data format and coop level
		IDirectInputDevice8* pMouse = m_pImpl->pMouse[i];
		hr = pMouse->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			env.pLogger->logMessage("Could not set mouse data format");
			return false;
		}

		hr = pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
		if (FAILED(hr))
		{
			env.pLogger->logMessage("Could not set mouse co-op level");
			//return false;
		}

		// set up for buffered mouse input
		DIPROPDWORD  dipdw; 
		dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		dipdw.diph.dwObj = 0; 
		dipdw.diph.dwHow = DIPH_DEVICE; 
		dipdw.dwData = 1024; 
		pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		// aqcuire the mouse device
		pMouse->Acquire();
	}

	// create keyboard device
	hr = m_pImpl->pDI->CreateDevice(
		GUID_SysKeyboard,
		&m_pImpl->pKeyboard,
		0);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not create keyboard device");
		return false;
	}

	// set the keyboard data format and co-op level
	IDirectInputDevice8* pKeyboard = m_pImpl->pKeyboard;
	hr = pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not set keyboard data format");
		return false;
	}

	hr = pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not set keyboard co-op level");
		//return false;
	}

	// set up for buffered keyboard input
	DIPROPDWORD  dipdw; 
	dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj = 0; 
	dipdw.diph.dwHow = DIPH_DEVICE; 
	dipdw.dwData = 32; 
	pKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	// acquire the keyboard device
	pKeyboard->Acquire();

	// init stick device
	hr = m_pImpl->pDI->CreateDevice(
		GUID_Joystick,
		&m_pImpl->pStick,
		0);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not create joystick device");
		return false;
	}
	IDirectInputDevice8* pStick = m_pImpl->pStick;
	dipdw.dwData = 1024; 
	pStick->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	
	hr = pStick->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not set joystick data format");
		return false;
	}

	hr = pStick->SetCooperativeLevel(hWnd, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		env.pLogger->logMessage("Could not set joystick co-op level");
		//return false;
	}

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
//---------------------------------------------------------------------------
bool Input::destroy()
{
	UnhookWindowsHookEx(s_hKeyHook);
	s_hKeyHook = 0;

	// shut down DInput8
	if (m_pImpl)
	{
		m_pImpl->pKeyboard->Unacquire();
		m_pImpl->pKeyboard->Release();

		for (int i=0; i<m_pImpl->numMice; ++i)
		{
			m_pImpl->pMouse[i]->Unacquire();
			m_pImpl->pMouse[i]->Release();
		}

		m_pImpl->pDI->Release();
	}

	// delete our impl
	delete m_pImpl;
	m_pImpl = 0;

	return true;
}
//---------------------------------------------------------------------------
static bool pollMouse(IDirectInputDevice8* pMouse, InputEvent& event)
{
	if (!pMouse)
		return false;

	DIDEVICEOBJECTDATA data[1024];
	DWORD dwNumElems = 1024;

	HRESULT hr = pMouse->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	if (FAILED(pMouse->Poll()))
	{
		return false;
	}

	// we want to get only the events in the same sequence (for 
	// example, mouse-move events that should be put together)
	hr = pMouse->GetDeviceData(
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
	hr = pMouse->GetDeviceData(
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
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
static bool pollKeyboard(IDirectInputDevice8* pKeyboard, InputEvent& event)
{
	if (!pKeyboard)
		return false;

	DIDEVICEOBJECTDATA data[32];
	DWORD dwNumElems = 32;
	HRESULT hr;

	hr = pKeyboard->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	hr = pKeyboard->GetDeviceData( 
		sizeof(DIDEVICEOBJECTDATA), 
		data, 
		&dwNumElems, 
		0);

	if(hr != DI_OK)
	{
		hr = pKeyboard->Acquire();

		if (FAILED(hr))
		{
			if (hr == E_ACCESSDENIED)
			{
				HRESULT  hr = pKeyboard->GetDeviceState(sizeof(s_keys), &s_keys);

				if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
				{
					hr = pKeyboard->Acquire();
					if (hr != DIERR_OTHERAPPHASPRIO)
						pKeyboard->GetDeviceState(sizeof(s_keys), &s_keys);
				}
			}

			while(hr == DIERR_INPUTLOST)
				hr = pKeyboard->Acquire();

			return false;
		}

		// try again...
		hr = pKeyboard->GetDeviceData( 
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
		event.key.keyCode = kc;
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
static bool pollJoystick(IDirectInputDevice8* pStick, InputEvent& event)
{
	if (!pStick)
		return false;

	DIDEVICEOBJECTDATA data[1024];
	DWORD dwNumElems = 1024;

	HRESULT hr;
	if (SUCCEEDED(pStick->Poll()))
	{
		hr = pStick->GetDeviceData(
			sizeof(DIDEVICEOBJECTDATA),
			data,
			&dwNumElems,
			DIGDD_PEEK);
	}
	else
	{
		hr = pStick->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = pStick->Acquire();

		if (SUCCEEDED(pStick->Poll()))
		{
			hr = pStick->GetDeviceData(
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
	hr = pStick->GetDeviceData(
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

	if (!m_pImpl)
		return false;

	// keyboard and at least one mouse have to be present
	if (!m_pImpl->pMouse[0] || !m_pImpl->pKeyboard)
	{
		return false;
	}

	if (pollJoystick(m_pImpl->pStick, event))
	{
		event.evtType.type = InputEvent::STICK;
		return true;
	}

	if (pollKeyboard(m_pImpl->pKeyboard, event))
	{
		event.evtType.type = InputEvent::KEYBOARD;

		if (event.key.keyCode == DIK_LCONTROL) m_pImpl->bLCtrl = event.key.bKeyDown;
		if (event.key.keyCode == DIK_LALT) m_pImpl->bLAlt = event.key.bKeyDown;
		if (event.key.keyCode == DIK_LSHIFT) m_pImpl->bLShift = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RCONTROL) m_pImpl->bRCtrl = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RALT) m_pImpl->bRAlt = event.key.bKeyDown;
		if (event.key.keyCode == DIK_RSHIFT) m_pImpl->bRShift = event.key.bKeyDown;

		event.key.lCtrlDown = m_pImpl->bLCtrl;
		event.key.lShiftDown = m_pImpl->bLShift;
		event.key.lAltDown = m_pImpl->bLAlt;
		event.key.rCtrlDown = m_pImpl->bRCtrl;
		event.key.rShiftDown = m_pImpl->bRShift;
		event.key.rAltDown = m_pImpl->bRAlt;

		return true;
	}

	// poll all active mice
	for (int i=0; i<m_pImpl->numMice; ++i)
	{
		if (pollMouse(m_pImpl->pMouse[i], event))
		{
			Impl::AbsPos& mousePos = m_pImpl->mousePos[i];

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
			ScreenToClient(m_pImpl->hWnd, &point);
			event.mouse.absX = (short)point.x;
			event.mouse.absY = (short)point.y;
			event.mouse.absZ = (short)mousePos.z;
			event.mouse.isInside = (
				   point.x >= 0 && point.x < m_extentX
				&& point.y >= 0 && point.y < m_extentY
				);
			return true;
		}
	}

	// lowest priority since it's typically UI-bound keyboard events
	if (pollRawWin32(event))
	{
		event.evtType.type = InputEvent::RAW_WIN32;
		return true;
	}
	
	return false;
}
//---------------------------------------------------------------------------
void Input::getExtents(int& x, int& y)
{
	x = m_extentX;
	y = m_extentY;
}
//---------------------------------------------------------------------------
void Input::setExtents(int x, int y)
{
	m_extentX = x;
	m_extentY = y;
}
//---------------------------------------------------------------------------
void Input::suspend()
{
	for (int i=0; i<m_pImpl->numMice; ++i)
	{
		if (m_pImpl->pMouse[i])
			m_pImpl->pMouse[i]->Unacquire();
	}

	if (m_pImpl->pKeyboard)
		m_pImpl->pKeyboard->Unacquire();
}
//---------------------------------------------------------------------------
void Input::resume()
{
	for (int i=0; i<m_pImpl->numMice; ++i)
	{
		if (m_pImpl->pMouse[i])
			m_pImpl->pMouse[i]->Acquire();
	}

	if (m_pImpl->pKeyboard)
		m_pImpl->pKeyboard->Acquire();
}
