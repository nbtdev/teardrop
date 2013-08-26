/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FileChooserFactory.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>

using namespace Teardrop;
using namespace Tools;

FileChooserWidget::FileChooserWidget(QWidget* parent, QtStringPropertyManager* propMgr, QtProperty* prop, const QString& initialValue)
	: QWidget(parent)
	, mPathname(0)
	, mButton(0)
	, mPropMgr(propMgr)
	, mProperty(prop)
{
	mPathname = new QLineEdit;
	mButton = new QToolButton;

	mPathname->setText(initialValue);
	mPathname->setCursorPosition(0);

	QHBoxLayout *lt = new QHBoxLayout(this);
	lt->setContentsMargins(4, 0, 0, 0);
	lt->setSpacing(0);
	lt->addWidget(mPathname);

	mButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
	mButton->setFixedWidth(20);
	setFocusProxy(mButton);
	setFocusPolicy(mButton->focusPolicy());
	mButton->setText(tr("..."));

	connect(mButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	lt->addWidget(mButton);

}

FileChooserWidget::~FileChooserWidget()
{

}

void FileChooserWidget::setValue(const QString& value)
{
	mPathname->setText(value);
}

void FileChooserWidget::buttonClicked()
{
	QFileDialog dlg(this);
	dlg.setFileMode(QFileDialog::ExistingFile);
	dlg.setNameFilter(tr("All Files (*.*)"));
	QStringList files;

	if (dlg.exec()) {
		files = dlg.selectedFiles();
		setValue(files.at(0));
		emit valueChanged(files.at(0));
		mPropMgr->setValue(mProperty, files.at(0));
	}
}

FileChooserFactory::FileChooserFactory(QObject* parent)
	: QtAbstractEditorFactory(parent)
{

}

FileChooserFactory::~FileChooserFactory()
{

}

void FileChooserFactory::connectPropertyManager(QtStringPropertyManager* manager)
{

}

void FileChooserFactory::disconnectPropertyManager(QtStringPropertyManager* manager)
{

}

QWidget* FileChooserFactory::createEditor(QtStringPropertyManager* manager, QtProperty* property, QWidget* parent)
{
	return new FileChooserWidget(parent, manager, property, manager->value(property));
}
