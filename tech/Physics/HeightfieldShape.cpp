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
#include "HeightfieldShape.h"
#include "Util/Environment.h"
#include "Util/Logger.h"
#include "Memory/Allocators.h"

using namespace CoS;
//---------------------------------------------------------------------------
HeightfieldShape::HeightfieldShape()
{
	m_pData = 0;
}
//---------------------------------------------------------------------------
HeightfieldShape::~HeightfieldShape()
{
}
//---------------------------------------------------------------------------
bool HeightfieldShape::initialize(
	void* pData, // this object makes a copy of this data
	size_t width, 
	size_t height,
	size_t bpp, // bytes/pixel, i.e. 8-bit greyscale = 1 byte/pixel
	const Vector4& scale // world scaling terms (x, y, z)
)
{
	if (!pData)
	{
		return false;
	}

	size_t bufSize = width * height * bpp;
	m_pData = COS_ALLOCATE_ALIGNED(DEFAULT, bufSize, 16);
	memcpy(m_pData, pData, bufSize);

	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_scale = scale;

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShape::destroy()
{
	if (m_pData)	
	{
		COS_DEALLOCATE(DEFAULT, m_pData);
		m_pData = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
bool HeightfieldShape::update(float deltaT)
{
	return true;
}
