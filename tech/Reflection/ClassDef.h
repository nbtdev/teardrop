/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(CLASSDEF_INCLUDED)
#define CLASSDEF_INCLUDED

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
		class PropertyDef;
		class EnumDef;

		/*
			Base object in reflective object hierarchy
		*/
		class ClassDef
		{
			const char* m_pName;
			const char* m_pBaseClassName;
			unsigned __int64 m_classId;
			PropertyDef* m_pProps;
			EnumDef* m_pEnums;
			ClassDef* m_pBaseClass;
			unsigned int m_numProps;
			bool m_bCreatable;

		public:
			ClassDef* m_pNext;
			ClassDef(const char* pName, const char* pBaseClassName);
			virtual ~ClassDef();
			void setCreatable();
			void addProperty(PropertyDef* pProp);
			void addEnum(EnumDef* pEnum);
			virtual Object* createInstance() const = 0;

			bool isCreatable() const { return m_bCreatable; }
			unsigned __int64 getId() const { return m_classId; }
			const char* getName() const { return m_pName; }
			const char* getBaseClassName() const { return m_pBaseClassName; }
			ClassDef* getBaseClass();
			bool isA(const ClassDef* pClass);
			const PropertyDef* getProps() const { return m_pProps; }
			const PropertyDef* findProperty(const char* pPropName, bool deep=false);
			const PropertyDef* findPropertyById(int propId) const;
			const EnumDef* getEnums() const { return m_pEnums; }
			const EnumDef* findEnum(const char* pEnumName, bool deep=false);

			static ClassDef* findClassDef(const char* pClassName);
			static ClassDef* getClasses();
		};

		template <class _T>
		class ConcreteClassDef : public ClassDef
		{
		public:
			//ConcreteClassDef(const char* pName, ClassDef* pBaseClassDef) : ClassDef(pName, pBaseClassDef) {}
			ConcreteClassDef(const char* pName, const char* pBaseClassName) : ClassDef(pName, pBaseClassName) {}

			Object* createInstance() const
			{
				Object* pObj = TD_NEW _T;
				pObj->setupPropertyDefaults();
				pObj->onInstanceCreated();
				return pObj;
			}
		};
	} // namespace Reflection
} // namespace Teardrop

#endif // CLASSDEF_INCLUDED
