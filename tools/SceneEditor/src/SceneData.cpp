/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "stdafx.h"
#include "SceneData.h"
#include "MessageBus.h"
#include "Game/SceneUtil.h"
#include "Game/Scene.h"
#include "Util/Hash.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
const unsigned int SceneData::TYPE = hashString("SceneData");
//---------------------------------------------------------------------------
SceneData::SceneData(MessageBus* pMsgBus)
{
	m_pScene = TD_NEW Scene;

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
