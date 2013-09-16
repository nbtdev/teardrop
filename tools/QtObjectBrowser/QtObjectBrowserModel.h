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
	namespace Tools
	{
		class QtProjectItem;

		class QtObjectBrowserModel : public QAbstractListModel
		{
		public:
			QtObjectBrowserModel(QtProjectItem* topLevelItem, QObject* parent=0);
			~QtObjectBrowserModel();

			// QAbstractListModel implementation
			QVariant data(const QModelIndex &proxyIndex, int role /* = Qt::DisplayRole */) const;
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);

			bool recursive() const;
			void setRecursive(bool recursive);

		protected:
			QtProjectItem* mTopLevelItem;
			QList<QtProjectItem*> mImmediateChildren;
			QList<QtProjectItem*> mRecursiveChildren;
			bool mRecursive;
		};
	}
}
#endif // QTOBJECTBROWSERMODEL_INCLUDED
