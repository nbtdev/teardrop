/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGrid.h"
#include "QtPropertyGridDelegate.h"
#include "QtPropertyGridModel.h"
#include "QtPropertyGridItem.h"
#include "QtUtils/ObjectDragDropData.h"
#include <QDragEnterEvent>
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
	//setHeaderHidden(true);
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
	mModel = new QtPropertyGridModel(object, metadata);
	setModel(mModel);
	expandAll();
}

static bool canDrop(QtPropertyGridItem* item, const QMimeData* data)
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
			DragDropData* ddd = static_cast<DragDropData*>(od);
			if (ddd->type() == DragDropData::DDD_OBJECT) {
				ObjectDragDropData* oddd = static_cast<ObjectDragDropData*>(ddd);
				Reflection::Object* obj = oddd->object();

				// TODO: find a way to compare classdef's?
				if (String(obj->getDerivedClassDef()->getName()) != item->property()->getTypeName())
					return false;
			}
		}
	}

	return true;
}

void QtPropertyGrid::dragEnterEvent(QDragEnterEvent* event)
{
	QModelIndex idx = indexAt(event->pos());

	if (idx.isValid()) {
		QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(idx.internalPointer());
		if (canDrop(item, event->mimeData())) {
			event->acceptProposedAction();
			return;
		}
	}

	event->ignore();
}

void QtPropertyGrid::dragMoveEvent(QDragMoveEvent* event)
{
	QModelIndex idx = indexAt(event->pos());

	if (idx.isValid()) {
		QtPropertyGridItem* item = static_cast<QtPropertyGridItem*>(idx.internalPointer());
		if (item && item->isPointer()) {
			event->acceptProposedAction();
			return;
		}
	}

	event->ignore();
}

void QtPropertyGrid::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}
