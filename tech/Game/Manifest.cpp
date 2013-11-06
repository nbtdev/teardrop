/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Manifest.h"
#include "Component.h"
#include "ComponentHost.h"
#include "Variant.h"
#include "Mountable.h"
#include "Reflection/Reflection.h"
#include "Util/Environment.h"
#include "Util/FileSystem.h"
#include "Util/Logger.h"
#include "Util/Hash.h"
#include "Util/StringUtil.h"
#include "tinyxml.h"

using namespace Teardrop;
using namespace Reflection;
//---------------------------------------------------------------------------
Manifest::Manifest()
{
}
//---------------------------------------------------------------------------
Manifest::~Manifest()
{
}
//---------------------------------------------------------------------------
bool Manifest::getCategories(StringList& categories) const
{
	categories.clear();

	for(GroupedNVP::const_iterator it = m_grouped.begin(); it != m_grouped.end(); ++it)
	{
		categories.push_back(it->first);
	}

	return true;
}
//---------------------------------------------------------------------------
void Manifest::initializeDefinitions()
{
	Environment& env = Environment::get();
	String findPath(env.pDataPath);
	String findSpec("def.xml");
	FileSystem::FileList files;

	FileSystem::findAll(findPath, findSpec, files, true);

	// TODO: future optimization; use parallel_for if the size of the file list
	// gets too large, since XML parsing isn't something you want to do in
	// serial for a large number of XML files

	for (FileSystem::FileList::iterator it = files.begin(); it != files.end(); ++it)
	{
		TiXmlDocument doc;
		doc.LoadFile(*it);

		if (doc.Error())
		{
			String msg("Manifest::initializeDefinitions: Could not open/parse file ");
			msg += *it;
			env.pLogger->logMessage(msg);
			continue;
		}

		TiXmlElement* pRoot = doc.RootElement();
		const char* pCategory = pRoot->Attribute("category");
		const char* pName = pRoot->Attribute("name");
		const char* pClass = pRoot->Attribute("class");

		if (pCategory && pName)
		{
			// insert or retrieve the list from m_grouped
			std::pair<GroupedNVP::iterator, bool> pr = 
				m_grouped.insert(GroupedNVP::value_type(String(pCategory), EntryList()));

			String relPath;
			FileSystem::makeRelativePath(relPath, findPath, *it);

			// strip the ".\" off the front and the filename ("\def.xml") off the back
			String pathSection(relPath.substr(2, relPath.length()-8));

			ManifestEntry ent;
			ent.id = pr.first->second.size();
			ent.pClassDef = ClassDef::findClassDef(pClass);
			ent.val = pathSection;

			m_nvp[String(pName)] = ent;

			ent.val = String(pName);
			pr.first->second.push_back(ent);

			// also need to deserialize the entry into the ComponentHosts lookup table
			String assetPath(Environment::get().pDataPath);
			assetPath += pathSection;
			deserializeComponentHost(pRoot, assetPath);
		}
	}
}
//---------------------------------------------------------------------------
void Manifest::initializeVariants(const String& findPath)
{
	Environment& env = Environment::get();
	String findSpec("variants.xml");
	FileSystem::FileList files;

	FileSystem::findAll(findPath, findSpec, files, true);

	// TODO: future optimization; use parallel_for if the size of the file list
	// gets too large, since XML parsing isn't something you want to do in
	// serial for a large number of XML files

	for (FileSystem::FileList::iterator it = files.begin(); it != files.end(); ++it)
	{
		TiXmlDocument doc;
		doc.LoadFile(*it);

		if (doc.Error())
		{
			String msg("Manifest::initializeVariants: Could not open/parse file ");
			msg += *it;
			env.pLogger->logMessage(msg);
			continue;
		}

		TiXmlElement* pRoot = doc.RootElement();
		TiXmlElement* pElem = pRoot->FirstChildElement("variant");

		while (pElem)
		{
			deserializeVariant(pElem);
			pElem = pElem->NextSiblingElement("variant");
		}
	}
}
//---------------------------------------------------------------------------
bool Manifest::initialize()
{
	initializeDefinitions();

	// initialize the "stock" variants
	initializeVariants(String(Environment::get().pDataPath));

	// initialize the player's variants
	String path;
	FileSystem::getAppDataPath(path);
	initializeVariants(path);

	return true;
}
//---------------------------------------------------------------------------
bool Manifest::destroy()
{
	// need to delete things we created
	for (ComponentHostTable::iterator it = m_compHostDefs.begin();
		it != m_compHostDefs.end(); ++it)
	{
		delete it->second;
	}

	for (VariantTable::iterator it = m_variants.begin(); it != m_variants.end(); ++it) {
		delete it->second;
	}

	return true;
}
//---------------------------------------------------------------------------
bool Manifest::getDefinitionsByCategory(
	const String& category, EntryList& list) const
{
	list.clear();

	GroupedNVP::const_iterator it = m_grouped.find(category);
	if (it == m_grouped.end())
	{
		return false;
	}

	// copy the list contents
	list = it->second;
	return true;
}
//---------------------------------------------------------------------------
void Manifest::deserializeVariant(TiXmlElement* pElem)
{
	const char* pName = pElem->Attribute("name");
	const char* pClass = pElem->Attribute("class");
	const char* pTarget = pElem->Attribute("target");

	if (pName && pClass && pTarget)
	{
		ClassDef* pClassDef = ClassDef::findClassDef(pClass);
		if (!pClassDef)
			return;

		// also check to see if the target has been defined yet (it should
		// exist in the m_compHostDefs table)
		const ComponentHost* pHost = findComponentHost(pClassDef, String(pTarget));
		if (!pHost)
			return;

		// we can add an entry now
		Variant* v = new Variant(pName, pTarget, pClassDef);
		VariantTable::iterator it = m_variants.insert(
			VariantTable::value_type(pClassDef, v));

		String hashStr(pName);
		hashStr += pTarget;
		int id = hashString(hashStr);

		// and also put this in the id-to-Variant lookup table
		m_variantIdTable[id] = v;

		deserializeSlots(v, pElem);
	}
}
//---------------------------------------------------------------------------
void Manifest::deserializeSlots(Variant* v, TiXmlElement* pElem)
{
	TiXmlElement* pSlot = pElem->FirstChildElement("slot");
	while (pSlot)
	{
		const char* pOrdinal = pSlot->Attribute("ordinal");
		int ordinal = 0;
		if (pOrdinal)
			StringUtil::fromString(pOrdinal, ordinal);

		if (ordinal >= 0)
		{
			// get the equipment items off the slot definition
			TiXmlElement* pEquip = pSlot->FirstChildElement("equipment");
			while (pEquip)
			{
				const char* pName = pEquip->Attribute("name");
				if (pName)
				{
					// find the corresponding ManifestEntry
					ObjectTemplatesByName::iterator it = mObjectTemplates.find(pName);
					if (it != mObjectTemplates.end()) {
						v->addEquipment(ordinal, static_cast<const Mountable*>(it->second));
					}
						
				}

				pEquip = pEquip->NextSiblingElement("equipment");
			}
		}

		pSlot = pSlot->NextSiblingElement("slot");
	}
}
//---------------------------------------------------------------------------
void Manifest::deserializeComponentHost(TiXmlElement* pElem, const String& assetPath)
{
	// root-level attributes
	const char* pCategory = pElem->Attribute("category");
	const char* pName = pElem->Attribute("name");
	const char* pClass = pElem->Attribute("class");
	const char* pType = pElem->Attribute("type");

	if (!pClass)
		return;

	// try to find the ClassDef that matches the "class" value
	Reflection::ClassDef* pClassDef = ClassDef::findClassDef(pClass);
	if (!pClassDef)
		return;

	// otherwise, we're good to go
	ComponentHostTable::iterator it = m_compHostDefs.insert(
		ComponentHostTable::value_type(pClassDef, TD_NEW ComponentHost));
	it->second->m_pClassDef = pClassDef;

	if (pCategory)
		it->second->m_category = pCategory;
	if (pName)
		it->second->m_name = pName;
	if (pType)
		it->second->m_type = pType;

	// read in object's properties
	deserializeProperties(it->second->m_properties, pElem);
	deserializeComponents(it->second->m_components, pElem);

	it->second->m_assetPath = assetPath;

	// stealth...add an instance of this thing to the object template table
	const Reflection::Object* o = createInstance(it->second);
	mObjectTemplates[pName] = o;
}
//---------------------------------------------------------------------------
void Manifest::deserializeProperties(Properties& props, TiXmlElement* pElem)
{
	TiXmlElement* pPropElem = pElem->FirstChildElement("property");
	while (pPropElem)
	{
		const char* pName = pPropElem->Attribute("name");
		const char* pValue = pPropElem->Attribute("value");

		if (pName)
		{
			size_t idx = props.size();
			props.push_back(Property());
			props[idx].m_name = pName;

			if (pValue)
			{
				props[idx].m_value = pValue;
			}
			else
			{
				// collection property? check for child element "value"
				TiXmlElement* pValue = pPropElem->FirstChildElement("value");
				while (pValue)
				{
					const char* pText = pValue->GetText();
					if (pText)
						props[idx].m_values.push_back(pText);

					pValue = pValue->NextSiblingElement("value");
				}

				props[idx].m_bIsCollection = (props[idx].m_values.size() > 0);
			}
		}

		pPropElem = pPropElem->NextSiblingElement("property");
	}
}
//---------------------------------------------------------------------------
void Manifest::deserializeComponents(Components& comps, TiXmlElement* pElem)
{
	TiXmlElement* pComps = pElem->FirstChildElement("components");
	if (!pComps)
		return;

	TiXmlElement* pCompElem = pComps->FirstChildElement("component");
	while (pCompElem)
	{
		const char* pClass = pCompElem->Attribute("type");

		if (pClass)
		{
			ClassDef* pClassDef = ClassDef::findClassDef(pClass);
			if (pClassDef)
			{
				size_t idx = comps.size();
				comps.push_back(Component());
				comps[idx].m_pClassDef = pClassDef;
				deserializeProperties(comps[idx].m_properties, pCompElem);
			}
		}

		pCompElem = pCompElem->NextSiblingElement("component");
	}
}
//---------------------------------------------------------------------------
const Manifest::ComponentHost* Manifest::findComponentHost(
	const Reflection::ClassDef* pClassDef, const String& name) const
{
	ComponentHostTable::const_iterator beg = m_compHostDefs.lower_bound(pClassDef);
	ComponentHostTable::const_iterator end = m_compHostDefs.upper_bound(pClassDef);

	while (beg != end)
	{
		if (beg->second->m_name == name)
			return beg->second;

		++beg;
	}

	return 0;
}
//---------------------------------------------------------------------------
const Variant* Manifest::findVariant(
	const Reflection::ClassDef* pClassDef, const String& name) const
{
	VariantTable::const_iterator beg = m_variants.lower_bound(pClassDef);
	VariantTable::const_iterator end = m_variants.upper_bound(pClassDef);

	while (beg != end)
	{
		if (beg->second->name() == name)
			return beg->second;

		++beg;
	}

	return 0;
}
//---------------------------------------------------------------------------
void Manifest::findVariants(
	const Reflection::ClassDef* pClassDef, Variants& variants) const
{
	VariantTable::const_iterator beg = m_variants.lower_bound(pClassDef);
	VariantTable::const_iterator end = m_variants.upper_bound(pClassDef);

	while (beg != end)
	{
		variants.push_back(beg->second);
		++beg;
	}
}
//---------------------------------------------------------------------------
const Variant* Manifest::findVariant(unsigned int id) const
{
	VariantIDLookupTable::const_iterator it = m_variantIdTable.find(id);
	if (it != m_variantIdTable.end())
		return it->second;

	return 0;
}
//---------------------------------------------------------------------------
static void populateProperties(
	Reflection::Object* pObject, const Manifest::Properties& props)
{
	if (!pObject)
		return;

	const Reflection::ClassDef* pClassDef = pObject->getDerivedClassDef();
	
	while (pClassDef)
	{
		for (size_t p=0; p<props.size(); ++p)
		{
			const Reflection::PropertyDef* pProp = 
				pClassDef->findProperty(props[p].m_name);

			if (pProp)
			{
				if (props[p].m_bIsCollection)
				{
					for (size_t v=0; v<props[p].m_values.size(); ++v)
						pProp->setDataFromString(pObject, props[p].m_values[v], v);
				}
				else
					pProp->setDataFromString(pObject, props[p].m_value);
			}
		}

		pClassDef = pClassDef->getBaseClass();
	}
}
//---------------------------------------------------------------------------
Reflection::Object* Manifest::createInstance(
	const Manifest::ComponentHost* pDef)
{
	if (!pDef)
		return 0;

	Reflection::Object* pObject = pDef->m_pClassDef->createInstance();

	if (pObject)
	{
		// set object property defaults first
		pObject->setupPropertyDefaults();

		// then fill in properties and populate components
		populateProperties(pObject, pDef->m_properties);

		// then do the components
		Teardrop::ComponentHost* pHost = dynamic_cast<Teardrop::ComponentHost*>(pObject);
		if (pHost) {
			for (size_t i=0; i<pDef->m_components.size(); ++i)
			{
				Teardrop::Component* pComp = static_cast<Teardrop::Component*>(
					pDef->m_components[i].m_pClassDef->createInstance());

				if(!pComp->getServerComponent() && Environment::get().isServer)
				{
					delete pComp;
					pComp = 0;
				}

				if (pComp)
				{
					pComp->setAssetRootPath(pDef->m_assetPath);
					pComp->setupPropertyDefaults();
					populateProperties(pComp, pDef->m_components[i].m_properties);
					pHost->addComponent(pComp);
				}
			}
		}

		return pObject;
	}

	return 0;
}
