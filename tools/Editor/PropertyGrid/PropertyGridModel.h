/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_PROPERTYGRIDMODEL_INCLUDED)
#define TEARDROP_PROPERTYGRIDMODEL_INCLUDED

#include <QtCore/QAbstractItemModel>
#include <QtCore/QStringList>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class PropertyGridItem;

		class PropertyGridModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			PropertyGridModel(Reflection::Object* obj = 0, Reflection::Object* metadata = 0, QObject* parent = 0);
			~PropertyGridModel();

			// QAbstractItemModel implementation
			int columnCount(const QModelIndex& parent = QModelIndex() ) const;
			QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
			bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);
			QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
			QModelIndex parent(const QModelIndex& index) const;
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			Qt::ItemFlags flags(const QModelIndex& index) const;
			QVariant headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */) const;

			// drag and drop support
			QStringList mimeTypes() const;
			Qt::DropActions supportedDropActions() const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

		protected:
			Reflection::Object* mObject;
			Reflection::Object* mMetadata;
			PropertyGridItem* mRoot;
		};
	}
}
#endif // TEARDROP_PROPERTYGRIDMODEL_INCLUDED
