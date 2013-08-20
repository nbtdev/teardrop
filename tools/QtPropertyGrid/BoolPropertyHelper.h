/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(BOOLPROPERTYHELPER_INCLUDED)
#define BOOLPROPERTYHELPER_INCLUDED

#include "PropertyHelper.h"

class QtProperty;
class QtBoolPropertyManager;
class QString;

namespace Teardrop
{
	namespace Tools
	{
		class BoolPropertyHelper : public PropertyHelper
		{
			Q_OBJECT

		public:
			BoolPropertyHelper(QtBoolPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef);
			~BoolPropertyHelper();

			protected slots:
				void onValueChanged(QtProperty* prop, bool val);

		protected:
			QtBoolPropertyManager* mPropMgr;
		};
	}
}

#endif // BOOLPROPERTYHELPER_INCLUDED
