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

#if !defined(REFLECTION_INCLUDED)
#define REFLECTION_INCLUDED

#include <Reflection/ClassDef.h>
#include <Reflection/EnumDef.h>
#include <Reflection/PropertyDef.h>
#include <Reflection/Object.h>
#include <cstddef>
#include <cstdint>

namespace Teardrop
{
    typedef int Int32;
    typedef int64_t Int64;
    typedef float Real;
    typedef bool Boolean;
}

/*
	These macros simplify the declaration of reflection properties, as well
	as declare the class members
*/
#define TD_CLASS(c, b) \
	typedef c tClass; \
	typedef b tBaseClass; \
	static Teardrop::Reflection::Object* createInstance() \
	{ \
		return getClassDef()->createInstance(); \
	} \
	static struct ClassDefInitializer \
	{ \
		ClassDefInitializer() \
		{ \
			getClassDef(); \
			c _c; \
		} \
	} s_classDefInit; \
	public: \
	virtual const Teardrop::Reflection::ClassDef* getDerivedClassDef() const \
	{ \
		return getClassDef(); \
	} \
	static Teardrop::Reflection::ClassDef* getClassDef()\
	{ \
		/*static Teardrop::Reflection::ConcreteClassDef<c> s_classDef(#c, ##b::getClassDef());*/ \
		static Teardrop::Reflection::ConcreteClassDef<c> s_classDef(#c, #b); \
		return &s_classDef; \
	} \
    static uint64_t Class; \
    virtual uint64_t getClassId(); \

#define TD_CLASS_CREATABLE() \
	private: \
		class CreatableFlag \
		{ \
		public: CreatableFlag() { getClassDef()->setCreatable(); } \
		}; \
		CreatableFlag __CreatableFlag; \
	public: \

#define TD_CLASS_IMPL(c) \
	c::ClassDefInitializer c::s_classDefInit; \
    namespace Teardrop { uint64_t hashString64(const char* strVal); } \
    uint64_t c::Class = hashString64(#c); \
    uint64_t c::getClassId() \
	{ \
		static c _c; \
		return _c.getClassDef()->getId(); \
	} \


// need to indirect this concatenation; MSVC (at least through 2010) is non-standard-compliant
// and the use of DEF_UTIL below will fail under GCC (c++11, i.e. 4.9+)
#define DEF_UTIL(a, b) Teardrop::Reflection::a ## b

#define TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor, __PROPERTY_TYPE) \
	protected: \
		static Reflection::__PROPERTY_TYPE<propType>* get##propName##Def() \
		{ \
            static Reflection::__PROPERTY_TYPE<propType> sProp(#propName, #propType, ((std::size_t)&(((tClass*)0)->___##propName))); \
			return &sProp; \
		} \
		struct t##propName##Initializer \
		{ \
			t##propName##Initializer(const char* desc, const char* defaultVal, const char* pEditor) \
			{ \
				Reflection::__PROPERTY_TYPE<propType>* sProp = get##propName##Def(); \
				sProp->setDefault(defaultVal); \
				sProp->setDescription(desc); \
				if (pEditor) sProp->setEditor(pEditor); \
				sProp->setMetaFlags(); \
				getClassDef()->addProperty(sProp); \
			} \
		}; \
        struct t##propName##Type : public DEF_UTIL(__PROPERTY_TYPE, DefImpl)< propType > \
		{ \
			t##propName##Type() \
			{ \
				static t##propName##Initializer s_t##propName##Initializer(propDesc, #propDef, #propEditor); \
				set(propDef); \
			} \
		}; \
		t##propName##Type ___##propName; \

#define TD_COMPLEX_TYPE_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor) \
	protected: \
		static Reflection::Property<propType>* get##propName##Def() \
		{ \
            static Reflection::Property<propType> sProp(#propName, #propType, ((std::size_t)&(((tClass*)0)->___##propName))); \
			return &sProp; \
		} \
		struct t##propName##Initializer \
		{ \
			t##propName##Initializer(const char* desc, const char* def, const char* editor) \
			{ \
				Reflection::Property<propType>* sProp = get##propName##Def(); \
				sProp->setDescription(desc); \
				sProp->setDefault(def); \
				sProp->setEditor(editor); \
				sProp->setComplex(); \
				getClassDef()->addProperty(sProp); \
			} \
		}; \
		struct t##propName##Type : public Reflection::PropertyDefImpl< propType > \
		{ \
			t##propName##Type() \
			{ \
				static t##propName##Initializer s_t##propName##Initializer(propDesc, propDef, #propEditor); \
			} \
		}; \
		t##propName##Type ___##propName; \

#define TD_NESTED_PROPERTY_BASE(propName, propDesc, propType) \
	protected: \
		static Reflection::NestedObjectProperty<propType>* get##propName##Def() \
		{ \
            static Reflection::NestedObjectProperty<propType> sProp(#propName, #propType, ((std::size_t)&(((tClass*)0)->___##propName))); \
			return &sProp; \
		} \
		struct t##propName##Initializer \
		{ \
			t##propName##Initializer(const char* desc) \
			{ \
				Reflection::NestedObjectProperty<propType>* sProp = get##propName##Def(); \
				sProp->setDescription(desc); \
				getClassDef()->addProperty(sProp); \
			} \
		}; \
		struct t##propName##Type : public Reflection::NestedPropertyDefImpl< propType > \
		{ \
			t##propName##Type() \
			{ \
				static t##propName##Initializer s_t##propName##Initializer(propDesc); \
			} \
		}; \
		t##propName##Type ___##propName; \

#define TD_VECTOR_PROPERTY_BASE(propName, propDesc, baseType, propType, propDef, propEditor) \
	protected: \
		struct t##propName##Initializer \
		{ \
			static Reflection::Property<propType>* get##propName##Def() \
			{ \
                static Reflection::Property<propType> sProp(#propName, #propType, ((std::size_t)&(((tClass*)0)->___##propName))); \
				return &sProp; \
			} \
			t##propName##Initializer(const char* type, const char* desc) \
			{ \
				Reflection::Property<propType>* sProp = get##propName##Def(); \
				sProp->setDescription(desc); \
				sProp->setCollection(); \
				sProp->setTargetTypeName(type); \
				getClassDef()->addProperty(sProp); \
			} \
		}; \
		struct t##propName##Type : public baseType<propType> \
		{ \
			t##propName##Type() \
			{ \
			static t##propName##Initializer s_t##propName##Initializer(#propType, propDesc); \
			} \
		}; \
		t##propName##Type ___##propName; \

#define TD_PROPERTY(propName, propDesc, propType, propDef, propEditor) \
	TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor, Property) \
	public: \
		propType& get##propName() { return ___##propName.get(); } \
		void set##propName(propType __val) { notifyPropertyChanging(get##propName##Def()); ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

#define TD_COMPLEX_PROPERTY(propName, propDesc, propType, propDef, propEditor) \
	TD_COMPLEX_TYPE_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor) \
	public: \
		propType& get##propName() { return ___##propName.get(); } \
		void set##propName(const propType& __val) { notifyPropertyChanging(get##propName##Def()); ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

#define TD_NESTED_PROPERTY(propName, propDesc, propType) \
	TD_NESTED_PROPERTY_BASE(propName, propDesc, propType) \
	public: \
		propType& get##propName() { return ___##propName.get(); } \

#define TD_POINTER_PROPERTY(propName, propDesc, propType, propEditor) \
	TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, 0, propEditor, PointerProperty) \
	public: \
		propType* get##propName() { return ___##propName.get(); } \
		void set##propName(propType* __val) { notifyPropertyChanging(get##propName##Def()); ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

//#define TD_POINTER_PROPERTY(propName, propDesc, propType) \
//	TD_SCALAR_PROPERTY_BASE(propName, propDesc, Reflection::Object*, 0, ObjectBrowser) \
//	public: \
//		propType* get##propName() { return static_cast<propType*>(___##propName.get()); } \
//		void set##propName(propType* __val) { ___##propName.set(__val); }

//#define TD_COLLECTION(propName, propDesc, containedType) \
//	TD_VECTOR_PROPERTY_BASE(propName, propDesc, CollectionPropertyType<containedType>, , 0) \

#define TD_COLLECTION(propName, propDesc, containedType) \
	TD_VECTOR_PROPERTY_BASE(propName, propDesc, Reflection::CollectionPropertyType, containedType, , 0) \
	public: \
		t##propName##Type& get##propName() { return ___##propName; } \
        bool get##propName##At(int index, containedType& lval) { return ___##propName.getAt(index, lval); } \
        void set##propName##At(int index, containedType& rval) { ___##propName.setAt(index, rval); } \

#define TD_POINTER_COLLECTION(propName, propDesc, pointerType) \
	TD_VECTOR_PROPERTY_BASE(propName, propDesc, Reflection::PointerCollectionPropertyType, pointerType, , ObjectCollectionBrowser) \
	public: \
		t##propName##Type& get##propName() { return ___##propName; } \

#define TD_ENUM_PROPERTY(propName, propDesc, propType, propDef) \
	TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor, EnumProperty) \
	public: \
		propType get##propName() { return ___##propName.get(); } \
		void set##propName(propType __val) { ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

// this is hacky and ugly and needs to be done right at some point, if even possible...
#define TD_ENUM(c, enumName, enumDesc) \
	static Reflection::EnumDef* get##enumName##Def() \
	{ \
	static Reflection::EnumDef sEnum(#enumName, #enumDesc); \
		return &sEnum; \
	} \
	static struct __##enumName##EnumInitializer \
	{ \
		__##enumName##EnumInitializer() { \
			Reflection::EnumDef* enumDef = get##enumName##Def(); \
			c::getClassDef()->addEnum(enumDef); \
		} \
	} s__##enumName##EnumInitializer; \

#define TD_ENUM_VALUE(c, enumName, enumValue, enumValueDesc) \
	static struct c##enumValue##Initializer { \
		c##enumValue##Initializer() {\
			static Reflection::EnumValue enumVal(#enumValue, enumValueDesc, c::enumValue); \
			get##enumName##Def()->addValue(&enumVal); \
		} \
	} s_c##enumValue##Initializer;\

#endif // REFLECTION_INCLUDED
