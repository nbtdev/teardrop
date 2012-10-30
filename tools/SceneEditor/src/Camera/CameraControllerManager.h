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

#if !defined(CAMERACONTROLLERMANAGER_INCLUDED)
#define CAMERACONTROLLERMANAGER_INCLUDED

/*
	Finite state machine for managing which camera controller
	is in charge of a camera at any given time. A 1:1 relationship
	exists between a camera and its controller manager, so there 
	should always be one controller manager for each active camera.

*/

namespace CoS
{
	union InputEvent;
}

class CCMState;

class CameraControllerManager
{
	CCMState* m_pState;
	CCMState* m_pNextState;
	CoS::GfxCamera* m_pCam;
	size_t m_currentState;
	bool m_bAlt;

public:
	CameraControllerManager(CoS::GfxCamera* pControlledCamera);
	virtual ~CameraControllerManager();

	enum Controller
	{
		CCM_FREECAM,
		CCM_ORBIT,
	};

	bool handleInputEvent(const CoS::InputEvent& event);

	//! change the target of the camera (may animate to the new target as a result)
	void changeTarget(const CoS::Vector4& newPos);
	void changeTarget(CoS::ZoneObject* newObj);
	
	//! change the type of controller 
	void changeController(Controller type);

	//! update the manager and current controller
	bool update(float deltaT);

	COS_DECLARE_ALLOCATOR();
};

#endif // CAMERACONTROLLERMANAGER_INCLUDED
