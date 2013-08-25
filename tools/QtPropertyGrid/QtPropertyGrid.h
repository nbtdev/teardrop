/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEARDROP_QTPROPERTYGRID_INCLUDED)
#define TEARDROP_QTPROPERTYGRID_INCLUDED

#include "ThirdParty/Qt4/QtTreePropertyBrowser/qttreepropertybrowser.h"
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

		class QtPropertyGrid 
			: public QtTreePropertyBrowser
		{
			Q_OBJECT

		public:
			QtPropertyGrid(QWidget* parent);
			~QtPropertyGrid();

			// PropertyGrid implementation
			void setObject(Reflection::Object* object);

		protected:
			Reflection::Object* mObject;

			QtStringPropertyManager* mStringPropMgr;
			QtDoublePropertyManager* mDoublePropMgr;
			QtIntPropertyManager* mIntPropMgr;
			QtBoolPropertyManager* mBoolPropMgr;
			QtGroupPropertyManager* mGroupPropMgr;

			QtLineEditFactory* mLineEditFactory;
			QtDoubleSpinBoxFactory* mDoubleSpinBoxFactory;
			QtCheckBoxFactory* mCheckBoxFactory;

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