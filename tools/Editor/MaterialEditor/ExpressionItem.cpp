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

#include "ExpressionItem.h"
#include "ExpressionConnectorDatabase.h"
#include "Gfx/MaterialExpression.h"
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsSceneEvent>

namespace Teardrop {
namespace Tools {

ExpressionItem::ExpressionItem(Gfx::MaterialExpression* aExpr, ExpressionConnectorDatabase* aConnectorDb)
	: ExpressionItem(aExpr, aConnectorDb, 0, 0)
{

}

ExpressionItem::ExpressionItem(Gfx::MaterialExpression* aExpr, ExpressionConnectorDatabase* aConnectorDb, int x, int y)
	: mExpr(aExpr)
	, mConnectorDb(aConnectorDb)
	, mX(float(x))
	, mY(float(y))
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);

	// collect expression inputs and outputs
	const Gfx::MaterialExpression::Attributes& inputs = aExpr->inputAttributes();
	const Gfx::MaterialExpression::Attributes& outputs = aExpr->outputAttributes();

	// initialize our input and output connectors collections
	mInputs.resize(inputs.size());
	mOutputs.resize(outputs.size());

	// start with a box large enough to contain the expression's class name (without the "Expression")
	const String& name = aExpr->getDerivedClassDef()->getName();
	size_t pos = name.find("Expression");
	mDisplayName = name.substr(0, pos);

	mLabel.setText((const char*)mDisplayName);
	mWidth = mLabel.size().width() + 50.f;

	// create the input/output connectors
	size_t i = 0;
	qreal cy = 75.f;
	for (auto& attr : inputs) {
		ExpressionConnector* connector = TD_NEW ExpressionConnector(this, inputs[i], false);
		mInputs[i].reset(connector);
		mInputs[i]->setPos(mWidth + 10.f, cy);
		mConnectorDb->addConnector(mInputs[i], &connector->attribute());
		ExpressionConnectorCreated.raise(mInputs[i], &inputs[i]);
		++i;
		cy += 25.f;
	}

	i = 0;
	cy = 50.f;
	for (auto& attr : outputs) {
		ExpressionConnector* connector = TD_NEW ExpressionConnector(this, outputs[i], false);
		mOutputs[i].reset(connector);
		mOutputs[i]->setPos(0.f, cy);
		mConnectorDb->addConnector(mOutputs[i], &connector->attribute());
		ExpressionConnectorCreated.raise(mOutputs[i], &outputs[i]);
		++i;
		cy += 25.f;
	}

	// height is based on how many inputs/outputs we have; take the largest of the two and amke enough room
	// (give the label/name 50, and then 50 for each connector)
	int nMax = std::max((int)inputs.size(), (int)outputs.size());
	mHeight = 50.f + 30.f * nMax;
}

QVariant ExpressionItem::itemChange(GraphicsItemChange aChange, const QVariant& aValue) {
	if (aChange == ItemPositionChange) {
		QPointF newPos = aValue.toPointF();

		for (auto& i : mInputs) {
			i->notifyMoved(newPos);
		}

		for (auto& o : mOutputs) {
			o->notifyMoved(newPos);
		}
	}

	return QGraphicsItem::itemChange(aChange, aValue);
}

QRectF ExpressionItem::boundingRect() const 
{
	return QRectF(mX, mY, mWidth + 20, mHeight);
}

void ExpressionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) 
{
	painter->save();
	painter->setBrush(Qt::lightGray);
	painter->setPen(Qt::white);
	painter->drawRoundedRect(10, 0, mWidth, mHeight, 5, 5);
	painter->restore();

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
		// draw attribute name
		painter->drawText(x + 15.f, y + 10.f, outputs[i].mName);
		y += 25.f;
	}

	int x2 = boundingRect().right() - 10.f;
	y = 75.f;
	for (size_t i = 0; i < inputs.size(); ++i) {
		// draw attribute name
		painter->drawText(x + 15.f, y, mWidth - 10.f, 20.f, Qt::AlignRight, inputs[i].mName);
		y += 25.f;
	}
}

// EditorCanvasItem implementation
bool ExpressionItem::isPath() const 
{ 
	return false; 
}

bool ExpressionItem::isItem() const
{
	return true;
}

bool ExpressionItem::isConnector() const
{
	return false;
}

Gfx::MaterialExpression* ExpressionItem::expression()
{
	return mExpr;
}

ExpressionConnector::ConstRef ExpressionItem::connectorAt(const QPoint& aPos) 
{
	// check all inputs and outputs
	QPointF pf(aPos.x(), aPos.y());

	for (auto& o : mOutputs) {
		if (o->isWithin(pf))
			return o;
	}

	for (auto& i : mInputs) {
		if (i->isWithin(pf))
			return i;
	}

	return ExpressionConnector::ConstRef();
}

} // namespace Tools
} // namespace Teardrop
