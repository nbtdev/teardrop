/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(QTPROPERTYGRIDITEM_INCLUDED)
#define QTPROPERTYGRIDITEM_INCLUDED

#include <QString>
#include <QList>

namespace Teardrop {

	namespace Reflection
	{
		class Object;
		class PropertyDef;
	}

	namespace Tools 
	{
		class QtPropertyGridItem
		{
		public:
			QtPropertyGridItem(Reflection::Object* obj, const Reflection::PropertyDef* propDef, QtPropertyGridItem* parent=0);
			QtPropertyGridItem(const QString& name, QtPropertyGridItem* parent=0);
			~QtPropertyGridItem();

			Reflection::Object* object() const;
			const Reflection::PropertyDef* property() const;
			const QString& name() const;
			QString valueAsString() const;
			QString nameTooltip() const;
			QString valueTooltip() const;
			QtPropertyGridItem* child(int row) const;
			QtPropertyGridItem* parent() const;
			int row() const;
			int numChildren() const;
			void append(QtPropertyGridItem* child);
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

		private:
			Reflection::Object* mObject;
			const Reflection::PropertyDef* mProp;
			QList<QtPropertyGridItem*> mChildren;
			QtPropertyGridItem* mParent;
			QString mName;
			QString mAltValue;
		};
	}
}

#endif // QTPROPERTYGRIDITEM_INCLUDED
