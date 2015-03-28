/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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


