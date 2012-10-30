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

#if !defined(MESSAGEBUS_INCLUDED)
#define MESSAGEBUS_INCLUDED

#include "FastDelegate.h"

namespace CoS
{
	class Zone;
	class ZoneObject;
	class Scene;

	namespace Reflection
	{
		class Object;
	}
}

class MessageBus
{
public:
	MessageBus();
	virtual ~MessageBus();

	// Raised when before zones/objects are loaded into scene
	func::delegate1<CoS::Scene*> SceneLoadBegin;
	// Raised when scene loading completes
	func::delegate1<CoS::Scene*> SceneLoadEnd;
	// Raised when a new zone is created and added to the scene
	func::delegate1<CoS::Zone*> ZoneCreated;	
	// Raised when a new object is created, but before it is added to a zone
	func::delegate1<CoS::ZoneObject*> ZoneObjectCreated;
	// Raised when an object added to a zone
	func::delegate2<CoS::Zone*, CoS::ZoneObject*> ZoneObjectAdded;

	// ANIMATION CONTROL
	// raised when Play button is clicked in the AnimationControlsDlg
	func::delegate0<> StartAnimationPlayback;
	// raised when Stop button is clicked in the AnimationControlsDlg
	func::delegate0<> StopAnimationPlayback;

	// broadcast updates to interested listeners
	func::delegate1<float> Update;
	func::delegate2<
		void*,						// sender
		CoS::Reflection::Object*	// the newly selected object
	> ObjectSelectionChanged;

	// report a context menu selection to listenerrs
	func::delegate2<
		void*,						// sender
		size_t						// the message (menu item) id
	> ContextMenuSelection;

protected:
};

#endif // MESSAGEBUS_INCLUDED
