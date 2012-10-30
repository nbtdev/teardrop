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

#if !defined(QUATERNION_MARSHAL_INCLUDED)
#define QUATERNION_MARSHAL_INCLUDED

// include the native version
#include "Math/include/Quaternion.h"
#include "Embedded.h"

namespace CoS
{
	namespace Math
	{
		ref class Vector4;
		ref class Matrix44;

		public ref class Quaternion
		{
		internal:
			Embedded<CoS::Quaternion> m_q;

		public:
			Quaternion();
			Quaternion(const CoS::Quaternion& other);
			Quaternion(const Math::Quaternion ^ other);
			Quaternion(float x, float y, float z, float w);
			~Quaternion();
			!Quaternion();

			//! STATIC DEFINES
			static const Quaternion ^IDENTITY = gcnew Quaternion(CoS::Quaternion::IDENTITY);

			//! PROPERTIES
			property float X { float get(); void set(float); }
			property float Y { float get(); void set(float); }
			property float Z { float get(); void set(float); }
			property float W { float get(); void set(float); }

			//! OPERATORS
			Quaternion^ operator=(const Quaternion^ other);
			Quaternion^ operator=(const CoS::Quaternion& other);
			Quaternion^ operator*(Quaternion^ q);
			Quaternion^ operator-();
			bool operator==(Quaternion^ other);
			bool operator!=(Quaternion^ other);

			//! METHODS
			void fromAngleAxis(float angle /*radians*/, Vector4^ axis);
			void toAngleAxis(float% angle /*radians*/, Vector4^ axis);
			void toAzimuthElevation(float% azimuth, float% elevation);
			void toRotationMatrix(/*out*/Matrix44^ out);
			float normalize();
			float getNormalized(/*out*/Quaternion^ q);
			const CoS::Quaternion& _getInternal() { return *m_q; }

			//! (STATIC) FREE FUNCTIONS
			static Vector4^ operator*(Quaternion^ q, Vector4^ v); // rotate vector by quaternion
			static void invert(/*out*/Quaternion^ q); // inverts in place, assumes unit quaternion!!!
		};
	}
}


#endif // QUATERNION_MARSHAL_INCLUDED
