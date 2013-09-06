/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtPropertyGrid.h"
#include "QtPropertyGridDelegate.h"
#include "QtPropertyGridModel.h"
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

void QtPropertyGrid::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void QtPropertyGrid::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

void QtPropertyGrid::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}
