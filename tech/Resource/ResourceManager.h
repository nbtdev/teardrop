/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCEMANAGER_INCLUDED)
#define RESOURCEMANAGER_INCLUDED

#include "Resource/ResourceHandle.h"
#include "Util/FourCC.h"
#include "Memory/Allocators.h"
#include <list>
#include <map>

namespace Teardrop
{
	struct Environment;
	class Resource;
	class ResourceFactory;
	class FileSystemWatcher;

	class IResourceListener
	{
	public:
		enum LoadStatus
		{
			SUCCEEDED,
			FAILED,
		};

		virtual void resourceLoadCompleted(LoadStatus status, const char* url) = 0;
		virtual void resourceLoadStarted(const char* url) = 0;
	};

	class ResourceManager
	{
		class AsyncLoad;
		struct DeferredRelease;
		AsyncLoad* m_pAsyncLoads;
		AsyncLoad* m_pDeferred;
		void* m_pDeferredReleases;
		FileSystemWatcher* m_pDataTreeWatcher;
		size_t m_pendingLoads;

		struct ResourceEntry
		{
			void* pData;
			Resource* pResource;
			volatile long refCount;
			size_t isDynamic;
			ResourceFactory* pCreator;
			Allocator* pAlloc;
			FourCC fourCC;
			volatile long isReleased;
			unsigned __int64 id;

			ResourceEntry()
			{
				pData = 0;
				pResource = 0;
				pCreator = 0;
				refCount = 0;
				isDynamic = 0;
				pAlloc = 0;
				isReleased = 1;
				id = 0;
			}

			TD_DECLARE_ALLOCATOR();
		};
		ResourceEntry* m_pResources;

		typedef std::list<HResource> FreeList;
		FreeList m_freelist;

		typedef std::map<FourCC, ResourceFactory*> FactoryMap;
		FactoryMap m_factories;

		typedef std::map<FourCC, Resource*> Placeholders;
		Placeholders m_placeholders;

		typedef std::multimap<unsigned __int64, ResourceEntry*> IdLookupTable;
		typedef std::pair<IdLookupTable::iterator, IdLookupTable::iterator> IdLookupTableRange;
		IdLookupTable m_lutId;

		Environment& m_env;

	public:
		ResourceManager();
		ResourceManager(Environment& env);
		~ResourceManager();

		bool initialize();
		bool destroy();
		bool update();

		/*
			Acquire a resource by (optional) name and (required) type. 
			If name is null, new (dynamic) resource will be created based
			on registered factory for the fourCC type. If no factory can
			be found, will return INVALID_RESOURCE_HANDLE.

			If name is provided, the manager will look for the resource by 
			its name in the locations known to it. If resource cannot be 
			found by its name, this method will return INVALID_RESOURCE_HANDLE.
		*/
		HResource acquire(const FourCC& fourCC, const char* name=0, IResourceListener* pCB=0);
		/*
			Acquire a resource by its unique ID, if known.
		*/
		HResource acquire(unsigned __int64 id);
		/*
			Release a resource. Note this does not immediately delete
			the resource, it just lets the resource system know that 
			the caller is done with it. The caller should treat the 
			resource as if it no longer exists.
		*/
		void release(HResource handle);
		/*
			Dereference a handle to the data it references. USE ONLY IF YOU 
			KNOW WHAT YOU ARE DOING -- this method is intended to allow 
			the creators of dynamic resources to access their resource
			data; since this data can be used simultaneously by multiple threads
			you must ensure that when editing this data, that you have 
			exclusive access to the data; the resource management system
			will not ensure this exclusivity for you.
		*/
		Resource* deref(HResource handle);
		/*
			Allow external systems to register themselves as factories for
			the type of object noted by the fourCC value.
		*/
		void registerFactory(ResourceFactory* pFactory, const FourCC& fourCC);
		/*
			Allow external systems to unregister themselves as factories for
			the type of object noted by the fourCC value.
		*/
		void unregisterFactory(ResourceFactory* pFactory, const FourCC& fourCC);
		/*
			Destroy a resource, optionally keeping the ResourceEntry block (for 
			supporting resource hot-swap)
		*/
		void destroyResource(ResourceEntry* pEnt, bool bKeepEntry=false);

		TD_DECLARE_ALLOCATOR();

	private:
		//! starts background load of resource
		void initiateLoad(
			AsyncLoad** ppCurrent, 
			const FourCC& fourCC,
			const char* name, 
			HResource hRes, 
			IResourceListener* pCB=0);

		ResourceManager(const ResourceManager& other);
		ResourceManager& operator=(const ResourceManager& other);
	};
}

#endif // RESOURCEMANAGER_INCLUDED
