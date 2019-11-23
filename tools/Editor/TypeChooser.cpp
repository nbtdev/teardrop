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

#include "TypeChooser.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QCompleter>
#include <QtCore/QStringList>
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
