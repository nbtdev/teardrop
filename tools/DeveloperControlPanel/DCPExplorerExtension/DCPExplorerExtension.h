/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(DCPExplorerExtension_INCLUDED)
#define DCPExplorerExtension_INCLUDED

#include <shlguid.h>
#include <comdef.h>
#include <shlobj.h>
#include "resource.h"

struct __declspec(uuid("{000214E4-0000-0000-C000-000000000046}")) IContextMenu;
//struct __declspec(uuid("{000214F4-0000-0000-C000-000000000046}")) IContextMenu2;
//struct __declspec(uuid("{BCFCE0A0-EC17-11d0-8D10-00A0C90F2719}")) IContextMenu3;

class DCPExplorerExtension :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DCPExplorerExtension, &CLSID_DCPExplorerExtension>,
	public IShellExtInit,
	public IContextMenu
{
	// we have to cache the file(s)/folder(s) that the Explorer shell sends us
	typedef std::list<std::string> StringList;
	StringList m_files;

	// we also need to store our command IDs
	UINT m_cmdBuild;
	UINT m_cmdBuildAdmin;

	// check to see if we are on Vista, so that we can
	// elevate privileges if needed
	bool isVista();

public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DCPEXPLOREREXTENSION)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DCPExplorerExtension)
		COM_INTERFACE_ENTRY(IContextMenu)
		COM_INTERFACE_ENTRY(IShellExtInit)
	END_COM_MAP()

	DCPExplorerExtension();
	~DCPExplorerExtension();

	// IShellExtInit implementation
	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdObj, HKEY hKeyProgId);

	// IContextMenu implementation
	STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pici);
};

#endif // DCPExplorerExtension_INCLUDED
