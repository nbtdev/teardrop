/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PROPERTYGRIDITEM_INCLUDED)
#define PROPERTYGRIDITEM_INCLUDED

#include <QtCore/QString>
#include <QtCore/QList>

namespace Teardrop {

	namespace Reflection
	{
		class Object;
		class PropertyDef;
	}

	namespace Tools 
	{
		class PropertyGridItem
		{
		public:
			PropertyGridItem(Reflection::Object* obj, const Reflection::PropertyDef* propDef, PropertyGridItem* parent=0);
			PropertyGridItem(const QString& name, PropertyGridItem* parent=0);
			~PropertyGridItem();

			Reflection::Object* object() const;
			const Reflection::PropertyDef* property() const;
			const QString& name() const;
			QString valueAsString() const;
			QString nameTooltip() const;
			QString valueTooltip() const;
			PropertyGridItem* child(int row) const;
			PropertyGridItem* parent() const;
			int row() const;
			int numChildren() const;
			void append(PropertyGridItem* child);
			void setAltValue(const QString& altValue);

			// true if this item represents a property group and not an actual property
			bool isGroup() const;
			// return true if this property refers to a pointer property (can't edit, should be grayed out)
			bool isPointer() const;
			// return true if this property refers to a boolean property (should be displayed and edited as a checkbox)
			bool isBoolean() const;
			// return whether this property is read-only by definition
			bool isReadOnly() const;
			// return true if this is an enum type
			bool isEnum() const;
			// return true if this is a nested property
			bool isNested() const;

		private:
			Reflection::Object* mObject;
			const Reflection::PropertyDef* mProp;
			QList<PropertyGridItem*> mChildren;
			PropertyGridItem* mParent;
			QString mName;
			QString mAltValue;
		};
	}
}

#endif // PROPERTYGRIDITEM_INCLUDED
