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

#if !defined(PACKAGE_INCLUDED)
#define PACKAGE_INCLUDED

#include "Memory/Memory.h"
#include <list>
#include <map>

namespace Teardrop
{
	class Executable;
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

		// the one special object instance each package can manage...
		Executable* executable();
        void makeExecutable(Executable* exe);
        Executable* makeExecutable(const Reflection::ClassDef* classDef=nullptr);

		TD_DECLARE_ALLOCATOR();

	protected:
		friend class PackageSerializer;
		void addSymTabEntry(Reflection::Object* obj);

		Objects mObjects;
		unsigned char* mData;
		typedef std::map<UUID, Reflection::Object*> SymbolTable;
		SymbolTable mSymTab;

		Executable* mExecutable;
	};
} // namespace Teardrop

#endif // PACKAGE_INCLUDED
