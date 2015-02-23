/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_PROPERTYGRID_INCLUDED)
#define TEARDROP_PROPERTYGRID_INCLUDED

#include <QtWidgets/QTreeView>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class PropertyGridDelegate;
		class PropertyGridModel;

		class PropertyGrid : public QTreeView
		{
			Q_OBJECT

		public:
			PropertyGrid(QWidget* parent);
			~PropertyGrid();

			void setObject(Reflection::Object* object, Reflection::Object* metadata = 0);

			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);

		protected:
			PropertyGridDelegate* mDelegate;
			PropertyGridModel* mModel;
			Reflection::Object* mObject;
			Reflection::Object* mMetadata;
		};
	}
}
#endif // TEARDROP_PROPERTYGRID_INCLUDED
