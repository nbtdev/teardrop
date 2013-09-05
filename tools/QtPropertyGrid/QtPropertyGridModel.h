/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_QTPROPERTYGRIDMODEL_INCLUDED)
#define TEARDROP_QTPROPERTYGRIDMODEL_INCLUDED

#include <QAbstractItemModel>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class QtPropertyGridItem;

		class QtPropertyGridModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			QtPropertyGridModel(Reflection::Object* obj = 0, QObject* parent = 0);
			~QtPropertyGridModel();

			// QAbstractItemModel implementation
			int columnCount(const QModelIndex& parent = QModelIndex() ) const;
			QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
			QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
			QModelIndex parent(const QModelIndex& index) const;
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			Qt::ItemFlags flags(const QModelIndex& index) const;

		protected:
			Reflection::Object* mObject;
			QtPropertyGridItem* mRoot;
		};
	}
}
#endif // TEARDROP_QTPROPERTYGRIDMODEL_INCLUDED
