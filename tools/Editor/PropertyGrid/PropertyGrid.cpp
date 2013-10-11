/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "PropertyGrid.h"
#include "PropertyGridDelegate.h"
#include "PropertyGridModel.h"
#include "PropertyGridItem.h"
#include "ProjectExplorer/ProjectItem.h"
#include <QDragEnterEvent>
#include "Reflection/Reflection.h"
#include "Game/ComponentHost.h"
#include "Game/Component.h"

using namespace Teardrop;
using namespace Tools;

PropertyGrid::PropertyGrid(QWidget* parent)
	: QTreeView(parent)
	, mModel(0)
	, mObject(0)
	, mMetadata(0)
{
	mDelegate = new PropertyGridDelegate;
	setItemDelegate(mDelegate);
	//setHeaderHidden(true);
	setAcceptDrops(true);
}

PropertyGrid::~PropertyGrid()
{
	delete mModel;
	delete mDelegate;
}

void PropertyGrid::setObject(Reflection::Object* object, Reflection::Object* metadata /* = 0 */)
{
	delete mModel;
	mModel = new PropertyGridModel(object, metadata);
	setModel(mModel);
	expandAll();
}

static bool canDrop(PropertyGridItem* item, const QMimeData* data)
{
	// have to have a valid item under us
	if (!item)
		return false;

	// only pointer property types can be drop targets (for now)
	if (!item->isPointer())
		return false;

	// the type of the dragged data has to match the pointer type
	if (data) {
		QObjectUserData* od = data->userData(0);
		if (od) {
			ProjectItemData* projItemData = static_cast<ProjectItemData*>(od);
			if (projItemData) {
				ProjectItem* projItem = projItemData->item();
				Reflection::Object* obj = projItem->object();

				// TODO: find a way to compare classdef's?
				if (String(obj->getDerivedClassDef()->getName()) != item->property()->getTypeName())
					return false;
			}
		}
	}

	return true;
}

void PropertyGrid::dragEnterEvent(QDragEnterEvent* event)
{
	QModelIndex idx = indexAt(event->pos());

	if (idx.isValid()) {
		PropertyGridItem* item = static_cast<PropertyGridItem*>(idx.internalPointer());
		if (canDrop(item, event->mimeData())) {
			event->acceptProposedAction();
			return;
		}
	}

	event->ignore();
}

void PropertyGrid::dragMoveEvent(QDragMoveEvent* event)
{
	QModelIndex idx = indexAt(event->pos());

	if (idx.isValid()) {
		PropertyGridItem* item = static_cast<PropertyGridItem*>(idx.internalPointer());
		if (item && item->isPointer()) {
			event->acceptProposedAction();
			return;
		}
	}

	event->ignore();
}

void PropertyGrid::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}
