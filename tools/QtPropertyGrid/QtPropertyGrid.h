/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_QTPROPERTYGRID_INCLUDED)
#define TEARDROP_QTPROPERTYGRID_INCLUDED

#include <QTreeView>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class QtPropertyGridDelegate;
		class QtPropertyGridModel;

		class QtPropertyGrid : public QTreeView
		{
			Q_OBJECT

		public:
			QtPropertyGrid(QWidget* parent);
			~QtPropertyGrid();

			void setObject(Reflection::Object* object, Reflection::Object* metadata = 0);

			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);

		protected:
			QtPropertyGridDelegate* mDelegate;
			QtPropertyGridModel* mModel;
			Reflection::Object* mObject;
			Reflection::Object* mMetadata;
		};
	}
}
#endif // TEARDROP_QTPROPERTYGRID_INCLUDED
