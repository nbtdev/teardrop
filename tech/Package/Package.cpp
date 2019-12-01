/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#include "Package.h"
#include "Core/Executable.h"
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

        if (*it == mExecutable) {
            mExecutable = nullptr;
        }
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

void Package::makeExecutable(Executable* exe)
{
    mExecutable = exe;
}

Executable* Package::makeExecutable(const Reflection::ClassDef* classDef/* =0 */)
{
    // a classDef of 0 means clear the current executable, if any
    if (mExecutable) {
        mExecutable->destroy();
        delete mExecutable;
        mExecutable = nullptr;
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
