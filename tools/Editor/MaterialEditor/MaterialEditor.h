/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(MATERIALEDITOR_INCLUDED)
#define MATERIALEDITOR_INCLUDED

#include <QWidget>
#include <QStringListModel>
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

		class MaterialEditor : public QWidget
		{
			Q_OBJECT

		public:
			MaterialEditor(ProjectItem* materialItem, QWidget* parent=0);
			~MaterialEditor();

			Package* package();

		private:
			Gfx::Material* mMaterial;
			PropertyGrid* mPropGrid;
			QWidget* m3DView;
			QGraphicsView* mView;
			QListView* mExpressionListView;

			// material expressions and list model
			QStringListModel mExpressionModel;
			std::vector<Reflection::ClassDef*> mExpressionClasses;

			// editors create/own temporary packages to hold their objects
			Package* mPackage;
		};
	}
}

#endif // MATERIALEDITOR_INCLUDED
