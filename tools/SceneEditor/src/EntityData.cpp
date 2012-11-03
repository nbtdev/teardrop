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
#include "EntityData.h"
#include "MessageBus.h"
#include "Game/Manifest.h"
#include "Game/ObjectLibrary.h"
#include "Game/Scene.h"
#include "Game/Zone.h"
#include "Game/ZoneObject.h"
#include "Game/PointLight.h"
#include "Game/Component_Animation.h"
#include "Util/Hash.h"
#include "Util/_String.h"

using namespace CoS;
//---------------------------------------------------------------------------
const unsigned int EntityData::TYPE = hashString("EntityData");
//---------------------------------------------------------------------------
EntityData::EntityData(MessageBus* pMsgBus)
: SceneData(pMsgBus)
, m_animationSpeed(0)
, m_prevAnimationSpeed(0)
{
	pMsgBus->StartAnimationPlayback.connect(
		this, &EntityData::OnStartAnimationPlayback);
	pMsgBus->StopAnimationPlayback.connect(
		this, &EntityData::OnStopAnimationPlayback);
}
//---------------------------------------------------------------------------
EntityData::~EntityData()
{
	m_pMsgBus->StartAnimationPlayback.disconnect(
		this, &EntityData::OnStartAnimationPlayback);
	m_pMsgBus->StopAnimationPlayback.disconnect(
		this, &EntityData::OnStopAnimationPlayback);
}
//---------------------------------------------------------------------------
unsigned int EntityData::getType() const
{
	return TYPE;
}
//---------------------------------------------------------------------------
bool EntityData::load(const char* filepath)
{
	// clone one of these if we can
	if (!filepath)
		return false;

	String fullname(filepath);
	size_t pos = fullname.findLast('/');
	String className(fullname.substr(0, pos));
	String entName(fullname.substr(pos+1));

	m_pObject = ObjectLibrary::instance()->create(className, entName);
	if (!m_pObject)
		return false;

	m_pObject->initialize();

	m_pScene->destroy();
	m_pScene->initialize();

	m_pMsgBus->SceneLoadBegin(m_pScene);

	Zone* pZone = m_pScene->createZone("Zone");
	if (!pZone)
	{
		return false;
	}
	pZone->setOrigin(Vector4::ZERO);
	pZone->setDimensions(Vector4::UNIT_SCALE * 1000);
	pZone->setAmbient(Vector4(0.588f, 0.588f, 0.588f, 1));

	m_pMsgBus->ZoneCreated(pZone);
	m_pScene->setCurrentZone(*pZone);
	pZone->initialize(m_pScene->getWorld());

	m_pMsgBus->ZoneObjectCreated(m_pObject);

	// add a nice happy point light at (500,500,500) (paint it Prussian Blue...)
	// TODO: make a handle to this thing somewhere?
	PointLight* pLight = COS_NEW PointLight;
	pLight->setName("PointLight1");
	Transform xform(Transform::IDENTITY);
	xform.trans = Vector4::UNIT_SCALE * 500;
	pLight->setTransform(xform);
	m_pMsgBus->ZoneObjectCreated(pLight);
	pZone->addObject(pLight);
	pLight->initialize();
	m_pMsgBus->ZoneObjectAdded(pZone, pLight);

	pZone->addObject(m_pObject);
	m_pMsgBus->ZoneObjectAdded(pZone, m_pObject);
	m_pObject->setTransform(Transform::IDENTITY);

	m_pMsgBus->SceneLoadEnd(m_pScene);
	return true;
}
//---------------------------------------------------------------------------
bool EntityData::save(const char* /*filepath*/)
{
	return true;
}
//---------------------------------------------------------------------------
ZoneObject* EntityData::getObject()
{
	return m_pObject;
}
//---------------------------------------------------------------------------
void EntityData::OnStartAnimationPlayback()
{
	if (m_prevAnimationSpeed == 0)
		m_prevAnimationSpeed = 1;

	m_animationSpeed = m_prevAnimationSpeed;
}
//---------------------------------------------------------------------------
void EntityData::OnStopAnimationPlayback()
{
	m_prevAnimationSpeed = m_animationSpeed;
	m_animationSpeed = 0;
}
//---------------------------------------------------------------------------
bool EntityData::update(float deltaT)
{
	if (m_pObject)
	{
		AnimationComponent* pAnimComp;
		if (m_pObject->findComponents(
			AnimationComponent::getClassDef(), (Component**)&pAnimComp))
		{
			pAnimComp->update(deltaT * m_animationSpeed);
		}

		// for animated objects such as MACRO, this call will also try to update
		// the animation, but since the object's kinematic velocity is zero, 
		// nothing will happen with animation update in this call
		return m_pObject->update(deltaT);
	}

	return true;
}
