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

#include "Gfx/VertexElement.h"

#include <vector>

namespace Teardrop {
namespace Gfx {

class Material;
class FragmentShader;
class RenderTarget;
class VertexShader;

///
/// \brief Class describing the graphics pipeline needed to render a Submesh with
///        a given Material.
///
class Pipeline
{
public:
    Pipeline();
    virtual ~Pipeline();

    void setMaterial(Material* material);
    void setRenderTarget(RenderTarget* renderTarget);

    void beginGeometryStream();
    void endGeometryStream();
    void addVertexElement(VertexElement const& vertexElement);
    Material* material();

    RenderTarget* renderTarget() const;
    FragmentShader* fragmentShader() const;
    VertexShader* vertexShader() const;

    virtual void build() = 0;

    TD_DECLARE_ALLOCATOR();

protected:
    Material* mMaterial;
    Material* mOwnedMaterial;

    // references to platform/renderer-specific shader instances used by this pipeline
    FragmentShader* mFragmentShader;
    VertexShader* mVertexShader;

    RenderTarget* mRenderTarget;

    typedef std::vector<Gfx::VertexElement> GeometryStream;
    typedef std::vector<GeometryStream> GeometryStreams;
    GeometryStreams mGeometryStreams;
};

} // namespace Gfx
} // namespace Teardrop
