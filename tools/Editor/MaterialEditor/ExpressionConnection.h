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

#if !defined(__TEARDROP_EXPRESSION_CONNECTION_H__)
#define __TEARDROP_EXPRESSION_CONNECTION_H__

#include "EditorCanvasItem.h"
#include "ExpressionConnector.h"
#include <QtWidgets/QGraphicsPathItem>
#include <QtCore/QRect>

namespace Teardrop {
	namespace Tools {

		class ExpressionConnection : public QGraphicsPathItem, public EditorCanvasItem
		{
			ExpressionConnector::ConstPtr mFrom;
			ExpressionConnector::ConstPtr mTo;

		public:
			ExpressionConnection(ExpressionConnector::ConstPtr aFrom, ExpressionConnector::ConstPtr aTo);
			~ExpressionConnection();
			void onConnectorChangedPosition(ExpressionConnector* aConn);

			// EditorCanvasItem implementation
			bool isPath() const;
			bool isItem() const;

			QRectF boundingRect() const;
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
		};

	} // namespace Tools
} // namespace Teardrop

#endif // __TEARDROP_EXPRESSION_CONNECTION_H__
