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

#include "EditorCanvas.h"
#include "ExpressionItem.h"
#include "ExpressionConnection.h"
#include "ExpressionConnector.h"
#include "Gfx/Attribute.h"
#include <QtGui/QMouseEvent>
#include <QDebug>

namespace Teardrop {
namespace Tools {

EditorCanvas::EditorCanvas(Gfx::Material* aMaterial, QWidget* aParent)
	: QGraphicsView(aParent)
	, mMaterial(aMaterial)
{
	setBackgroundBrush(Qt::black);
}

EditorCanvas::~EditorCanvas()
{
}

void EditorCanvas::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);

	QGraphicsItem* item = itemAt(event->pos());

	if (item) {
		ExpressionItem* ei = static_cast<ExpressionItem*>(item);
		EditorCanvasItem* canvasItem = static_cast<EditorCanvasItem*>(ei);

		if (canvasItem) {
			if (canvasItem->isItem())
				ItemSelected.raise(static_cast<ExpressionItem*>(item));
			else if (canvasItem->isPath())
				PathSelected.raise(static_cast<ExpressionConnection*>(item));

			ExpressionConnector::ConstRef connRef = ei->connectorAt(event->pos() - ei->pos().toPoint());
			ExpressionConnector::ConstPtr conn = connRef.lock();
			if (conn) {
				qDebug(conn->attribute().mName);
			}
		}
	}
	else {
		SelectionCleared.raise();
	}
}

void EditorCanvas::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);

	//QGraphicsItem* item = itemAt(event->pos());

	//if (item) {
	//	ExpressionItem* ei = static_cast<ExpressionItem*>(item);
	//	EditorCanvasItem* canvasItem = static_cast<EditorCanvasItem*>(ei);

	//	ExpressionConnector::ConstRef connRef = ei->connectorAt(event->pos());
	//	ExpressionConnector::ConstPtr conn = connRef.lock();
		//if (conn) {
		//	qDebug(conn->attribute().mName);
		//}
	//}
}

} // namespace Tools
} // namespace Teardrop