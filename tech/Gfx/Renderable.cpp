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

#include "Renderable.h"

#include "Mesh.h"

namespace Teardrop {
namespace Gfx {

Renderable::Renderable()
    : mMesh(nullptr)
{

}

Renderable::Renderable(Mesh* mesh, Pipeline** subMeshPipelines)
    : mMesh(mesh)
{
    size_t nSubmesh = mesh->submeshCount();
    mPipelines.reserve(nSubmesh);
    for (size_t i=0; i<nSubmesh; ++i) {
        mPipelines.push_back(subMeshPipelines[i]);
    }
}

Renderable::~Renderable()
{

}

void Renderable::setMesh(Mesh* mesh)
{
    mMesh = mesh;
}

void Renderable::setTransform(Transform const& transform)
{
    mTransform = transform;
}

void Renderable::addPipeline(Pipeline* submeshPipeline)
{
    mPipelines.push_back(submeshPipeline);
}

void Renderable::setPipeline(Pipeline* submeshPipeline, size_t index)
{
    if (index >= mPipelines.size()) {
        return;
    }

    mPipelines[index] = submeshPipeline;
}

Mesh* Renderable::mesh() const
{
    return mMesh;
}

Pipeline* Renderable::pipeline(size_t index) const
{
    if (index >= mPipelines.size()) {
        return nullptr;
    }

    return mPipelines[index];
}

Transform const& Renderable::transform() const
{
    return mTransform;
}

} // namespace Gfx
} // namespace Teardrop
