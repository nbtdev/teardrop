/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "MaterialEditor.h"
#include "PropertyGrid/PropertyGrid.h"
#include "ProjectExplorer/ProjectItem.h"
#include "PackageManager/Metadata.h"
#include "Package/Package.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialExpression.h"
#include "Reflection/ClassDef.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QListView>
#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtGui/QIcon>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QGraphicsRectItem>

using namespace Teardrop;
using namespace Tools;

class TestItem : public QGraphicsItem
{
public:
	QRectF boundingRect() const {
		return QRectF(10, 100, 200, 50);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
		painter->drawRoundedRect(10, 100, 200, 50, 5, 5);
	}
};

MaterialEditor::MaterialEditor(ProjectItem* materialItem, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mPackage(0)
{
	mMaterial = static_cast<Gfx::Material*>(materialItem->object());
	Metadata* meta = materialItem->metadata();

	setWindowIcon(QIcon("icons/td-icon-32.png"));

	QVBoxLayout* vlayout = new QVBoxLayout(this);
	QWidget* top = new QWidget(this);
	mPropGrid = new PropertyGrid(this);
	vlayout->addWidget(top);
	vlayout->addWidget(mPropGrid);

	QHBoxLayout* hlayout = new QHBoxLayout(top);
	m3DView = new QWidget(this);
	mView = new QGraphicsView(this);
	hlayout->addWidget(m3DView);
	hlayout->addWidget(mView);

	QWidget* viewWidget = new QWidget(this);
	QVBoxLayout* vlayout2 = new QVBoxLayout(viewWidget);
	QLabel* label = new QLabel(viewWidget);
	label->setText("Material Expressions");

	mExpressionListView = new QListView(this);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mExpressionListView->sizePolicy().hasHeightForWidth());
	mExpressionListView->setSizePolicy(sizePolicy);

	vlayout2->addWidget(label);
	vlayout2->addWidget(mExpressionListView);

	hlayout->addWidget(viewWidget);

	setAttribute(Qt::WA_DeleteOnClose);
	resize(800, 600);

	QString title("Material Editor - ");
	title.append(meta->getName());
	setWindowTitle(title);

	// populate the expression list with candidates
	QStringList exprNames;
	Reflection::ClassDef* classDef = Reflection::ClassDef::getClasses();
    Reflection::ClassDef* mtlExprClass = Gfx::MaterialExpression::getClassDef();

    while (classDef) {
        if (classDef->isA(mtlExprClass) && classDef != mtlExprClass && classDef->isCreatable()) {
			exprNames.append(classDef->getName());
			mExpressionClasses.push_back(classDef);
		}

		classDef = classDef->m_pNext;
	}

	mExpressionModel.setStringList(exprNames);
	mExpressionListView->setModel(&mExpressionModel);

	mView->setScene(new QGraphicsScene);
	mView->scene()->addItem(new QGraphicsRectItem(10, 100, 200, 100));
}

MaterialEditor::~MaterialEditor()
{
	delete mPackage;
}

Package* MaterialEditor::package()
{
	if (!mPackage) {
		mPackage = TD_NEW Package;
	}

	return mPackage;
}
