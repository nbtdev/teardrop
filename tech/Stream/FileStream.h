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

#if !defined(COSFILESTREAM_INCLUDED)
#define COSFILESTREAM_INCLUDED

#include "Stream.h"
#include "Memory/Allocators.h"

namespace CoS
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
		bool read(void* pData, size_t len, bool async=false);
		//! write data to this file stream
		bool write(const void* pData, size_t len, bool async=false);
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

		COS_DECLARE_ALLOCATOR();
	};
}

#endif // COSFILESTREAM_INCLUDED
