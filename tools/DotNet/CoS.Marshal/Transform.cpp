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
#include "Transform.h"
#include "Vector4.h"
#include "Quaternion.h"

#include "Math/include/Transform.h"
#include "Math/include/Vector4.h"
#include "Math/include/Quaternion.h"

using namespace CoS;
using namespace Math;
//---------------------------------------------------------------------------
Math::Transform::Transform()
{
}
//---------------------------------------------------------------------------
Math::Transform::Transform(const CoS::Transform& other)
{
	m_xform = other;
}
//---------------------------------------------------------------------------
Math::Transform::Transform(const Math::Transform ^ other)
{
	m_xform = (other->m_xform);
}
//---------------------------------------------------------------------------
Math::Transform::Transform(Vector4^ trans, Quaternion^ rot, Vector4^ scale)
{
	m_xform = CoS::Transform(*(trans->m_vec), *(rot->m_q), *(scale->m_vec));
}
//---------------------------------------------------------------------------
Math::Transform::~Transform()
{
}
//---------------------------------------------------------------------------
Math::Transform::!Transform()
{
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Transform::Translation::get()
{
	return gcnew Vector4(m_xform->trans);
}
//---------------------------------------------------------------------------
Math::Quaternion^ Math::Transform::Orientation::get()
{
	return gcnew Quaternion(m_xform->rot);
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Transform::Scale::get()
{
	return gcnew Vector4(m_xform->scale);
}
//---------------------------------------------------------------------------
void Math::Transform::Translation::set(Math::Vector4^ val)
{
	m_xform->trans = *(val->m_vec);
}
//---------------------------------------------------------------------------
void Math::Transform::Orientation::set(Math::Quaternion^ val)
{
	m_xform->rot = *(val->m_q);
}
//---------------------------------------------------------------------------
void Math::Transform::Scale::set(Math::Vector4^ val)
{
	m_xform->scale = *(val->m_vec);
}
//---------------------------------------------------------------------------
Math::Transform^ Math::Transform::operator=(Transform^ other)
{
	m_xform = other->m_xform;
	return this;
}
//---------------------------------------------------------------------------
Math::Transform^ Math::Transform::operator=(const CoS::Transform& other)
{
	*m_xform = other;
	return this;
}
