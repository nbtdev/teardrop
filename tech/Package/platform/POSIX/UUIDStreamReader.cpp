/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package/UUIDStreamReader.h"
#include "Stream/Stream.h"
#include "Util/UUID.h"

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
    return mStream.read(&mUUID, sizeof(mUUID));
}

} // namespace Teardrop


