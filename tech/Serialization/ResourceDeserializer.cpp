/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "Serialization.h"
#include "ResourceHeader.h"
#include "ResourceRelocEntry.h"
#include "ResourceDeserializer.h"
#include "SerialPointer.h"
#include "Stream/Stream.h"
#include <assert.h>
#include <string.h>
#include <new.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
ResourceDeserializer::ResourceDeserializer()
{
}
//---------------------------------------------------------------------------
ResourceDeserializer::~ResourceDeserializer()
{
}
//---------------------------------------------------------------------------
void* ResourceDeserializer::get(void* pData)
{
	return (void*)((char*)pData + sizeof(ResourceHeader));
}
//---------------------------------------------------------------------------
const char* ResourceDeserializer::getDependencies(void* pData)
{
	ResourceHeader* hdr = (ResourceHeader*)pData;
	return (const char*)pData + hdr->stringTableOffset;
}
//---------------------------------------------------------------------------
void ResourceDeserializer::dumpDependencies(void* pData, Stream& strm)
{
	ResourceHeader* hdr = (ResourceHeader*)pData;
	char* pString = (char*)pData + hdr->stringTableOffset;

	while (pString && *pString)
	{
		size_t len = strlen(pString);
		strm.write((void*)pString, len);
		pString += (len + 1);
		strm.write("\n", 1);
	}
}
//---------------------------------------------------------------------------
void ResourceDeserializer::fix(void* pData, size_t index, void* pTarget)
{
	size_t i = 0;

	// read the header at the top of the stream
	ResourceHeader* hdr = (ResourceHeader*)pData;

	// advance to the fixup table
	ResourceRelocEntry* pFixups = 
		(ResourceRelocEntry*)((char*)pData + hdr->fixupTableOffset);

	// go until the fixups are zero
	while (pFixups->at)
	{
		if (pFixups->type == ResourceRelocEntry::INVALID)
		{
			break;
		}

		if (pFixups->type == ResourceRelocEntry::EXTERNAL)
		{
			if (index == i)
			{
				// find this fixup and set its value
				void* pAt = (char*)pData + pFixups->at;
				SerialPointer<void>* p;
				p = (SerialPointer<void>*)pAt;
				*p = pTarget;
				break;
			}

			++i;
		}

		++pFixups;
	}
}
//---------------------------------------------------------------------------
void ResourceDeserializer::fix(void* pData)
{
	// read the header at the top of the stream
	ResourceHeader* hdr = (ResourceHeader*)pData;

	// advance to the fixup table
	ResourceRelocEntry* pFixups = 
		(ResourceRelocEntry*)((char*)pData + hdr->fixupTableOffset);

	// go until the fixups are zero
	while (pFixups->at)
	{
		if (pFixups->type == ResourceRelocEntry::INVALID)
		{
			break;
		}

		switch (pFixups->type)
		{
		// entry patching to pointers within the resource
		case ResourceRelocEntry::INTERNAL:
			{
				void* pAt = (char*)pData + pFixups->at;
				void* pTo = (char*)pData + pFixups->to;
				SerialPointer<void>* p;
				p = (SerialPointer<void>*)pAt;
				*p = pTo;
				++pFixups;
			}
			break;

		// entry refers to something outside the resource data
		case ResourceRelocEntry::EXTERNAL:
			{
				//void* pAt = (char*)pData + pFixups->at;
				//void* pTo = (char*)pData + pFixups->to;
				//SerialPointer<void>* p;
				//p = (SerialPointer<void>*)pAt;
				//*p = pTo;
				++pFixups;
			}
			break;

		// entry says that we need to assign a vtable entry
		case ResourceRelocEntry::VTABLE:
			{
				void* pAt = (char*)pData + pFixups->at;
				const void* pVtabl = SerialClass::findClassById(pFixups->id)->getAddr();
				SerialPointer<void>* p;
				p = (SerialPointer<void>*)pAt;
				*p = pVtabl;
				++pFixups;
			}
			break;
		}
	}
}
