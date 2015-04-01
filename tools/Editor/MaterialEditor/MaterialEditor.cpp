/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "MaterialEditor.h"
#include "PropertyGrid/PropertyGrid.h"
#include "ProjectExplorer/ProjectItem.h"
#include "PackageManager/Metadata.h"
#include "Package/Package.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Reflection/ClassDef.h"
#include "Util/_String.h"
#include <QtCore/QString>
#include <QtGui/QPainterPath>
#include <QtGui/QIcon>
#include <QtGui/QStaticText>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsSceneEvent.h>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QListView>
#include <QDebug>

namespace Teardrop {
namespace Tools {

class Connection : public QGraphicsPathItem
{
public:
	Connection() {
		setFlag(ItemIsMovable);
		setFlag(ItemIsSelectable);
	}
};

class ExpressionItem : public QGraphicsItem
{
	Gfx::MaterialExpression* mExpr = nullptr;
	QStaticText mLabel;
	String mDisplayName;
	qreal mWidth = 0;
	qreal mHeight = 0;

public:
	ExpressionItem(Gfx::MaterialExpression* aExpr) 
		: mExpr(aExpr)
	{
		setFlag(ItemIsMovable);
		setFlag(ItemIsSelectable);

		// collect expression inputs and outputs
		const Gfx::MaterialExpression::Attributes& inputs = aExpr->inputAttributes();
		const Gfx::MaterialExpression::Attributes& outputs = aExpr->outputAttributes();

		// start with a box large enough to contain the expression's class name (without the "Expression")
		const String& name = aExpr->getDerivedClassDef()->getName();
		size_t pos = name.find("Expression");
		mDisplayName = name.substr(0, pos);

		mLabel.setText((const char*)mDisplayName);
		mWidth = mLabel.size().width() + 30.f;

		// height is based on how many inputs/outputs we have; take the largest of the two and amke enough room
		// (give the label/name 50, and then 50 for each connector)
		int nMax = std::max((int)inputs.size(), (int)outputs.size());
		mHeight = 50.f + 30.f * nMax;
	}

	QRectF boundingRect() const {
		return QRectF(0, 0, mWidth+20, mHeight);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
		painter->drawRoundedRect(10, 0, mWidth, mHeight, 5, 5);

		// figure out where the text goes -- top-center of the box
		qreal tw = mLabel.size().width();
		qreal x = (boundingRect().size().width() - tw) / 2.f;
		painter->drawStaticText(QPoint(x, 15), mLabel);

		// draw a small box for each connector; data flows from right to left 
		// on the material editor canvas, so outputs go on the left, inputs on the 
		// right.
		// TODO: connectors in their own class?
		const Gfx::MaterialExpression::Attributes& inputs = mExpr->inputAttributes();
		const Gfx::MaterialExpression::Attributes& outputs = mExpr->outputAttributes();

		x = boundingRect().left();
		qreal y = 50.f;
		for (size_t i = 0; i < outputs.size(); ++i) {
			painter->drawRect(x, y, 10, 10);

			// draw attribute name too
			painter->drawText(x + 15.f, y + 10.f, outputs[i].mName);

			y += 25.f;
		}

		int x2 = boundingRect().right() - 10.f;
		y = 75.f;
		for (size_t i = 0; i < inputs.size(); ++i) {
			painter->drawRect(x2, y, 10, 10);

			// draw attribute name too
			painter->drawText(x + 15.f, y, mWidth - 10.f, 20.f, Qt::AlignRight, inputs[i].mName);

			y += 25.f;
		}
	}
};

MaterialEditor::MaterialEditor(ProjectItem* materialItem, QWidget* parent/* =0 */)
	: QWidget(parent)
	, mPackage(0)
	, mState(IDLE)
{
	mMaterial = static_cast<Gfx::Material*>(materialItem->object());
	Metadata* meta = materialItem->metadata();

	setWindowIcon(QIcon("icons/td-icon-32.png"));

	QVBoxLayout* vlayout = TD_NEW QVBoxLayout(this);
	QWidget* top = TD_NEW QWidget(this);
	mPropGrid = TD_NEW PropertyGrid(this);
	vlayout->addWidget(top);
	vlayout->addWidget(mPropGrid);

	QHBoxLayout* hlayout = TD_NEW QHBoxLayout(top);
	m3DView = TD_NEW QWidget(this);
	mView = TD_NEW QGraphicsView(this);
	hlayout->addWidget(m3DView);
	hlayout->addWidget(mView);

	QWidget* viewWidget = TD_NEW QWidget(this);
	QVBoxLayout* vlayout2 = TD_NEW QVBoxLayout(viewWidget);
	QLabel* label = TD_NEW QLabel(viewWidget);
	label->setText("Material Expressions");

	mExpressionListView = TD_NEW QListView(this);
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

	mView->setScene(TD_NEW QGraphicsScene);

	// add material output expression
	if (mMaterial) {
		Gfx::MaterialOutput* out = mMaterial->getOutput();
		if (out) {
			ExpressionItem* item = TD_NEW ExpressionItem(out);
			mView->scene()->addItem(item);
		}
	}
}

MaterialEditor::~MaterialEditor()
{
	delete mPackage;
}

void MaterialEditor::onClose()
{
	close();
}
Package* MaterialEditor::package()
{
	if (!mPackage) {
		mPackage = TD_NEW Package;
	}

	return mPackage;
}

} // namespace Tools
} // namespace Teardrop
