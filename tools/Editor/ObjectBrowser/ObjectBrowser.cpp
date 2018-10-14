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

#include "ObjectBrowser.h"
#include "ObjectBrowserModel.h"
#include "ListViewDelegate.h"
#include "IconViewDelegate.h"
#include "ProjectExplorer/ProjectItem.h"
#include "PackageManager/PackageManager.h"
#include "Reflection/Reflection.h"
#include "Reflection/ClassDef.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QMenu>
#include <QtGui/QDragEnterEvent>

// hopefully temp...
#include "Gfx/Material.h"

using namespace Teardrop;
using namespace Tools;

class QtIconView : public QListView
{
public:
	QtIconView(QWidget* parent) : QListView(parent) {}
	~QtIconView() {}
	void dragMoveEvent(QDragMoveEvent* event) {
		QModelIndex idx = indexAt(event->pos());
		if (!idx.isValid()) {
			QListView::dragMoveEvent(event);
			event->acceptProposedAction();
			return;
		}

		QListView::dragMoveEvent(event);	
		event->ignore();
	}
};

class QtListView : public QListView
{
public:
	QtListView(QWidget* parent) : QListView(parent) {}
	~QtListView() {}
	void dragMoveEvent(QDragMoveEvent* event) {
		QModelIndex idx = indexAt(event->pos());
		if (!idx.isValid()) {
			QListView::dragMoveEvent(event);
			event->acceptProposedAction();
			return;
		}

		QListView::dragMoveEvent(event);	
		event->ignore();
	}
};

ObjectBrowser::ObjectBrowser(QWidget* parent)
	: QWidget(parent)
	, mListView(0)
	, mIconView(0)
	, mCmdList(0)
	, mCmdIcon(0)
	, mCmdHSplit(0)
	, mCmdVSplit(0)
	, mSplitter(0)
	, mViewType(VIEW_ICON)
	, mModel(0)
	, mListViewDelegate(new ListViewDelegate)
	, mIconViewDelegate(new IconViewDelegate)
{
	QVBoxLayout* vLayout = new QVBoxLayout(this);
	QWidget* viewWidget = new QWidget(this);
	QWidget* buttonWidget = new QWidget(this);
	QHBoxLayout* tmp = new QHBoxLayout(viewWidget);
	vLayout->addWidget(viewWidget);
	vLayout->addWidget(buttonWidget);

	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(viewWidget->sizePolicy().hasHeightForWidth());
	viewWidget->setSizePolicy(sizePolicy);

	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(buttonWidget->sizePolicy().hasHeightForWidth());
	buttonWidget->setSizePolicy(sizePolicy1);

	mSplitter = new QSplitter(viewWidget);
	mListView = new QtListView(mSplitter);
	mListView->setViewMode(QListView::ListMode);
	mListView->setItemDelegate(mListViewDelegate);
	mIconView = new QtIconView(mSplitter);
	mIconView->setViewMode(QListView::IconMode);
	mIconView->setItemDelegate(mIconViewDelegate);
	mSplitter->addWidget(mListView);
	mSplitter->addWidget(mIconView);
	tmp->addWidget(mSplitter);

	mListView->setAcceptDrops(true);
	mListView->setDragEnabled(true);
	mListView->setDropIndicatorShown(true);
	mListView->setContextMenuPolicy(Qt::CustomContextMenu);

	mIconView->setAcceptDrops(true);
	mIconView->setDragEnabled(true);
	mIconView->setDropIndicatorShown(true);
	mIconView->setContextMenuPolicy(Qt::CustomContextMenu);

	QHBoxLayout* hLayout = new QHBoxLayout(buttonWidget);
	mCmdList = new QPushButton(buttonWidget);
	mCmdIcon = new QPushButton(buttonWidget);
	mCmdHSplit = new QPushButton(buttonWidget);
	mCmdVSplit = new QPushButton(buttonWidget);
	hLayout->addWidget(mCmdList);
	hLayout->addWidget(mCmdIcon);
	hLayout->addWidget(mCmdHSplit);
	hLayout->addWidget(mCmdVSplit);
	hLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Minimum));

	mCmdList->setText("List");
	mCmdHSplit->setText("Split Horiz");
	mCmdVSplit->setText("Split Vert");
	mCmdIcon->setText("Icon");

	setViewType(mViewType);

	connect(mCmdIcon, SIGNAL(clicked()), this, SLOT(onCmdIcon()));
	connect(mCmdList, SIGNAL(clicked()), this, SLOT(onCmdList()));
	connect(mCmdHSplit, SIGNAL(clicked()), this, SLOT(onCmdHSplit()));
	connect(mCmdVSplit, SIGNAL(clicked()), this, SLOT(onCmdVSplit()));

	connect(mListView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
	connect(mIconView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
	connect(mListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemDoubleClicked(const QModelIndex&)));
	connect(mIconView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemDoubleClicked(const QModelIndex&)));
	connect(mListView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onListContextMenu(const QPoint&)));
	connect(mIconView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onIconContextMenu(const QPoint&)));
}

ObjectBrowser::~ObjectBrowser()
{
	delete mListViewDelegate;
	delete mIconViewDelegate;
	delete mModel;
}

void ObjectBrowser::onListContextMenu(const QPoint& pt)
{
	onContextMenu(mListView, pt);
}

void ObjectBrowser::onIconContextMenu(const QPoint& pt)
{
	onContextMenu(mIconView, pt);
}

void ObjectBrowser::onContextMenu(QAbstractItemView* view, const QPoint& pt)
{
	// new context menu at location pt
	QPoint globalPt = mapToGlobal(pt);

	// what is underneath us?
	QModelIndex index = view->indexAt(pt);
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
	if (item) {
		if (item->isPackage() || item->isFolder()) {
			// should be impossible
			return;
		}
		else {
			QMenu menu;
			QAction* action = menu.addAction("Delete");
			action->setData(QVariant(1));

			action = menu.exec(globalPt);

			if (action) {
				switch(action->data().toInt()) {
				case 1:
					// remove object from package
					item->packageManager()->remove(item->object());

					// and from our model
					mModel->remove(item);

					// and finally remove item from its parent
					item->parent()->remove(item);

					// and then tell the world it's no longer selected
					onItemClicked(QModelIndex());
					break;
				}
			}
		}
	}
	else {
		// RMB on empty space...
		QMenu menu;
		QMenu* newMenu = menu.addMenu("New");

		// TODO: should this be happening here?
		QAction* action = newMenu->addAction("Material");
		action->setData(qVariantFromValue((void*)Gfx::Material::getClassDef()));

		action = menu.exec(globalPt);

		if (action) {
			Reflection::ClassDef* classDef = (Reflection::ClassDef*)action->data().value<void*>();
			ProjectItem* newItem = mModel->addNew(classDef);
		}
	}
}

void ObjectBrowser::setViewType(ViewType viewType)
{
	mViewType = viewType;

	QList<int> sizes;
	sizes.append(mSplitter->parentWidget()->width() / 10);
	sizes.append(mSplitter->parentWidget()->width() / 10 * 9);

	switch (mViewType) {
		case VIEW_ICON:
			mIconView->setHidden(false);
			mListView->setHidden(true);
			break;
		case VIEW_LIST:
			mIconView->setHidden(true);
			mListView->setHidden(false);
			break;
		case VIEW_HSPLIT:
			mIconView->setHidden(false);
			mListView->setHidden(false);
			mSplitter->setSizes(sizes);
			mSplitter->setOrientation(Qt::Horizontal);
			break;
		case VIEW_VSPLIT:
			mIconView->setHidden(false);
			mListView->setHidden(false);
			mSplitter->setSizes(sizes);
			mSplitter->setOrientation(Qt::Vertical);
			break;
	}
}

void ObjectBrowser::clear()
{
	onItemSelected(0);
}

void ObjectBrowser::onItemSelected(ProjectItem* item)
{
	delete mModel;
	mModel = 0;

	if (item && !item->isObject()) {
		mModel = new ObjectBrowserModel(item);
	}

	mListView->setModel(mModel);
	mIconView->setModel(mModel);

	// share selection models
	mListView->setSelectionModel(mIconView->selectionModel());
}

void ObjectBrowser::onCmdList()
{
	setViewType(VIEW_LIST);
}

void ObjectBrowser::onCmdIcon()
{
	setViewType(VIEW_ICON);
}

void ObjectBrowser::onCmdHSplit()
{
	setViewType(VIEW_HSPLIT);
}

void ObjectBrowser::onCmdVSplit()
{
	setViewType(VIEW_VSPLIT);
}

void ObjectBrowser::onItemClicked(const QModelIndex& index)
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
    ItemClicked.raise(item);
}

void ObjectBrowser::onItemDoubleClicked(const QModelIndex& index)
{
	ProjectItem* item = static_cast<ProjectItem*>(index.internalPointer());
    ItemDoubleClicked.raise(item);
}
