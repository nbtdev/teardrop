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

#include "ExpressionConnector.h"
#include "ExpressionItem.h"
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

namespace Teardrop {
namespace Tools {

ExpressionConnector::ExpressionConnector(ExpressionItem* aParent, const Gfx::Attribute& aAttr, bool aOutput)
	: QGraphicsItem(aParent)
	, mAttr(aAttr)
	, mOutput(aOutput) 
{
	setAcceptHoverEvents(true);
}

void ExpressionConnector::onPositionChanged(const QPointF& aPos) 
{
	PositionChanged.raise(this);
}

const Gfx::Attribute& ExpressionConnector::attribute() const
{
	return mAttr;
}

bool ExpressionConnector::isOutput() const 
{
	return mOutput;
}

bool ExpressionConnector::isWithin(const QPointF& aPos) const 
{
	QRectF r = boundingRect();

	float x = aPos.x();
	float y = aPos.y();

	return (r.left() < x && x < r.right() && r.top() < y && y < r.bottom());
}

QPointF ExpressionConnector::globalPos() const
{
	return pos() + parentItem()->pos();
}

QRectF ExpressionConnector::boundingRect() const
{
	return QRectF(0.f, 0.f, 10.f, 10.f);
}

void ExpressionConnector::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();
	painter->setBrush(Qt::gray);
	painter->drawRect(0.f, 0.f, 10.f, 10.f);
	painter->restore();

}

void ExpressionConnector::hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent)
{
	QGraphicsItem::hoverEnterEvent(aEvent);
	QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
}

void ExpressionConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent)
{
	QGraphicsItem::hoverLeaveEvent(aEvent);
	QApplication::restoreOverrideCursor();
}

} // namespace Tools
} // namespace Teardrop
