/******************************************************************************
Copyright (c) 2019 Teardrop Games

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

#include "Util/_String.h"

namespace Teardrop {
namespace Gfx {

class RenderTarget;
class Texture;
class IndexBuffer;
class VertexBuffer;

class RenderPass
{
public:
    RenderPass(String const& name = String::EMPTY);
    virtual ~RenderPass();

    virtual void attachInputBuffer(IndexBuffer* buffer) = 0;
    virtual void attachInputBuffer(VertexBuffer* buffer) = 0;
    virtual void attachInputTexture(Texture* texture) = 0;
    virtual void attachOutput(RenderTarget* renderTarget) = 0;

    String const& name() const;

protected:
    String mName;
};

} // namespace Gfx
} // namespace Teardrop