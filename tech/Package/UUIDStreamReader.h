/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef UUIDSTREAMREADER_H
#define UUIDSTREAMREADER_H

#include "UUIDReader.h"

namespace Teardrop
{
    class Stream;

    class UUIDStreamReader : public UUIDReader
    {
    public:
        UUIDStreamReader(Stream& aStream, UUID& aUUID);
        ~UUIDStreamReader();

        uint32_t read();

    protected:
        Stream& mStream;
    };
}

#endif // UUIDSTREAMREADER_H
