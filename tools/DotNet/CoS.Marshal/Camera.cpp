/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgements in Readme.txt

This software is provided free-of-charge for non-commercial use, under the 
GNU Lesser General Public License as published by the Free Software Foundation; 
either version 2 of the License, or (at your option) any later version. This 
program is distributed in the hope that it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License 
along with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

For commercial applications, contact the Clash Of Steel Team at 
info@clashofsteel.net for commercial licensing terms and information.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "Camera.h"
#include "Vector4.h"
#include "Quaternion.h"
#include <msclr/marshal.h>
#include "Gfx/include/GfxCommon.h"
#include "Gfx/include/GfxCamera.h"

using namespace CoS;
using namespace Gfx;
using namespace msclr::interop;
//---------------------------------------------------------------------------
Camera::Camera()
{
	m_pCam = COS_NEW GfxCamera;
}
//---------------------------------------------------------------------------
Camera::~Camera()
{
	delete m_pCam;
}
//---------------------------------------------------------------------------
Camera::!Camera()
{
}
//---------------------------------------------------------------------------
void Camera::Yaw(float radians)
{
	m_pCam->yaw(radians);
}
//---------------------------------------------------------------------------
void Camera::Pitch(float radians)
{
	m_pCam->pitch(radians);
}
//---------------------------------------------------------------------------
void Camera::Roll(float radians)
{
	m_pCam->roll(radians);
}
//---------------------------------------------------------------------------
void Camera::Translate(Math::Vector4^ trans)
{
	m_pCam->translate(
		CoS::Vector4(trans->X, trans->Y, trans->Z, 0)
		);
}
//---------------------------------------------------------------------------
Math::Vector4^ Camera::Position::get()
{
	return gcnew Math::Vector4(m_pCam->getPosition());
}
//---------------------------------------------------------------------------
void Camera::Position::set(Math::Vector4^ pos)
{
	m_pCam->setPosition(
		CoS::Vector4(pos->X, pos->Y, pos->Z, pos->W)
		);
}
//---------------------------------------------------------------------------
Math::Vector4^ Camera::LookAt::get()
{
	return gcnew Math::Vector4(m_pCam->getLookAt());
}
//---------------------------------------------------------------------------
void Camera::LookAt::set(Math::Vector4^ lookAt)
{
	m_pCam->setLookAt(
		CoS::Vector4(lookAt->X, lookAt->Y, lookAt->Z, lookAt->W)
		);
}
//---------------------------------------------------------------------------
Math::Quaternion^ Camera::Orientation::get()
{
	return gcnew Math::Quaternion(m_pCam->getOrientation());
}
//---------------------------------------------------------------------------
void Camera::Orientation::set(Math::Quaternion^ rot)
{
	m_pCam->setOrientation(
		CoS::Quaternion(rot->W, rot->X, rot->Y, rot->Z)
		);
}
//---------------------------------------------------------------------------
float Camera::FovY::get()
{
	return m_pCam->getFovY();
}
//---------------------------------------------------------------------------
void Camera::FovY::set(float val)
{
	m_pCam->setFovY(val);
}
//---------------------------------------------------------------------------
float Camera::NearClip::get()
{
	return m_pCam->getNearClip();
}
//---------------------------------------------------------------------------
void Camera::NearClip::set(float val)
{
	m_pCam->setNearClip(val);
}
//---------------------------------------------------------------------------
float Camera::FarClip::get()
{
	return m_pCam->getFarClip();
}
//---------------------------------------------------------------------------
void Camera::FarClip::set(float val)
{
	m_pCam->setFarClip(val);
}
//---------------------------------------------------------------------------
float Camera::Aspect::get()
{
	return m_pCam->getAspect();
}
//---------------------------------------------------------------------------
void Camera::Aspect::set(float val)
{
	m_pCam->setAspect(val);
}
