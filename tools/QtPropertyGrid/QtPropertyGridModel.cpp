/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGridModel.h"
#include "QtPropertyGridItem.h"
#include "Reflection/Reflection.h"
#include <stack>

using namespace Teardrop;
using namespace Tools;

static void populate(std::stack<Reflection::ClassDef*>& classes, Reflection::ClassDef* classDef)
{
	classes.push(classDef);
	if (classDef->getBaseClass()) {
		populate(classes, classDef->getBaseClass());
	}
}

QtPropertyGridModel::QtPropertyGridModel(Reflection::Object* obj, QObject* parent)
	: QAbstractItemModel(parent)
	, mObject(obj)
	, mRoot(new QtPropertyGridItem("Properties"))
{
	// set up model data from Object 
	if (obj) {
		// start with classes...
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
				QtPropertyGridItem* group = new QtPropertyGridItem(classDef->getName(), mRoot);
				mRoot->append(group);

				while (prop) {
					QtPropertyGridItem* propItem = new QtPropertyGridItem(obj, prop, group);
					group->append(propItem);
					prop = prop->m_pNext;
				}
			}
		}
#if 0
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
#endif
	}
}

QtPropertyGridModel::~QtPropertyGridModel()
{
	delete mRoot;
}

int QtPropertyGridModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant QtPropertyGridModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	QtPropertyGridItem *item = static_cast<QtPropertyGridItem*>(index.internalPointer());

	if (index.column() == 0)
		return item->name();
	else
		return item->valueAsString();
}

QModelIndex QtPropertyGridModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	QtPropertyGridItem* parentItem;
	if (!parent.isValid())
		parentItem = mRoot;
	else parentItem = static_cast<QtPropertyGridItem*>(parent.internalPointer());

	QtPropertyGridItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex QtPropertyGridModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	QtPropertyGridItem* childItem = static_cast<QtPropertyGridItem*>(index.internalPointer());
	QtPropertyGridItem *parentItem = childItem->parent();

	if (parentItem == mRoot)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int QtPropertyGridModel::rowCount(const QModelIndex& parent) const
{
	QtPropertyGridItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = mRoot;
	else
		parentItem = static_cast<QtPropertyGridItem*>(parent.internalPointer());

	return parentItem->numChildren();
}

Qt::ItemFlags QtPropertyGridModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable);

	QtPropertyGridItem* item = 0;
	if (index.isValid())
		item = static_cast<QtPropertyGridItem*>(index.internalPointer());

	if (index.column() == 1 && (item && !item->isPointer()))
		f |= Qt::ItemIsEditable;

	if (item && item->isPointer()) {
		f &= ~(Qt::ItemIsEnabled);
		f |= Qt::ItemIsDropEnabled;
	}

	return f;
}