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

#if !defined(CLASSDEF_INCLUDED)
#define CLASSDEF_INCLUDED

#include <Memory/Allocators.h>
#include <Reflection/Object.h>
#include <cstdint>

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
			uint64_t m_classId;
			PropertyDef* m_pProps;
			EnumDef* m_pEnums;
			mutable ClassDef* m_pBaseClass;
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
			uint64_t getId() const { return m_classId; }
			const char* getName() const { return m_pName; }
			const char* getBaseClassName() const { return m_pBaseClassName; }
			const ClassDef* getBaseClass() const;
			bool isA(const ClassDef* pClass) const;
			const PropertyDef* getProps() const { return m_pProps; }
			const PropertyDef* findProperty(const char* pPropName, bool deep=false) const;
			const PropertyDef* findPropertyById(int propId) const;
			const EnumDef* getEnums() const { return m_pEnums; }
			const EnumDef* findEnum(const char* pEnumName, bool deep=false) const;

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
