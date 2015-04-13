/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "InputXWindow.h"
#include "Core/InputEvent.h"

using Teardrop::InputEvent;

namespace Teardrop {
namespace XWindow {

Input::Input(Display* aDisplay)
    : mDisplay(aDisplay)
{
}

Input::~Input()
{
}

void Input::initialize(uintptr_t aWindow)
{
    mWindow = (Window)aWindow;
}

void Input::destroy()
{
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

//---------------------------------------------------------------------------
// Application is expected to call this method until no more events are pending
bool Input::getNextEvent(InputEvent& /*out*/ event)
{
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
}

void Input::resume()
{
}

} // namespace DirectInput
} // namespace Teardrop
