/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

#if !defined(PROJECTMODEL_INCLUDED)
#define PROJECTMODEL_INCLUDED

#include <QtCore/QAbstractItemModel>
#include <QtCore/QStringList>

namespace Teardrop
{
	namespace Reflection
	{
		class ClassDef;
	}

	namespace Tools
	{
		class Project;
		class ProjectItem;

		class ProjectModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			ProjectModel(Project* project, QObject* parent = 0);
			~ProjectModel();

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
			void deleteFolder(const QModelIndex& parent, bool bRecursive=false);
			void addObject(const QModelIndex& parent, Reflection::ClassDef* classDef);
			void addPackage(const char* path = 0);
			void removePackage(const QModelIndex& package);

			// item appearance/state
			void setActiveIndex(const QModelIndex& index);

		protected:
			Project* mProject;
			ProjectItem* mRoot;
		};
	}
}
#endif // PROJECTMODEL_INCLUDED
