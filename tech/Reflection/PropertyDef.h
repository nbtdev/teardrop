/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined (PROPERTYDEF_INCLUDED)
#define PROPERTYDEF_INCLUDED

#include "Reflection/Property.h"
#include "Reflection/Reflection.h"

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

		protected:
			size_t m_bPointer;
			size_t m_bCollection;
			size_t m_bComplex;
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
						(CollectionPropertyType<_T>*)((unsigned long)pObj + m_offset);
					if (pColl->getCount() <= size_t(index))
						pColl->resize(index+1);
					pColl->setAtFromString(index, pVal);
				}
				else
				{
					PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((unsigned long)pObj + m_offset);
					pProp->fromString(pVal);
				}

				pObj->notifyPropertyChanged(this);
			}

			void setData(Object* pObj, const void* val) const
			{
				PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((unsigned long)pObj + m_offset);
				pProp->set(*((_T*)val));
				pObj->notifyPropertyChanged(this);
			}

			void getData(const Object* pObj, const void* val) const
			{
				PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((unsigned long)pObj + m_offset);
				*((_T*)val) = pProp->get();
			}

			const void* getDataPointer(const Object* pObj) const
			{
				return (const void*)((unsigned long)pObj + m_offset);
			}

			void getDataAsString(const Object* pObj, String& sVal) const
			{
				PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((unsigned long)pObj + m_offset);
				pProp->toString(sVal);
			}

			void copyTo(Object* pDest, const Object* pSrc) const
			{
				if (m_bCollection)
				{
					CollectionPropertyType<_T>* pProp = (CollectionPropertyType<_T>*)((unsigned long)pSrc + m_offset);
					CollectionPropertyType<_T>* pOtherProp = (CollectionPropertyType<_T>*)((unsigned long)pDest + m_offset);
					*pOtherProp = *pProp;
				}
				else
				{
					PropertyDefImpl<_T>* pProp = (PropertyDefImpl<_T>*)((unsigned long)pSrc + m_offset);
					PropertyDefImpl<_T>* pOtherProp = (PropertyDefImpl<_T>*)((unsigned long)pDest + m_offset);
					*pOtherProp = *pProp;
				}
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
				PointerPropertyDefImpl<_T>* pProp = (PointerPropertyDefImpl<_T>*)((unsigned long)pObj + m_offset);
				_T* val = (_T*)v;
				(*pProp) = val;
				pObj->notifyPropertyChanged(this);
			}

			void getData(const Object* pObj, const void* v) const
			{
				PointerPropertyDefImpl<_T>& prop = *((PointerPropertyDefImpl<_T>*)((unsigned long)pObj + m_offset));
				_T** val = (_T**)v;
				*val = prop;
			}

			const void* getDataPointer(const Object* pObj) const
			{
				return (const void*)((unsigned long)pObj + m_offset);
			}

			void getDataAsString(const Object* pObj, String& sVal) const
			{
				PointerPropertyDefImpl<_T>& prop = *((PointerPropertyDefImpl<_T>*)((unsigned long)pObj + m_offset));
				Reflection::Object* obj = (Reflection::Object*)(_T*)prop;
				if (obj)
					obj->getObjectId().toString(sVal);
			}

			void copyTo(Object* pDest, const Object* pSrc) const
			{
				PointerPropertyDefImpl<_T>* pProp = (PointerPropertyDefImpl<_T>*)((unsigned long)pSrc + m_offset);
				PointerPropertyDefImpl<_T>* pOtherProp = (PointerPropertyDefImpl<_T>*)((unsigned long)pDest + m_offset);
				*pOtherProp = *pProp;
			}
		};
	} // namespace Reflection
} // namespace Teardrop

#endif // PROPERTYDEF_INCLUDED
