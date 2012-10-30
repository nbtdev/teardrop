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

#if !defined(MATRIX44_MARSHAL_INCLUDED)
#define MATRIX44_MARSHAL_INCLUDED

// include the native version
#include "Math/include/Matrix44.h"
#include "Embedded.h"

namespace CoS
{
	namespace Math
	{
		ref class Vector4;

		public ref class Matrix44
		{
		internal:
			Embedded<CoS::Matrix44> m_mat;

		public:
			Matrix44();
			Matrix44(const CoS::Matrix44& other);
			Matrix44(const Math::Matrix44 ^ other);
			//Matrix44(float x, float y, float z, float w);
			~Matrix44();
			!Matrix44();

			//! STATIC DEFINES
			static const Matrix44 ^IDENTITY = gcnew Matrix44(CoS::Matrix44::IDENTITY);

			//! PROPERTIES

			//! OPERATORS

			//! METHODS
			const CoS::Matrix44& _getInternal() { return *m_mat; }

			//! (STATIC) FREE FUNCTIONS
		};
	}
}


#endif // MATRIX44_MARSHAL_INCLUDED
