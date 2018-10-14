/******************************************************************************
Copyright (c) 2018 Teardrop Games

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

#if !defined (PROPERTYDEF_INCLUDED)
#define PROPERTYDEF_INCLUDED

#include "Reflection/ClassDef.h"
#include "Reflection/EnumDef.h"
#include "Reflection/Property.h"
#include "Reflection/Object.h"

#include <cstdint>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
        
		class PropertyDef
		{
		public:
			PropertyDef* m_pNext;
			PropertyDef();
			~PropertyDef();

			const char* getName() const { return m_pName; }
			const char* getTypeName() const { return m_pTypeName; }
			const char* getEditor() const { return m_pEditor; }
			const char* getDescription() const { return m_pDescription; }
			const char* getDefault() const { return m_pDefault; }
			const char* getFilter() const { return m_pFilter; }
			const char* getTargetTypeName() const { return m_pTargetType; }
			int getId() const { return m_id; } 
			unsigned int getOffset() const { return m_offset; }
			bool isPointer() const { return (0 != m_bPointer); }
			bool isCollection() const { return (0 != m_bCollection); }
			bool isComplex() const { return (0 != m_bComplex); }
			bool isBoolean() const { return (0 != m_bBoolean); }
			bool isEnum() const { return (0 != m_bEnum); }
			bool isNested() const { return (0 != m_bNested); }
			bool isHidden() const { return (0 != m_bHidden); }

			void setName(const char* name) { m_pName = name; }
			void setTypeName(const char* pTypeName) { m_pTypeName = pTypeName; }
			void setEditor(const char* editor) { m_pEditor = editor; }
			void setDescription(const char* description) { m_pDescription = description; }
			void setDefault(const char* def);
			void setFilter(const char* filter) { m_pFilter = filter; }
			void setTargetTypeName(const char* type) { m_pTargetType = type; }
			void setId(int id) { m_id = id; }
			void setOffset(unsigned int offset) { m_offset = offset; }
			void setPointer() { m_bPointer = 1; }
			void setCollection() { m_bCollection = 1; }
			void setComplex() { m_bComplex = 1; }
			void setMetaFlags();

			virtual void setDataFromString(Object* pObj, const String& pVal, int index=-1) const = 0;
			virtual void setData(/*in*/Object* pObj, /*in*/const void* pVal) const = 0;
			virtual void getData(/*in*/const Object* pObj, /*out*/const void* pVal) const = 0;
			virtual const void* getDataPointer(/*in*/const Object* pObj) const = 0;
			virtual void getDataAsString(/*in*/const Object* pObj, /*out*/String& sVal) const = 0;
			virtual void copyTo(/*in*/Object* pDest, const Object* pSrc) const = 0;
			virtual ClassDef* getClassDef() const = 0;

		protected:
			size_t m_bPointer;
			size_t m_bCollection;
			size_t m_bComplex;
			size_t m_bBoolean;
			size_t m_bEnum;
			size_t m_bNested;
			size_t m_bHidden;
			const char* m_pName;
			const char* m_pTypeName;
			const char* m_pEditor;
			const char* m_pFilter;
			const char* m_pDescription;
			const char* m_pDefault;
			const char* m_pTargetType;
			unsigned int m_offset;
			int m_id;
		};

		template<class _T>
		class Property : public PropertyDef
		{
		public:
			Property(const char* pPropName, const char* pTypeName, unsigned int offset)
				: PropertyDef()
			{
				m_pTypeName = pTypeName;
				m_pName = pPropName;
				m_offset = offset;
				m_bPointer = false;
				setMetaFlags();
			}

			void setDataFromString(Object* pObj, const String& pVal, int index=-1) const
			{
				if (index >= 0)
				{
					CollectionPropertyType<_T>* pColl = 
                        (CollectionPropertyType<_T>*)((uintptr_t)pObj + m_offset);
					if (pColl->getCount() <= size_t(index))
						pColl->resize(index+1);
					pColl->setAtFromString(index, pVal);
				}
				else
				{
                    PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset);
					pProp->fromString(pVal);
				}

				pObj->notifyPropertyChanged(this);
			}

			void setData(Object* pObj, const void* val) const
			{
                PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset);
				pProp->set(*((_T*)val));
				pObj->notifyPropertyChanged(this);
			}

			void getData(const Object* pObj, const void* val) const
			{
                PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset);
				*((_T*)val) = pProp->get();
			}

			const void* getDataPointer(const Object* pObj) const
			{
                return (const void*)((uintptr_t)pObj + m_offset);
			}

			void getDataAsString(const Object* pObj, String& sVal) const
			{
                PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset);
				pProp->toString(sVal);
			}

			void copyTo(Object* pDest, const Object* pSrc) const
			{
				if (m_bCollection)
				{
                    CollectionPropertyType<_T>* pProp = (CollectionPropertyType<_T>*)((uintptr_t)pSrc + m_offset);
                    CollectionPropertyType<_T>* pOtherProp = (CollectionPropertyType<_T>*)((uintptr_t)pDest + m_offset);
					*pOtherProp = *pProp;
				}
				else
				{
                    PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((uintptr_t)pSrc + m_offset);
                    PropertyDefImpl<_T>* pOtherProp = (PropertyDefImpl<_T>*)((uintptr_t)pDest + m_offset);
					*pOtherProp = *pProp;
				}
			}

			ClassDef* getClassDef() const
			{
				// non-Object/Pointer/Collection types have no ClassDef
				return nullptr;
			}
		};

		template<class _T>
		class NestedObjectProperty : public PropertyDef
		{
		public:
			NestedObjectProperty(const char* pPropName, const char* pTypeName, unsigned int offset)
				: PropertyDef()
			{
				m_pTypeName = pTypeName;
				m_pName = pPropName;
				m_offset = offset;
				m_bPointer = 0;
				m_bNested = 1;
				setMetaFlags();
			}

			void setDataFromString(Object* /*pObj*/, const String& /*pVal*/, int /*index=-1*/) const
			{
				// cannot set or get data on a nested property, nor makes copies
			}

			void setData(Object* /*pObj*/, const void* /*val*/) const
			{
				// cannot set or get data on a nested property, nor makes copies
			}

			void getData(const Object* /*pObj*/, const void* /*val*/) const
			{
				// cannot set or get data on a nested property, nor makes copies
			}

			const void* getDataPointer(const Object* pObj) const
			{
                return (const void*)((uintptr_t)pObj + m_offset);
			}

			void getDataAsString(const Object* /*pObj*/, String& /*sVal*/) const
			{
				// cannot set or get data on a nested property, nor makes copies
			}

			void copyTo(Object* /*pDest*/, const Object* /*pSrc*/) const
			{
				// cannot set or get data on a nested property, nor makes copies
			}

			ClassDef* getClassDef() const
			{
				return _T::getClassDef();
			}
		};

		template<class _T>
		class PointerProperty : public PropertyDef
		{
		public:
			PointerProperty(const char* pPropName, const char* pTypeName, unsigned int offset)
				: PropertyDef()
			{
				m_pTypeName = pTypeName;
				m_pName = pPropName;
				m_offset = offset;
				m_bPointer = true;
				setMetaFlags();
			}

			void setDataFromString(Object* /*pObj*/, const String& /*pVal*/, int /*index=-1*/) const
			{
			}

			void setData(Object* pObj, const void* v) const
			{
                PointerPropertyDefImpl<_T>* pProp = (PointerPropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset);
				_T* val = (_T*)v;
				(*pProp) = val;
				pObj->notifyPropertyChanged(this);
			}

			void getData(const Object* pObj, const void* v) const
			{
                PointerPropertyDefImpl<_T>& prop = *((PointerPropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset));
				_T** val = (_T**)v;
				*val = prop;
			}

			const void* getDataPointer(const Object* pObj) const
			{
                return (const void*)((uintptr_t)pObj + m_offset);
			}

			void getDataAsString(const Object* pObj, String& sVal) const
			{
                PointerPropertyDefImpl<_T>& prop = *((PointerPropertyDefImpl<_T>*)((uintptr_t)pObj + m_offset));
				Reflection::Object* obj = (Reflection::Object*)(_T*)prop;
				if (obj)
					obj->getObjectId().toString(sVal);
			}

			void copyTo(Object* pDest, const Object* pSrc) const
			{
                PointerPropertyDefImpl<_T>* pProp = (PointerPropertyDefImpl<_T>*)((uintptr_t)pSrc + m_offset);
                PointerPropertyDefImpl<_T>* pOtherProp = (PointerPropertyDefImpl<_T>*)((uintptr_t)pDest + m_offset);
				*pOtherProp = *pProp;
			}

			ClassDef* getClassDef() const
			{
				return _T::getClassDef();
			}
		};

		template<typename T>
		class EnumProperty : public PropertyDef
		{
		public:
			EnumProperty(const char* pPropName, const char* pTypeName, unsigned int offset)
				: PropertyDef()
			{
				m_pTypeName = pTypeName;
				m_pName = pPropName;
				m_offset = offset;
				m_bEnum = 1;
				setMetaFlags();
			}

			void setDataFromString(Object* pObj, const String& str, int /*index=-1*/) const
			{
				// pull value from our list of possible values
				const ClassDef* classDef = pObj->getDerivedClassDef();
				const EnumDef* enumDef = classDef->findEnum(m_pTypeName);
				if (enumDef) {
					const EnumValue* enumVal = enumDef->values();
					while (enumVal) {
						if (String(enumVal->id()) == str) {
                            EnumPropertyDefImpl<T>* pProp = (EnumPropertyDefImpl<T>*)((uintptr_t)pObj + m_offset);
							*pProp = (T)enumVal->value();
							pObj->notifyPropertyChanged(this);
						}

						enumVal = enumVal->next();
					}
				}
			}

			void setData(Object* pObj, const void* v) const
			{
                EnumPropertyDefImpl<T>* pProp = (EnumPropertyDefImpl<T>*)((uintptr_t)pObj + m_offset);
				int val = *((int*)v);
				*pProp = (T)val;
				pObj->notifyPropertyChanged(this);
			}

			void getData(const Object* pObj, const void* v) const
			{
                EnumPropertyDefImpl<T>& prop = *((EnumPropertyDefImpl<T>*)((uintptr_t)pObj + m_offset));
				int* val = (int*)v;
				*val = int(prop);
			}

			const void* getDataPointer(const Object* pObj) const
			{
                return (const void*)((uintptr_t)pObj + m_offset);
			}

			void getDataAsString(const Object* pObj, String& sVal) const
			{
                EnumPropertyDefImpl<T>& prop = *((EnumPropertyDefImpl<T>*)((uintptr_t)pObj + m_offset));
				T val = prop;

				const ClassDef* classDef = pObj->getDerivedClassDef();
				const EnumDef* enumDef = classDef->findEnum(m_pTypeName);
				if (enumDef) {
					const EnumValue* enumVal = enumDef->values();
					while (enumVal) {
						if (enumVal->value() == val) {
							sVal = enumVal->id();
						}

						enumVal = enumVal->next();
					}
				}
			}

			void copyTo(Object* pDest, const Object* pSrc) const
			{
                EnumPropertyDefImpl<T>* pProp = (EnumPropertyDefImpl<T>*)((uintptr_t)pSrc + m_offset);
                EnumPropertyDefImpl<T>* pOtherProp = (EnumPropertyDefImpl<T>*)((uintptr_t)pDest + m_offset);
				*pOtherProp = *pProp;
			}

			ClassDef* getClassDef() const
			{
				// Enum properties have no ClassDef
				return nullptr;
			}
		};
	} // namespace Reflection
} // namespace Teardrop

#endif // PROPERTYDEF_INCLUDED
