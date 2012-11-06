/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CHASECAMCONTROLLER_INCLUDED)
#define CHASECAMCONTROLLER_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class ZoneObject;

	/*
		Chase-camera (3PV) implementation class
	*/

	class ChaseCamController 
		: public CameraController
	{
	public:
		TD_CLASS(ChaseCamController, CameraController);

		ChaseCamController();
		virtual ~ChaseCamController();

		/*
			CameraController implementation
		*/
		bool update(float deltaT);

		TD_DECLARE_ALLOCATOR();

	private:
	};
}

#endif // CHASECAMCONTROLLER_INCLUDED
