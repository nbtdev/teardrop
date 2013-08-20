/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FLOATPROPERTYHELPER_INCLUDED)
#define FLOATPROPERTYHELPER_INCLUDED

#include "PropertyHelper.h"

class QtProperty;
class QtDoublePropertyManager;
class QString;

namespace Teardrop
{
	namespace Tools
	{
		class FloatPropertyHelper : public PropertyHelper
		{
			Q_OBJECT

		public:
			FloatPropertyHelper(QtDoublePropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef);
			~FloatPropertyHelper();

			protected slots:
				void onValueChanged(QtProperty* prop, double val);

		protected:
			QtDoublePropertyManager* mPropMgr;
		};
	}
}

#endif // FLOATPROPERTYHELPER_INCLUDED
