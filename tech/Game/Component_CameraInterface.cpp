/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component_CameraInterface.h"
#include "CameraController.h"
#include "SceneRenderer.h"
#include "Gfx/GfxCamera.h"
#include "Math/Transform.h"

using namespace Teardrop;
using Reflection::ClassDef;
using Reflection::PropertyDef;
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
	PropertyChanged.bind(this, &CameraInterfaceComponent::onPropertyChanged);

	return rtn;
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::destroy()
{
	if (m_pCamControl)
		m_pCamControl->destroy();
	
	delete m_pCamControl;

	return true;
}
//---------------------------------------------------------------------------
void CameraInterfaceComponent::setupSceneRenderer(SceneRenderer* pRend)
{
	if (m_pCamControl && m_pCamControl->getControlledCamera())
		pRend->setCamera(m_pCamControl->getControlledCamera());
}
//---------------------------------------------------------------------------
bool CameraInterfaceComponent::getCameraTransform(Transform& xform)
{
	if (m_pCamControl)
	{
		GfxCamera* pCam = m_pCamControl->getControlledCamera();
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
		GfxCamera* pCam = m_pCamControl->getControlledCamera();
		if (pCam)
			return pCam->getViewportWorldRay(ray, x, y);
	}

	return false;
}
//---------------------------------------------------------------------------
const Plane* CameraInterfaceComponent::getViewFrustum()
{
	if (m_pCamControl)
		if (m_pCamControl->getControlledCamera())
			return m_pCamControl->getControlledCamera()->getFrustumPlanes();

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
		pCamControl = static_cast<CameraController*>(pClassDef->createInstance(0));

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
	pCamControl->setTarget(m_pHost);
	m_pCamControl = pCamControl;

	return true;
}
