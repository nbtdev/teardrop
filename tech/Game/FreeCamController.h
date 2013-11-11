/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FREECAMCONTROLLER_INCLUDED)
#define FREECAMCONTROLLER_INCLUDED

#include "Game/CameraController.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	/*
		Free-camera implementation class
	*/

	class FreeCamInputFilter;
	class InputFilter;

	class FreeCamController 
		: public CameraController
	{
	public:
		TD_CLASS(FreeCamController, CameraController);

		FreeCamController();
		virtual ~FreeCamController();

		/*
			CameraController implementation
		*/
		bool initialize();
		bool destroy();
		bool update(float deltaT);
		InputFilter* getInputFilter() const;

		void reset();

		TD_DECLARE_ALLOCATOR();

	private:
		void evaluate(float deltaT); 

		FreeCamInputFilter* mInputFilter;
		float mMove;
		float mStrafe;
		float mZoom;
		float mRotX;
		float mRotY;
	};
}

#endif // FREECAMCONTROLLER_INCLUDED
