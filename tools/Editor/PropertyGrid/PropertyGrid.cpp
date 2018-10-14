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

#include "PropertyGrid.h"
#include "PropertyGridDelegate.h"
#include "PropertyGridModel.h"
#include "PropertyGridItem.h"
#include "ProjectExplorer/ProjectItem.h"
#include <QtCore/QMimeData>
#include <QtGui/QDragEnterEvent>
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
				if (obj->getDerivedClassDef()->isA(item->property()->getClassDef()))
					return true;
			}
		}
	}

	return false;
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
