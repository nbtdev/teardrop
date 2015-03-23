/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#ifndef UUIDSTREAMWRITER_H
#define UUIDSTREAMWRITER_H

#include "UUIDWriter.h"

namespace Teardrop
{
    class Stream;

    class UUIDStreamWriter : public UUIDWriter
    {
    public:
        UUIDStreamWriter(Stream& aStream, const UUID& aUUID);
        ~UUIDStreamWriter();

        uint32_t write();

    protected:
        Stream& mStream;
    };
}

#endif // UUIDSTREAMWRITER_H
