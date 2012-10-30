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

#if !defined(OBJECT_MARSHAL_INCLUDED)
#define OBJECT_MARSHAL_INCLUDED

namespace CoS
{
	namespace Reflection
	{
		class Object;
	}

	public ref class Object
	{
		Reflection::Object* m_pObj;

	public:
		Object();
		~Object();
		!Object();

		void _setRuntimeObject(Reflection::Object* pObj);
		Reflection::Object* _getRuntimeObject();

	protected:
		void PropertyChanged(System::String^ propName);
	};
}

#endif // OBJECT_MARSHAL_INCLUDED
