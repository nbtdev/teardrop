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

#if !defined(__TEARDROP_EXPRESSION_ITEM_H__)
#define __TEARDROP_EXPRESSION_ITEM_H__

#include "EditorCanvasItem.h"
#include "ExpressionConnector.h"
#include "Util/Event.h"
#include "Util/_String.h"
#include <QtGui/QStaticText>
#include <QtWidgets/QGraphicsItem>

namespace Teardrop {

	namespace Gfx { class MaterialExpression; }

	namespace Tools {

		class ExpressionConnectorDatabase;

		class ExpressionItem : public QGraphicsItem, public EditorCanvasItem
		{
			Gfx::MaterialExpression* mExpr = nullptr;
			QStaticText mLabel;
			String mDisplayName;
			qreal mX = 0.f;
			qreal mY = 0.f;
			qreal mWidth = 0.f;
			qreal mHeight = 0.f;

			ExpressionConnectorDatabase* mConnectorDb;

			std::vector<ExpressionConnector::Ptr> mInputs;
			std::vector<ExpressionConnector::Ptr> mOutputs;

		protected:
			QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue);

		public:
			Event<ExpressionConnector::ConstPtr, const Gfx::Attribute*> ExpressionConnectorCreated;

			ExpressionItem(Gfx::MaterialExpression* aExpr, ExpressionConnectorDatabase* aConnectorDb);
			ExpressionItem(Gfx::MaterialExpression* aExpr, ExpressionConnectorDatabase* aConnectorDb, int x, int y);

			QRectF boundingRect() const;
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

			// EditorCanvasItem implementation
			bool isPath() const;
			bool isItem() const;

			Gfx::MaterialExpression* expression();
			ExpressionConnector::ConstRef connectorAt(const QPoint& aPos);
		};

	} // namespace Tools
} // namespace Teardrop

#endif // __TEARDROP_EXPRESSION_ITEM_H__
