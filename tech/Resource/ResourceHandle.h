/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(RESOURCEHANDLE_INCLUDED)
#define RESOURCEHANDLE_INCLUDED

namespace Teardrop
{

	typedef void* HResource;
	const HResource INVALID_RESOURCE_HANDLE = (void*)-1;

	/*
		This utility class is provided for automatic deref 
		and release (upon leaving scope) for refcounted
		handle-based resources. Typical usage would look
		like

		GfxMesh* pMesh = AutoResPtr<GfxMesh>(myHandle);

		This will automatically release the resource when the 
		AutoResPtr goes out of scope, as well as provide a 
		simple means of access to the object to which the 
		handle refers.

		Note the implicit dependency (for now) on Environment
		and ResourceManager -- you will need to include the 
		headers for these classes in your code which uses
		AutoResPtr.
	*/
	template<class T>
	struct AutoResPtr
	{
		T* pT;
		HResource hT;

		AutoResPtr(HResource hRes)
		{
			hT = hRes;
			pT = static_cast<T*>(
				Environment::get().pResourceMgr->deref(hT));
		}
		~AutoResPtr()
		{
			Environment::get().pResourceMgr->release(hT);
		}
		operator T*() { return pT; }
		operator const T*() const { return pT; }
	};
}

#endif // RESOURCEHANDLE_INCLUDED
