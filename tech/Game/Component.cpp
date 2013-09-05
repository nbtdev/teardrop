/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Component.h"
#include "Game/ComponentHost.h"

using namespace Teardrop;
//---------------------------------------------------------------------------
TD_CLASS_IMPL(Component);
//---------------------------------------------------------------------------
Component::Component()
{
	m_pHost = 0;
}
//---------------------------------------------------------------------------
Component::~Component()
{
}
//---------------------------------------------------------------------------
void Component::setHost(ComponentHost* pHost)
{
	m_pHost = pHost;
}
//---------------------------------------------------------------------------
bool Component::initialize()
{
	return true;
}
//---------------------------------------------------------------------------
bool Component::destroy()
{
	return true;
}
//---------------------------------------------------------------------------
Reflection::Object* Component::clone() const
{
	// utilize the base functionality 
	Reflection::Object* obj = Reflection::Object::clone();
	Component* comp = static_cast<Component*>(obj);

	// and we have a couple of extra things to copy
	comp->setAssetRootPath(m_assetPath);

	return obj;
}
//---------------------------------------------------------------------------
bool Component::update(float /*deltaT*/)
{
	return true;
}
//---------------------------------------------------------------------------
ComponentHost* Component::getHost() const
{
	return m_pHost;
}
//---------------------------------------------------------------------------
void Component::addedToZone(Zone* /*pZone*/)
{
}
//---------------------------------------------------------------------------
void Component::removeFromZone(Zone* /*pZone*/)
{
}
//---------------------------------------------------------------------------
void Component::setAssetRootPath(const String& rootPath)
{
	m_assetPath = rootPath;
}
//---------------------------------------------------------------------------
const String& Component::getAssetRootPath() const
{
	return m_assetPath;
}
