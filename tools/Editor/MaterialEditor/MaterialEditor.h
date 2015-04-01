/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
