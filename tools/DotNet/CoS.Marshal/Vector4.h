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

#if !defined(VECTOR4_MARSHAL_INCLUDED)
#define VECTOR4_MARSHAL_INCLUDED

// include the native version
#include "Math/include/Vector4.h"
#include "Embedded.h"

namespace CoS
{
	namespace Math
	{
		public ref class Vector4
		{
		internal:
			Embedded<CoS::Vector4> m_vec;

		public:
			Vector4();
			Vector4(const CoS::Vector4& other);
			Vector4(const Math::Vector4 ^ other);
			Vector4(float x, float y, float z, float w);
			~Vector4();
			!Vector4();

			//! STATIC DEFINES
			static const Vector4 ^ZERO = gcnew Vector4(CoS::Vector4::ZERO);
			static const Vector4 ^UNIT_SCALE = gcnew Vector4(CoS::Vector4::UNIT_SCALE);
			static const Vector4 ^UNIT_X = gcnew Vector4(CoS::Vector4::UNIT_X);
			static const Vector4 ^UNIT_Y = gcnew Vector4(CoS::Vector4::UNIT_Y);
			static const Vector4 ^UNIT_Z = gcnew Vector4(CoS::Vector4::UNIT_X);
			static const Vector4 ^NEGATIVE_INFINITY = gcnew Vector4(CoS::Vector4::NEGATIVE_INFINITY);
			static const Vector4 ^POSITIVE_INFINITY = gcnew Vector4(CoS::Vector4::POSITIVE_INFINITY);

			//! PROPERTIES
			property float X { float get(); void set(float); }
			property float Y { float get(); void set(float); }
			property float Z { float get(); void set(float); }
			property float W { float get(); void set(float); }

			//! OPERATORS
			Vector4^ operator=(Vector4^ other);
			Vector4^ operator=(const CoS::Vector4& other);
			Vector4^ operator+=(Vector4^ other);
			Vector4^ operator-=(Vector4^ other);
			Vector4^ operator*=(float factor);
			Vector4^ operator/=(float factor);
			Vector4^ operator-();

			//! METHODS
			float Dot(Vector4^ other);
			float GetLength();
			float GetLengthSquared();
			float Normalize();
			Vector4^ GetNormalized();
			Vector4^ ApplyScale(Vector4^ scaleVec);
			void MakeAbsolute();
			const CoS::Vector4& _getInternal() { return *m_vec; }

			//! (STATIC) FREE FUNCTIONS
			static float Dot(Vector4^ a, Vector4^ b);
			static Vector4^ Cross(Vector4^ a, Vector4^ b);
		};
	}
}


#endif // VECTOR4_MARSHAL_INCLUDED
