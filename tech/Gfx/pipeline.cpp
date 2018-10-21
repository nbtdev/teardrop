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

#include "Gfx/Pipeline.h"

#include "Gfx/Material.h"

namespace Teardrop {
namespace Gfx {

Pipeline::Pipeline()
    : Pipeline(nullptr)
{
}

Pipeline::Pipeline(Material* material)
    : mMaterial(material)
    , mOwnedMaterial(nullptr)
    , mFragmentShader(nullptr)
    , mVertexShader(nullptr)
{
}

Pipeline::~Pipeline()
{
    if (mOwnedMaterial) {
        mOwnedMaterial->destroy();
    }

    delete mOwnedMaterial;
}

void Pipeline::setMaterial(Material* material)
{
    mMaterial = material;
    build();
}

Material* Pipeline::material()
{
    if (!mMaterial) {
        mOwnedMaterial = TD_NEW Material;
        mOwnedMaterial->initialize();
        mMaterial = mOwnedMaterial;
        build();
    }

    return mMaterial;
}

void Pipeline::beginGeometryStream()
{
    mGeometryStreams.push_back(GeometryStream());
}

void Pipeline::endGeometryStream()
{
    build();
}

void Pipeline::addVertexElement(VertexElement const& vertexElement)
{
    mGeometryStreams.back().push_back(vertexElement);
}

FragmentShader* Pipeline::fragmentShader()
{
    return nullptr;
}

VertexShader* Pipeline::vertexShader()
{
    return nullptr;
}

void Pipeline::build()
{
}

} // namespace Gfx
} // namespace Teardrop
