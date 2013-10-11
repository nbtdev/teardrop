/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyGridModel.h"
#include "PropertyGridItem.h"
#include "ProjectExplorer/ProjectItem.h"
#include <QMimeData>
#include "ObjectDragDropData.h"
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

PropertyGridModel::PropertyGridModel(Reflection::Object* obj, Reflection::Object* metadata, QObject* parent)
	: QAbstractItemModel(parent)
	, mObject(obj)
	, mMetadata(metadata)
	, mRoot(new PropertyGridItem("Properties"))
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
				PropertyGridItem* group = new PropertyGridItem(classDef->getName(), mRoot);
				mRoot->append(group);

				while (prop) {
					if (prop->isNested()) {
						PropertyGridItem* nestedProp = new PropertyGridItem(obj, prop, group);
						group->append(nestedProp);

						// add this guy's props
						Reflection::Object* nestedObj = static_cast<Reflection::Object*>(const_cast<void*>(prop->getDataPointer(obj)));
						Reflection::ClassDef* nestedClassDef = nestedObj->getDerivedClassDef();
						const Reflection::PropertyDef* nestedProps = nestedClassDef->getProps();

						// only one level of nesting!
						while (nestedProps) {
							PropertyGridItem* np = new PropertyGridItem(nestedObj, nestedProps, nestedProp);
							nestedProp->append(np);
							nestedProps = nestedProps->m_pNext;
						}
					} 
					else {
						PropertyGridItem* propItem = new PropertyGridItem(obj, prop, group);
						group->append(propItem);
					}

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
#endif
		// finally, all properties on the metadata object (if any)
		if (mMetadata) {
			Reflection::ClassDef* classDef = mMetadata->getDerivedClassDef();
			populate(classes, classDef);

			if (classes.size()) {
				PropertyGridItem* meta = new PropertyGridItem("Metadata", mRoot);
				mRoot->append(meta);

				while (!classes.empty()) {
					Reflection::ClassDef* classDef = classes.top();
					classes.pop();

					// properties
					const Reflection::PropertyDef* prop = classDef->getProps();
					while (prop) {
						PropertyGridItem* metaItem = new PropertyGridItem(mMetadata, prop, meta);
						meta->append(metaItem);
						prop = prop->m_pNext;
					}
				}
			}
		}
	}
}

PropertyGridModel::~PropertyGridModel()
{
	delete mRoot;
}

int PropertyGridModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant PropertyGridModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	PropertyGridItem *item = static_cast<PropertyGridItem*>(index.internalPointer());
	const Reflection::PropertyDef* prop = 0;
	if (item)
		prop = item->property();

	if (role == Qt::BackgroundColorRole) {
		if (item) {
			if (item->isGroup() || item->isNested()) {
				return Qt::gray;
			}
		}
	}

	if (role == Qt::ToolTipRole) {
		if (index.column() == 0)
			return item->nameTooltip();
		else
			return item->valueTooltip();
	}
	else if (role == Qt::CheckStateRole) {
		if (prop && index.column() == 1) {
			Reflection::Object* obj = item->object();
			if (obj && item->isBoolean()) {
				bool b;
				prop->getData(obj, &b);
				return int((b ? Qt::Checked : Qt::Unchecked));
			}
		}
	}
	else if (role == Qt::DisplayRole || role == Qt::EditRole) {
		if (index.column() == 0)
			return item->name();
		else {
			if (prop) {
				if (!prop->isNested()) {
					if (String("ColorEditor") == prop->getEditor())
						return QString("");
				}
			}

			return item->valueAsString();
		}
	}

	return QVariant();
}

bool PropertyGridModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	PropertyGridItem* item = static_cast<PropertyGridItem*>(index.internalPointer());
	if (item) {
		const Reflection::PropertyDef* prop = item->property();
		Reflection::Object* obj = item->object();

		if (prop && obj) {
			if (role == Qt::EditRole) {
				if (item->isEnum()) {
					int val = value.toInt();
					prop->setData(obj, &val);
				}
				else {
					prop->setDataFromString(obj, value.toString().toLatin1().data());
				}
			}
			else if (role == Qt::CheckStateRole) {
				if (item->isBoolean()) {
					bool b = value.toBool();
					prop->setData(obj, &b);
				}
			}
		}

		return true;
	}

	return false;
}

QModelIndex PropertyGridModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	PropertyGridItem* parentItem;
	if (!parent.isValid())
		parentItem = mRoot;
	else parentItem = static_cast<PropertyGridItem*>(parent.internalPointer());

	PropertyGridItem* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex PropertyGridModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();

	PropertyGridItem* childItem = static_cast<PropertyGridItem*>(index.internalPointer());
	PropertyGridItem *parentItem = childItem->parent();

	if (parentItem == mRoot)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int PropertyGridModel::rowCount(const QModelIndex& parent) const
{
	PropertyGridItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = mRoot;
	else
		parentItem = static_cast<PropertyGridItem*>(parent.internalPointer());

	return parentItem->numChildren();
}

Qt::ItemFlags PropertyGridModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags f = (Qt::ItemIsEnabled|Qt::ItemIsSelectable);

	PropertyGridItem* item = 0;
	if (index.isValid())
		item = static_cast<PropertyGridItem*>(index.internalPointer());

	if (index.column() == 1) {
		if (item) {
			if (item->isPointer()) {
				f &= ~(Qt::ItemIsEnabled);
				f |= Qt::ItemIsDropEnabled;
			}
			else if (!item->isGroup()) {
				f |= Qt::ItemIsEditable;
			}
			
			if (item->isBoolean()) {
				f |= Qt::ItemIsUserCheckable;
			}
		}
	}

	if (item && !item->isNested() && item->isReadOnly()) {
		f &= ~(Qt::ItemIsEnabled);
	}

	return f;
}

QStringList PropertyGridModel::mimeTypes() const
{
	QStringList list = QAbstractItemModel::mimeTypes();
	list.append("text/plain");
	return list;
}

Qt::DropActions PropertyGridModel::supportedDropActions() const
{
	Qt::DropActions actions = QAbstractItemModel::supportedDropActions();
	return actions;
}

bool PropertyGridModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (data) {
		QObjectUserData* od = data->userData(0);
		if (od) {
			PropertyGridItem* item = static_cast<PropertyGridItem*>(parent.internalPointer());
			if (item) {
				// make sure it's a pointer property...
				if (item->isPointer()) {
					ProjectItemData* projItemData = static_cast<ProjectItemData*>(od);
					ProjectItem* projItem = projItemData->item();
					if (projItem->isObject()) {
						item->property()->setData(item->object(), projItem->object());
						item->setAltValue((const char*)projItem->path());
					}
				}
			}
		}
	}

	return true;
}

QVariant PropertyGridModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}