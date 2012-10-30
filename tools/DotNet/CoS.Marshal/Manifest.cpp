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
#include "Manifest.h"
#include <msclr/marshal.h>
#include "Game/include/Manifest.h"

using namespace CoS;
using namespace Game;
using namespace msclr::interop;
//---------------------------------------------------------------------------
Game::Manifest::Manifest(CoS::Manifest* pManifest)
{
	m_pManifest = pManifest;
}
//---------------------------------------------------------------------------
Game::Manifest::~Manifest()
{
}
//---------------------------------------------------------------------------
Game::Manifest::!Manifest()
{
}
//---------------------------------------------------------------------------
Game::Manifest::Entry Game::Manifest::GetEntry(System::String^ key)
{
	marshal_context^ ctx = gcnew marshal_context();
	const CoS::Manifest::NVP& nvp = m_pManifest->getDefinitions();
	CoS::Manifest::NVP::const_iterator it = nvp.find(
		ctx->marshal_as<const char*>(key));
	delete ctx;

	Entry ent;
	if (it != nvp.end())
	{
		ent.id = it->second.id;
		ent.val = gcnew System::String(it->second.val.c_str());
	}

	return ent;
}
//---------------------------------------------------------------------------
System::Collections::ArrayList^ 
Game::Manifest::GetEntriesForClassType(
	System::String^ classType
	)
{
	marshal_context^ ctx = gcnew marshal_context();
	
	CoS::Manifest::EntryList list;
	m_pManifest->getDefinitionsByHeader(
		ctx->marshal_as<const char*>(classType),
		list);

	System::Collections::ArrayList^ al = 
		gcnew System::Collections::ArrayList();

	for (CoS::Manifest::EntryList::iterator it = list.begin();
		it != list.end(); ++it)
	{
		Entry ent;
		ent.id = it->id;
		ent.val = gcnew System::String(it->val.c_str());
		al->Add(ent);
	}

	delete ctx;
	return al;
}
