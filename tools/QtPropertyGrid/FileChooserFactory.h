/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FILECHOOSER_FACTORY_INCLUDED)
#define FILECHOOSER_FACTORY_INCLUDED

#include "ThirdParty/Qt4/QtTreePropertyBrowser/qteditorfactory.h"
#include <list>

class QLabel;
class QLineEdit;
class QToolButton;

namespace Teardrop {
	namespace Tools {

		class FileChooserWidget : public QWidget
		{
			Q_OBJECT

			QLineEdit* mPathname;
			QToolButton* mButton;
			QtProperty* mProperty;
			QtStringPropertyManager* mPropMgr;

		public:
			FileChooserWidget(QWidget* parent, QtStringPropertyManager* propMgr, QtProperty* prop, const QString& initialValue);
			~FileChooserWidget();

			public Q_SLOTS:
				void setValue(const QString& value);

			private Q_SLOTS:
				void buttonClicked();

			Q_SIGNALS:
				void valueChanged(const QString& value);
		};

		class FileChooserFactory : public QtAbstractEditorFactory<QtStringPropertyManager>
		{
			Q_OBJECT

		public:
			FileChooserFactory(QObject* parent=0);
			~FileChooserFactory();

		protected:
			FileChooserWidget* mWidget;

			void connectPropertyManager(QtStringPropertyManager* manager);
			QWidget* createEditor(QtStringPropertyManager* manager, QtProperty* property, QWidget* parent);
			void disconnectPropertyManager(QtStringPropertyManager* manager);
		};
	}
}

#endif // FILECHOOSER_FACTORY_INCLUDED
