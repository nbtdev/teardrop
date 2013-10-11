/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTMATERIALEDITOR_INCLUDED)
#define QTMATERIALEDITOR_INCLUDED

#include <QWidget>
class QGraphicsView;
class QListView;

namespace Teardrop
{
	namespace Gfx
	{
		class Material;
	}

	namespace Tools
	{
		class QtPropertyGrid;

		class QtMaterialEditor : public QWidget
		{
			Q_OBJECT

		public:
			QtMaterialEditor(Gfx::Material* mtl, QWidget* parent=0);
			~QtMaterialEditor();

		private:
			Gfx::Material* mMaterial;
			QtPropertyGrid* mPropGrid;
			QWidget* m3DView;
			QGraphicsView* mView;
			QListView* mExpressionListView;
		};
	}
}

#endif // QTMATERIALEDITOR_INCLUDED
