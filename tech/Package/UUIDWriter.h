/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef UUIDWRITER_H
#define UUIDWRITER_H

#include <cstdint>

namespace Teardrop
{
    class UUID;

    class UUIDWriter
    {
    public:
        UUIDWriter(const UUID& aUUID);
        virtual ~UUIDWriter();
        virtual uint32_t write() = 0;

    protected:
        const UUID& mUUID;
    };
}

#endif // UUIDWRITER_H

