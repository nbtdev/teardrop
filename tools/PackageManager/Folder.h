/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(FOLDER_INCLUDED)
#define FOLDER_INCLUDED

#include "Memory/Memory.h"
#include "Util/_String.h"
#include <list>
#include "FastDelegate.h"

namespace Teardrop 
{
	namespace Reflection
	{
		class Object;
	}

	namespace Tools
	{
		class Folder;
		typedef std::list<Reflection::Object*> Objects;
		typedef std::list<Folder*> Folders;

		class Folder
		{
		public:
			// return just the folders
			const Folders& folders() const;
			// return just the objects
			const Objects& objects() const;
			// return my parent folder
			Folder* parent() const;
			// folder name
			const String& name() const;

			TD_DECLARE_ALLOCATOR();

			fastdelegate::FastDelegate1<const char*> NameChanged;

		private:
			friend class PackageMetadata;
			Folder(const String& name, Folder* parent);
			~Folder();
			// creates a new folder and makes it a child of "this"; takes ownership of the new folder while part of this tree
			Folder* createSubfolder(const String& name);
			// if this returns non-null, it will be the "folder" passed in and indicates that removal succeeded; otherwise, failure to remove indicated by null return value
			Folder* deleteSubfolder(Folder* folder);
			// add object to this folder (does not take ownership)
			void add(Reflection::Object* obj);
			// remove object from this folder
			void remove(Reflection::Object* obj);
			// set this folder's name
			void setName(const String& name);

			Folder* mParent;
			String mName;

			Objects mObjects;
			Folders mFolders;
		};
	}
}

#endif // FOLDER_INCLUDED