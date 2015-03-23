/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#include "Package/UUIDStreamWriter.h"
#include "Stream/Stream.h"
#include "Util/UUID.h"

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
    return mStream.write(&mUUID, sizeof(mUUID));
}

} // namespace Teardrop


