/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "FileSystemWatcherWin32.h"
#include "Logger.h"
#include "Environment.h"
#include "Timer.h"

using namespace Teardrop;
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
		m_pData = (char*)TD_ALLOCATE(DEFAULT, BUFFER_SIZE);
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

			TD_DEALLOCATE(DEFAULT, m_pData);
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
		TD_DEALLOCATE(DEFAULT, m_pData);
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
			int len = pInfo->FileNameLength / 2;
			WCHAR* tmp = pInfo->FileName + len;
			WCHAR c = *tmp;
			size_t numConv;
			*tmp = 0;
			wcstombs_s(&numConv, buf, MAX_PATH, pInfo->FileName, len);
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
