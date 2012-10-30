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

#if !defined(RUNTIME_MARSHAL_INCLUDED)
#define RUNTIME_MARSHAL_INCLUDED

/*
	This class is responsible for the base, low-level housekeeping that normally 
	would take place in a "Game" class in a C++ application that uses the CoS
	libraries. Things like setting up memory and allocators, and making sure that 
	all of the systems that need allocators set have them set, and so on.

	There should only ever be one instance of this class made, and it should 
	happen as part of the startup for the marshaling library; there should not be
	any need to create one of these manually.
*/

#include "Embedded.h"
#include "Game/include/Manifest.h"
#include "Game/include/Input.h"
#include "Resource/include/ResourceManager.h"
#include "Stream/include/FileStream.h"
#include "Util/include/Timer.h"

namespace CoS
{
	namespace Game
	{
		ref class Manifest;
	}

	namespace Gfx
	{
		ref class Renderer;
		ref class RenderWindow;
	}

	public ref class Runtime
	{
		Embedded<CoS::Manifest> m_manifest;
		Embedded<CoS::FileStream> m_logStrm;
		Embedded<CoS::Timer> m_timer;
		Embedded<CoS::ResourceManager> m_resMgr;
		Embedded<CoS::Input> m_input;

		static System::Reflection::Assembly^ m_assembly = nullptr;

		System::Windows::Forms::Control^ m_rootWindow;

	public:
		Runtime();
		~Runtime();
		!Runtime();

		bool Initialize();
		void Shutdown();

		property Game::Manifest^ Manifest { Game::Manifest^ get(); }
		static property System::Reflection::Assembly^ Classdef { System::Reflection::Assembly^ get() ; }
		static property System::String^ DataPath { System::String^ get() ; }
		property CoS::Gfx::Renderer^ Renderer { CoS::Gfx::Renderer^ get() ; }
		property CoS::Gfx::RenderWindow^ RenderWindow { CoS::Gfx::RenderWindow^ get() ; }
		property System::Windows::Forms::Control^ RootWindow 
		{
			System::Windows::Forms::Control^ get(); 
			void set(System::Windows::Forms::Control^); 
		}

	private:
		void generateClassLibrary();
	};
}

#endif // RUNTIME_MARSHAL_INCLUDED

