/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(CAMERAINTERFACE_COMPONENT_INCLUDED)
#define CAMERAINTERFACE_COMPONENT_INCLUDED

#include "Game/Component.h"
#include "Util/_String.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace Teardrop
{
	class CameraController;
	class Ray;
	class Plane;
	class Transform;
	class SceneRenderer;

	class CameraInterfaceComponent
		: public Component
	{
	public:
		TD_CLASS(CameraInterfaceComponent, Component);
		TD_PROPERTY(CameraController, "Name of camera controller to attach to host object", String, "", 0);
		TD_PROPERTY(MountPoint, "Name of mount point on host object to use for camera origin", String, "", 0);
		TD_COMPLEX_PROPERTY(Offset, 
			"Camera offset from host object's origin (local space), "
			"useful if camera not associated with a mount point (Offset "
			"uses mount point transform as origin if mount point is set)", 
			Vector4, "(0,0,0,0)", 0);
		TD_PROPERTY(PitchBias, "Camera pitch bias (in radians, added to current camera rotation)", float, 0, 0);
		TD_PROPERTY(YawBias, "Camera pitch bias (in radians, added to current camera rotation)", float, 0, 0);
		TD_PROPERTY(NearClip, "Camera far clip distance", float, 2500, 0);
		TD_PROPERTY(FarClip, "Camera near clip distance", float, 1.5, 0);
		TD_PROPERTY(FovY, "Camera field of view (vertical)", float, 1, 0);
		TD_PROPERTY(AspectRatio, "Camera aspect ratio", float, 1, 0);
		TD_PROPERTY(FovYAnimateTime, "Camera vertical field-of-view animation time", float, 0, 0);
		TD_PROPERTY(Enabled, "Enable/disable camera", bool, true, 0);

		CameraInterfaceComponent();
		~CameraInterfaceComponent();

		bool initialize();
		bool destroy();
		bool update(float deltaT);

		bool getCameraViewportWorldRay(Ray& ray, float x, float y);
		const Plane* getViewFrustum();
		bool getCameraTransform(Transform& xform);
		void setupSceneRenderer(SceneRenderer* pStep);

	protected:
		CameraController* m_pCamControl;

		bool changeCameraController();

		// to be able to react to changes in value of some properties
		void onPropertyChanged(const Reflection::PropertyDef* pPropDef);

	private:
		void onInstanceCreated();
	};
}

#endif // CAMERAINTERFACE_COMPONENT_INCLUDED
