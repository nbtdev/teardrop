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

#if !defined(CAMERA_MARSHAL_INCLUDED)
#define CAMERA_MARSHAL_INCLUDED

namespace CoS
{
	class GfxCamera;

	namespace Math
	{
		ref class Vector4;
		ref class Quaternion;
	}

	namespace Gfx
	{
		public ref class Camera
		{
			CoS::GfxCamera* m_pCam;

		public:
			Camera();
			~Camera();
			!Camera();

			void Yaw(float radians);
			void Pitch(float radians);
			void Roll(float radians);
			void Translate(Math::Vector4^ trans);

			property Math::Vector4^ Position { Math::Vector4^ get(); void set(Math::Vector4^); }
			property Math::Vector4^ LookAt { Math::Vector4^ get(); void set(Math::Vector4^); }
			property Math::Quaternion^ Orientation { Math::Quaternion^ get(); void set(Math::Quaternion^); }
			property float FovY { float get(); void set(float); }
			property float NearClip { float get(); void set(float); }
			property float FarClip { float get(); void set(float); }
			property float Aspect { float get(); void set(float); }

		internal:
			CoS::GfxCamera* _getNative() { return m_pCam; }

		private:
		};
	}
}

#endif // CAMERA_MARSHAL_INCLUDED
