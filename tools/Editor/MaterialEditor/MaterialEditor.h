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

#if !defined(MATERIALEDITOR_INCLUDED)
#define MATERIALEDITOR_INCLUDED

#include <QtWidgets/QWidget>
#include <QtCore/QStringListModel>
#include <vector>

class QGraphicsView;
class QListView;

namespace Teardrop
{
	class Package;

	namespace Gfx
	{
		class Material;
	}

	namespace Reflection
	{
		class ClassDef;
	}

	namespace Tools
	{
		class PropertyGrid;
		class ProjectItem;
		class ExpressionItem;
		class ExpressionConnection;
		class EditorCanvas;

		class MaterialEditor : public QWidget
		{
			Q_OBJECT

		public:
			MaterialEditor(ProjectItem* materialItem, QWidget* parent=0);
			~MaterialEditor();

			Package* package();

		public slots:
			void onClose();

		private:
			Gfx::Material* mMaterial;
			PropertyGrid* mPropGrid;
			QWidget* m3DView;
			EditorCanvas* mView;
			QListView* mExpressionListView;

			// material expressions and list model
			QStringListModel mExpressionModel;
			std::vector<Reflection::ClassDef*> mExpressionClasses;
			std::vector<ExpressionItem*> mExpressionItems;

			// editors create/own temporary packages to hold their objects
			Package* mPackage;

			enum State {
				IDLE,
				MOVE_OBJECT,
			};

			State mState;

			// editor canvas event handlers
			void onItemSelected(ExpressionItem* aItem);
			void onConnectionSelected(ExpressionConnection* aConnection);
			void onSelectionCleared();
		};
	}
}

#endif // MATERIALEDITOR_INCLUDED
