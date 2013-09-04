/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(STRINGPROPERTYHELPER_INCLUDED)
#define STRINGPROPERTYHELPER_INCLUDED

#include "PropertyHelper.h"

class QtProperty;
class QtStringPropertyManager;
class QString;

namespace Teardrop
{
	namespace Tools
	{
		class StringPropertyHelper : public PropertyHelper
		{
			Q_OBJECT

		public:
			StringPropertyHelper(QtStringPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef);
			~StringPropertyHelper();

		protected slots:
			// this is for when the user changes the value in a property editor
			void onValueChanged(QtProperty* prop, const QString& val);

		protected:
			// this is for when the user does something that changes the object property value itself 
			void onValueChanged(const Reflection::PropertyDef* propDef);
			QtStringPropertyManager* mPropMgr;
		};
	}
}

#endif // STRINGPROPERTYHELPER_INCLUDED
