/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(TDHASH_INCLUDED)
#define TDHASH_INCLUDED

#include <cstddef>
#include <cstdint>

namespace Teardrop
{
	//! hash a null-terminated string to a word-length unique hash value
	size_t hashString(const char* strVal);
	//! hash an arbitrary byte stream of length len to a word-length unique hash value
	size_t hashData(void* pData, size_t len);
	//! hash a null-terminated string to a word-length unique hash value
	uint64_t hashString64(const char* strVal);
	//! hash an arbitrary byte stream of length len to a word-length unique hash value
	uint64_t hashData64(void* pData, size_t len);
}

#endif // TDHASH_INCLUDED
