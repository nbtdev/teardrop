/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined(__TEARDROP_EDITOR_CANVAS_H__)
#define __TEARDROP_EDITOR_CANVAS_H__

#include "FauxConnector.h"
#include "Util/Event.h"
#include <QtWidgets/QGraphicsView>

namespace Teardrop {

	namespace Gfx { class Material; }

	namespace Tools {

		class EditorCanvasItem;
		class ExpressionItem;
		class ExpressionConnection;

		class EditorCanvas : public QGraphicsView
		{
			Gfx::Material* mMaterial;

		public:
			Event<ExpressionItem*> ItemSelected;
			Event<ExpressionConnection*> PathSelected;
			Event<> SelectionCleared;

			EditorCanvas(Gfx::Material* aMaterial, QWidget* aParent);
			~EditorCanvas();

		protected:
			void mousePressEvent(QMouseEvent* event);

			// used as the "to" end of a connection rubberband; this connector 
			// doesn't draw, it just makes the ExpressionConnection work
			FauxConnector::Ptr mFauxConnector;

			// temporary rubber-band connection path object
			ExpressionConnection* mRBConnection;
		};

	} // namespace Tools
} // namespace Teardrop

#endif // __TEARDROP_EDITOR_CANVAS_H__
