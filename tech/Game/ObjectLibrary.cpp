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

#include "ObjectLibrary.h"
#include "Component.h"
#include "MACRO.h"
#include "Prop.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Manifest.h"
#include "ZoneObject.h"
#include "Zone.h"
#include "SceneUtil.h"
#include "BeamWeapon.h"
#include "VFX/ParticleSystem.h"
#include "Util/Environment.h"
#include "Stream/FileStream.h"
#include "Memory/Memory.h"
#include "tinyxml.h"

#include "TerrainZone.h"

using namespace CoS;
using namespace Reflection;
//---------------------------------------------------------------------------
ObjectLibrary* ObjectLibrary::instance()
{
	static ObjectLibrary s;
	return &s;
}
//---------------------------------------------------------------------------
static const char* s_categoryStrings[] =
{
	"Vehicle",
	"Prop",
	"Scene",
	"UI",
	"VFX",
	"Weapon",
};
//---------------------------------------------------------------------------
ObjectLibrary::ObjectLibrary()
{
}
//---------------------------------------------------------------------------
ObjectLibrary::~ObjectLibrary()
{
}
//---------------------------------------------------------------------------
bool ObjectLibrary::initialize()
{
	// fill in the type-by-name LUT
	m_categoryByNameLut[String("Vehicle")] = VEHICLE;
	m_categoryByNameLut[String("Prop")] = PROP;
	m_categoryByNameLut[String("Scene")] = ZONE;
	m_categoryByNameLut[String("UI")] = UI;
	m_categoryByNameLut[String("VFX")] = VFX;
	m_categoryByNameLut[String("Weapon")] = VFX;

	for (int i=0; i<MAX_TYPE; ++i)
	{
		// make sure we have a live reference to the type block
		std::pair<CategoryLUT::iterator, bool> pr = 
			m_lut.insert(CategoryLUT::value_type((Category)i, NameLUT()));
		CategoryLUT::iterator it = pr.first;

		// scan the manifest for entries in this class
		Manifest::EntryList list;
		Environment::get().pManifest->getDefinitionsByCategory(
			s_categoryStrings[i], list);

		// then go through the list and make some templates of these
		for (Manifest::EntryList::iterator ent = list.begin();
			ent != list.end(); ++ent)
		{
			// look this guy up in the manifest's NVP
			const Manifest::NVP& nvp = 
				Environment::get().pManifest->getDefinitions();

			Manifest::NVP::const_iterator def = nvp.find(ent->val);
			if (def != nvp.end())
			{
				// make one of these
				String path(Environment::get().pDataPath);
				path += def->second.val;

				String filename(path);
				filename += "/def.xml";

				FileStream strm;
				ZoneObject* pZoneObj = 0;

				if (strm.open(filename, READ))
				{
					 pZoneObj = create(strm, path);
				}
				strm.close();

				it->second[ent->val] = pZoneObj;

				if (pZoneObj)
					pZoneObj->setTemplateName(ent->val);
			}
		}
	}

	// hacky for static-init registration of these types
	CoS::MACRO _macro;
	CoS::Prop _prop;
	CoS::SpawnPoint _spawnPoint;
	CoS::PointLight _pointLight;
	CoS::SpotLight _spotLight;
	CoS::DirectionalLight _dirLight;
	CoS::TerrainZone _tzone;
	CoS::ParticleSystem _pSys;
	CoS::BeamWeapon _pBeamWeapon;

	return true;
}
//---------------------------------------------------------------------------
bool ObjectLibrary::destroy()
{
	for (CategoryLUT::iterator it = m_lut.begin(); it != m_lut.end(); ++it)
	{
		for (NameLUT::iterator i=it->second.begin(); i != it->second.end(); ++i)
		{
			if (i->second)
				i->second->destroy();
			delete i->second;
		}
	}

	m_lut.clear();
	m_categoryByNameLut.clear();
	return true;
}
//---------------------------------------------------------------------------
ZoneObject* ObjectLibrary::getByName(Category type, const char* name)
{
	CategoryLUT::iterator it = m_lut.find(type);
	if (it != m_lut.end())
	{
		NameLUT& nameLut = it->second;
		NameLUT::iterator obj = nameLut.find(name);
		if (obj != nameLut.end())
			return obj->second;
	}

	return 0;
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
			if (name)
			{
				const PropertyDef* pProp = pClassDef->findProperty(name);

				if (pProp)
				{
					if (pProp->isCollection())
					{
						// read the values from the nested elements
						TiXmlElement* pValue = pPropElem->FirstChildElement("value");
						int i=0;
						while (pValue)
						{
							const char* pText = pValue->GetText();
							if (pText)
							{
								pProp->setDataFromString(&obj, String(pText), i);
							}

							++i;
							pValue = pValue->NextSiblingElement("value");
						}
					}
					else
					{
						if (value)
							pProp->setDataFromString(&obj, String(value));
					}
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
static void loadObjectComponents(
	TiXmlElement* pNode, 
	ZoneObject& object,
	const char* assetRootPath
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
				pComponent->setAssetRootPath(assetRootPath);

				// this would usually be false if this is a non-server 
				// component and we are in server mode
				if (!object.addComponent(pComponent))
				{
					delete pComponent;
				}
			}
		}
				
		pComponentNode = pComponentNode->NextSiblingElement("component");
	}
}
//---------------------------------------------------------------------------
ZoneObject* ObjectLibrary::clone(ZoneObject* pTemplate)
{
	ZoneObject* pRtn = static_cast<ZoneObject*>(Object::clone(*pTemplate));
	pRtn->setBoundingBox(pTemplate->getBoundingBox());
	pRtn->setTemplateName(pTemplate->getTemplateName());

	// and then clone all of its components too...
	const ZoneObject::Components& comps = pTemplate->getComponents();
	for (ZoneObject::Components::const_iterator it = comps.begin(); 
		it != comps.end(); ++it)
	{
		Component* pComponent = 
			static_cast<Component*>(Object::clone(*(it->second)));
		pRtn->addComponent(pComponent);
		pComponent->setAssetRootPath(it->second->getAssetRootPath());
	}

	return pRtn;
}
//---------------------------------------------------------------------------
ZoneObject* ObjectLibrary::create(
	const String& objectClass,
	const String& objectTypeName,
	IPlayerControlled** ppPC)
{
	// although we most likely have one of these to clone, we should make
	// sure first...
	CategoryByNameLUT::iterator it = m_categoryByNameLut.find(objectClass);
	if (it == m_categoryByNameLut.end())
		return 0;

	CategoryLUT::iterator type = m_lut.find(it->second);
	if (type == m_lut.end())
		return 0;

	// then we can look up the actual template instance in yet another LUT...
	NameLUT::iterator ent = type->second.find(objectTypeName);
	if (ent == type->second.end())
		return 0;

	ZoneObject* pTemplate = ent->second;

	// finally, we can clone one...
	ZoneObject* pLive = clone(pTemplate);

	// TODO: this needs moved out of here...
	if (ppPC)
		*ppPC = dynamic_cast<IPlayerControlled*>(pLive);

	return pLive;
}
//---------------------------------------------------------------------------
ZoneObject* ObjectLibrary::create(
	Stream& strm, 
	const char* path)
{
	// and if not, then create a new one (and cache it to be cloned later)
	char* buf = (char*)COS_ALLOCATE(DEFAULT, strm.length() + 1);
	strm.read(buf, strm.length());
	buf[strm.length()] = 0;

	TiXmlDocument doc;
	doc.Parse(buf);

	COS_DEALLOCATE(DEFAULT, buf);

	if (doc.Error())
		return 0;
	
	TiXmlElement *root = doc.RootElement();
	if (String("object") != root->Value())
		return 0;

	const char* className = root->Attribute("class");
	if (!className)
		return 0;

	// first, figure out what it is we are making
	ClassDef* pClassDef = ClassDef::findClassDef(className);
	if (!pClassDef)
		return 0;

	// this whole thing is for ZoneObjects only...
	if (!pClassDef->isA(ZoneObject::getClassDef()))
		return 0;

	// this won't fail at this point -- createInstance() basically calls COS_NEW
	Reflection::Object* pObject = pClassDef->createInstance(0);

	// read in the object properties
	setObjectProperties(*root, *pObject);

	// deserialize the object's AABB
	AABB aabb;
	SceneUtil::deserializeAABB(root->FirstChildElement("AABB"), aabb);
	static_cast<ZoneObject*>(pObject)->setBoundingBox(aabb);

	// load components
	loadObjectComponents(
		root->FirstChildElement("components"), 
		static_cast<ZoneObject&>(*pObject),
		path);

	return static_cast<ZoneObject*>(pObject);
}
