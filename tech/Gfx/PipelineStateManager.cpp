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

#include "Gfx/PipelineStateManager.h"

#include "Gfx/Pipeline.h"
#include "Gfx/Submesh.h"
#include "Gfx/Material.h"
#include "Gfx/ShaderManager.h"
#include "Gfx/VertexBuffer.h"
#include "Gfx/VertexElement.h"
#include "Util/Hash.h"

namespace {

using namespace Teardrop;
using namespace Gfx;

uint64_t hashSubmeshAndMaterial(Submesh* submesh, Material* material)
{
    assert(submesh);
    assert(material);

    if (!submesh && !material) {
        return 0;
    }

    // hash the material first, if necessary; this will give us a seed we
    // can use to begin hashing the geometry structure(s)
    uint64_t hash = material->hash();
    size_t nVB = submesh->vertexBufferCount();

    for (size_t i=0; i<nVB; ++i) {
        Gfx::VertexBuffer* vb = submesh->vertexBuffer(i);
        for (size_t j=0; j<vb->vertexElementCount(); ++j) {
            Gfx::VertexElement const* elem = vb->vertexElement(j);
            hash = hashData64(hash, elem, sizeof(*elem));
        }
    }

    return hash;
}

Pipeline* createNewPipeline(Submesh* submesh, Material* material)
{
    return nullptr;
}

} // namespace

template<> Teardrop::Gfx::PipelineStateManager* Teardrop::Singleton<Teardrop::Gfx::PipelineStateManager>::mInst = 0;

namespace Teardrop {
namespace Gfx {

PipelineStateManager::PipelineStateManager()
{
    assert(!mInst);
    mInst = this;
}

PipelineStateManager::~PipelineStateManager()
{
    for (auto pr : mPipelines) {
        delete pr.second;
    }
}

PipelineData PipelineStateManager::createOrFindPipelineState(Submesh* submesh, Material* material)
{
    PipelineData pipelineData;
    pipelineData.hash = hashSubmeshAndMaterial(submesh, material);

    Pipeline* pipeline = findPipelineState(pipelineData.hash);
    if (!pipeline) {
        // have to make a new one then
        pipeline = createNewPipeline(submesh, material);
    }

    pipelineData.pipeline = pipeline;

    return pipelineData;
}

Pipeline* PipelineStateManager::findPipelineState(uint64_t pipelineHash)
{
    auto it = mPipelines.find(pipelineHash);
    if (it != mPipelines.end()) {
        return it->second;
    }

    return nullptr;
}

} // namespace Gfx
} // namespace Teardrop
