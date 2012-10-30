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
#include "Scene.h"
#include "Zone.h"
#include "Camera.h"
#include "Object.h"
#include <msclr/marshal.h>
#include <msclr/auto_gcroot.h>
#include "Game/include/Scene.h"
#include "Game/include/Zone.h"
#include "Game/include/ZoneObject.h"
#include "Game/include/LoadListener.h"
#include "Game/include/SceneUtil.h"

using namespace CoS;
using namespace Game;
using namespace msclr::interop;
//---------------------------------------------------------------------------
struct Listener : public CoS::LoadListener
{
	msclr::auto_gcroot<Game::Scene^> scene;

	Listener(Game::Scene^ s) : scene(s) {}
	void onSceneLoadBegin(CoS::Scene* pScene);
	void onZoneCreated(CoS::Zone* pZone);
	void onZoneObjectCreated(CoS::ZoneObject* pObject);
	void onZoneObjectAdded(CoS::Zone* pZone, CoS::ZoneObject* pObject);
	void onSceneLoadEnd(CoS::Scene* pScene);
};
void Listener::onSceneLoadBegin(CoS::Scene* pScene)
{
}
void Listener::onZoneCreated(CoS::Zone* pZone)
{
	Game::Zone^ zone = gcnew Game::Zone(pZone);
	scene->_RaiseZoneCreated(zone);
}
void Listener::onZoneObjectCreated(CoS::ZoneObject* pObject)
{
}
void Listener::onZoneObjectAdded(CoS::Zone* pZone, CoS::ZoneObject* pObject)
{
}
void Listener::onSceneLoadEnd(CoS::Scene* pScene)
{
}
//---------------------------------------------------------------------------
void Game::Scene::_RaiseZoneCreated(Zone^ zone)
{
	ZoneCreated(zone);
}
//---------------------------------------------------------------------------
void Game::Scene::_RaiseZoneObjectCreated(CoS::Object^ obj)
{
	ZoneObjectCreated(obj);
}
//---------------------------------------------------------------------------
void Game::Scene::_RaiseZoneObjectAdded(Zone^ zone, CoS::Object^ obj)
{
	ZoneObjectAdded(zone, obj);
}
//---------------------------------------------------------------------------
Game::Scene::Scene()
{
	m_pScene = 0;
}
//---------------------------------------------------------------------------
Game::Scene::~Scene()
{
}
//---------------------------------------------------------------------------
Game::Scene::!Scene()
{
	delete m_pScene;
}
//---------------------------------------------------------------------------
bool Game::Scene::Update(double deltaT)
{
	if (m_pScene)
		return m_pScene->update(static_cast<float>(deltaT));

	return false;
}
//---------------------------------------------------------------------------
bool Game::Scene::Load(System::String^ filepath)
{
	assert(!m_pScene);
	if (m_pScene)
	{
		m_pScene->destroy();
		delete m_pScene;
	}

	marshal_context^ ctx = gcnew marshal_context();
	CoS::String pathname(ctx->marshal_as<const char*>(filepath));
	delete ctx;

	m_pScene = COS_NEW CoS::Scene;
	Listener l(this);
	return SceneUtil::loadScene(m_pScene, pathname, &l);
}
//---------------------------------------------------------------------------
bool Game::Scene::Initialize()
{
	if (m_pScene) 
		return m_pScene->initialize();

	return false;
}
//---------------------------------------------------------------------------
bool Game::Scene::Destroy()
{
	if (m_pScene) 
		return m_pScene->destroy();

	return false;
}
//---------------------------------------------------------------------------
bool Game::Scene::Save(System::String^ filepath)
{
	return true;
}
//---------------------------------------------------------------------------
System::Collections::Generic::List<CoS::Object^>^
Game::Scene::GetVisibleObjects(Gfx::Camera^ cam)
{
	System::Collections::Generic::List<CoS::Object^>^ rtn = 
		gcnew System::Collections::Generic::List<CoS::Object^>();

	if (m_pScene)
	{
		CoS::ZoneObjects objs;
		m_pScene->getVisibleObjects(*cam->_getNative(), objs);

		for (CoS::ZoneObjects::iterator it = objs.begin();
			it != objs.end(); ++it)
		{
			CoS::Object^ pObj = gcnew CoS::Object;
			pObj->_setRuntimeObject(*it);
			rtn->Add(pObj);
		}
	}

	return rtn;
}
