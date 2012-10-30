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
#include "FileSystemWatcherWin32.h"
#include "Logger.h"
#include "Environment.h"
#include "Timer.h"

using namespace CoS;
//---------------------------------------------------------------------------
FileSystemWatcherWin32::FileSystemWatcherWin32(
	const String& watchPath, bool bRecursive, float timeout)
	: m_bRecursive(bRecursive)
	, m_timeout(timeout)
{
	m_pData = 0;
	m_hIOCP = 0;
	ZeroMemory(&m_ovl, sizeof(m_ovl));

	m_hDir = CreateFile(
		watchPath,
		GENERIC_READ | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		0
		);

	if (m_hDir == INVALID_HANDLE_VALUE)
		m_hDir = 0;
	else
	{
		m_pData = (char*)COS_ALLOCATE(DEFAULT, BUFFER_SIZE);
		if (!ReadDirectoryChangesW(
			m_hDir,
			m_pData,
			BUFFER_SIZE,
			bRecursive ? TRUE : FALSE,
			FILE_NOTIFY_CHANGE_LAST_WRITE,
			0,
			&m_ovl,
			0
			))
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL );

			Environment::get().pLogger->logMessage((char*)lpMsgBuf);
			LocalFree(lpMsgBuf);

			COS_DEALLOCATE(DEFAULT, m_pData);
			m_pData = 0;
			CloseHandle(m_hDir);
			m_hDir = 0;
		}
		else
		{
			m_hIOCP = CreateIoCompletionPort(
				m_hDir,
				0,
				0,
				0);
		}
	}
}
//---------------------------------------------------------------------------
FileSystemWatcherWin32::~FileSystemWatcherWin32()
{
	if (m_hDir)
		CloseHandle(m_hDir);
	if (m_hIOCP)
		CloseHandle(m_hIOCP);
	if (m_pData)
		COS_DEALLOCATE(DEFAULT, m_pData);
}
//---------------------------------------------------------------------------
void FileSystemWatcherWin32::update(FileSystemChanges& changed)
{
	changed.clear();

	// check for our magic early-out value
	if (m_hDir == 0)
		return;

	float t = Environment::get().pMasterClock->getCurrentTime();

	LPOVERLAPPED ovl;
	ULONG_PTR pKey;
	DWORD dwBytes;

	while (GetQueuedCompletionStatus(
		m_hIOCP,
		&dwBytes,
		&pKey,
		&ovl,
		0))
	{
		// walk through the buffer and extract the changed file information
		size_t offset = size_t(-1);
		FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)m_pData;

		// "offset == 0" flags the last entry in the list, so we can stop
		// after that one has been processed
		while (offset)
		{
			char buf[MAX_PATH];

			// temporarily null-terminate the next filename
			WCHAR* tmp = pInfo->FileName + (pInfo->FileNameLength/2);
			WCHAR c = *tmp;
			*tmp = 0;
			wcstombs(buf, pInfo->FileName, MAX_PATH);
			*tmp = c;
			offset = pInfo->NextEntryOffset;

			// check to see if this guy already exists in the active table
			String fname(buf);
			ActiveFiles::iterator it = m_activeFiles.find(fname);
			if (it != m_activeFiles.end())
			{
				// update the time since the last change on this file
				it->second = t;
			}
			else
			{
				// insert a new one and set its initial time
				m_activeFiles.insert(ActiveFiles::value_type(fname, t));
			}

			pInfo = (FILE_NOTIFY_INFORMATION*)(m_pData + offset);
		}
	}

	// and finally, have to restart the watching...thanks to 
	// http://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw.html
	// for this tidbit
	ReadDirectoryChangesW(
		m_hDir,
		m_pData,
		BUFFER_SIZE,
		m_bRecursive ? TRUE : FALSE,
		FILE_NOTIFY_CHANGE_LAST_WRITE,
		0,
		&m_ovl,
		0
		);

	// now walk the table and see if anyone is past their timeout (it's possible that 
	// we got multiple updates for a single file in this poll, so we want to do this 
	// traversal outside the previous loop)
	ActiveFiles::iterator it = m_activeFiles.begin(); 
	ActiveFiles::iterator end = m_activeFiles.end();

	while (it != end)
	{
		if ((t - it->second) > m_timeout)
		{
			changed.push_back(it->first);
			m_activeFiles.erase(it++);
		}
		else
			++it;
	}
}
