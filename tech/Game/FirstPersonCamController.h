/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FIRSTPERSONCAMCONTROLLER_INCLUDED)
#define FIRSTPERSONCAMCONTROLLER_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class ZoneObject;

	/*
		First-person camera (FFP, FPV) implementation class
	*/

	class FirstPersonCamController 
		: public CameraController
	{
		ZoneObject* m_pObject;

	public:
		TD_CLASS(FirstPersonCamController, CameraController);

		FirstPersonCamController();
		virtual ~FirstPersonCamController();

		/*
			CameraController implementation
		*/
		bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // FIRSTPERSONCAMCONTROLLER_INCLUDED
