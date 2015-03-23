/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
		// the first half (int and two shorts) of the UUID need to 
		// have their byte order swapped
		::UUID& u = (::UUID&)mUUID;
		u.Data1 = ntohl(u.Data1);
		u.Data2 = ntohs(u.Data2);
		u.Data3 = ntohs(u.Data3);

		return mStream.read(&u, sizeof(u));
	}

} // namespace Teardrop


