/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package.h"
#include "Executable.h"
#include "Reflection/Reflection.h"
#include "Util/UUID.h"
#include <algorithm>

using namespace Teardrop;

Package::Package()
	: mData(0)
	, mExecutable(0)
{
}

Package::~Package()
{
	delete [] mData;

	for (Objects::iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
		delete *it;
	}

	delete mExecutable;
}

void Package::add(Reflection::Object* object)
{
	mObjects.push_back(object);
	addSymTabEntry(object);
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

Executable* Package::executable()
{
	return mExecutable;
}

Executable* Package::makeExecutable(const Reflection::ClassDef* classDef/* =0 */)
{
	// a classDef of 0 means clear the current executable, if any
	if (classDef == 0 && mExecutable) {
		mExecutable->destroy();
		delete mExecutable;
		mExecutable = 0;
	}

	// then if classDef not null, make a new one
	if (classDef) {
		assert(classDef->isA(Executable::getClassDef()));

		if (classDef->isA(Executable::getClassDef())) {
			mExecutable = static_cast<Executable*>(classDef->createInstance());
			mExecutable->initialize();
		}
	}

	return mExecutable;
}
