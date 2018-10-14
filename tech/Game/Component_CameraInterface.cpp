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

#include "Component_CameraInterface.h"
#include "CameraController.h"
#include "SceneRenderer.h"
#include "Gfx/Camera.h"
#include "Math/Transform.h"

using namespace Teardrop;
using Reflection::ClassDef;
using Reflection::PropertyDef;
using namespace std::placeholders;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(CameraInterfaceComponent);
//---------------------------------------------------------------------------
CameraInterfaceComponent::CameraInterfaceComponent()
{
	m_pHost = 0;
	m_pCamControl = 0;
}
//---------------------------------------------------------------------------
CameraInterfaceComponent::~CameraInterfaceComponent()
{
}
//---------------------------------------------------------------------------
void CameraInterfaceComponent::onInstanceCreated()
{
	setServerComponent(false); // render not needed on a server
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::initialize()
{
	bool rtn = changeCameraController();

	// we don't want to put this in the c'tor since it will fire when the 
	// camera controller property is set
    PropertyChanged.bind(std::bind(&CameraInterfaceComponent::onPropertyChanged, this, _1));

	return rtn;
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::destroy()
{
    PropertyChanged.unbind(std::bind(&CameraInterfaceComponent::onPropertyChanged, this, _1));

	if (m_pCamControl)
		m_pCamControl->destroy();
	
	delete m_pCamControl;

	return true;
}
//---------------------------------------------------------------------------
//void CameraInterfaceComponent::setupSceneRenderer(SceneRenderer* pRend)
//{
//	if (m_pCamControl && m_pCamControl->camera())
//		pRend->setCamera(m_pCamControl->camera());
//}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::getCameraTransform(Transform& xform)
{
	if (m_pCamControl)
	{
		Gfx::Camera* pCam = m_pCamControl->camera();
		if (pCam)
		{
			xform.scale = Vector4::UNIT_SCALE;
			xform.trans = pCam->getPosition();
			xform.rot = pCam->getOrientation();
			return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::getCameraViewportWorldRay(
	Ray& ray, float x, float y)
{
	if (m_pCamControl)
	{
		Gfx::Camera* pCam = m_pCamControl->camera();
		if (pCam)
			return pCam->getViewportWorldRay(ray, x, y);
	}

	return false;
}
//---------------------------------------------------------------------------
const Plane* CameraInterfaceComponent::getViewFrustum()
{
	if (m_pCamControl)
		if (m_pCamControl->camera())
			return m_pCamControl->camera()->getFrustumPlanes();

	return 0;
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::update(float deltaT)
{
	if (m_pCamControl)
	{
		m_pCamControl->setOffset(getOffset());
		m_pCamControl->setYawBias(getYawBias());
		m_pCamControl->setPitchBias(getPitchBias());
		m_pCamControl->setEnabled(getEnabled());
		m_pCamControl->update(deltaT);
	}

	return true;
}
//---------------------------------------------------------------------------
void CameraInterfaceComponent::onPropertyChanged(const PropertyDef* pPropDef)
{
	// handle changes to camera controller
	if (String("CameraController") == pPropDef->getName())
		changeCameraController();

	if (m_pCamControl)
	{
		// handle changes to aspect ratio
		if (String("AspectRatio") == pPropDef->getName())
			m_pCamControl->setAspectRatio(getAspectRatio());

		// handle changes to FovY
		if (String("FovY") == pPropDef->getName())
			m_pCamControl->setFovY(getFovY());

		// handle changes to aspect ratio
		if (String("FarClip") == pPropDef->getName())
			m_pCamControl->setFarClip(getFarClip());

		// handle changes to aspect ratio
		if (String("NearClip") == pPropDef->getName())
			m_pCamControl->setNearClip(getNearClip());
	}
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::changeCameraController()
{
	// create an instance of whatever camera is defined on us
	ClassDef* pClassDef	= ClassDef::findClassDef(getCameraController());
	
	CameraController* pCamControl = 0;
	if (pClassDef)
		pCamControl = static_cast<CameraController*>(pClassDef->createInstance());

	if (!pCamControl)
		return false;

	// otherwise, clean up any existing controller and set controller to the new one
	if (m_pCamControl)
	{
		m_pCamControl->destroy();

		// TODO: don't we want to call a ClassDef::destroyInstance() method instead?
		delete m_pCamControl;
	}

	pCamControl->initialize();
	//pCamControl->setTargetPos(m_pHost);
	m_pCamControl = pCamControl;

	return true;
}
