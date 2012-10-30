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

#if !defined(RESOURCESERIALIZER_INCLUDED)
#define RESOURCESERIALIZER_INCLUDED

#include "Memory/include/Allocators.h"
#include <map>
#include <list>
#include <vector>
#include "Serialization/include/SerialPointerArray.h"
#include "Serialization/include/ResourceHeader.h"
#include "Serialization/include/ResourceRelocEntry.h"

namespace CoS
{
	class Stream;

	class ResourceSerializer
	{
	public:
		static const int VERSION;

		ResourceSerializer(Stream& stream);
		~ResourceSerializer();

		//! set the global id for this resource
		void setId(unsigned __int64 id);
		//! start a new object; sets up metadata tables
		bool start();
		//! end serialization and write metadata
		bool finalize();

		// used to relocate a vtabl instance
		void relocate(unsigned __int64 classId); 

		// used to relocate a class instance
		void relocate(void*, size_t); 

		// generate an external reference, given the resource filename
		void relocate(
			SerialPointerBase&, 
			const char*); 

		// used to track pointer fixups
		void relocate(
			SerialPointerBase&, 
			void* pBase=0, 
			ResourceRelocEntry::Type type=ResourceRelocEntry::INTERNAL); 

		// used to track pointer array fixups
		void relocate(
			SerialPointerArrayBase&, 
			void* pBase=0,
			ResourceRelocEntry::Type type=ResourceRelocEntry::INTERNAL); 

		COS_DECLARE_ALLOCATOR();

	private:

		// the metadata for the memory image
		ResourceHeader m_header;

		// maps relocations to their offset in the output data
		typedef std::map<void*, size_t> Relocations;
		Relocations m_relocations;

		// string table entry for external references
		struct StringTableEntry
		{
			const char* string;
			size_t offset;
			COS_DECLARE_ALLOCATOR();
		};
		typedef std::vector<StringTableEntry> StringTable;
		StringTable m_stringTable;

		// keeps track of the offsets of relocated pointers, and 
		// what they were pointing to, in order to have an index into
		// the above Relocations
		struct RelocatedPointer
		{
			size_t offset;
			void* originalTarget;
			ResourceRelocEntry::Type type;
			unsigned __int64 id; // classID for vtable* lookups, resource ID for external types
			COS_DECLARE_ALLOCATOR();
		};
		typedef std::list<RelocatedPointer> RelocatedPointers;
		RelocatedPointers m_relocatedPointers;

		unsigned int m_currentOffset;
		unsigned int m_startPosition;
		void* m_pCurrentBase;
		Stream& m_out;

		// write a chunk to the stream (updates the offset counter)
		bool write(void* pData, size_t len);
		void align(); // aligns stream to preferred byte alignment (usually 16)

	private:
		ResourceSerializer(const ResourceSerializer& other);
		ResourceSerializer& operator=(const ResourceSerializer& other);
	};
}

#endif // RESOURCESERIALIZER_INCLUDED
