/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCESERIALIZER_INCLUDED)
#define RESOURCESERIALIZER_INCLUDED

#include "Memory/Allocators.h"
#include <map>
#include <list>
#include <vector>
#include "Serialization/SerialPointerArray.h"
#include "Serialization/ResourceHeader.h"
#include "Serialization/ResourceRelocEntry.h"

namespace Teardrop
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

		TD_DECLARE_ALLOCATOR();

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
			TD_DECLARE_ALLOCATOR();
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
			TD_DECLARE_ALLOCATOR();
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
