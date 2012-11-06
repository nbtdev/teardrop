/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Config.h"
#include "ResourceLibrary.h"
#include "ResourceHeader.h"
#include "Stream/Stream.h"
#include "Memory/Memory.h"
#include "Memory/Allocators.h"
#include <assert.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
ResourceLibrary::ResourceLibrary()
{
	m_refCount = 0;
	m_pData = 0;
}
//---------------------------------------------------------------------------
ResourceLibrary::~ResourceLibrary()
{
	assert(!m_refCount);
	assert(!m_pData);
}
//---------------------------------------------------------------------------
bool ResourceLibrary::initialize()
{
	assert(!m_pData);
	assert(!m_refCount);

	memset(&m_header, 0, sizeof(m_header));

	return true;
}
//---------------------------------------------------------------------------
bool ResourceLibrary::destroy()
{
	assert(!m_refCount);
	if (m_refCount)
	{
		return false;
	}

	// delete any resource allocations
	for (Resources::iterator it = m_resources.begin();
		it != m_resources.end(); ++it)
	{
		GetDEFAULTAllocator()->Deallocate(*it);
	}

	// and clear the map and list
	m_lut.clear();
	m_resources.clear();

	GetDEFAULTAllocator()->Deallocate(m_pData);
	return true;
}
//---------------------------------------------------------------------------
void ResourceLibrary::addResource(Stream& inStrm)
{
	// make some memory for the incoming data
	void* pRes = GetDEFAULTAllocator()->AllocateAligned(inStrm.length(), 16 TD_ALLOC_SITE);
	inStrm.read(pRes, inStrm.length());

	// add this resource to the list and to the lut
	ResourceHeader* pHdr = (ResourceHeader*)pRes;
	m_lut[pHdr->resourceId] = pRes;
	m_resources.push_back(pRes);
}
//---------------------------------------------------------------------------
bool ResourceLibrary::findResource(
	unsigned __int64 resId, void** pOut, size_t& len)
{
	ResourceLUT::iterator it = m_lut.find(resId);
	if (it == m_lut.end())
	{
		return false;
	}

	ResourceHeader* pHdr = (ResourceHeader*)it->second;
	*pOut = (char*)it->second + sizeof(ResourceHeader);
	len = pHdr->size;

	return true;
}
//---------------------------------------------------------------------------
bool ResourceLibrary::serialize(Stream& strm, Stream& /*exports*/)
{
	size_t offset = 0;

	// write out the header as a placeholder
	strm.write(&m_header, sizeof(m_header));
	offset += sizeof(m_header);



	return true;
}
//---------------------------------------------------------------------------
bool ResourceLibrary::deserialize(Stream& strm)
{
	m_pData = GetDEFAULTAllocator()->AllocateAligned(strm.length(), 16 TD_ALLOC_SITE);
	strm.read(m_pData, strm.length());

	return true;
}
//---------------------------------------------------------------------------
size_t ResourceLibrary::getRefCount()
{
	return m_refCount;
}
//---------------------------------------------------------------------------
size_t ResourceLibrary::addRef()
{
	++m_refCount;
	return m_refCount;
}
//---------------------------------------------------------------------------
size_t ResourceLibrary::decRef()
{
	if (--m_refCount == 0)
	{
		// then the library is able to unload safely...in theory
	}

	return m_refCount;
}
