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

#pragma once

#include "Game/ZoneObject.h"
#include "Gfx/RenderableProvider.h"
#include "Memory/Allocators.h"

namespace Teardrop {

class TerrainZone;

namespace Gfx {
    class Texture;
}

class TerrainPatch :
    public ZoneObject
{
    TerrainZone* m_pOwner;

public:
    TD_CLASS(TerrainPatch, ZoneObject);

    TerrainPatch();
    ~TerrainPatch();

    bool initialize();
    bool destroy();

    // we don't want these serialized since they are dynamically created
    bool shouldSerialize() const { return false; }

    void notifyPropertyChangedLocal(const Reflection::PropertyDef*);

    TD_DECLARE_ALLOCATOR();
};

} // namespace Teardrop

