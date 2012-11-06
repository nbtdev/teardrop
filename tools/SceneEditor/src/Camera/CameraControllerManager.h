/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CAMERACONTROLLERMANAGER_INCLUDED)
#define CAMERACONTROLLERMANAGER_INCLUDED

/*
	Finite state machine for managing which camera controller
	is in charge of a camera at any given time. A 1:1 relationship
	exists between a camera and its controller manager, so there 
	should always be one controller manager for each active camera.

*/

namespace Teardrop
{
	union InputEvent;
}

class CCMState;

class CameraControllerManager
{
	CCMState* m_pState;
	CCMState* m_pNextState;
	Teardrop::GfxCamera* m_pCam;
	size_t m_currentState;
	bool m_bAlt;

public:
	CameraControllerManager(Teardrop::GfxCamera* pControlledCamera);
	virtual ~CameraControllerManager();

	enum Controller
	{
		CCM_FREECAM,
		CCM_ORBIT,
	};

	bool handleInputEvent(const Teardrop::InputEvent& event);

	//! change the target of the camera (may animate to the new target as a result)
	void changeTarget(const Teardrop::Vector4& newPos);
	void changeTarget(Teardrop::ZoneObject* newObj);
	
	//! change the type of controller 
	void changeController(Controller type);

	//! update the manager and current controller
	bool update(float deltaT);

	TD_DECLARE_ALLOCATOR();
};

#endif // CAMERACONTROLLERMANAGER_INCLUDED
