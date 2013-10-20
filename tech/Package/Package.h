/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGE_INCLUDED)
#define PACKAGE_INCLUDED

#include "Memory/Memory.h"
#include <list>
#include <map>

namespace Teardrop
{
	class String;
	struct UUID;

	namespace Reflection
	{
		class Object;
		class ClassDef;
	}

	typedef std::list<Reflection::Object*> Objects;

	class Package
	{
	public:
		Package();
		~Package();

		void add(Reflection::Object* object);
		void remove(Reflection::Object* object);
		const Objects& objects() const;
		void initializeAllObjects();

		// I hate this...these must *only* be used during package linking...
		Reflection::Object* findById(const String& id);
		Reflection::Object* findById(const UUID& id);
		void findAllByType(/*out*/std::list<Reflection::Object*>& objs, /*in*/Reflection::ClassDef* type, bool includeDerivedTypes);

		TD_DECLARE_ALLOCATOR();

	protected:
		friend class PackageSerializer;
		void addSymTabEntry(Reflection::Object* obj);

		Objects mObjects;
		unsigned char* mData;
		typedef std::map<UUID, Reflection::Object*> SymbolTable;
		SymbolTable mSymTab;
	};
} // namespace Teardrop

#endif // PACKAGE_INCLUDED
