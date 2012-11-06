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
		void* m_pData;
		size_t m_len;

	public:
		MemoryStream(void* pData, size_t len);
		virtual ~MemoryStream();

		// Stream implementation
		//! read data from this file stream
		bool read(void* pData, size_t len, bool async=false);
		//! write data to this file stream
		bool write(const void* pData, size_t len, bool async=false);

		TD_DECLARE_ALLOCATOR();
	};
}

#endif // TDMEMORYSTREAM_INCLUDED
