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
#include <QDebug>

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
}

ExpressionConnection::~ExpressionConnection() {
	mFrom->PositionChanged.unbind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
	mTo->PositionChanged.unbind(std::bind(&ExpressionConnection::onConnectorChangedPosition, this, std::placeholders::_1));
}

void ExpressionConnection::onConnectorChangedPosition(ExpressionConnector* aConn)
{

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

QRectF ExpressionConnection::boundingRect() const 
{
	return QRectF(/*mX, mY, mWidth + 20, mHeight*/);
}

void ExpressionConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) 
{
	QPointF from = mFrom->globalPos();
	QPointF to = mTo->globalPos();
	qDebug() << "from, to: " << from << ", " << to;
}

} // namespace Tools
} // namespace Teardrop
