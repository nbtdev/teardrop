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

#include "Package/UUIDStreamReader.h"
#include "Stream/Stream.h"
#include "Util/UUID.h"
#include <Windows.h>

namespace Teardrop
{

	UUIDStreamReader::UUIDStreamReader(Stream& aStream, UUID& aUUID)
		: UUIDReader(aUUID)
		, mStream(aStream)
	{

	}

	UUIDStreamReader::~UUIDStreamReader()
	{

	}

	uint32_t
		UUIDStreamReader::read()
	{
		uint32_t nBytes = mStream.read(&mUUID, sizeof(mUUID));

		// the first half (int and two shorts) of the UUID need to 
		// have their byte order swapped
		::UUID& u = (::UUID&)mUUID;
		u.Data1 = ntohl(u.Data1);
		u.Data2 = ntohs(u.Data2);
		u.Data3 = ntohs(u.Data3);

		return nBytes;
	}

} // namespace Teardrop
