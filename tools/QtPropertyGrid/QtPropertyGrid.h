/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_QTPROPERTYGRID_INCLUDED)
#define TEARDROP_QTPROPERTYGRID_INCLUDED

#include "QtPropertyBrowser/qttreepropertybrowser.h"
#include <list>

class QtStringPropertyManager;
class QtDoublePropertyManager;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtGroupPropertyManager;
class QtLineEditFactory;
class QtCheckBoxFactory;
class QtDoubleSpinBoxFactory;

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
		class PropertyDef;
	}

	namespace Tools
	{
		class PropertyHelper;
		class FileChooserFactory;

		class QtPropertyGrid 
			: public QtTreePropertyBrowser
		{
			Q_OBJECT

		public:
			QtPropertyGrid(QWidget* parent);
			~QtPropertyGrid();

			// PropertyGrid implementation
			void setObject(Reflection::Object* object, Reflection::Object* metadata = 0);

		protected:
			Reflection::Object* mObject;
			Reflection::Object* mMetadata;

			QtStringPropertyManager* mStringPropMgr;
			QtStringPropertyManager* mPathPropMgr;
			QtDoublePropertyManager* mDoublePropMgr;
			QtIntPropertyManager* mIntPropMgr;
			QtBoolPropertyManager* mBoolPropMgr;
			QtGroupPropertyManager* mGroupPropMgr;

			QtLineEditFactory* mLineEditFactory;
			QtDoubleSpinBoxFactory* mDoubleSpinBoxFactory;
			QtCheckBoxFactory* mCheckBoxFactory;
			FileChooserFactory* mFileChooserFactory;

		private:
			void clearHelpers();
			void addProperty(QtProperty* parent, Reflection::Object* obj, const Reflection::PropertyDef* propDef);

			typedef std::list<PropertyHelper*> PropertyHelpers;
			PropertyHelpers mHelpers;
		
		private slots:
		};
	}
}

#endif // TEARDROP_QTPROPERTYGRID_INCLUDED
