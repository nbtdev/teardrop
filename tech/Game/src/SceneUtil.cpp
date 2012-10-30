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

#include "SceneUtil.h"
#include "Scene.h"
#include "Component.h"
#include "ZoneObject.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "LoadListener.h"
#include "SpawnPoint.h"
#include "Zone.h"
#include "ObjectLibrary.h"
#include "Gfx/include/GfxMesh.h"
#include "Physics/include/Physics.h"
#include "Physics/include/World.h"
#include "Math/include/MathUtil.h"
#include "Math/include/AABB.h"
#include "Math/include/Vector2.h"
#include "Math/include/Vector4.h"
#include "Math/include/Quaternion.h"
#include "Math/include/Transform.h"
#include "Reflection/include/Reflection.h"
#include "Reflection/include/PropertyDef.h"
#include "Stream/include/FileStream.h"
#include "Util/include/Hash.h"
#include "Util/include/StringUtil.h"
#include "Util/include/_String.h"
#include "tinyxml.h"

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
void SceneUtil::serializeAABB(
	TiXmlElement& aabbElem,
	const AABB& aabb)
{
	Vector4 minCorner(aabb.center - aabb.extents);
	Vector4 maxCorner(aabb.center + aabb.extents);
	minCorner.w = maxCorner.w = 0;

	TiXmlElement minElem("min");
	String str;
	StringUtil::toString(minCorner, str);
	minElem.SetAttribute("value", str);

	TiXmlElement maxElem("max");
	StringUtil::toString(maxCorner, str);
	maxElem.SetAttribute("value", str);

	aabbElem.InsertEndChild(minElem);
	aabbElem.InsertEndChild(maxElem);
}
//---------------------------------------------------------------------------
void SceneUtil::deserializeAABB(
	TiXmlElement* pAABB,
	AABB& aabb)
{
	Vector4 minCorner, maxCorner;
	if (pAABB)
	{
		TiXmlElement* pMinCorner = pAABB->FirstChildElement("min");
		TiXmlElement* pMaxCorner = pAABB->FirstChildElement("max");

		if (pMinCorner && pMaxCorner)
		{
			StringUtil::fromString(pMinCorner->Attribute("value"), minCorner);
			StringUtil::fromString(pMaxCorner->Attribute("value"), maxCorner);

			aabb.fromCorners(minCorner, maxCorner);
		}
	}
}
//---------------------------------------------------------------------------
void SceneUtil::deserializeTransform(
	TiXmlElement* pXformNode,
	Transform& xform)
{
	if (!pXformNode)
		return;

	const char* pAttr = pXformNode->Attribute("value");

	StringUtil::fromString(pAttr, xform);
}
//---------------------------------------------------------------------------
static void serializeObjectProperty(
	const PropertyDef* pProp,
	TiXmlElement& elem, 
	const Reflection::Object* pObj)
{
	String val;
	pProp->getDataAsString(pObj, val);

	// don't write out values that are at their defaults
	if (val != pProp->getDefault())
	{
		TiXmlElement prop("property");
		prop.SetAttribute("name", pProp->getName());
		prop.SetAttribute("value", val);

		elem.InsertEndChild(prop);
	}
}
//---------------------------------------------------------------------------
static void serializeObjectProperties(
	TiXmlElement& elem, 
	const Reflection::Object* pObj)
{
	ClassDef* pClassDef = pObj->getDerivedClassDef();
	while (pClassDef)
	{
		const PropertyDef* pProp = pClassDef->getProps();
		while (pProp)
		{
			serializeObjectProperty(pProp, elem, pObj);
			pProp = pProp->m_pNext;
		}

		// this will stop before Reflection::Object, since it has no classdef
		pClassDef = pClassDef->findClassDef(pClassDef->getBaseClassName());
	}
}
//---------------------------------------------------------------------------
static void serializeZone(TiXmlElement& zoneElem, Zone* pZone)
{
	String id;
	StringUtil::toString(pZone->getId(), id);
	zoneElem.SetAttribute("id", id);
	zoneElem.SetAttribute("type", pZone->getDerivedClassDef()->getName());

	// write out the zone's properties, starting with the most derived
	// class and working back through the Zone base class
	serializeObjectProperties(zoneElem, pZone);

	// then do objects; spawn points are a special case of object
	TiXmlElement objects("objects");
	const ZoneObjects& zoneObjects = pZone->getZoneObjects();
	for (size_t i=0; i<zoneObjects.size(); ++i)
	{
		ZoneObject* pObj = zoneObjects[i]; 

		// deleted items had their entry set to 0
		if (!pObj || !pObj->shouldSerialize())
			continue;

		TiXmlElement object("object");
		object.SetAttribute("type", pObj->getDerivedClassDef()->getName());
		if (pObj->getTemplateName().length())
			object.SetAttribute("name", pObj->getTemplateName());

		String id;
		StringUtil::toString(pObj->getObjectId(), id);
		object.SetAttribute("id", id);

		if (!pObj->getTemplateName().length())
		{
			// then save all the properties
			serializeObjectProperties(object, pObj);

			// and then repeat for any components this object might have
			const ZoneObject::Components& comps = pObj->getComponents();
			TiXmlElement components("components");
			for (ZoneObject::Components::const_iterator it = comps.begin(); 
				it != comps.end(); ++it)
			{
				Component* pComp = it->second;
				TiXmlElement component("component");
				component.SetAttribute("type", pComp->getDerivedClassDef()->getName());
				serializeObjectProperties(component, pComp);
				components.InsertEndChild(component);
			}
			object.InsertEndChild(components);
		}
		else
		{
			// then only a couple of property overrides on ZoneObject
			// since it's an instance of a template
			const PropertyDef* pProp = pObj->getClassDef()->findProperty("Transform");
			serializeObjectProperty(pProp, object, pObj);
			pProp = pObj->getClassDef()->findProperty("Name");
			serializeObjectProperty(pProp, object, pObj);
		}

		objects.InsertEndChild(object);
	}

	zoneElem.InsertEndChild(objects);
}
//---------------------------------------------------------------------------
bool SceneUtil::saveScene(const Scene *pScene, const char* filepath)
{
	if (!pScene || !filepath)
		return false;

	// start at the top and work our way down...
	TiXmlDocument doc;
	TiXmlElement root("scene");

	// add scene-level elements
	TiXmlElement initialZone("initial_zone");
	initialZone.SetAttribute("id", "12345678"); // TODO: fix this
	root.InsertEndChild(initialZone);

	// walk the zones
	AABB worldAABB; // defaults to zeroes
	for (size_t i=0; i<pScene->m_zones.size(); ++i)
	{
		TiXmlElement zone("zone");
		serializeZone(zone, pScene->m_zones[i].pZone);
		// TODO: serialize neighbor data

		root.InsertEndChild(zone);

		worldAABB.merge(pScene->m_zones[i].pZone->getAABB());
	}

	// add closing scene-level elements
	TiXmlElement aabb("aabb");
	serializeAABB(aabb, worldAABB);
	root.InsertEndChild(aabb);

	doc.InsertEndChild(root);

	// pretty-print to XML and write that to the stream
	TiXmlPrinter p;
	doc.Accept(&p);

	FileStream strm;
	if (!strm.open(filepath, WRITE|TRUNCATE))
		return false;

	strm.write(p.CStr(), strlen(p.CStr()));
	strm.close();

	return true;
}
//---------------------------------------------------------------------------
static void loadZoneObjects(
	TiXmlElement& node, 
	Zone& zone, 
	LoadListener* pCallback
	);
static bool loadSceneFromXml(
	Scene* pScene, 
	World*& pWorld,
	const char* filepath, 
	LoadListener* pCallback);
//---------------------------------------------------------------------------
bool SceneUtil::loadScene(
	Scene *pScene, const char* filepath, LoadListener* pListener)
{
	if (!pScene || !filepath)
		return false;

	bool rtn = loadSceneFromXml(pScene, pScene->m_pWorld, filepath, pListener);
	return rtn;
}
//---------------------------------------------------------------------------
static void setObjectProperties(
	 TiXmlElement& elem,
	 Reflection::Object& obj
	)
{
	TiXmlElement* pPropElem = elem.FirstChildElement("property");
	while (pPropElem)
	{
		ClassDef* pClassDef = obj.getDerivedClassDef();
		const char* name = pPropElem->Attribute("name");
		const char* value = pPropElem->Attribute("value");

		while (pClassDef)
		{
			if (name && value)
			{
				const PropertyDef* pProp = pClassDef->findProperty(name);

				if (pProp)
				{
					pProp->setDataFromString(&obj, String(value));
					break;
				}
			}

			// this will stop before Reflection::Object, since it has no classdef
			pClassDef = pClassDef->findClassDef(pClassDef->getBaseClassName());
		}

		pPropElem = pPropElem->NextSiblingElement("property");
	}
}
//---------------------------------------------------------------------------
static void deserializeZone(
	Scene& scene,
	const char* dataPath,
	World& world,
	TiXmlElement& zoneElem,
	LoadListener* pCallback
	)
{
	const char* pType = zoneElem.Attribute("type");
	Zone* pZone = scene.createZone(pType, pCallback);
	setObjectProperties(zoneElem, *pZone);

	if (!pZone)
		return;

	if (pCallback)
		pCallback->onZoneCreated(pZone);

	pZone->setDataRootPath(dataPath);
	pZone->initialize(&world);
	scene.setCurrentZone(*pZone);

	// load the defined objects into the zone
	loadZoneObjects(
		*zoneElem.FirstChildElement("objects"), 
		*pZone,
		pCallback);
}
//---------------------------------------------------------------------------
static bool loadSceneFromXml(
	Scene* pScene, 
	World*& pWorld,
	const char* sceneFile, 
	LoadListener* pCallback)
{
	TiXmlDocument doc;
	doc.LoadFile(sceneFile);

	if (doc.Error())
	{
		return false;
	}

	String filepath(sceneFile);
	String dataPath(filepath.substr(0, filepath.findLast('/')));

	if (pCallback)
		pCallback->onSceneLoadBegin(pScene);

	TiXmlElement* pRoot = doc.RootElement();
	AABB worldAABB;
	TiXmlElement* pSceneAABB = pRoot->FirstChildElement("aabb");
	if (!pSceneAABB)
	{
		return false;
	}

	SceneUtil::deserializeAABB(pSceneAABB, worldAABB);

	worldAABB.extents = worldAABB.extents * 2;
	pWorld = Physics::createWorld(worldAABB);
	pWorld->lock();
	Physics::addWorldToDebugger(pWorld);

	// get the "initial zone" info
	TiXmlElement* pInitZone = pRoot->FirstChildElement("initial_zone");
	if (!pInitZone)
	{
		return false;
	}

	//size_t initId = hashString(pInitZone->Attribute("id"));

	// and then the actual zones
	TiXmlElement* pZone = pRoot->FirstChildElement("zone");
	while (pZone)
	{
		deserializeZone(*pScene, dataPath, *pWorld, *pZone, pCallback);
		pZone = pZone->NextSiblingElement("zone");
	}

	pWorld->unlock();
	return true;
}
//---------------------------------------------------------------------------
static void loadObjectComponents(
	TiXmlElement* pNode, 
	ZoneObject& object
)
{
	if (!pNode)
		return;

	TiXmlElement* pComponentNode = pNode->FirstChildElement("component");
	while (pComponentNode)
	{
		const char* type = pComponentNode->Attribute("type");
		if (type)
		{
			ClassDef* pClassDef = ClassDef::findClassDef(type);
			if (pClassDef)
			{
				Component* pComponent = 
					static_cast<Component*>(pClassDef->createInstance(0));
				setObjectProperties(*pComponentNode, *pComponent);
				pComponent->setHost(&object);
				pComponent->initialize();
				object.addComponent(pComponent);
			}
		}
				
		pComponentNode = pComponentNode->NextSiblingElement("component");
	}
}
//---------------------------------------------------------------------------
static void loadZoneObjects(
	TiXmlElement& node, 
	Zone& zone, 
	LoadListener* pCallback)
{
	TiXmlElement* pObject = node.FirstChildElement();
	while (pObject)
	{
		const char* type = pObject->Attribute("type");
		const char* name = pObject->Attribute("name");
		const char* id = pObject->Attribute("id");
		if (type && id)
		{
			unsigned int objId;
			StringUtil::fromString(id, objId);

			ZoneObject* pZoneObject = 0;
			if (name)
				pZoneObject = ObjectLibrary::instance()->create(type, name);
	
			if (!pZoneObject)
			{
				ClassDef* pClassDef = ClassDef::findClassDef(type);
				if (pClassDef)
				{
					// make one fresh; it might not be a cached type
					pZoneObject = static_cast<ZoneObject*>(
						pClassDef->createInstance(objId));
				}
			}

			if (pZoneObject)
			{
				setObjectProperties(*pObject, *pZoneObject);
				// hack to jumpstart the transform init
				pZoneObject->setTransform(pZoneObject->getTransform());

				if (pCallback)
					pCallback->onZoneObjectCreated(pZoneObject);
				
				if (zone.addObject(pZoneObject))
				{
					if (pCallback)
						pCallback->onZoneObjectAdded(&zone, pZoneObject);
				}

				// load components (for now, depends on host object having its zone pointer set)
				loadObjectComponents(
					pObject->FirstChildElement("components"), *pZoneObject);

				// initialize the object now
				pZoneObject->initialize();
				pZoneObject->update(0);
			}
		}

		pObject = pObject->NextSiblingElement();
	}
}
