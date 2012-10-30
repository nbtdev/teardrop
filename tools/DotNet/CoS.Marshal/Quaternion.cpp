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
#include "Quaternion.h"
#include "Vector4.h"
#include "Matrix44.h"

#include "Math/include/Vector4.h"
#include "Math/include/Matrix44.h"

using namespace CoS;
using namespace Math;
//---------------------------------------------------------------------------
Math::Quaternion::Quaternion()
{
}
//---------------------------------------------------------------------------
Math::Quaternion::Quaternion(const CoS::Quaternion& other)
{
	m_q = other;
}
//---------------------------------------------------------------------------
Math::Quaternion::Quaternion(const Math::Quaternion ^ other)
{
	m_q = (other->m_q);
}
//---------------------------------------------------------------------------
Math::Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_q = CoS::Quaternion(x, y, z, w);
}
//---------------------------------------------------------------------------
Math::Quaternion::~Quaternion()
{
}
//---------------------------------------------------------------------------
Math::Quaternion::!Quaternion()
{
}
//---------------------------------------------------------------------------
float Math::Quaternion::X::get()
{
	return m_q->x;
}
//---------------------------------------------------------------------------
float Math::Quaternion::Y::get()
{
	return m_q->y;
}
//---------------------------------------------------------------------------
float Math::Quaternion::Z::get()
{
	return m_q->z;
}
//---------------------------------------------------------------------------
float Math::Quaternion::W::get()
{
	return m_q->w;
}
//---------------------------------------------------------------------------
void Math::Quaternion::X::set(float val)
{
	m_q->x = val;
}
//---------------------------------------------------------------------------
void Math::Quaternion::Y::set(float val)
{
	m_q->y = val;
}
//---------------------------------------------------------------------------
void Math::Quaternion::Z::set(float val)
{
	m_q->z = val;
}
//---------------------------------------------------------------------------
void Math::Quaternion::W::set(float val)
{
	m_q->w = val;
}
//---------------------------------------------------------------------------
Math::Quaternion^ Math::Quaternion::operator=(const Quaternion^ other)
{
	m_q = other->m_q;
	return this;
}
//---------------------------------------------------------------------------
Math::Quaternion^ Math::Quaternion::operator=(const CoS::Quaternion& other)
{
	*m_q = other;
	return this;
}
//---------------------------------------------------------------------------
Math::Quaternion^ Math::Quaternion::operator*(Quaternion^ other)
{
	return gcnew Math::Quaternion((*m_q) * *(other->m_q));
}
//---------------------------------------------------------------------------
Math::Quaternion^ Math::Quaternion::operator-()
{
	return gcnew Math::Quaternion(-(*m_q));
}
//---------------------------------------------------------------------------
bool Math::Quaternion::operator==(Quaternion^ other)
{
	return (*m_q == *(other->m_q));
}
//---------------------------------------------------------------------------
bool Math::Quaternion::operator!=(Quaternion^ other)
{
	return !(this == other);
}
//---------------------------------------------------------------------------
void Math::Quaternion::fromAngleAxis(float angle, Vector4^ axis)
{
	m_q->fromAngleAxis(angle, *(axis->m_vec));
}
//---------------------------------------------------------------------------
void Math::Quaternion::toAngleAxis(float% angle /*radians*/, Vector4^ axis)
{
}
//---------------------------------------------------------------------------
void Math::Quaternion::toAzimuthElevation(float% azimuth, float% elevation)
{
	float az, el;
	m_q->toAzimuthElevation(az, el);
	azimuth = az;
	elevation = el;
}
//---------------------------------------------------------------------------
void Math::Quaternion::toRotationMatrix(/*out*/Matrix44^ out)
{
}
//---------------------------------------------------------------------------
float Math::Quaternion::normalize()
{
	return m_q->normalize();
}
//---------------------------------------------------------------------------
float Math::Quaternion::getNormalized(/*out*/Quaternion^ q)
{
	q = gcnew Math::Quaternion(this);
	return q->normalize();
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Quaternion::operator*(Quaternion^ q, Vector4^ v)
{
	return gcnew Math::Vector4(*(q->m_q) * *(v->m_vec));
}
//---------------------------------------------------------------------------
void Math::Quaternion::invert(/*out*/Quaternion^ q)
{
	CoS::invert(*(q->m_q));
}
