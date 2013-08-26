/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(REFLECTION_INCLUDED)
#define REFLECTION_INCLUDED

#include "Memory/Memory.h"
#include "Serialization/Serialization.h"
#include "Util/_String.h"
#include "FastDelegate.h"

namespace Teardrop
{
	namespace Reflection
	{
		class ClassDef; 
		class PropertyDef;

		/*
			Base object in reflective object hierarchy
		*/
		class Object
		{
			unsigned __int64 m_objId;

		public:
			Object();
			virtual ~Object();

			/*
				Initialize this object -- perform derived initialization
				in an override of this method and not in constructor. Note
				that in order to ensure proper initialization of the hierarchy,
				you need to call the base class initialize() method in all
				derived classes (usually call it as the first code in your 
				initialize() override).
			*/
			virtual bool initialize();

			/*
				Destroy this object -- perform derived destruction
				in an override of this method and not in destructor. Note
				that in order to ensure proper destruction of the hierarchy,
				you need to call the base class destroy() method in all
				derived classes (usually call it as the final code in your 
				destroy() override).
			*/
			virtual bool destroy();

			/*
				Object ID accessor/mutator. Typically, you do not call the 
				mutator from derived or client code -- the object serialization 
				framework is the only thing that should be calling the mutator.
			*/
			unsigned __int64 getObjectId();
			void setObjectId(unsigned __int64 objectId);

			/*
				These are provided as hooks into derived classes, but without the
				need to make Object itself partake in reflection (makes sense, sorta)
			*/
			virtual ClassDef* getDerivedClassDef() const;

			/*
				This method is intended to allow derived classes to provide their own
				String representation of themselves (useful in editors, on-screen name tags, 
				and so on). The return is intentionally by-value so as not to restrict
				how derived classes might create their string value -- if you prefer not to
				pass String objects by value, use the in-out version instead.
			*/
			virtual String toString() const;
			virtual void toString(/*inout*/ String& str) const;

			void setupPropertyDefaults();
			
			virtual Object* clone() const;

			TD_DECLARE_ALLOCATOR();

			/*
				This sort of hacky way to do this, is here so that we do not have to couple 
				directly to a method by which property change events can be broadcast; instead,
				if the Reflection library is used with such an event broadcast/message-passing 
				system, then it can set us up with a pointer to a function that will create and
				post such a message for us.
			*/
			typedef void (*BroadcastPropertyChanged)(Object* pObj, const PropertyDef* pPropDef);
			static void setBroadcastPropertyChangedFn(BroadcastPropertyChanged fn);

			/*
				These methods are the root of the "property changed" event/notification 
				system. When a property is changed (using the get/set methods for it),
				it will call this method, which in turn will call an overrideable method
				as well as notifying the wider system. This allows for both non-broadcast
				notifications (so that an object can take action without having to subscribe
				to some round-trip notification/bulletin-board system to find out about 
				changes in its own data), as well providing those broadcast notifications
				for systems and objects external to ourselves. If you derive from Object and
				want to handle change notifications for various of your properties, implement
				the notifyPropertyChangedImpl() method in your derived class.
			*/
		public:
			void notifyPropertyChanged(const PropertyDef* pPropDef);
			// called after object instance created, and after property default have been set
			virtual void onInstanceCreated();

		protected:
			virtual void notifyPropertyChangedLocal(const PropertyDef* pPropDef);
			static BroadcastPropertyChanged s_propChangedFn;

			// the preferred way to get notifications about property changes
			fastdelegate::FastDelegate1<const PropertyDef*> PropertyChanged;

		private:
		};
	}
}

/*
	These macros simplify the declaration of reflection properties, as well
	as declare the class members
*/
#define TD_CLASS(c, b) \
	typedef c tClass; \
	typedef b tBaseClass; \
	static Teardrop::Reflection::Object* createInstance(unsigned int instanceId) \
	{ \
		return getClassDef()->createInstance(instanceId); \
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
	virtual Teardrop::Reflection::ClassDef* getDerivedClassDef() const \
	{ \
		return getClassDef(); \
	} \
	static Teardrop::Reflection::ClassDef* getClassDef()\
	{ \
		/*static Teardrop::Reflection::ConcreteClassDef<c> s_classDef(#c, ##b::getClassDef());*/ \
		static Teardrop::Reflection::ConcreteClassDef<c> s_classDef(#c, #b); \
		return &s_classDef; \
	} \
	static unsigned __int64 Class; \
	virtual unsigned __int64 getClassId(); \

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
	namespace Teardrop { unsigned __int64 hashString64(const char* strVal); } \
	unsigned __int64 c::Class = hashString64(#c); \
	unsigned __int64 c::getClassId() \
	{ \
		static c _c; \
		return _c.getClassDef()->getId(); \
	} \


#define TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor) \
	protected: \
		static Reflection::Property<propType>* get##propName##Def() \
		{ \
			static Reflection::Property<propType> sProp(#propName, #propType, (unsigned int)offsetof(tClass, ___##propName)); \
			return &sProp; \
		} \
		struct t##propName##Initializer \
		{ \
			t##propName##Initializer(const char* desc, const char* defaultVal, const char* pEditor) \
			{ \
				Reflection::Property<propType>* sProp = get##propName##Def(); \
				sProp->setDefault(defaultVal); \
				sProp->setDescription(desc); \
				if (pEditor) sProp->setEditor(pEditor); \
				getClassDef()->addProperty(sProp); \
			} \
		}; \
		struct t##propName##Type : public Teardrop::Reflection::PropertyDefImpl< propType > \
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
			static Reflection::Property<propType> sProp(#propName, #propType, (unsigned int)offsetof(tClass, ___##propName)); \
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

#define TD_VECTOR_PROPERTY_BASE(propName, propDesc, baseType, propType, propDef, propEditor) \
	protected: \
		struct t##propName##Initializer \
		{ \
			static Reflection::Property<propType>* get##propName##Def() \
			{ \
				static Reflection::Property<propType> sProp(#propName, #propType, (unsigned int)offsetof(tClass, ___##propName)); \
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
	TD_SCALAR_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor) \
	public: \
		propType& get##propName() { return ___##propName.get(); } \
		void set##propName(propType __val) { ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

#define TD_COMPLEX_PROPERTY(propName, propDesc, propType, propDef, propEditor) \
	TD_COMPLEX_TYPE_PROPERTY_BASE(propName, propDesc, propType, propDef, propEditor) \
	public: \
		propType& get##propName() { return ___##propName.get(); } \
		void set##propName(propType __val) { ___##propName.set(__val); notifyPropertyChanged(get##propName##Def()); }

#define TD_POINTER_PROPERTY(propName, propDesc, propType) \
	TD_SCALAR_PROPERTY_BASE(propName, propDesc, PointerPropertyType<propType>, 0, ObjectBrowser) \
	public: \
		propType* get##propName() { return ___##propName.get(); } \
		void set##propName(propType* __val) { ___##propName.set(__val); }

//#define TD_COLLECTION(propName, propDesc, containedType) \
//	TD_VECTOR_PROPERTY_BASE(propName, propDesc, CollectionPropertyType<containedType>, , 0) \

#define TD_COLLECTION(propName, propDesc, containedType) \
	TD_VECTOR_PROPERTY_BASE(propName, propDesc, Reflection::CollectionPropertyType, containedType, , 0) \
	public: \
		t##propName##Type& get##propName() { return ___##propName; } \
		bool get##propName##At(int index, /*out*/##containedType& lval) { return ___##propName.getAt(index, lval); } \
		void set##propName##At(int index, /*in*/##containedType& rval) { ___##propName.setAt(index, rval); } \

#define TD_POINTER_COLLECTION(propName, propDesc, pointerType) \
	TD_VECTOR_PROPERTY_BASE(propName, propDesc, Reflection::PointerCollectionPropertyType, pointerType, , ObjectCollectionBrowser) \
	public: \
		t##propName##Type& get##propName() { return ___##propName; } \

#include "Reflection/ClassDef.h"
#include "Reflection/PropertyDef.h"
//#include "Reflection/Enum.h"
namespace Teardrop
{
	typedef int Int32;
	typedef __int64 Int64;
	typedef float Real;
	typedef bool Bool;
}

#endif // REFLECTION_INCLUDED
