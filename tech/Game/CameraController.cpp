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

void CameraController::injectMouseMove(int /*absX*/, int /*absY*/, int /*relX*//* =0 */, int /*relY*//* =0 */)
{

}

void CameraController::injectMouseWheel(int /*absZ*/, int /*relZ*//* =0 */)
{

}
