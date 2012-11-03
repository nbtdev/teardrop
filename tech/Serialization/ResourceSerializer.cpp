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

#include "Config.h"
#include "ResourceSerializer.h"
#include "SerialUtil.h"
#include "Stream/Stream.h"
#include <string.h>
#include <assert.h>

using namespace CoS;
//---------------------------------------------------------------------------
const int ResourceSerializer::VERSION = 1;
//---------------------------------------------------------------------------
ResourceSerializer::ResourceSerializer(Stream& stream)
: m_out(stream)
{
	m_currentOffset = 0;
	m_pCurrentBase = 0;
	m_startPosition = (unsigned int)m_out.getPosition();
}
//---------------------------------------------------------------------------
ResourceSerializer::~ResourceSerializer()
{
}
//---------------------------------------------------------------------------
void ResourceSerializer::relocate(unsigned __int64 classId)
{
	// if classId is not 0, we need to note a vtabl reloc entry
	if (classId)
	{
		// the class data will be aligned, so we need to do this here
		align();

		RelocatedPointer ent;
		ent.offset = m_currentOffset;
		ent.originalTarget = 0;
		ent.id = classId;
		ent.type = ResourceRelocEntry::VTABLE;
		m_relocatedPointers.push_back(ent);
	}
}
//---------------------------------------------------------------------------
void ResourceSerializer::relocate(
	SerialPointerBase& base, const char* name)
{
	// add a string to the table
	StringTableEntry ent;
	ent.string = name;
	ent.offset = 0;

	// relocate the pointer, with the current index as offset (resolved later)
	unsigned __int64 tmp = base._u.i64;
	base._u.i64 = m_stringTable.size();
	relocate(base, 0, ResourceRelocEntry::EXTERNAL);
	base._u.i64 = tmp;

	m_stringTable.push_back(ent);
}
//---------------------------------------------------------------------------
void ResourceSerializer::relocate(void* pObj, size_t len)
{
	// align the start of each chunk
	align();

	// write an arbitary data chunk to the output stream; save the current
	// stream position as the chunk's relocation "address"
	m_relocations[pObj] = m_currentOffset;
	m_pCurrentBase = pObj;
	write(pObj, len);
}
//---------------------------------------------------------------------------
void ResourceSerializer::relocate(
	SerialPointerBase& base, void* pBase, ResourceRelocEntry::Type type)
{
	// copy the serial pointer data to the stream, and save not only its
	// relocation address, but what it pointed to as well
	void* pBaseAddr = pBase ? pBase : m_pCurrentBase;
	size_t localOffset = (char*)&base - (char*)pBaseAddr;
	Relocations::iterator it = m_relocations.find(pBaseAddr);
	size_t baseOffset = 0;
	if (it != m_relocations.end())
	{
		baseOffset = it->second;
	}
	else
	{
		baseOffset = m_currentOffset;
	}

	RelocatedPointer p;
	p.id = 0;
	p.offset = baseOffset + localOffset;
	p.originalTarget = base._u.pC;
	p.type = type;

	if (!base._u.pC)
	{
		assert(!base._u.i64);
	}

	m_relocatedPointers.push_back(p);
	m_relocations[&base] = p.offset;
}
//---------------------------------------------------------------------------
void ResourceSerializer::relocate(
	SerialPointerArrayBase& base, void* pBase, ResourceRelocEntry::Type type)
{
	// we don't write anything here, because these are (supposed to be) 
	// embedded in the class instance data; instead, we just need to track
	// the relocations for the SerialPointer contained within

	void* pBaseAddr = pBase ? pBase : m_pCurrentBase;
	size_t localOffset = (char*)&(base.first) - (char*)pBaseAddr;
	Relocations::iterator it = m_relocations.find(pBaseAddr);
	size_t baseOffset = 0;
	if (it != m_relocations.end())
	{
		baseOffset = it->second;
	}

	RelocatedPointer p;
	p.type = type;
	p.id = 0;
	p.offset = baseOffset + localOffset;
	p.originalTarget = base.first;
	m_relocatedPointers.push_back(p);

	relocate(base.first);

	// have to write out the pointer array too
	for (size_t i=0; i<(size_t)base.sz; ++i)
	{
		relocate(base.first[i], &base.first[i]);

		// need to write out the pointer too
		write(&(base.first[i]), sizeof(SerialPointerBase));
	}
}
//---------------------------------------------------------------------------
bool ResourceSerializer::write(void* pData, size_t len)
{
	m_out.write(pData, len);
	m_currentOffset += (unsigned int)len;
	return true;
}
//---------------------------------------------------------------------------
void ResourceSerializer::align()
{
	// todo -- make alignment configurable
	const int alignment = 16;
	char buf[alignment];
	memset(buf, 0, sizeof(buf));
	size_t rem = m_currentOffset % alignment;
	size_t bytesNeeded = rem ? alignment - rem : 0;
	write(buf, bytesNeeded);
}
//---------------------------------------------------------------------------
void ResourceSerializer::setId(unsigned __int64 id)
{
	m_header.resourceId = id;
}
//---------------------------------------------------------------------------
bool ResourceSerializer::start()
{
	// write out the header info
	align();
	unsigned __int64 id = m_header.resourceId;
	memset(&m_header, 0, sizeof(m_header));
	m_header.resourceId = id;
	write(&m_header, sizeof(m_header));
	return true;
}
//---------------------------------------------------------------------------
bool ResourceSerializer::finalize()
{
	m_header.version = VERSION;

	// write out the string table first, so that we can get the string
	// offsets to fix up the fixups that use them
	m_header.stringTableOffset = m_currentOffset;
	for (size_t i=0; i<m_stringTable.size(); ++i)
	{
		StringTableEntry& ent = m_stringTable[i];
		ent.offset = m_currentOffset;

		char buf[1024];
		Serial::Util::mapResourceDependencyName(
			ent.string, buf, 1024);
		write((void*)buf, strlen(buf) + 1);
	}

	// write 8 extra bytes' worth of 0 as a null terminator
	char zeroes[] = {0,0,0,0,0,0,0,0};
	write((void*)zeroes, 8);

	// align if needed
	align();

	// then, note current position
	m_header.fixupTableOffset = m_currentOffset;

	// then write out the pointer fixup table
	for (RelocatedPointers::iterator it = m_relocatedPointers.begin();
		it != m_relocatedPointers.end(); ++it)
	{
		if (it->originalTarget || it->type > ResourceRelocEntry::INTERNAL)
		{
			Relocations::iterator r = m_relocations.find(it->originalTarget);
			unsigned int to = 0;

			if (it->type == ResourceRelocEntry::EXTERNAL)
			{
				// fixup the entry with the actual string table offset
				to = (unsigned int)m_stringTable[(size_t)it->originalTarget].offset;
			}
			else
			{
				if (r != m_relocations.end())
				{
					to = (unsigned int)r->second;
				}
			}

			ResourceRelocEntry entry;
			entry.at = (unsigned int)it->offset;
			entry.to = to;
			entry.id = it->id;
			entry.type = it->type;

			write(&entry, sizeof(entry));
		}
	}

	ResourceRelocEntry null;
	write(&null, sizeof(null));

	// then seek back to m_startPosition and write out the header contents
	m_header.size = m_currentOffset;
	m_out.seek(0, Stream::BEGIN);
	write(&m_header, sizeof(m_header));

	// and then back to the end of us, because we might not be the 
	// only thing in this stream...
	m_out.seek(m_header.size, Stream::BEGIN);

	return true;
}
