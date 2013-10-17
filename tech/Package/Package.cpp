/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package.h"
#include "Reflection/Reflection.h"
#include "Util/UUID.h"
#include <algorithm>

using namespace Teardrop;

Package::Package()
	: mData(0)
{
}

Package::~Package()
{
	delete [] mData;
}

void Package::add(Reflection::Object* object)
{
	mObjects.push_back(object);
}

void Package::remove(Reflection::Object* object)
{
	Objects::iterator it = std::find(mObjects.begin(), mObjects.end(), object);
	mObjects.erase(it);

	// remove symbol table entry too
	SymbolTable::iterator symIt = mSymTab.find(object->getObjectId());
	if (symIt != mSymTab.end())
		mSymTab.erase(symIt);
}

const Objects& Package::objects() const
{
	return mObjects;
}

void Package::initializeAllObjects()
{
	for (Objects::iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
		(*it)->initialize();
	}
}

void* Package::createDataStorage(int len)
{
	delete [] mData;
	mData = new unsigned char[len];
	return mData;
}

void Package::addSymTabEntry(Reflection::Object* obj)
{
	mSymTab[obj->getObjectId()] = obj;
}

Reflection::Object* Package::findById(const UUID& id)
{
	SymbolTable::iterator it = mSymTab.find(id);
	if (it != mSymTab.end())
		return it->second;

	return 0;
}

Reflection::Object* Package::findById(const String& id)
{
	UUID uuid;
	uuid.fromString(id);
	return findById(uuid);
}

void Package::findAllByType(std::list<Reflection::Object*>& objs, Reflection::ClassDef* type, bool includeDerivedTypes)
{
	objs.clear();

	for (Objects::iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
		Reflection::Object* obj = *it;

		if (includeDerivedTypes) {
			if (obj->getDerivedClassDef()->isA(type)) {
				objs.push_back(obj);
			}
		}
		else {
			if (obj->getDerivedClassDef() == type) {
				objs.push_back(obj);
			}
		}
	}
}