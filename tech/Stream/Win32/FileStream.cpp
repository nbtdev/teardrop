/******************************************************************************
Copyright (c) 2018 Teardrop Games

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
******************************************************************************/

#include "Config.h"
#include "FileStream.h"
#include "Util/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace Teardrop;
//---------------------------------------------------------------------------
FileStream::FileStream() 
{
	m_eof = false;
	m_handle = 0;
	m_mode = 0;
	m_length = 0;
	m_position = 0;
	m_asyncState = NOT_ASYNC;
	memset(&m_async, 0, sizeof(m_async));
}
//---------------------------------------------------------------------------
FileStream::~FileStream() 
{
	if (m_handle)
		CloseHandle(m_handle);
}
//---------------------------------------------------------------------------
int FileStream::read(void* buffer, size_t size, bool async) 
{
	if (((m_mode & READ) && m_handle) == 0)
		return 0;

	if (async)
	{
		m_asyncState = IDLE;
	}

	LPOVERLAPPED pO = (LPOVERLAPPED)&m_async;
	DWORD bytesRead = 0;
	BOOL b = ReadFile(m_handle, buffer, (DWORD)size, &bytesRead, pO);

	// since we're always overlapped, ReadFile always returns 0 and
	// the end-of-file condition is reported via GetLastError() == ERROR_HANDLE_EOF
	if (b == 0)
	{
		DWORD err = GetLastError();

		if (err == ERROR_HANDLE_EOF)
		{
			// EOF
			m_eof = true;
			return 0;
		}
	}

	if (!async)
	{
		BOOL r = GetOverlappedResult(
			m_handle, pO, &bytesRead, TRUE);
		if (TRUE == r) {
			m_eof = (r && b && bytesRead == 0);
			m_position += bytesRead;
			pO->Offset += bytesRead;
			return bytesRead;
		}
	}

	m_asyncState = READ_PENDING;
	return bytesRead;
}
//---------------------------------------------------------------------------
int FileStream::write(const void* buffer, size_t size, bool async) 
{
	if (((m_mode & (WRITE|APPEND)) && m_handle) == 0)
		return 0;

	if (async)
	{
		m_asyncState = IDLE;
	}

	//LPOVERLAPPED pO = async ? (LPOVERLAPPED)&m_async : 0;
	LPOVERLAPPED pO = (LPOVERLAPPED)&m_async;
	DWORD bytesWritten = 0;
	WriteFile(m_handle, buffer, (DWORD)size, &bytesWritten, pO);

	if (!async)
	{
		if (TRUE == GetOverlappedResult(
			m_handle, pO, &bytesWritten, TRUE)) {
			m_position += bytesWritten;
			pO->Offset += bytesWritten;
			return bytesWritten;
		}
	}

	m_asyncState = WRITE_PENDING;
	return bytesWritten;
}
//---------------------------------------------------------------------------
bool FileStream::open(const char* filename, int mode, bool /*async*/) 
{
	// close any open file first
	if (m_handle != 0)
		close();

	m_mode = mode;
	m_position = 0;

	DWORD access = 0;
	DWORD shareMode = 0;
	DWORD creation = 0;

	if (mode & READ) {
		access |= GENERIC_READ;
		shareMode |= FILE_SHARE_READ;
		creation = OPEN_EXISTING;
	}

	if (mode & WRITE) {
		access |= GENERIC_WRITE;
		shareMode |= (FILE_SHARE_WRITE | FILE_SHARE_READ);
	}

	if (mode & APPEND)
		creation = OPEN_ALWAYS;

	if (mode & TRUNCATE)
		creation = CREATE_ALWAYS;
	
	if (mode & CREATENEW)
		creation = CREATE_NEW;
	
	m_handle = CreateFile(
		filename,
		access,
		shareMode,
		0,
		creation,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		0);

	if (m_handle == INVALID_HANDLE_VALUE)
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
		
		LocalFree(lpMsgBuf);
		return false;
	}

	m_length = GetFileSize(m_handle, NULL);
	return true;
}
//---------------------------------------------------------------------------
void FileStream::close(bool /*async*/) 
{
	CloseHandle(m_handle);
	m_handle = 0;
	m_position = 0;
	memset(&m_async, 0, sizeof(m_async));
}
//---------------------------------------------------------------------------
bool FileStream::isOpen() {
	return (m_handle != 0);
}
//---------------------------------------------------------------------------
bool FileStream::seek(int offset, SeekType seekType, bool /*async*/)
{	
	//if (async)
	//{
		LARGE_INTEGER size;
		GetFileSizeEx(m_handle, &size);

		LPOVERLAPPED lpO = (LPOVERLAPPED)&m_async;
		if (seekType == BEGIN)
		{
			lpO->Offset = offset;
		}
		else if (seekType == END)
		{
			// this will bite us when file sizes get close to and over 4GB
			lpO->Offset = (DWORD)size.QuadPart - offset;
		}
		else
		{
			lpO->Offset = (int)m_position + offset;
		}

		return true;
	//}
	//else
	//{
	//	DWORD dwMoveMethod = FILE_CURRENT;

	//	if (seekType == BEGIN)
	//	{
	//		dwMoveMethod = FILE_BEGIN;
	//	}

	//	if (seekType == END)
	//	{
	//		dwMoveMethod = FILE_END;
	//	}

	//	DWORD rtn = SetFilePointer(m_handle, (LONG)offset, NULL, dwMoveMethod);
	//	bool success = (rtn != INVALID_SET_FILE_POINTER);

	//	if (success)
	//	{
	//		m_position = (int)m_position + offset;
	//	}

	//	return success;
	//}
}
//---------------------------------------------------------------------------
void FileStream::update()
{
	if (m_asyncState == NOT_ASYNC)
	{
		return;
	}

	// otherwise, if an operation is pending check the status
	if (m_asyncState != IDLE)
	{
		LPOVERLAPPED pO = (LPOVERLAPPED)&m_async;
		DWORD cbBytes = 0;
		BOOL r = GetOverlappedResult(
			m_handle,
			pO,
			&cbBytes,
			FALSE);

		if (r == FALSE)
		{
			if (GetLastError() == ERROR_IO_INCOMPLETE)
			{
				return;
			}
		}

		// IO has completed
		m_eof = (r && (cbBytes == 0 && m_asyncState == READ_PENDING));
		m_asyncState = IDLE;
		m_position += cbBytes;
	}
}
