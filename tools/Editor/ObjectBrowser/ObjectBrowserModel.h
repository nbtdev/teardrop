/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTOBJECTBROWSERMODEL_INCLUDED)
#define QTOBJECTBROWSERMODEL_INCLUDED

#include <QAbstractListModel>
#include <QList>

namespace Teardrop
{
	namespace Reflection
	{
		class ClassDef;
	}

	namespace Tools
	{
		class ProjectItem;

		class ObjectBrowserModel : public QAbstractListModel
		{
		public:
			ObjectBrowserModel(ProjectItem* topLevelItem, QObject* parent=0);
			~ObjectBrowserModel();

			// QAbstractListModel implementation
			QVariant data(const QModelIndex &proxyIndex, int role /* = Qt::DisplayRole */) const;
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const;
			QStringList mimeTypes() const;
			QMimeData* mimeData(const QModelIndexList &indexes) const;
			Qt::DropActions supportedDropActions() const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
			Qt::ItemFlags flags(const QModelIndex& index) const;
			bool recursive() const;
			void setRecursive(bool recursive);
			ProjectItem* addNew(Reflection::ClassDef* classDef);
			void remove(ProjectItem* item);

		protected:
			ProjectItem* mTopLevelItem;
			QList<ProjectItem*> mImmediateChildren;
			QList<ProjectItem*> mRecursiveChildren;
			bool mRecursive;
		};
	}
}
#endif // QTOBJECTBROWSERMODEL_INCLUDED
