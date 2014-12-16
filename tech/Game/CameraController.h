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
	namespace Gfx 
	{
		class Camera;
	}

	class InputFilter;

	/*
		Protocol class for camera controllers
	*/

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

		Gfx::Camera* camera() const { return mCamera; }

		void setAspectRatio(float aspect);
		void setFovY(float fovY, float time=0);
		void setNearClip(float near);
		void setFarClip(float far);

		virtual void injectMouseMove(int absX, int absY, int relX=0, int relY=0);
		virtual void injectMouseWheel(int absZ, int relZ=0);

	protected:
		Gfx::Camera* mCamera;
	};
}

#endif // CAMERACONTROLLER_INCLUDED
