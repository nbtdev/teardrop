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

#include "ZoneObject.h"
#include "Stream/Stream.h"
#include "Memory/Memory.h"
#include "Gfx/GfxMeshInstance.h"
#include "Gfx/GfxCommand.h"
#include "Gfx/GfxCommandBuffer.h"
#include "Util/Environment.h"
#include "Math/Transform.h"
#include "Math/Quaternion.h"
#include "Math/Vector4.h"
#include "tinyxml/tinyxml.h"

using namespace CoS;
//---------------------------------------------------------------------------
bool ZoneObject::loadXml(const char* pXml)
{
	TiXmlDocument doc;
	doc.Parse(pXml);

	if (doc.Error())
	{
		return false;
	}

	// for now just pick off the render component and have the renderer give 
	// us a handle for it
	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pBasePathElem = pRoot->FirstChildElement("base_path");
	TiXmlElement* pRender = pRoot->FirstChildElement("render");
	const char* pBasePath = 0;

	if (!pBasePathElem || !pRender)
	{
		return false;
	}

	pBasePath = pBasePathElem->GetText();
	if (!pBasePath)
	{
		return false;
	}

	TiXmlElement* pMeshElem = pRender->FirstChildElement("mesh");
	if (!pMeshElem)
	{
		return false;
	}

	const char* pAssetFile = pMeshElem->Attribute("asset");
	if (!pAssetFile)
	{
		return false;
	}

	std::string meshFile(pBasePath);
	meshFile += "/";
	meshFile += pAssetFile;

	return true;
}
