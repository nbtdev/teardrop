/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGrid.h"
#include "StringPropertyHelper.h"
#include "FloatPropertyHelper.h"
#include "IntPropertyHelper.h"
#include "BoolPropertyHelper.h"
#include "Reflection/Reflection.h"
#include "Game/ComponentHost.h"
#include "Game/Component.h"
#include "ThirdParty/Qt4/QtTreePropertyBrowser/qteditorfactory.h"
#include "ThirdParty/Qt4/QtTreePropertyBrowser/qtpropertymanager.h"
#include <stack>

using namespace Teardrop;
using namespace Tools;

QtPropertyGrid::QtPropertyGrid(QWidget* parent)
	: QtTreePropertyBrowser(parent)
	, mObject(0)
	, mStringPropMgr(0)
	, mDoublePropMgr(0)
	, mIntPropMgr(0)
	, mBoolPropMgr(0)
	, mGroupPropMgr(0)
	, mLineEditFactory(0)
	, mDoubleSpinBoxFactory(0)
{
	//setHeaderVisible(false);

	mStringPropMgr = new QtStringPropertyManager;
	mDoublePropMgr = new QtDoublePropertyManager;
	mIntPropMgr = new QtIntPropertyManager;
	mBoolPropMgr = new QtBoolPropertyManager;
	mGroupPropMgr = new QtGroupPropertyManager;

	mLineEditFactory = new QtLineEditFactory;
	mDoubleSpinBoxFactory = new QtDoubleSpinBoxFactory;
	mCheckBoxFactory = new QtCheckBoxFactory;

	setFactoryForManager(mStringPropMgr, mLineEditFactory);
	setFactoryForManager(mDoublePropMgr, mDoubleSpinBoxFactory);
	setFactoryForManager(mBoolPropMgr, mCheckBoxFactory);
}

QtPropertyGrid::~QtPropertyGrid()
{
	clear();
	clearHelpers();

	delete mCheckBoxFactory;
	delete mDoubleSpinBoxFactory;
	delete mLineEditFactory;
	delete mGroupPropMgr;
	delete mBoolPropMgr;
	delete mIntPropMgr;
	delete mDoublePropMgr;
	delete mStringPropMgr;
}

void QtPropertyGrid::clearHelpers()
{
	for (PropertyHelpers::iterator it = mHelpers.begin(); it != mHelpers.end(); ++it) {
		delete *it;
	}

	mHelpers.clear();
}

static void populate(std::stack<Reflection::ClassDef*>& classes, Reflection::ClassDef* classDef)
{
	classes.push(classDef);
	if (classDef->getBaseClass()) {
		populate(classes, classDef->getBaseClass());
	}
}

void QtPropertyGrid::addProperty(QtProperty* parent, Reflection::Object* obj, const Reflection::PropertyDef* propDef)
{
	if (String("String") == propDef->getTypeName() && !propDef->isCollection()) {
		String val;
		propDef->getDataAsString(obj, val);
		QtProperty* subProp = mStringPropMgr->addProperty(propDef->getName());
		mStringPropMgr->setValue(subProp, (const char*)val);
		mHelpers.push_back(new StringPropertyHelper(mStringPropMgr, subProp, obj, propDef));
		parent->addSubProperty(subProp);
	}

	if (String("float") == propDef->getTypeName() && !propDef->isCollection()) {
		float val;
		propDef->getData(obj, &val);
		QtProperty* subProp = mDoublePropMgr->addProperty(propDef->getName());
		mDoublePropMgr->setValue(subProp, double(val));
		mHelpers.push_back(new FloatPropertyHelper(mDoublePropMgr, subProp, obj, propDef));
		parent->addSubProperty(subProp);
	}

	if (String("int") == propDef->getTypeName() && !propDef->isCollection()) {
		int val;
		propDef->getData(obj, &val);
		QtProperty* subProp = mIntPropMgr->addProperty(propDef->getName());
		mIntPropMgr->setValue(subProp, val);
		mHelpers.push_back(new IntPropertyHelper(mIntPropMgr, subProp, obj, propDef));
		parent->addSubProperty(subProp);
	}

	if (String("bool") == propDef->getTypeName() && !propDef->isCollection()) {
		bool val;
		propDef->getData(obj, &val);
		QtProperty* subProp = mBoolPropMgr->addProperty(propDef->getName());
		mBoolPropMgr->setValue(subProp, val);
		mHelpers.push_back(new BoolPropertyHelper(mBoolPropMgr, subProp, obj, propDef));
		parent->addSubProperty(subProp);
	}
}

void QtPropertyGrid::setObject(Reflection::Object* object)
{
	clear();
	clearHelpers();
	mObject = object;

	// calling with NULL means clear the editor (unsets the current object)
	if (!mObject)
		return;

	// add headers
	QtProperty* header = mGroupPropMgr->addProperty("Object");
	QtTreePropertyBrowser::addProperty(header);

	// iterate object class hierarchy, start adding properties with the deepest that isn't Object
	std::stack<Reflection::ClassDef*> classes;
	Reflection::ClassDef* classDef = mObject->getDerivedClassDef();
	populate(classes, classDef);

	while (!classes.empty()) {
		Reflection::ClassDef* classDef = classes.top();
		classes.pop();

		QtProperty* superclass = mGroupPropMgr->addProperty(classDef->getName());
		header->addSubProperty(superclass);

		// properties
		const Reflection::PropertyDef* prop = classDef->getProps();
		while (prop) {
			addProperty(superclass, mObject, prop);
			prop = prop->m_pNext;
		}
	}

	// next the components (if any, and if this is a ComponentHost)
	ComponentHost* compHost = dynamic_cast<ComponentHost*>(mObject);
	if (compHost) {
		header = mGroupPropMgr->addProperty("Components");
		QtTreePropertyBrowser::addProperty(header);

		Component* comps[256];
		int nComps = compHost->getComponents(comps, 256);

		for (int i=0; i<nComps; ++i) {
			Component* comp = comps[i];

			Reflection::ClassDef* classDef = comp->getDerivedClassDef();
			QtProperty* compName = mGroupPropMgr->addProperty(classDef->getName());
			header->addSubProperty(compName);

			// properties
			const Reflection::PropertyDef* prop = classDef->getProps();
			while (prop) {
				addProperty(compName, comp, prop);
				prop = prop->m_pNext;
			}
		}
	}
}
