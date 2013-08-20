/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(INTPROPERTYHELPER_INCLUDED)
#define INTPROPERTYHELPER_INCLUDED

#include "PropertyHelper.h"

class QtProperty;
class QtIntPropertyManager;
class QString;

namespace Teardrop
{
	namespace Tools
	{
		class IntPropertyHelper : public PropertyHelper
		{
			Q_OBJECT

		public:
			IntPropertyHelper(QtIntPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef);
			~IntPropertyHelper();

			protected slots:
				void onValueChanged(QtProperty* prop, int val);

		protected:
			QtIntPropertyManager* mPropMgr;
		};
	}
}

#endif // INTPROPERTYHELPER_INCLUDED
