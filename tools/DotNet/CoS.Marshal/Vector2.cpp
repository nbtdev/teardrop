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
#include "Vector2.h"

using namespace CoS;
using namespace Math;
//---------------------------------------------------------------------------
Math::Vector2::Vector2()
{
}
//---------------------------------------------------------------------------
Math::Vector2::Vector2(const CoS::Vector2& other)
{
	m_vec = other;
}
//---------------------------------------------------------------------------
Math::Vector2::Vector2(const Math::Vector2 ^ other)
{
	m_vec = (other->m_vec);
}
//---------------------------------------------------------------------------
Math::Vector2::Vector2(float x, float y)
{
	m_vec = CoS::Vector2(x, y);
}
//---------------------------------------------------------------------------
Math::Vector2::~Vector2()
{
}
//---------------------------------------------------------------------------
Math::Vector2::!Vector2()
{
}
//---------------------------------------------------------------------------
float Math::Vector2::X::get()
{
	return m_vec->x;
}
//---------------------------------------------------------------------------
float Math::Vector2::Y::get()
{
	return m_vec->y;
}
//---------------------------------------------------------------------------
void Math::Vector2::X::set(float val)
{
	m_vec->x = val;
}
//---------------------------------------------------------------------------
void Math::Vector2::Y::set(float val)
{
	m_vec->y = val;
}
//---------------------------------------------------------------------------
Math::Vector2^ Math::Vector2::operator=(Vector2^ other)
{
	m_vec = other->m_vec;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector2^ Math::Vector2::operator=(const CoS::Vector2& other)
{
	*m_vec = other;
	return this;
}
//---------------------------------------------------------------------------
Math::Vector2^ Math::Vector2::operator+=(Vector2^ other)
{
	*m_vec += *(other->m_vec);
	return this;
}
//---------------------------------------------------------------------------
Math::Vector2^ Math::Vector2::operator-=(Vector2^ other)
{
	*m_vec -= *(other->m_vec);
	return this;
}
//---------------------------------------------------------------------------
float Math::Vector2::GetLength()
{
	return m_vec->length();
}
//---------------------------------------------------------------------------
float Math::Vector2::GetLengthSquared()
{
	return m_vec->length2();
}
//---------------------------------------------------------------------------
float Math::Vector2::GetDistance(Math::Vector2 ^ other)
{
	return m_vec->distance(*other->m_vec);
}
//---------------------------------------------------------------------------
float Math::Vector2::GetDistanceSquared(Math::Vector2 ^ other)
{
	return m_vec->distance2(*other->m_vec);
}
