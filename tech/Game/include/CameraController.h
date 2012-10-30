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

#if !defined(CAMERACONTROLLER_INCLUDED)
#define CAMERACONTROLLER_INCLUDED

#include "Reflection/include/Reflection.h"
#include "Math/include/Vector4.h"
#include "Math/include/Quaternion.h"
#include "Memory/include/Allocators.h"

namespace CoS
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
		COS_CLASS(CameraController, Object);
		COS_COMPLEX_PROPERTY(Offset, "Camera position offset in local camera space", Vector4, "(0,0,0,0)", 0);
		COS_COMPLEX_PROPERTY(Position, "Camera position in parent space", Vector4, "(0,0,0,0)", 0);
		COS_COMPLEX_PROPERTY(Orientation, "Camera orientation in parent space", Quaternion, "(1,0,0,0)", 0);
		COS_PROPERTY(PitchBias, "Camera pitch bias (radians)", float, 0, 0);
		COS_PROPERTY(YawBias, "Camera yaw bias (radians)", float, 0, 0);
		COS_PROPERTY(Enabled, "Camera enable/disable", bool, true, 0);

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

		COS_DECLARE_ALLOCATOR();

	protected:
		GfxCamera* m_pCamera;
		ComponentHost* m_pTarget;
	};
}

#endif // CAMERACONTROLLER_INCLUDED
