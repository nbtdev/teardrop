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
#include "ObjectLibrary.h"
#include "Runtime.h"
#include "Object.h"
#include <msclr/marshal.h>
#include "Game/include/ObjectLibrary.h"
#include "Game/include/ZoneObject.h"

using namespace CoS;
using namespace Game;
using namespace msclr::interop;
//---------------------------------------------------------------------------
Game::ObjectLibrary::ObjectLibrary()
{
}
//---------------------------------------------------------------------------
Game::ObjectLibrary::~ObjectLibrary()
{
}
//---------------------------------------------------------------------------
Game::ObjectLibrary::!ObjectLibrary()
{
}
//---------------------------------------------------------------------------
CoS::Object^ Game::ObjectLibrary::Create(
	System::String^ classType, // class type 
	System::String^ objectType // object type 
	)
{
	marshal_context^ ctx = gcnew marshal_context();

	CoS::String _class(ctx->marshal_as<const char*>(classType));
	CoS::String _name(ctx->marshal_as<const char*>(objectType));
	delete ctx;

	// create the two instances (one is the actual runtime object, 
	// the other is the property-only .NET facade)
	CoS::ObjectLibrary* pObjLib = CoS::ObjectLibrary::instance();
	CoS::ZoneObject* pObj = pObjLib->create(_class, _name);

	CoS::Object^ obj = nullptr;

	if (pObj)
	{
		pObj->initialize();

		// we need to create the wrapper "property" class as well, and 
		// feed it with this instance
		obj = (CoS::Object^)Runtime::Classdef->CreateInstance(
			gcnew System::String("CoS.") + classType);

		// initial property sync
		obj->_setRuntimeObject(pObj);

		// TODO: hook up the two-way property update path
	}

	return obj;
}
