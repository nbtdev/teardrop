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

#if !defined(RESOURCELIBRARY_INCLUDED)
#define RESOURCELIBRARY_INCLUDED

#include "Memory/Allocators.h"
#include <list>
#include <map>

namespace CoS
{
	class Stream;

	class ResourceLibrary
	{
		struct Header
		{
			unsigned int version;
			unsigned int size;
			unsigned int exportTableOffset;
			unsigned int importTableOffset;
			unsigned __int64 reserved;
			unsigned __int64 libraryId;

			COS_DECLARE_ALLOCATOR();
		};

		struct ExportEntry
		{
			//! what resource we are exposing
			unsigned __int64 resId;

			COS_DECLARE_ALLOCATOR();
		};

		struct ImportEntry
		{
			//! what resource we want
			unsigned __int64 resId;
			//! the library it is in (at static link time)
			unsigned __int64 libraryId;

			COS_DECLARE_ALLOCATOR();
		};

		Header m_header;
		size_t m_refCount;
		void* m_pData;

		// used offline and online
		typedef std::map<unsigned __int64, void*> ResourceLUT;
		ResourceLUT m_lut;

		// usually used only during offline library creation
		typedef std::list<void*> Resources;
		Resources m_resources;

	public:
		ResourceLibrary();
		~ResourceLibrary();

		//! initialize the library 
		bool initialize();
		//! destroy the library and release its resources
		bool destroy();

		//! adds a resource to the library
		void addResource(Stream& inStrm);
		//! find a resource in the library by its ID
		bool findResource(unsigned __int64 resId, void** pOut, size_t& len);

		//! increment the ref count on the library
		size_t addRef();
		//! decrement the ref count on the library
		size_t decRef();
		//! get the current ref count
		size_t getRefCount();

		//! serialize the current state of the library to the provided stream
		bool serialize(Stream& strm, Stream& exports);
		//! load the library's data from the provided stream
		bool deserialize(Stream& strm);

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // RESOURCELIBRARY_INCLUDED
