/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROPERTYHELPER_INCLUDED)
#define PROPERTYHELPER_INCLUDED

#include <QObject>
class QtProperty;

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
		class PropertyDef;
	}

	namespace Tools
	{
		class PropertyHelper : public QObject
		{
			Q_OBJECT

		public:
			PropertyHelper(Reflection::Object* obj, const Reflection::PropertyDef* propDef, QtProperty* prop);
			virtual ~PropertyHelper();

			// callback when property value in Reflection::Object instance changes
			void propValueChanged(const Reflection::PropertyDef* propDef);

		protected:
			virtual void onValueChanged(const Reflection::PropertyDef* propDef);

			Reflection::Object* mObject;
			const Reflection::PropertyDef* mPropDef;
			QtProperty* mMyProp;
		};
	}
}

#endif // PROPERTYHELPER_INCLUDED
