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
#include "Matrix44.h"
#include "Vector4.h"

#include "Math/include/Vector4.h"

using namespace CoS;
using namespace Math;
//---------------------------------------------------------------------------
Math::Matrix44::Matrix44()
{
}
//---------------------------------------------------------------------------
Math::Matrix44::Matrix44(const CoS::Matrix44& other)
{
	m_mat = other;
}
//---------------------------------------------------------------------------
Math::Matrix44::Matrix44(const Math::Matrix44 ^ other)
{
	m_mat = (other->m_mat);
}
//---------------------------------------------------------------------------
//Math::Matrix44::Matrix44(float x, float y, float z, float w)
//{
//	m_q = CoS::Quaternion(x, y, z, w);
//}
//---------------------------------------------------------------------------
Math::Matrix44::~Matrix44()
{
}
//---------------------------------------------------------------------------
Math::Matrix44::!Matrix44()
{
}
