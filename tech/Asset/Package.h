/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(PACKAGE_INCLUDED)
#define PACKAGE_INCLUDED

#include "Util/_String.h"
#include "Memory/Memory.h"
#include <list>

namespace Teardrop
{
	namespace Reflection
	{
		class Object;
	}

	class Folder;
	typedef std::list<Reflection::Object*> Objects;
	typedef std::list<Folder*> Folders;

	class Folder
	{
	public:
		Folder(const String& name, Folder* parent=0);
		~Folder();

		// returns false if object of same name already exists
		bool add(Reflection::Object* obj);
		Folder* newFolder(const String& name);

		// return just the folders
		const Folders& folders() const;
		// return just the objects
		const Objects& objects() const;

		const String& name() const;
		void setName(const String& name);

		TD_DECLARE_ALLOCATOR();

	protected:
		Folder* mParent;
		String mName;

		Objects mObjects;
		Folders mFolders;
	};

	class Package
	{
	public:
		Package(const String& name);
		virtual ~Package();

		const Folder& root() const;
		Folder& root();

		const String& name() const;
		void setName(const String& name);

		TD_DECLARE_ALLOCATOR();

	protected:
		Folder mRoot;
		String mName;
	};
} // namespace Teardrop

#endif // PACKAGE_INCLUDED
