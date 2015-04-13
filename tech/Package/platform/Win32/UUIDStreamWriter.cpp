/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#include "Package/UUIDStreamWriter.h"
#include "Stream/Stream.h"
#include "Util/UUID.h"
#include <Windows.h>

namespace Teardrop
{

	UUIDStreamWriter::UUIDStreamWriter(Stream& aStream, const UUID& aUUID)
		: UUIDWriter(aUUID)
		, mStream(aStream)
	{

	}

	UUIDStreamWriter::~UUIDStreamWriter()
	{

	}

	uint32_t
		UUIDStreamWriter::write()
	{
		UUID tmp = mUUID;

		// the first half (int and two shorts) of the UUID need to 
		// have their byte order swapped
		::UUID& u = (::UUID&)tmp;
		u.Data1 = htonl(u.Data1);
		u.Data2 = htons(u.Data2);
		u.Data3 = htons(u.Data3);

		return mStream.write(&u, sizeof(u));
	}

} // namespace Teardrop


