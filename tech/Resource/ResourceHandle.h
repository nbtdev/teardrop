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

#if !defined(RESOURCEHANDLE_INCLUDED)
#define RESOURCEHANDLE_INCLUDED

namespace CoS
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
