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

#include "SceneRenderStep.h"

#include "Component_Render.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Zone.h"
#include "ZoneObject.h"
#include "Gfx/Camera.h"
#include "Gfx/CommandBuffer.h"
#include "Gfx/CommandQueue.h"
#include "Gfx/Material.h"
#include "Gfx/Mesh.h"
#include "Gfx/Renderable.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderPass.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/RenderQueue.h"
#include "Gfx/Submesh.h"
#include "Reflection/Reflection.h"
#include "Reflection/ClassDef.h"

namespace {

using namespace Teardrop;
using namespace Gfx;

void renderSubmesh(CommandBuffer* cmdBuf, Submesh* submesh)
{

}

} // namespace

namespace Teardrop {

SceneRenderStep::SceneRenderStep()
    : mVP(nullptr)
{
}

SceneRenderStep::~SceneRenderStep()
{
}

void SceneRenderStep::render(const VisibleObjects& objects, Context* context, Scene* pScene)
{
    assert(context);

//    mCamera->setAspect(renderTarget->aspect());

    Gfx::Viewport* vp = mVP;
    if (!vp) {
        // use the default viewport on the render target
        vp = context->renderTarget()->viewport();
    }

    assert(vp);
    if (!vp) {
        return;
    }

    // if we have not yet created a render pass, do so now
    if (!mRenderPass) {
        mRenderPass = context->renderer()->createRenderPass("SceneRenderPass");
        mRenderPass->attachOutput(context->renderTarget());
        mRenderPass->setClearColor(0.f, 0.f, 0.f, 1.f);
    }

    // then update the main scene

	// first, find out if there is anything to render
	// get the visible objects from the scene; if any, pass over to the renderer
    Gfx::RenderQueue renderQueue;

    if (objects.size()) {
		// go through the list and make another of things we need to render
		for(ZoneObjects::const_iterator it = objects.begin();
			it != objects.end(); ++it)
		{
			ZoneObject* pObj = const_cast<ZoneObject*>(*it);
			ZoneObject::ComponentList renderables;
			pObj->findComponents(RenderComponent::getClassDef(), renderables);
            for (auto r : renderables)
			{
				RenderComponent* pRenderable = 
                    static_cast<RenderComponent*>(r);

				// update render component's affecting light list if needed. this
				// call will internally short-circuit if the object is not lit or
				// the light list doesn't need rebuilt
				pRenderable->updateLightList(pScene);
                pRenderable->queueForRendering(&renderQueue);
			}
		}
	}

    // render the main scene; start with a one-shot command buffer (we don't want to keep them
    // around because the contents of the scene likely are constantly changing, so there is no
    // point in pre-recording anything)
    std::unique_ptr<Gfx::CommandBuffer> commandBuffer(context->renderer()->createCommandBuffer(false));
    commandBuffer->beginRecording();
    commandBuffer->beginRenderPass(mRenderPass.get(), context->renderTarget());

    Gfx::Pipeline* currentPipeline = nullptr;

    // first-pass, naive approach -- render everything in unsorted order
    // TODO: at least sort by Pipeline
    size_t nRenderables = renderQueue.renderableCount();
    for (size_t i=0; i<nRenderables; ++i) {
        Gfx::Renderable* renderable = renderQueue.renderable(i);
        size_t nSubmesh = renderable->mesh()->submeshCount();
        for (size_t s=0; s<nSubmesh; ++s) {
            Gfx::Submesh* submesh = renderable->mesh()->submesh((int)s);
            Gfx::Pipeline* pipeline = renderable->pipeline(s);

            if (!submesh || !pipeline) {
                continue;
            }

            if (currentPipeline != pipeline) {
                currentPipeline = pipeline;
                commandBuffer->bindPipeline(currentPipeline);
            }

            renderSubmesh(commandBuffer.get(), submesh);
        }
    }

    commandBuffer->endRenderPass();
    commandBuffer->endRecording();

    std::unique_ptr<Gfx::CommandQueueSubmission> submission = context->graphicsQueue()->createSubmission();
    submission->addCommandBuffer(commandBuffer.get());
    context->graphicsQueue()->submit(submission.get(), 1, nullptr);
}

} // namespace Teardrop
