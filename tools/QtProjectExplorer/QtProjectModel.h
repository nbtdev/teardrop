/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTPROJECTMODEL_INCLUDED)
#define QTPROJECTMODEL_INCLUDED

#include <QAbstractItemModel>
#include <QStringList>

namespace Teardrop
{
	namespace Reflection
	{
		class ClassDef;
	}

	namespace Tools
	{
		class Project;
		class QtProjectItem;

		class QtProjectModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			QtProjectModel(Project* project, QObject* parent = 0);
			~QtProjectModel();

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
			QMimeData* mimeData(const QModelIndexList &indexes) const;
			Qt::DropActions supportedDropActions() const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

			// editing the model 
			void addFolder(const QModelIndex& parent);
			void addObject(const QModelIndex& parent, Reflection::ClassDef* classDef);
			void addPackage();

		protected:
			Project* mProject;
			QtProjectItem* mRoot;
		};
	}
}
#endif // QTPROJECTMODEL_INCLUDED
