/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef UUIDREADER_H
#define UUIDREADER_H

#include <cstdint>

namespace Teardrop
{
    struct UUID;

    class UUIDReader
    {
    public:
        UUIDReader(UUID& aUUID);
        virtual ~UUIDReader();

        virtual uint32_t read() = 0;

    protected:
        UUID& mUUID;
    };
}

#endif // UUIDREADER_H

