/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "QtObjectBrowser.h"
#include "QtObjectBrowserModel.h"
#include "QtProjectExplorer/QtProjectItem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QPushButton>
#include <QSplitter>
#include <QSpacerItem>
#include <QStyledItemDelegate>
#include <QPainter>

using namespace Teardrop;
using namespace Tools;

class Teardrop::Tools::QtListDelegate : public QStyledItemDelegate
{
public:
	QtListDelegate(QObject* parent=0) : QStyledItemDelegate(parent) {}
	~QtListDelegate() {}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QStyleOptionViewItemV4 opt = option;
		initStyleOption(&opt, index);

		painter->save();

		// just draw the text
		QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
		if (item) {
			QSize sz = sizeHint(opt, index);
			QString val(item->toString());
			QRectF rf = painter->boundingRect(opt.rect, Qt::AlignLeft, val);

			if (opt.state & QStyle::State_Selected) {
				QBrush brush(Qt::lightGray);
				painter->fillRect(rf, brush);
			}

			painter->drawText(rf, val);
		}
		else
			QStyledItemDelegate::paint(painter, option, index);

		painter->restore();
	}

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
		if (item) {
			QRect r = option.fontMetrics.boundingRect(QString(item->toString()));
			return QSize(r.width(), r.height());
		}

		return QSize();
	}
};

QtObjectBrowser::QtObjectBrowser(QWidget* parent)
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
	, mListDelegate(new QtListDelegate)
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
	mListView = new QListView(mSplitter);
	mListView->setViewMode(QListView::ListMode);
	mListView->setItemDelegate(mListDelegate);
	mIconView = new QListView(mSplitter);
	mIconView->setViewMode(QListView::IconMode);
	mSplitter->addWidget(mListView);
	mSplitter->addWidget(mIconView);
	tmp->addWidget(mSplitter);

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
}

QtObjectBrowser::~QtObjectBrowser()
{
	delete mListDelegate;
	delete mModel;
}

void QtObjectBrowser::setViewType(ViewType viewType)
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

void QtObjectBrowser::onItemSelected(QtProjectItem* item)
{
	delete mModel;
	mModel = 0;

	if (item && !item->isObject()) {
		mModel = new QtObjectBrowserModel(item);
	}

	mListView->setModel(mModel);
	mIconView->setModel(mModel);

	// share selection models
	mListView->setSelectionModel(mIconView->selectionModel());
}

void QtObjectBrowser::onCmdList()
{
	setViewType(VIEW_LIST);
}

void QtObjectBrowser::onCmdIcon()
{
	setViewType(VIEW_ICON);
}

void QtObjectBrowser::onCmdHSplit()
{
	setViewType(VIEW_HSPLIT);
}

void QtObjectBrowser::onCmdVSplit()
{
	setViewType(VIEW_VSPLIT);
}

void QtObjectBrowser::onItemClicked(const QModelIndex& index)
{
	if (index.isValid()) {
		QtProjectItem* item = static_cast<QtProjectItem*>(index.internalPointer());
		if (ItemClicked) {
			ItemClicked(item);
		}
	}
}