/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtProjectExplorer.h"
#include "QtProjectModel.h"

using namespace Teardrop;
using namespace Tools;

QtProjectExplorer::QtProjectExplorer(QWidget* parent)
	: QTreeView(parent)
	, mModel(0)
{

}

QtProjectExplorer::~QtProjectExplorer()
{
	delete mModel;
}

void QtProjectExplorer::setProject(Project* project)
{
	delete mModel;
	mModel = 0;

	if (project) {
		mModel = new QtProjectModel(project);
		setModel(mModel);
		expandAll();
	}
}

void QtProjectExplorer::mouseMoveEvent(QMouseEvent* event)
{

}

void QtProjectExplorer::dragEnterEvent(QDragEnterEvent* event)
{

}

void QtProjectExplorer::dragLeaveEvent(QDragLeaveEvent* event)
{

}

void QtProjectExplorer::dragMoveEvent(QDragMoveEvent* event)
{
	
}

void QtProjectExplorer::onDragStart(DragDropData* ddd)
{

}

void QtProjectExplorer::onContextMenu(const QPoint& pt)
{

}
