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
#include "Gfx/Attribute.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Reflection/ClassDef.h"
#include "Util/Event.h"
#include "Util/_String.h"
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainterPath>
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

class ExpressionConnector
{
	Gfx::Attribute& mAttr;
	QPointF mPos;
	bool mOutput; // true if the connector is an output, false otherwise

public:
	Event<ExpressionConnector* /*this*/> PositionChanged;

	ExpressionConnector(Gfx::Attribute& aAttr, bool aOutput) : mAttr(aAttr), mOutput(aOutput) {}

	void onPositionChanged(const QPointF& aPos) {
		mPos = aPos;
		PositionChanged.raise(this);
	}

	Gfx::Attribute& attribute() {
		return mAttr;
	}

	bool isOutput() {
		return mOutput;
	}
};

class EditorCanvasItem
{
public:
	virtual bool isPath() const abstract;
	virtual bool isItem() const abstract;
};

class ExpressionConnection : public QGraphicsPathItem, public EditorCanvasItem
{
	ExpressionConnector* mA;
	ExpressionConnector* mB;

public:
	ExpressionConnection(ExpressionConnector* aA=nullptr, ExpressionConnector* aB=nullptr) 
		: mA(aA)
		, mB(aB)
	{
		setFlag(ItemIsMovable);
		setFlag(ItemIsSelectable);

		mA->PositionChanged.bind(std::bind(ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
		mB->PositionChanged.bind(std::bind(ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
	}

	~ExpressionConnection() {
		mA->PositionChanged.unbind(std::bind(ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
		mB->PositionChanged.unbind(std::bind(ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
	}

	void onConnectorChangedPosition(ExpressionConnector* aConn) {

	}

	// EditorCanvasItem implementation
	bool isPath() const { return true; }
	bool isItem() const { return false; }
	
	QRectF boundingRect() const {
		return QRectF(/*mX, mY, mWidth + 20, mHeight*/);
	}

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	}
};

class ExpressionItem : public QGraphicsItem, public EditorCanvasItem
{
	Gfx::MaterialExpression* mExpr = nullptr;
	QStaticText mLabel;
	String mDisplayName;
	qreal mX = 0.f;
	qreal mY = 0.f;
	qreal mWidth = 0.f;
	qreal mHeight = 0.f;

public:
	ExpressionItem(Gfx::MaterialExpression* aExpr)
		: ExpressionItem(aExpr, 0, 0)
	{

	}
	
	ExpressionItem(Gfx::MaterialExpression* aExpr, int x, int y)
		: mExpr(aExpr)
		, mX(float(x))
		, mY(float(y))
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
		mWidth = mLabel.size().width() + 50.f;

		// height is based on how many inputs/outputs we have; take the largest of the two and amke enough room
		// (give the label/name 50, and then 50 for each connector)
		int nMax = std::max((int)inputs.size(), (int)outputs.size());
		mHeight = 50.f + 30.f * nMax;
	}

	QRectF boundingRect() const {
		return QRectF(mX, mY, mWidth+20, mHeight);
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

	// EditorCanvasItem implementation
	bool isPath() const { return false; }
	bool isItem() const { return true; }

	Gfx::MaterialExpression* expression()
	{
		return mExpr;
	}
};

class EditorCanvas : public QGraphicsView
{
	Gfx::Material* mMaterial;

public:
	Event<ExpressionItem*> ItemSelected;
	Event<ExpressionConnection*> PathSelected;
	Event<> SelectionCleared;

	EditorCanvas(Gfx::Material* aMaterial, QWidget* aParent) : QGraphicsView(aParent), mMaterial(aMaterial) {}

protected:
	void mousePressEvent(QMouseEvent* event) {
		QGraphicsView::mousePressEvent(event);

		QGraphicsItem* item = itemAt(event->pos());

		if (item) {
			EditorCanvasItem* canvasItem = reinterpret_cast<EditorCanvasItem*>(item);
			if (canvasItem->isItem())
				ItemSelected.raise(static_cast<ExpressionItem*>(item));
			else if (canvasItem->isPath())
				PathSelected.raise(static_cast<ExpressionConnection*>(item));
		} else {
			SelectionCleared.raise();
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
	mPropGrid->setObject(mMaterial);
	vlayout->addWidget(top);
	vlayout->addWidget(mPropGrid);

	QHBoxLayout* hlayout = TD_NEW QHBoxLayout(top);
	m3DView = TD_NEW QWidget(this);
	mView = TD_NEW EditorCanvas(mMaterial, this);
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
	resize(1280, 800);

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

	// add the expressions present in the material
	if (mMaterial) {
		// any material created in the tools will have at least a MaterialOutput
		assert(mMaterial->getOutput());

		mMaterial->sortExpressions();
		Gfx::MaterialExpression** exprs = mMaterial->sortedExpressions();
		int nExpr = mMaterial->expressionCount();
		mExpressionItems.resize(nExpr);

		// TODO: when the user makes changes to the material editor item layout, we should save 
		// the item positions off so that the layout can be loaded again later; for now, we just 
		// spread them out in a grid

		// figure out grid row count by taking square root of number of expressions; we will 
		// take the floor of that value for the number of rows, and the ceiling of that value
		// as the number of columns
		qreal sqr = std::sqrtf(float(nExpr));
		int rows = int(std::floorf(sqr));
		int cols = int(std::ceilf(sqr));

		int k = 0;
		qreal x = 0.f;
		qreal y = 0.f;
		for (int j = 0; j < rows; ++j, y += 300.f) {
			for (int i = 0; i < cols; ++i, x -= 150.f) {
				Gfx::MaterialExpression* expr = exprs[k];
				ExpressionItem* item = TD_NEW ExpressionItem(expr);
				mExpressionItems[k] = item;
				mView->scene()->addItem(item);
				item->setPos(x, y);
				++k;
			}
		}
	}

	mView->ItemSelected.bind(std::bind(&MaterialEditor::onItemSelected, this, std::placeholders::_1));
	mView->PathSelected.bind(std::bind(&MaterialEditor::onConnectionSelected, this, std::placeholders::_1));
	mView->SelectionCleared.bind(std::bind(&MaterialEditor::onSelectionCleared, this));
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

void MaterialEditor::onItemSelected(ExpressionItem* aItem)
{
	mPropGrid->setObject(aItem->expression());
}

void MaterialEditor::onConnectionSelected(ExpressionConnection* aItem)
{
	mPropGrid->setObject(nullptr);
}

void MaterialEditor::onSelectionCleared()
{
	mPropGrid->setObject(mMaterial);
}

} // namespace Tools
} // namespace Teardrop
