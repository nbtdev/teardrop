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

#if !defined(TRANSFORM_MARSHAL_INCLUDED)
#define TRANSFORM_MARSHAL_INCLUDED

// include the native version
#include "Math/include/Transform.h"
#include "Embedded.h"

namespace CoS
{
	namespace Math
	{
		ref class Vector4;
		ref class Quaternion;

		public ref class Transform
		{
		internal:
			Embedded<CoS::Transform> m_xform;

		public:
			Transform();
			Transform(const CoS::Transform& other);
			Transform(const Math::Transform ^ other);
			Transform(Vector4^ trans, Quaternion^ rot, Vector4^ scale);
			~Transform();
			!Transform();

			//! STATIC DEFINES
			static const Transform ^IDENTITY = gcnew Transform(CoS::Transform::IDENTITY);

			//! PROPERTIES
			[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
			property Vector4^ Translation { Vector4^ get(); void set(Vector4^); }
			[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
			property Quaternion^ Orientation { Quaternion^ get(); void set(Quaternion^); }
			[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
			property Vector4^ Scale { Vector4^ get(); void set(Vector4^); }

			//! OPERATORS
			Transform^ operator=(Transform^ other);
			Transform^ operator=(const CoS::Transform& other);

			//! METHODS
			const CoS::Transform& _getInternal() { return *m_xform; }

			//! (STATIC) FREE FUNCTIONS
		};
	}
}

#endif // TRANSFORM_MARSHAL_INCLUDED
