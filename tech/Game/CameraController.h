/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CAMERACONTROLLER_INCLUDED)
#define CAMERACONTROLLER_INCLUDED

#include "Reflection/Reflection.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	/*
		Protocol class for camera controllers
	*/

	struct Environment;
	class InputFilter;
	class GfxCamera;
	class ComponentHost;

	class CameraController
		: public Reflection::Object
	{
	public:
		TD_CLASS(CameraController, Object);
		TD_COMPLEX_PROPERTY(Offset, "Camera position offset in local camera space", Vector4, "(0,0,0,0)", 0);
		TD_COMPLEX_PROPERTY(Position, "Camera position in parent space", Vector4, "(0,0,0,0)", 0);
		TD_COMPLEX_PROPERTY(Orientation, "Camera orientation in parent space", Quaternion, "(1,0,0,0)", 0);
		TD_PROPERTY(PitchBias, "Camera pitch bias (radians)", float, 0, 0);
		TD_PROPERTY(YawBias, "Camera yaw bias (radians)", float, 0, 0);
		TD_PROPERTY(Enabled, "Camera enable/disable", bool, true, 0);

		CameraController();
		virtual ~CameraController();

		bool initialize();
		bool destroy();
		
		virtual bool update(float deltaT);
		virtual InputFilter* getInputFilter() const;

		GfxCamera* getControlledCamera() const { return m_pCamera; }

		//! provide pTarget = 0 to detach
		virtual void setTarget(ComponentHost* pTarget);

		void setAspectRatio(float aspect);
		void setFovY(float fovY, float time=0);
		void setNearClip(float near);
		void setFarClip(float far);

		TD_DECLARE_ALLOCATOR();

	protected:
		GfxCamera* m_pCamera;
		ComponentHost* m_pTarget;
	};
}

#endif // CAMERACONTROLLER_INCLUDED
