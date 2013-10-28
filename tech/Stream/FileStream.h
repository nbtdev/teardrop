/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDFILESTREAM_INCLUDED)
#define TDFILESTREAM_INCLUDED

#include "Stream.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	//
	// FileSystem-specific manifest constants
	// 
	const int READ		= 0x00000001;
	const int WRITE		= 0x00000002;
	const int APPEND	= 0x00000004;
	const int CREATENEW	= 0x00000008;
	const int TRUNCATE	= 0x00000010;
	const int BINARY	= 0x00000100;
	const int TEXT		= 0x00000200;

	class FileStream : public Stream
	{
		void* m_handle;
		int m_mode;
		size_t m_length;
		size_t m_position;
		bool m_eof;
		int m_asyncState;
		char m_async[32]; // async I/O working area

	public:
		FileStream();
		virtual ~FileStream();

		//! open a new file into this stream
		bool open(const char* pFilename, int mode, bool async=false);
		//! close this file stream (closes the underlying file)
		void close(bool async=false);
		// check to see if this file stream is open
		bool isOpen();

		// Stream implementation
		//! read data from this file stream
		int read(void* pData, size_t len, bool async=false);
		//! write data to this file stream
		int write(const void* pData, size_t len, bool async=false);
		//! return stream length
		size_t length() { return m_length; }
		//! return stream length
		size_t getPosition() { return m_position; }
		//! seek to another part of this stream
		bool seek(int offset, SeekType seekType = CURRENT, bool async=false);
		//! check to see if we are at the end of the stream
		bool isEnd() { return m_eof; }

		enum AsyncState
		{
			NOT_ASYNC,
			IDLE,
			READ_PENDING,
			WRITE_PENDING,
		};
		AsyncState getAsyncState() { return (AsyncState)m_asyncState; }

		//! used for async I/O operations
		void update();

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDFILESTREAM_INCLUDED
