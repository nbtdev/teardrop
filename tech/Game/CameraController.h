/******************************************************************************
Copyright (c) 2015 Teardrop Games

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
******************************************************************************/

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
