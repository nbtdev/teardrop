/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "CameraController.h"
#include "Gfx/Camera.h"
#include "Math/MathUtil.h"

using namespace Teardrop;
TD_CLASS_IMPL(CameraController);

CameraController::CameraController()
{
	mCamera = 0;
}

CameraController::~CameraController()
{
}

bool CameraController::initialize()
{
	// create main camera, set reasonable defaults
	mCamera = TD_NEW Gfx::Camera;
	mCamera->initialize();
	mCamera->setFovY(MathUtil::HALF_PI/1.5f);
	mCamera->setNearClip(1.5f);
	mCamera->setFarClip(2500);

	return true;
}

bool CameraController::destroy()
{
	if (mCamera)
		mCamera->destroy();

	delete mCamera;

	return true;
}

bool CameraController::update(float /*deltaT*/)
{
	// derived classes should implement this
	return true;
}

InputFilter* CameraController::getInputFilter() const
{
	// derived classes should implement this, if needed
	return 0;
}

void CameraController::setAspectRatio(float aspect)
{
	if (mCamera)
		mCamera->setAspect(aspect);
}

void CameraController::setFovY(float fovY, float /*time*/)
{
	// TODO: animate over time if non-zero
	if (mCamera)
		mCamera->setFovY(fovY);
}

void CameraController::setNearClip(float near)
{
	if (mCamera)
		mCamera->setNearClip(near);
}

void CameraController::setFarClip(float far)
{
	if (mCamera)
		mCamera->setFarClip(far);
}
