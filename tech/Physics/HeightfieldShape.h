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

#if !defined(HEIGHTFIELDSHAPE_INCLUDED)
#define HEIGHTFIELDSHAPE_INCLUDED

#include "Math/Vector4.h"

namespace CoS
{
	class Vector4;

	class HeightfieldShape
	{
	public:
		HeightfieldShape();
		virtual ~HeightfieldShape();

		virtual bool initialize(
			void* pData, 
			size_t width, 
			size_t height,
			size_t bpp, // bytes/pixel, i.e. 8-bit greyscale = 1 byte/pixel
			const Vector4& scale // world scaling terms (x, y, z)
			);
		virtual bool destroy();
		virtual bool update(float deltaT);
		
		COS_DECLARE_ALLOCATOR();

	protected:
		size_t m_width;
		size_t m_height;
		size_t m_bpp;
		Vector4 m_scale;
		void* m_pData;
	};
}

#endif // SHAPE_INCLUDED
