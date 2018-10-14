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

#if !defined(FOLDER_INCLUDED)
#define FOLDER_INCLUDED

#include "Memory/Memory.h"
#include "Util/Event.h"
#include "Util/_String.h"
#include <list>

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

            Event<const char*> NameChanged;

		private:
			friend class PackageMetadata;
			Folder(const String& name, Folder* parent);
			~Folder();
			// creates a new folder and makes it a child of "this"; takes ownership of the new folder while part of this tree
			Folder* createSubfolder(const String& name);
			// if this returns non-null, it will be the "folder" passed in and indicates that removal succeeded; otherwise, failure to remove indicated by null return value
			Folder* removeSubfolder(Folder* folder);
			// add subfolder
			void add(Folder* folder);
			// add object to this folder (does not take ownership)
			void add(Reflection::Object* obj);
			// remove object from this folder
			void remove(Reflection::Object* obj);
			// remove subfolder from this folder
			void remove(Folder* folder);
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
