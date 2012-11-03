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

#include "CameraController.h"
#include "Gfx/GfxCamera.h"
#include "Math/MathUtil.h"

using namespace CoS;
//---------------------------------------------------------------------------
COS_CLASS_IMPL(CameraController);
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
	m_pCamera = COS_NEW GfxCamera;
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
