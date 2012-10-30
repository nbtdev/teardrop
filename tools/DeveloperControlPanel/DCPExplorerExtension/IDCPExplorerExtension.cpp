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

#include "stdafx.h"
#include "shlobj.h"
#include "DCPExplorerExtension_h.h"
#include "DCPExplorerExtension.h"
#include "strsafe.h"
#include <fstream>

//---------------------------------------------------------------------------
// Menu command verbs (these are non-internationalizable)
static const char* s_pBuildVerb = "BUILD";
static const wchar_t* s_pBuildVerbW = L"BUILD";
//---------------------------------------------------------------------------
// Explorer basically probes each extension first, before issuing any
// commands on the file(s) it mentions in this call, so whatever we get,
// we need to cache for a later command issue
HRESULT DCPExplorerExtension::Initialize(
	LPCITEMIDLIST pidlFolder, 
	LPDATAOBJECT lpdObj, 
	HKEY hKeyProgId)
{
	const UINT MAXPATHLEN = 16384;
	char buf[MAXPATHLEN] = {0,};

	// clear any files we have cached
	m_files.clear();

	// this will be true if the user selected one or more files in Explorer
	if (lpdObj)
	{
		FORMATETC obFmt = 
		{
			CF_HDROP, (DVTARGETDEVICE FAR *)NULL,
			DVASPECT_CONTENT, -1, TYMED_HGLOBAL 
		};

		STGMEDIUM obMedium;
		HRESULT hres = lpdObj->GetData(&obFmt, &obMedium);

		if(!SUCCEEDED(hres))
		{
			MessageBox(NULL,"Failed GetData call.","",MB_OK);
		}

		if(SUCCEEDED(hres))
		{
			// Get the file data
			if(obMedium.hGlobal)
			{
				UINT cbFiles = DragQueryFile((HDROP)obMedium.hGlobal, (UINT)-1, 0, 0);
				for (UINT i=0; i<cbFiles; ++i)
				{
					DragQueryFile((HDROP)obMedium.hGlobal, i, buf, MAXPATHLEN);
					m_files.push_back(std::string(buf));
				}
			}
		}

		ReleaseStgMedium(&obMedium);
		return hres; //S_OK;
	}
	// otherwise, it's just them clicking on the background of Explorer or 
	// the desktop
	else
	{
		if (pidlFolder)
		{
			SHGetPathFromIDList(pidlFolder, buf);
			m_files.push_back(std::string(buf));
		}
	}

	return E_INVALIDARG;
}
//---------------------------------------------------------------------------
HRESULT DCPExplorerExtension::GetCommandString(
	UINT_PTR idCmd, 
	UINT uFlags, 
	UINT* /* pwReserved */, 
	LPSTR pszName, 
	UINT cchMax)
{
	const UINT BUFLEN = 1024;
	char buf[BUFLEN] = {0,};
	wchar_t bufW[BUFLEN] = {0,};

	HRESULT hr = E_INVALIDARG;

	if (idCmd != 0) // hardcode for now, just our single command
		return hr;

	switch (uFlags)
	{
	case GCS_HELPTEXTA:
		LoadString(_Module.m_hInst, IDS_DCP_BUILD_DESC, buf, BUFLEN);
		StringCchCopyNA(pszName, cchMax, buf, strlen(buf));
		break;

	case GCS_HELPTEXTW:
		{
			LoadString(_Module.m_hInst, IDS_DCP_BUILD_DESC, buf, BUFLEN);
			size_t dummy = 0;
			mbstowcs_s(&dummy, bufW, BUFLEN, buf, BUFLEN);
			StringCchCopyNW((LPWSTR)pszName, cchMax, bufW, strlen(buf));
		}
		break;

	case GCS_VERBA:
		StringCchCopyNA(pszName, cchMax, s_pBuildVerb, strlen(s_pBuildVerb));
		break;

	case GCS_VERBW:
		StringCchCopyNW((LPWSTR)pszName, cchMax, s_pBuildVerbW, wcslen(s_pBuildVerbW));
		break;
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT DCPExplorerExtension::QueryContextMenu(
	HMENU hMenu, 
	UINT indexMenu, 
	UINT idCmdFirst, 
	UINT /* idCmdLast */, 
	UINT /* uFlags */)
{
	const UINT BUFLEN = 1024;
	char buf[BUFLEN] = {0,};

	// store off our menu command ID
	m_cmdBuild = idCmdFirst;

	// build the "Developer" submenu
	if (!LoadString(_Module.m_hInst, IDS_DCP_BUILD, buf, BUFLEN))
	{
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, 0);
	}

	HMENU hDropDown = CreatePopupMenu();
	MENUITEMINFO sub;
	ZeroMemory(&sub, sizeof(sub));
	sub.cbSize = sizeof(sub);
	sub.fMask = MIIM_TYPE | MIIM_ID;
	sub.fType = MFT_STRING;
	sub.wID = m_cmdBuild;
	sub.dwTypeData = (LPSTR)buf;
	InsertMenuItem(hDropDown, 1, TRUE, &sub);

	// build the main entry in the context menu
	if (!LoadString(_Module.m_hInst, IDS_DROPDOWNMENULABEL, buf, BUFLEN))
	{
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, 0);
	}

	MENUITEMINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_TYPE | MIIM_SUBMENU;
	mi.fType = MFT_STRING;
	mi.hSubMenu = hDropDown;
	mi.dwTypeData = (LPSTR)buf;

	// insert separator before
	InsertMenu(hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL);

	// then put our menu into the context menu
	InsertMenuItem(
		hMenu,
		indexMenu++,
		TRUE,
		&mi);

	// insert separator after
	InsertMenu(hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL);

	if (isVista())
	{

	}

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, m_cmdBuild - idCmdFirst + 3);
}

HRESULT TellInvalidArg()
{
	MessageBox(
		0, 
		"Invalid argument detected.", 
		"Developer Shell Extension", 
		MB_ICONINFORMATION);

	return E_INVALIDARG;
}

bool winMutexExists( const char *mutex )
{
	HANDLE hMutex = CreateMutex( NULL, FALSE, mutex );
	if( !hMutex )
	{
		return true;
	}

	bool exists = false;

	if( GetLastError () == ERROR_ALREADY_EXISTS )
	{
		exists = true;
	}
	CloseHandle( hMutex );
	ReleaseMutex( hMutex );

	return exists;
}


//---------------------------------------------------------------------------
HRESULT DCPExplorerExtension::InvokeCommand(
	LPCMINVOKECOMMANDINFO pCmdInfo)
{
	const UINT BUFLEN = 1024;
	char buf[BUFLEN] = {0,};

	if (!pCmdInfo)
		return TellInvalidArg();

	if (HIWORD(pCmdInfo->lpVerb) != 0)
	{
		if (pCmdInfo->cbSize == sizeof(CMINVOKECOMMANDINFOEX) 
			&& (pCmdInfo->fMask & CMIC_MASK_UNICODE))
		{
			// verb as UNICODE
			CMINVOKECOMMANDINFOEX* pUCmdInfo = (CMINVOKECOMMANDINFOEX*)pCmdInfo;
			if (_wcsicmp(pUCmdInfo->lpVerbW, s_pBuildVerbW))
			{
				return TellInvalidArg();
			}
		}
		else
		{
			// verb as ANSI
			if (_stricmp(pCmdInfo->lpVerb, s_pBuildVerb))
			{
				return TellInvalidArg();
			}
		}
	}
	else if (LOWORD(pCmdInfo->lpVerb) != 0) // hardcoded offset into our "command ID array"
	{
		return TellInvalidArg();
	}

	// process the command -- simply write out the file list to the user's tmp dir
	// where the fsw in the ADCP will notice the change and process the file(s)
	if (SUCCEEDED(SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_DEFAULT, buf)))
	{
		// append it to the temp dir
		strcat_s(buf, BUFLEN, "\\DCP");

		// make sure it exists first
		CreateDirectory(buf, 0);

		// append the file name
		strcat_s(buf, BUFLEN, "\\dcp.txt");

		// create the file list file
		std::ofstream o(buf);
		if (o.is_open())
		{
			for (StringList::iterator it = m_files.begin(); 
				it != m_files.end(); ++it)
			{
				o << *it << std::endl;
			}

			o.close();
		}

		return S_OK;
	}

	MessageBox(
		0, 
		"Could not start build of the requested files", 
		"Developer Shell Extension", 
		MB_ICONINFORMATION);
	
	return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, 0x10000004);;
}
//---------------------------------------------------------------------------
DCPExplorerExtension::DCPExplorerExtension()
{
	m_cmdBuild = 0;
}
//---------------------------------------------------------------------------
DCPExplorerExtension::~DCPExplorerExtension()
{
}
//---------------------------------------------------------------------------
bool DCPExplorerExtension::isVista()
{
	return false;
}
