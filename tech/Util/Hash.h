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

#if !defined(TDHASH_INCLUDED)
#define TDHASH_INCLUDED

#include <cstddef>
#include <cstdint>

namespace Teardrop
{
	//! hash a null-terminated string to a word-length unique hash value
	size_t hashString(const char* strVal);
	//! hash an arbitrary byte stream of length len to a word-length unique hash value
    size_t hashData(void const* pData, size_t len);
    //! hash a null-terminated string to a word-length unique hash value
    uint64_t hashString64(const char* strVal);
    //! hash a null-terminated string to a word-length unique hash value, with a starting hash value
    uint64_t hashString64(uint64_t seed, const char* strVal);
    //! hash an arbitrary byte stream of length len to a word-length unique hash value
    uint64_t hashData64(void const* pData, size_t len);
    //! hash an arbitrary byte stream of length len to a word-length unique hash value, with
    //! a starting hash value
    uint64_t hashData64(uint64_t seed, void const* pData, size_t len);
}

#endif // TDHASH_INCLUDED
