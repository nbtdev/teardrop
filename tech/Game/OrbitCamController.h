/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ORBITCAMCONTROLLER_INCLUDED)
#define ORBITCAMCONTROLLER_INCLUDED

#include "Game/CameraController.h"
#include "Math/Vector4.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	/*
		Free-camera implementation class
	*/

	class OrbitCamInputFilter;
	class InputFilter;
	class ZoneObject;

	class OrbitCamController 
		: public CameraController
	{
		OrbitCamInputFilter* m_pInputFilter;
		Vector4 m_targetPos;
		float m_dist;
		float m_azimuth;
		float m_elevation;
		float m_zoomFactor;

	public:
		TD_CLASS(OrbitCamController, CameraController);

		OrbitCamController();
		virtual ~OrbitCamController();

		//! overrides CameraController::setTarget
		void setTarget(ComponentHost* pTarget);
		//! used if no target set
		void setTarget(const Vector4& targetPos);
		//! manually set the distance from the target
		void setDistanceToTarget(float units);
		//! init values from an existing position (relative to target)
		void reinit(const Vector4& pos);

		/*
			CameraController implementation
		*/
		bool initialize();
		bool destroy();
		bool update(float deltaT);
		InputFilter* getInputFilter() const;

		void reset();

		//! on a scale of [0..1]; default is 0.3
		void setZoomStepFactor(float factor);

		TD_DECLARE_ALLOCATOR();

	private:
		void evaluate(float deltaT);
	};
}

#endif // ORBITCAMCONTROLLER_INCLUDED
