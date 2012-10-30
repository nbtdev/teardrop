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
#include "Vector4.h"

using namespace CoS;
using namespace Math;
//---------------------------------------------------------------------------
Math::Vector4::Vector4()
{
}
//---------------------------------------------------------------------------
Math::Vector4::Vector4(const CoS::Vector4& other)
{
	m_vec = other;
}
//---------------------------------------------------------------------------
Math::Vector4::Vector4(const Math::Vector4 ^ other)
{
	m_vec = (other->m_vec);
}
//---------------------------------------------------------------------------
Math::Vector4::Vector4(float x, float y, float z, float w)
{
	m_vec = CoS::Vector4(x, y, z, w);
}
//---------------------------------------------------------------------------
Math::Vector4::~Vector4()
{
}
//---------------------------------------------------------------------------
Math::Vector4::!Vector4()
{
}
//---------------------------------------------------------------------------
float Math::Vector4::X::get()
{
	return m_vec->x;
}
//---------------------------------------------------------------------------
float Math::Vector4::Y::get()
{
	return m_vec->y;
}
//---------------------------------------------------------------------------
float Math::Vector4::Z::get()
{
	return m_vec->z;
}
//---------------------------------------------------------------------------
float Math::Vector4::W::get()
{
	return m_vec->w;
}
//---------------------------------------------------------------------------
void Math::Vector4::X::set(float val)
{
	m_vec->x = val;
}
//---------------------------------------------------------------------------
void Math::Vector4::Y::set(float val)
{
	m_vec->y = val;
}
//---------------------------------------------------------------------------
void Math::Vector4::Z::set(float val)
{
	m_vec->z = val;
}
//---------------------------------------------------------------------------
void Math::Vector4::W::set(float val)
{
	m_vec->w = val;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator=(Vector4^ other)
{
	m_vec = other->m_vec;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator=(const CoS::Vector4& other)
{
	*m_vec = other;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator+=(Vector4^ other)
{
	*m_vec += *(other->m_vec);
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator-=(Vector4^ other)
{
	*m_vec -= *(other->m_vec);
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator*=(float factor)
{
	*m_vec *= factor;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator/=(float factor)
{
	*m_vec /= factor;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::operator-()
{
	Vector4^ rtn = gcnew Vector4();
	rtn->m_vec = -(*m_vec);
	return rtn;
}
//---------------------------------------------------------------------------
float Math::Vector4::Dot(Vector4^ other)
{
	return CoS::dot(*m_vec, *(other->m_vec));
}
//---------------------------------------------------------------------------
float Math::Vector4::Dot(Math::Vector4^ a, Math::Vector4^ b)
{
	return a->Dot(b);
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::Cross(
	Math::Vector4^ a, Math::Vector4^ b)
{
	Vector4^ rtn = gcnew Vector4();
	CoS::cross(*(rtn->m_vec), *(a->m_vec), *(b->m_vec));
	return rtn;
}
//---------------------------------------------------------------------------
float Math::Vector4::GetLength()
{
	return m_vec->getLength();
}
//---------------------------------------------------------------------------
float Math::Vector4::GetLengthSquared()
{
	return m_vec->getLength2();
}
//---------------------------------------------------------------------------
float Math::Vector4::Normalize()
{
	return m_vec->normalize();
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::GetNormalized()
{
	Vector4^ rtn = gcnew Vector4(m_vec->getNormalized());
	return rtn;
}
//---------------------------------------------------------------------------
Math::Vector4^ Math::Vector4::ApplyScale(Vector4^ scaleVec)
{
	m_vec->applyScale(*(scaleVec->m_vec));
	return this;
}
//---------------------------------------------------------------------------
void Math::Vector4::MakeAbsolute()
{
	m_vec->makeAbsolute();
}
