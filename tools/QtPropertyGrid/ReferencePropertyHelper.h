/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(REFERENCEPROPERTYHELPER_INCLUDED)
#define REFERENCEPROPERTYHELPER_INCLUDED

#include "PropertyHelper.h"

class QtProperty;
class QtStringPropertyManager;
class QString;

namespace Teardrop
{
	namespace Tools
	{
		class ReferencePropertyHelper : public PropertyHelper
		{
			Q_OBJECT

		public:
			ReferencePropertyHelper(QtStringPropertyManager* propMgr, QtProperty* prop, Reflection::Object* obj, const Reflection::PropertyDef* propDef);
			~ReferencePropertyHelper();

			protected slots:
				void onValueChanged(QtProperty* prop, const QString& val);

		protected:
			void onValueChanged(const Reflection::PropertyDef* propDef);
			QtStringPropertyManager* mPropMgr;
		};
	}
}

#endif // REFERENCEPROPERTYHELPER_INCLUDED
