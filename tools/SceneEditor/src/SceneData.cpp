/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "SceneData.h"
#include "MessageBus.h"
#include "Game/include/SceneUtil.h"
#include "Game/include/Scene.h"
#include "Util/include/Hash.h"

using namespace CoS;
//---------------------------------------------------------------------------
const unsigned int SceneData::TYPE = hashString("SceneData");
//---------------------------------------------------------------------------
SceneData::SceneData(MessageBus* pMsgBus)
{
	m_pScene = COS_NEW Scene;

	assert(pMsgBus);
	m_pMsgBus = pMsgBus;
}
//---------------------------------------------------------------------------
SceneData::~SceneData()
{
	if (m_pScene)
		m_pScene->destroy();

	delete m_pScene;
}
//---------------------------------------------------------------------------
unsigned int SceneData::getType() const
{
	return TYPE;
}
//---------------------------------------------------------------------------
bool SceneData::update(float deltaT)
{
	if (m_pScene)
		return m_pScene->update(deltaT);
	
	return true;
}
//---------------------------------------------------------------------------
bool SceneData::load(const char* filepath)
{
	if (m_pScene)
		m_pScene->destroy();
	
	return SceneUtil::loadScene(m_pScene, filepath, this);
}
//---------------------------------------------------------------------------
bool SceneData::save(const char* filepath)
{
	return SceneUtil::saveScene(m_pScene, filepath);
}
//---------------------------------------------------------------------------
Scene* SceneData::getScene()
{
	return m_pScene;
}
//---------------------------------------------------------------------------
void SceneData::onSceneLoadBegin(Scene* pScene)
{
	m_pMsgBus->SceneLoadBegin.invoke(pScene);
}
//---------------------------------------------------------------------------
void SceneData::onSceneLoadEnd(Scene* pScene)
{
	m_pMsgBus->SceneLoadEnd(pScene);
}
//---------------------------------------------------------------------------
void SceneData::onZoneCreated(Zone* pZone)
{
	m_pMsgBus->ZoneCreated(pZone);
}
//---------------------------------------------------------------------------
void SceneData::onZoneObjectCreated(ZoneObject *pObject)
{
	// called after object creation, before added to zone
	m_pMsgBus->ZoneObjectCreated(pObject);
}
//---------------------------------------------------------------------------
void SceneData::onZoneObjectAdded(Zone* pZone, ZoneObject *pObject)
{
	m_pMsgBus->ZoneObjectAdded(pZone, pObject);
}
