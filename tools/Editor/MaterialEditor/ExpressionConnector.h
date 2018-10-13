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

#if !defined(__TEARDROP_EXPRESSION_CONNECTOR_H__)
#define __TEARDROP_EXPRESSION_CONNECTOR_H__

#include "EditorCanvasItem.h"
#include "Util/Event.h"
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QBrush>
#include <memory>

namespace Teardrop {

	namespace Gfx { struct Attribute;  }

	namespace Tools {

		class ExpressionItem;

		class ExpressionConnector : public QGraphicsItem, public EditorCanvasItem
		{
			const Gfx::Attribute& mAttr;
			QBrush mBrush;
			bool mOutput; // true if the connector is an output, false otherwise

		public:
			typedef std::shared_ptr<ExpressionConnector> Ptr;
			typedef std::shared_ptr<const ExpressionConnector> ConstPtr;
			typedef std::weak_ptr<ExpressionConnector> Ref;
			typedef std::weak_ptr<const ExpressionConnector> ConstRef;

			mutable Event<ExpressionConnector* /*this*/> PositionChanged;

			ExpressionConnector(ExpressionItem* aParent, const Gfx::Attribute& aAttr, bool aOutput);

			void notifyMoved(const QPointF& aPos);
			const Gfx::Attribute& attribute() const;
			bool isOutput() const;
			bool isWithin(const QPointF& aPos) const;
			QPointF globalPos() const;
			QPointF targetPos() const;

			// QGraphicsItem overrides
			QRectF boundingRect() const;
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
			void hoverEnterEvent(QGraphicsSceneHoverEvent* aEvent);
			void hoverLeaveEvent(QGraphicsSceneHoverEvent* aEvent);

			// EditorCanvasItem overrides
			bool isItem() const;
			bool isPath() const;
			bool isConnector() const;
		};

	} // namespace Tools
} // namespace Teardrop

#endif // __TEARDROP_EXPRESSION_CONNECTOR_H__
