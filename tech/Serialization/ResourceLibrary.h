/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCELIBRARY_INCLUDED)
#define RESOURCELIBRARY_INCLUDED

#include "Memory/Allocators.h"
#include <list>
#include <map>

namespace Teardrop
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

			TD_DECLARE_ALLOCATOR();
		};

		struct ExportEntry
		{
			//! what resource we are exposing
			unsigned __int64 resId;

			TD_DECLARE_ALLOCATOR();
		};

		struct ImportEntry
		{
			//! what resource we want
			unsigned __int64 resId;
			//! the library it is in (at static link time)
			unsigned __int64 libraryId;

			TD_DECLARE_ALLOCATOR();
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

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // RESOURCELIBRARY_INCLUDED
