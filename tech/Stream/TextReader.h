/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TEXTREADER_INCLUDED)
#define TEXTREADER_INCLUDED

#include "Memory/Allocators.h"

namespace Teardrop
{
	class Stream;

	class TextReader
	{
		Stream& m_strm;

	public:
		TextReader(Stream& s);
		virtual ~TextReader();

		bool getline(char* buf, size_t len);

		TD_DECLARE_ALLOCATOR();

	private:
		TextReader(const TextReader& other);
		TextReader& operator=(const TextReader& other);
	};

}

#endif // TEXTREADER_INCLUDED
