/******************************************************************************
Copyright (c) 2015 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

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
