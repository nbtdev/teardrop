/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MESSAGEBUS_INCLUDED)
#define MESSAGEBUS_INCLUDED

#include "FastDelegate.h"

namespace Teardrop
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
	func::delegate1<Teardrop::Scene*> SceneLoadBegin;
	// Raised when scene loading completes
	func::delegate1<Teardrop::Scene*> SceneLoadEnd;
	// Raised when a new zone is created and added to the scene
	func::delegate1<Teardrop::Zone*> ZoneCreated;	
	// Raised when a new object is created, but before it is added to a zone
	func::delegate1<Teardrop::ZoneObject*> ZoneObjectCreated;
	// Raised when an object added to a zone
	func::delegate2<Teardrop::Zone*, Teardrop::ZoneObject*> ZoneObjectAdded;

	// ANIMATION CONTROL
	// raised when Play button is clicked in the AnimationControlsDlg
	func::delegate0<> StartAnimationPlayback;
	// raised when Stop button is clicked in the AnimationControlsDlg
	func::delegate0<> StopAnimationPlayback;

	// broadcast updates to interested listeners
	func::delegate1<float> Update;
	func::delegate2<
		void*,						// sender
		Teardrop::Reflection::Object*	// the newly selected object
	> ObjectSelectionChanged;

	// report a context menu selection to listenerrs
	func::delegate2<
		void*,						// sender
		size_t						// the message (menu item) id
	> ContextMenuSelection;

protected:
};

#endif // MESSAGEBUS_INCLUDED
