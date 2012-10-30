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

#include "ResourceManager.h"
#include "ResourceFactory.h"
#include "Serialization/include/ResourceDeserializer.h"
#include "Resource.h"
#include "Memory/include/Memory.h"
#include "Memory/include/Allocators.h"
#include "Util/include/Hash.h"
#include "Util/include/Logger.h"
#include "Util/include/FileSystemWatcher.h"
#include "Util/include/StringUtil.h"
#include "Util/include/Environment.h"
#include "Stream/include/FileStream.h"
#include <string>
#include <assert.h>
#include <windows.h>

using namespace CoS;
//---------------------------------------------------------------------------
const int RESOURCE_ARRAY_SIZE = 32 * 1024;
//---------------------------------------------------------------------------
ResourceManager::ResourceManager()
: m_env(Environment::get())
{
	m_pResources = 0;
	m_pAsyncLoads = 0;
	m_pDeferred = 0;
	m_pendingLoads = 0;
	m_pDataTreeWatcher = 0;
}
//---------------------------------------------------------------------------
ResourceManager::ResourceManager(Environment& env)
: m_env(env)
{
	m_pResources = 0;
	m_pAsyncLoads = 0;
	m_pDeferred = 0;
	m_pendingLoads = 0;
	m_pDataTreeWatcher = 0;
}
//---------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	assert(!m_pResources);
}
//---------------------------------------------------------------------------
bool ResourceManager::initialize()
{
	// create enough resource containers for our needs
	m_pResources = COS_NEW ResourceEntry[RESOURCE_ARRAY_SIZE];

	// fill in the freelist with the addresses of these entries
	for (size_t i=0; i<RESOURCE_ARRAY_SIZE; ++i)
	{
		HResource r = &m_pResources[i];
		m_freelist.push_back(r);
	}

	// create a filesystem watcher for the data tree
	m_pDataTreeWatcher = FileSystemWatcher::create(
		String(Environment::get().pDataPath),
		true,
		0.5f);

	return true;
}
//---------------------------------------------------------------------------
bool ResourceManager::destroy()
{
	if (m_pDataTreeWatcher)
		FileSystemWatcher::destroy(m_pDataTreeWatcher);
	m_pDataTreeWatcher = 0;

	m_freelist.clear();

	if (m_pResources)
	{
		for (size_t i=0; i<RESOURCE_ARRAY_SIZE; ++i)
		{
			if (m_pResources[i].isDynamic)
			{
				m_pResources[i].pResource->destroy();
				delete m_pResources[i].pResource;
			}
			else
			{
				if (m_pResources[i].pResource)
				{
					m_pResources[i].pResource->release();

					// we made the memory for this, we need to release it
					m_pResources[i].pAlloc->DeallocateAligned(m_pResources[i].pData);
				}
			}
		}

		delete [] m_pResources;
		m_pResources = 0;
	}

	return true;
}
//---------------------------------------------------------------------------
HResource ResourceManager::acquire(
	const FourCC& fourCC, 
	const char* name, 
	IResourceListener* pCB)
{
	if (!name)
	{
		// create a new (dynamic) one of these
		if (m_freelist.size())
		{
			FactoryMap::iterator it = m_factories.find(fourCC);
			if (it != m_factories.end())
			{
				Resource* pRes = it->second->create(fourCC);

				if (pRes)
				{
					HResource hRes = m_freelist.front();
					m_freelist.pop_front();

					ResourceEntry* ent = (ResourceEntry*)hRes;
					ent->pResource = pRes;
					ent->isDynamic = 1;
					InterlockedExchange(&(ent->isReleased), 0);
					ent->pCreator = it->second;
					ent->fourCC = fourCC;
					InterlockedIncrement(&(ent->refCount));

					return hRes;
				}
			}
		}
	}
	else
	{
		// check to see if we already have one of these
		unsigned __int64 id = hashString64(name);
		HResource hRes = INVALID_RESOURCE_HANDLE;

		// TODO: we have an issue with doing this where Havok rigid bodies are involved,
		// as there is no clear path on how to clone the body instances while using the
		// same resource data; re-enable this when we have a workaround for that problem
		/*
		IdLookupTable::iterator it = m_lutId.find(id);
		if (it != m_lutId.end())
		{
			char buf[1024];
			sprintf_s(buf, 1024, "returning existing resource handle for %s ...", name);
			m_env.pLogger->logMessage(buf);
			InterlockedIncrement(&(it->second->refCount));
			return it->second;
		}
		*/
		// peel off a resource entry
		hRes = m_freelist.front();
		m_freelist.pop_front();

		ResourceEntry* ent = (ResourceEntry*)hRes;
		ent->pResource = 0; // for now, since we haven't loaded it yet
		ent->isDynamic = 0; // for now, it's static
		InterlockedExchange(&(ent->isReleased), 0); // it's no longer in "released" state
		InterlockedExchange(&(ent->refCount), 1);	// init ref count on resource
		ent->id = id;		// we've already calc'ed it, so...
		m_lutId.insert(IdLookupTable::value_type(id, ent));;
		
		FactoryMap::iterator f = m_factories.find(fourCC);
		if (f != m_factories.end())
		{
			ent->pAlloc = f->second->getAllocator();
		}

		/* Rules for finding these:

			1. Check loaded libraries
			2. Check local cache (mirror of remote repository)
			3. Check local filesystem
			4. Fetch from remote repository (store in cache, repeat #2)
		*/
		
		// todo: check loaded libraries...

		// initiate search/load for the named resource
        initiateLoad(&m_pDeferred, fourCC, name, hRes, pCB);
		
		// return our handle
		return hRes;
	}

	return INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
HResource ResourceManager::acquire(unsigned __int64 /*id*/)
{
	return INVALID_RESOURCE_HANDLE;
}
//---------------------------------------------------------------------------
void ResourceManager::release(HResource handle)
{
	if (handle && handle != INVALID_RESOURCE_HANDLE)
	{
		// dec the ref count and if it's zero, mark this resource for
		// cleanup
		ResourceEntry* ent = (ResourceEntry*)handle;
		if (InterlockedDecrement(&(ent->refCount)) <= 0)
			destroyResource(ent);
	}
}
//---------------------------------------------------------------------------
Resource* ResourceManager::deref(HResource handle)
{
	if (handle && handle != INVALID_RESOURCE_HANDLE)
	{
		ResourceEntry* ent = (ResourceEntry*)handle;
		InterlockedIncrement((LONG*)&ent->refCount);
		return ent->pResource;
	}

	return 0;
}
//---------------------------------------------------------------------------
void ResourceManager::registerFactory(
	ResourceFactory* pFactory, const CoS::FourCC& fourCC)
{
	// replace any existing entry for this type
	m_factories[fourCC] = pFactory;
}
//---------------------------------------------------------------------------
void ResourceManager::unregisterFactory(
	ResourceFactory* /*pFactory*/, const CoS::FourCC& fourCC)
{
	FactoryMap::iterator it = m_factories.find(fourCC);
	if (it != m_factories.end())
	{
		m_factories.erase(it);
	}
}
//---------------------------------------------------------------------------
class ResourceManager::AsyncLoad
{
public:
	FileStream fs;
	
	IResourceListener* pListener;
	HResource hRes;
	std::string url;    // before replacing / with \.
	const FourCC& fourCC;
	AsyncLoad* pNext;

    AsyncLoad(const FourCC& fcc) : fourCC(fcc)  {}

	COS_DECLARE_ALLOCATOR();

private:
	AsyncLoad& operator=(const AsyncLoad& other);
};
//---------------------------------------------------------------------------
bool ResourceManager::update()
{
	AsyncLoad* pLoad = m_pAsyncLoads;
	AsyncLoad** ppPrev = &m_pAsyncLoads;

	if (m_pDataTreeWatcher)
	{
		FileSystemWatcher::FileSystemChanges changedFiles;
		m_pDataTreeWatcher->update(changedFiles);

		for (FileSystemWatcher::FileSystemChanges::iterator file = changedFiles.begin();
			file != changedFiles.end(); ++file)
		{
			// invalidate all resources that (a) have this name, and (b) are 
			// already loaded
			String fullpath(m_env.pDataPath);
			String convFile(*file);
			StringUtil::toUNIXPath(convFile);
			fullpath += convFile;
			IdLookupTableRange range = m_lutId.equal_range(hashString64(fullpath));
			for (IdLookupTable::iterator it = range.first; it != range.second; ++it)
			{
				ResourceEntry* ent = static_cast<ResourceEntry*>(it->second);
				if (ent->pResource)
				{
					// destroy the underlying resource (keeping the entry block)
					destroyResource(ent, true);

					// and then queue this for (re)loading
					initiateLoad(&m_pDeferred, ent->fourCC, fullpath, it->second);

					char buf[1024];
					sprintf_s(buf, 1024, "Resource '%s' queued for re-loading...", fullpath);
					m_env.pLogger->logMessage(buf);
				}
			}
		}
	}

    // check to see if any of our pending async operations have completed,
	// in which case they can be removed from this list
	while (pLoad)
	{
        if (pLoad->fs.getAsyncState() == FileStream::IDLE)
		{
			// operation completed
			ResourceEntry* ent = (ResourceEntry*)pLoad->hRes;
			--m_pendingLoads;

			char buf[1024];
			sprintf_s(buf, 1024, "Resource '%s' finished loading...", pLoad->url.c_str());
			m_env.pLogger->logMessage(buf);

			if (pLoad->pListener)
			{
				pLoad->pListener->resourceLoadCompleted(IResourceListener::SUCCEEDED, pLoad->url.c_str());
			}

			// fix up the loaded resource
			ResourceDeserializer ser;
			ser.fix(ent->pData);
			ent->pResource = (Resource*)ser.get(ent->pData);

			// add us to the id lut
			m_lutId.insert(IdLookupTable::value_type(ent->id, ent));

			std::string url(pLoad->url);
			FourCC fourCC(pLoad->fourCC);
			
            pLoad->fs.close();

			// unlink us
			*ppPrev = pLoad->pNext;
			delete pLoad;
			pLoad = *ppPrev;
			
			// check for dependencies
			const char* deps = ser.getDependencies(ent->pData);
			size_t idx = 0;
			while (*deps)
			{
				const char* dep = deps;

				std::string s(url);
				s = s.substr(0, s.find_last_of("\\/")+1);
				s += dep;
				unsigned __int64 depId = hashString64(s.c_str());

				IdLookupTableRange range = m_lutId.equal_range(depId);
				IdLookupTable::iterator it = m_lutId.end();
				for (IdLookupTable::iterator _it = range.first; _it != range.second; ++_it)
				{
					if (_it->second == ent)
					{
						it = _it;
						break;
					}
				}

				if (it != m_lutId.end())
				{
					// fix us up with this handle
					ser.fix(ent->pData, idx, (void*)(it->second));
					++(it->second->refCount);
				}
				else
				{
					if (m_freelist.size())
					{
						// need to get a new resource handle for this one
						HResource hRes = m_freelist.front();
						m_freelist.pop_front();

						// fix us up with *this* handle
						ser.fix(ent->pData, idx, hRes);

						// add this dep's ID so we don't multiply load it
						ResourceEntry* newEnt = (ResourceEntry*)hRes;
						m_lutId.insert(IdLookupTable::value_type(depId, newEnt));
						newEnt->id = depId;
						InterlockedExchange(&(newEnt->isReleased), 0);
						InterlockedExchange(&(newEnt->refCount), 1);

						// make a new load task for this dep
						initiateLoad(&m_pDeferred, fourCC, s.c_str(), hRes);
					}
				}

				++idx;
				deps += strlen(deps) + 1;
			}

			continue;
		}
		else
		{
			pLoad->fs.update();
			ppPrev = &(pLoad->pNext);
			pLoad = pLoad->pNext;
		}
	}

	// once we reach the end, attach the deferred list to the main one
	*ppPrev = m_pDeferred;
	m_pDeferred = 0;

	return true;
};
//---------------------------------------------------------------------------
void ResourceManager::initiateLoad(
	AsyncLoad** ppCurrent, 
	const FourCC& fourCC,
	const char* name, 
	HResource hRes, 
	IResourceListener* pCB)
{
	char buf[1024];
	++m_pendingLoads;

	if (pCB)
	{
		pCB->resourceLoadStarted(name);
	}

	AsyncLoad* pLoad = COS_NEW AsyncLoad(fourCC);
	if (!pLoad->fs.open(name, READ|BINARY))
	{
		sprintf_s(buf, 1024, "Could not find resource %s...", name);
		m_env.pLogger->logMessage(buf);
		--m_pendingLoads;

		if (pCB)
		{
			pCB->resourceLoadCompleted(IResourceListener::FAILED, name);
		}

		delete pLoad;
		return;
	}

	pLoad->pNext = *ppCurrent;
	pLoad->pListener = pCB;
	*ppCurrent = pLoad;
	pLoad->hRes = hRes;

	// save off the pathname
	pLoad->url = name;

	ResourceEntry* ent = (ResourceEntry*)hRes;
	ent->pAlloc = GetDEFAULTAllocator();
	// try to find an alternate allocator based on factory lookup
	FactoryMap::iterator it = m_factories.find(fourCC);
	if (it != m_factories.end())
	{
		ent->pAlloc = it->second->getAllocator();
	}

	ent->isDynamic = false;
	ent->pCreator = 0;
	ent->pData = ent->pAlloc->AllocateAligned(pLoad->fs.length(), 16 COS_ALLOC_SITE);
	ent->pResource = 0;

	if (!pLoad->fs.read(ent->pData, pLoad->fs.length(), true))
	{
		// then...?
	}

	sprintf_s(buf, 1024, "Resource %s load requested...", name);
	m_env.pLogger->logMessage(buf);
}
//---------------------------------------------------------------------------
void ResourceManager::destroyResource(ResourceEntry* pEnt, bool bKeepEntry)
{
	if (pEnt->pResource)
	{
		pEnt->pResource->release();

		// dealloc the data associated with the resource
		assert(pEnt->pAlloc || pEnt->pCreator);
		if (pEnt->pAlloc)
			pEnt->pAlloc->DeallocateAligned(pEnt->pData);
		else if (pEnt->pCreator)
			pEnt->pCreator->destroy(pEnt->pResource, pEnt->fourCC);

		InterlockedExchange(&(pEnt->isReleased), 1);

		if (bKeepEntry)
		{
			// we only want to clear out some of the fields...
			pEnt->pResource = 0;
			pEnt->pCreator = 0;
			pEnt->pAlloc = 0;
			pEnt->id = 0;
			pEnt->pData = 0;
		}
		else
		{
			// fully release the resource entry block
			IdLookupTableRange range = m_lutId.equal_range(pEnt->id);
			for (IdLookupTable::iterator it = range.first; it != range.second; ++it)
			{
				if (it->second == pEnt)
				{
					m_lutId.erase(it);
					break;
				}
			}

			// clear and push the resource pEntry back into the freelist
			memset(pEnt, 0, sizeof(*pEnt));
			m_freelist.push_back(pEnt);
		}
	}
}
