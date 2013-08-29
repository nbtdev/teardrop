/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDMEMORYSTREAM_INCLUDED)
#define TDMEMORYSTREAM_INCLUDED

#include "Stream.h"
#include "Memory/Allocators.h"

namespace Teardrop
{
	class MemoryStream : public Stream
	{
		unsigned char* mData;
		int mLen;
		int mPos;
		int mCapacity;
		bool mOwnData;

	public:
		MemoryStream();
		MemoryStream(size_t len);
		MemoryStream(void* data, size_t len);
		~MemoryStream();

		// Stream implementation - note that async flags are ignored for MemoryStream
		//! read data from this stream
		bool read(void* pData, size_t len, bool async=false);
		//! write data to this stream
		bool write(const void* pData, size_t len, bool async=false);
		//! return stream length in bytes
		size_t length();
		//! return stream position
		size_t getPosition();
		//! seek to another part of this stream
		bool seek(int offset, SeekType seekType = CURRENT, bool async=false);
		//! check to see if we are at the end of the stream
		bool isEnd();

		// MemoryStream-specific
		// return a pointer to the start of the data
		void* data();

		TD_DECLARE_ALLOCATOR();

	protected:
		void expand();
	};
}

#endif // TDMEMORYSTREAM_INCLUDED
