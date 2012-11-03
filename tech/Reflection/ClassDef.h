/*
-----------------------------------------------------------------------------
This source file is part of the Clash Of Steel Project

For the latest info, see http://www.clashofsteel.net/

Copyright (c) The Clash Of Steel Team
Also see acknowledgments in Readme.txt

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
-----------------------------------------------------------------------------
*/

#if !defined(CLASSDEF_INCLUDED)
#define CLASSDEF_INCLUDED

namespace CoS
{
	namespace Reflection
	{
		class Object;
		class PropertyDef;

		/*
			Base object in reflective object hierarchy
		*/
		class ClassDef
		{
			const char* m_pName;
			const char* m_pBaseClassName;
			//ClassDef* m_pBaseClassDef;
			unsigned __int64 m_classId;
			PropertyDef* m_pProps;
			ClassDef* m_pBaseClass;
			unsigned int m_numProps;
			bool m_bCreatable;

		public:
			ClassDef* m_pNext;
			ClassDef(const char* pName, const char* pBaseClassName);
			virtual ~ClassDef();
			void setCreatable();
			void addProperty(PropertyDef* pProp);
			virtual Object* createInstance(unsigned int instanceId) = 0;

			bool isCreatable() const { return m_bCreatable; }
			unsigned __int64 getId() const { return m_classId; }
			const char* getName() const { return m_pName; }
			const char* getBaseClassName() const { return m_pBaseClassName; }
			ClassDef* getBaseClass();
			bool isA(const ClassDef* pClass) const;
			const PropertyDef* getProps() const { return m_pProps; }
			const PropertyDef* findProperty(const char* pPropName, bool deep=false);
			const PropertyDef* findPropertyById(int propId) const;

			static ClassDef* findClassDef(const char* pClassName);
			static ClassDef* getClasses();
		};

		template <class _T>
		class ConcreteClassDef : public ClassDef
		{
		public:
			//ConcreteClassDef(const char* pName, ClassDef* pBaseClassDef) : ClassDef(pName, pBaseClassDef) {}
			ConcreteClassDef(const char* pName, const char* pBaseClassName) : ClassDef(pName, pBaseClassName) {}

			Object* createInstance(unsigned int instanceId)
			{
				Object* pObj = COS_NEW _T;
				pObj->setupPropertyDefaults();
				pObj->setObjectId(instanceId);
				pObj->onInstanceCreated();
				return pObj;
			}
		};
	} // namespace Reflection
} // namespace CoS

#endif // CLASSDEF_INCLUDED
