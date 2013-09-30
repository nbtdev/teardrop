/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "TypeChooser.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include <QCompleter>
#include <QStringList>
#include "Reflection/ClassDef.h"

using namespace Teardrop;
using namespace Tools;
using namespace Reflection;

TypeChooser::TypeChooser(QWidget* parent, const ClassDef* baseClass)
	: QDialog(parent)
	, mBaseClass(baseClass)
	, mChosenClass(0)
{
	resize(382, 97);

	QVBoxLayout* verticalLayout = new QVBoxLayout(this);
	QLabel* label = new QLabel(this);
	verticalLayout->addWidget(label);

	mTypeList = new QComboBox(this);
	mTypeList->setEditable(true);
	verticalLayout->addWidget(mTypeList);

	QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	verticalLayout->addItem(verticalSpacer);

	mButtonBox = new QDialogButtonBox(this);
	mButtonBox->setOrientation(Qt::Horizontal);
	mButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	verticalLayout->addWidget(mButtonBox);

	connect(mButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(mButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	// populate the list with types of the given base class
	ClassDef* classDef = ClassDef::getClasses();
	QStringList list;
	while (classDef) {
		if (classDef->isA(baseClass) && classDef->isCreatable()) {
			QVariant v = qVariantFromValue((void*)classDef);
			mTypeList->addItem((const char*)classDef->getName(), v);
			list.append(classDef->getName());
		}

		classDef = classDef->m_pNext;
	}

	QCompleter* completer = new QCompleter(list, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	mTypeList->setCompleter(completer);
	mTypeList->setCurrentIndex(0);
}

TypeChooser::~TypeChooser()
{

}

const Reflection::ClassDef* TypeChooser::chosenClass()
{
	return mChosenClass;
}

void TypeChooser::accept()
{
	QVariant v = mTypeList->itemData(mTypeList->currentIndex());
	mChosenClass = (ClassDef*)v.value<void*>();
	QDialog::accept();
}