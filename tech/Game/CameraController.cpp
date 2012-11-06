/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "CameraController.h"
#include "Gfx/GfxCamera.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(CameraController);
//---------------------------------------------------------------------------
CameraController::CameraController()
{
	m_pTarget = 0;
	m_pCamera = 0;
}
//---------------------------------------------------------------------------
CameraController::~CameraController()
{
}
//---------------------------------------------------------------------------
void CameraController::setTarget(ComponentHost* pTarget)
{
	m_pTarget = pTarget;
}
//---------------------------------------------------------------------------
bool CameraController::initialize()
{
	// create main camera, set reasonable defaults
	m_pCamera = TD_NEW GfxCamera;
	m_pCamera->initialize();
	m_pCamera->setFovY(MathUtil::HALF_PI/1.5f);
	m_pCamera->setNearClip(1.5f);
	m_pCamera->setFarClip(2500);

	return true;
}
//---------------------------------------------------------------------------
bool CameraController::destroy()
{
	if (m_pCamera)
		m_pCamera->destroy();

	delete m_pCamera;

	return true;
}
//---------------------------------------------------------------------------
bool CameraController::update(float /*deltaT*/)
{
	// derived classes should implement this
	return true;
}
//---------------------------------------------------------------------------
InputFilter* CameraController::getInputFilter() const
{
	// derived classes should implement this, if needed
	return 0;
}
//---------------------------------------------------------------------------
void CameraController::setAspectRatio(float aspect)
{
	if (m_pCamera)
		m_pCamera->setAspect(aspect);
}
//---------------------------------------------------------------------------
void CameraController::setFovY(float fovY, float /*time*/)
{
	// TODO: animate over time if non-zero
	if (m_pCamera)
		m_pCamera->setFovY(fovY);
}
//---------------------------------------------------------------------------
void CameraController::setNearClip(float near)
{
	if (m_pCamera)
		m_pCamera->setNearClip(near);
}
//---------------------------------------------------------------------------
void CameraController::setFarClip(float far)
{
	if (m_pCamera)
		m_pCamera->setFarClip(far);
}
