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

#include "ExpressionConnection.h"
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QVector2D>

namespace Teardrop {
namespace Tools {

ExpressionConnection::ExpressionConnection(ExpressionConnector::ConstPtr aFrom, ExpressionConnector::ConstPtr aTo)
	: mFrom(aFrom)
	, mTo(aTo)
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);

	mFrom->PositionChanged.bind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
	mTo->PositionChanged.bind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));

	setZValue(-10.f);
}

ExpressionConnection::~ExpressionConnection() {
	mFrom->PositionChanged.unbind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
	mTo->PositionChanged.unbind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
}

void ExpressionConnection::onConnectorChangedPosition(ExpressionConnector* aConn)
{
	this->update();
}

// EditorCanvasItem implementation
bool ExpressionConnection::isPath() const 
{ 
	return true; 
}

bool ExpressionConnection::isItem() const
{
	return false;
}

bool ExpressionConnection::isConnector() const
{
	return false;
}

QRectF ExpressionConnection::boundingRect() const 
{
	QPointF from = mFrom->globalPos();
	QPointF to = mTo->globalPos();

	qreal left = std::min(from.x(), to.x());
	qreal top = std::min(from.y(), to.y());
	qreal width = std::fabs(from.x() - to.x());
	qreal height = std::fabs(from.y() - to.y());

	return QRectF(QPointF(left, top), QSizeF(width, height));
}

void ExpressionConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) 
{
	QPointF from = mFrom->targetPos();
	QPointF to = mTo->targetPos();

	QPainterPath path;
	path.moveTo(from);

	QPointF p1(from);
	QPointF p3(to);

	p1.setX(to.x());
	p3.setX(from.x());

	path.cubicTo(p1, p3, to);
	painter->save();
	painter->setPen(QColor(255, 128, 0));
	painter->drawPath(path);
	painter->restore();
}

} // namespace Tools
} // namespace Teardrop
