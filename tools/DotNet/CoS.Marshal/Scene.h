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

#if !defined(SCENE_MARSHAL_INCLUDED)
#define SCENE_MARSHAL_INCLUDED

namespace CoS
{
	class Scene;
	ref class Object;

	namespace Gfx
	{
		ref class Camera;
	}

	namespace Game
	{
		ref class Zone;

		public ref class Scene
		{
			CoS::Scene* m_pScene;

		public:
			bool Load(System::String^ filepath);
			bool Save(System::String^ filepath);

			delegate void OnZoneCreated(Zone^ zone);
			delegate void OnZoneObjectCreated(CoS::Object^ obj);
			delegate void OnZoneObjectAdded(Zone^ zone, CoS::Object^ obj);

			event OnZoneCreated^ ZoneCreated;
			event OnZoneObjectCreated^ ZoneObjectCreated;
			event OnZoneObjectAdded^ ZoneObjectAdded;

			Scene();
			~Scene();
			!Scene();

			bool Initialize();
			bool Update(double deltaT);
			bool Destroy();

			System::Collections::Generic::List<CoS::Object^>^ GetVisibleObjects(
				CoS::Gfx::Camera^ cam);

			void _RaiseZoneCreated(Zone^);
			void _RaiseZoneObjectCreated(CoS::Object^);
			void _RaiseZoneObjectAdded(Zone^, CoS::Object^);

		private:
		};
	}
}

#endif // SCENE_MARSHAL_INCLUDED
