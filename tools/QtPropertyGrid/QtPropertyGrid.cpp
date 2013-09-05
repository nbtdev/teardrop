/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(USE_QTTREEPROPERTYBROWSER)
#include "QtPropertyGrid.h"
#include "QtPropertyGridDelegate.h"
#include "QtPropertyGridModel.h"
#include "QtUtils/ObjectDragDropData.h"
#include "Reflection/Reflection.h"
#include "Game/ComponentHost.h"
#include "Game/Component.h"

using namespace Teardrop;
using namespace Tools;

QtPropertyGrid::QtPropertyGrid(QWidget* parent)
	: QTreeView(parent)
	, mModel(0)
	, mObject(0)
	, mMetadata(0)
{
	mDelegate = new QtPropertyGridDelegate;
	setItemDelegate(mDelegate);
	setHeaderHidden(true);
	setAcceptDrops(true);
}

QtPropertyGrid::~QtPropertyGrid()
{
	delete mModel;
	delete mDelegate;
}

void QtPropertyGrid::setObject(Reflection::Object* object, Reflection::Object* metadata /* = 0 */)
{
	delete mModel;
	mModel = new QtPropertyGridModel(object);
	setModel(mModel);
}

#else // USE_QTTREEPROPERTYBROWSER
#include "QtPropertyGrid.h"
#include "StringPropertyHelper.h"
#include "ReferencePropertyHelper.h"
#include "FloatPropertyHelper.h"
#include "IntPropertyHelper.h"
#include "BoolPropertyHelper.h"
#include "FileChooserFactory.h"
#include "QtUtils/ObjectDragDropData.h"
#include "Reflection/Reflection.h"
#include "Game/ComponentHost.h"
#include "Game/Component.h"
#include "QtPropertyBrowser/qteditorfactory.h"
#include "QtPropertyBrowser/qtpropertymanager.h"
#include <QDragEnterEvent>
#include <stack>

using namespace Teardrop;
using namespace Tools;

QtPropertyGrid::QtPropertyGrid(QWidget* parent)
	: QtTreePropertyBrowser(parent)
	, mObject(0)
	, mMetadata(0)
	, mStringPropMgr(0)
	, mPathPropMgr(0)
	, mDoublePropMgr(0)
	, mIntPropMgr(0)
	, mBoolPropMgr(0)
	, mGroupPropMgr(0)
	, mLineEditFactory(0)
	, mDoubleSpinBoxFactory(0)
	, mFileChooserFactory(0)
{
	//setHeaderVisible(false);

	mStringPropMgr = new QtStringPropertyManager;
	mPathPropMgr = new QtStringPropertyManager;
	mDoublePropMgr = new QtDoublePropertyManager;
	mIntPropMgr = new QtIntPropertyManager;
	mBoolPropMgr = new QtBoolPropertyManager;
	mGroupPropMgr = new QtGroupPropertyManager;

	mLineEditFactory = new QtLineEditFactory;
	mDoubleSpinBoxFactory = new QtDoubleSpinBoxFactory;
	mCheckBoxFactory = new QtCheckBoxFactory;
	mFileChooserFactory = new FileChooserFactory;

	setFactoryForManager(mStringPropMgr, mLineEditFactory);
	setFactoryForManager(mDoublePropMgr, mDoubleSpinBoxFactory);
	setFactoryForManager(mBoolPropMgr, mCheckBoxFactory);
	setFactoryForManager(mPathPropMgr, mFileChooserFactory);

	setAcceptDrops(true);
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

		QtProperty* subProp = 0;
		
		if (String(propDef->getEditor()).contains("FileChooser")) {
			subProp = mPathPropMgr->addProperty(propDef->getName());
			mPathPropMgr->setValue(subProp, (const char*)val);
			mHelpers.push_back(new StringPropertyHelper(mPathPropMgr, subProp, obj, propDef));
		}
		else {
			subProp = mStringPropMgr->addProperty(propDef->getName());

			if (String(propDef->getEditor()).contains("ReadOnly"))
				subProp->setEnabled(false);

			mStringPropMgr->setValue(subProp, (const char*)val);
			mHelpers.push_back(new StringPropertyHelper(mStringPropMgr, subProp, obj, propDef));
		}

		if (subProp) {
			parent->addSubProperty(subProp);
		}
	}

	if (propDef->isPointer()) {
		Reflection::Object* pO;
		propDef->getData(obj, &pO);

		QtProperty* subProp = mStringPropMgr->addProperty(propDef->getName());
		subProp->setEnabled(false);

		String sVal;
		if (pO) {
			pO->getObjectId().toString(sVal);
		}

		mStringPropMgr->setValue(subProp, (const char*)sVal);
		mHelpers.push_back(new ReferencePropertyHelper(mStringPropMgr, subProp, obj, propDef));

		if (subProp) {
			parent->addSubProperty(subProp);
		}
	}

	if (String("Vector4") == propDef->getTypeName() && !propDef->isCollection()) {
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

void QtPropertyGrid::setObject(Reflection::Object* object, Reflection::Object* metadata)
{
	clear();
	clearHelpers();
	mObject = object;
	mMetadata = metadata;

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

		// properties
		const Reflection::PropertyDef* prop = classDef->getProps();

		// don't put in a grouping for classes in the hierarchy with no properties; it's just confusing
		if (prop) {
			QtProperty* superclass = mGroupPropMgr->addProperty(classDef->getName());
			header->addSubProperty(superclass);

			while (prop) {
				addProperty(superclass, mObject, prop);
				prop = prop->m_pNext;
			}
		}
	}

	// next the components (if any, and if this is a ComponentHost)
	ComponentHost* compHost = dynamic_cast<ComponentHost*>(mObject);
	if (compHost) {
		header = mGroupPropMgr->addProperty("Components");
		QtTreePropertyBrowser::addProperty(header);

		Component* comps[256];
		int nComps = 0;//compHost->getComponents(comps, 256);

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

	// finally, all properties on the metadata object (if any)
	if (metadata) {
		mMetadata = metadata;

		Reflection::ClassDef* classDef = mMetadata->getDerivedClassDef();
		populate(classes, classDef);

		if (classes.size()) {
			QtProperty* metadata = mGroupPropMgr->addProperty("Metadata");
			QtTreePropertyBrowser::addProperty(metadata);

			while (!classes.empty()) {
				Reflection::ClassDef* classDef = classes.top();
				classes.pop();

				// properties
				const Reflection::PropertyDef* prop = classDef->getProps();
				while (prop) {
					addProperty(metadata, mMetadata, prop);
					prop = prop->m_pNext;
				}
			}
		}
	}
}

void QtPropertyGrid::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void QtPropertyGrid::dragMoveEvent(QDragMoveEvent* event)
{
	//QTreeWidgetItem* item = itemAt(event->pos());
	//if (item) {
	//	setCurrentItem(item);

	//	PackageExplorerItem* pei = static_cast<PackageExplorerItem*>(item);
	//	if (pei->itemType() == PackageExplorerItem::TYPE_FOLDER) {
			event->acceptProposedAction();
	//		return;
	//	}
	//}

	//event->ignore();
}

void QtPropertyGrid::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

void QtPropertyGrid::dropEvent(QDropEvent* event)
{
	QWidget::dropEvent(event);
#if 0
	DragDropData* ddd = (DragDropData*)event->mimeData()->userData(0);
	if (ddd->type() == DragDropData::DDD_OBJECT) {
		ObjectDragDropData* oddd = static_cast<ObjectDragDropData*>(ddd);
		Reflection::Object* obj = oddd->object();
		String id;
		obj->getObjectId().toString(id);
		QWidget* w = childAt(event->pos());
		while (w && w != this) {
			if (dynamic_cast<QtPropertyEditorView*>(w))
				break;

			w = w->parentWidget();
		}
	}

	event->ignore();
#endif
}
#endif // USE_QTTREEPROPERTYBROWSER
