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

#include "PipelineVulkan.h"

#include "AllocatorsVulkan.h"
#include "RenderPassVulkan.h"

#include "Gfx/Connection.h"
#include "Gfx/Material.h"
#include "Gfx/MaterialExpression.h"
#include "Gfx/MaterialOutput.h"
#include "Util/_String.h"

#include "glslang/Include/intermediate.h"
#include "glslang/MachineIndependent/localintermediate.h"
#include "glslang/Public/ShaderLang.h"

#include <memory>
#include <unordered_set>

#include <iostream>

namespace {

VkFormat getVertexElementFormat(Teardrop::Gfx::VertexElement const& elem)
{
    switch (elem.mType) {
    case Teardrop::Gfx::VET_FLOAT:
        if (elem.mCount == 1) { return VK_FORMAT_R32_SFLOAT; }
        else if (elem.mCount == 2) { return VK_FORMAT_R32G32_SFLOAT; }
        else if (elem.mCount == 3) { return VK_FORMAT_R32G32B32_SFLOAT; }
        else if (elem.mCount == 4) { return VK_FORMAT_R32G32B32A32_SFLOAT; }
        break;
    }

    return VK_FORMAT_UNDEFINED;
}

void generateSourceCode(Teardrop::Gfx::Material* material, std::vector<std::string>& lines)
{
    material->sortExpressions();

    size_t nExpressions = material->expressionCount();
    Teardrop::Gfx::MaterialExpression** expressions = material->sortedExpressions();

    for (size_t i=0; i<nExpressions; ++i) {
        Teardrop::Gfx::MaterialExpression* expr = expressions[i];
        if (!expr) {
            continue;
        }

        char const* name = (char const*)expr->getDerivedClassDef()->getName();
        if (name) {
            lines.push_back(name);
        }
    }
}

} // namespace

namespace Teardrop {
namespace Gfx {
namespace Vulkan {

Pipeline::Pipeline(VkDevice device, PipelineType type, Gfx::RenderTarget* renderTarget)
    : mDevice(device)
    , mPipeline(VK_NULL_HANDLE)
    , mType(type)
{
    setRenderTarget(renderTarget);
}

Pipeline::~Pipeline()
{
    if (mPipeline == VK_NULL_HANDLE) {
        return;
    }

    vkDestroyPipeline(mDevice, mPipeline, getAllocationCallbacks());
}

VkPipeline Pipeline::pipeline() const
{
    return mPipeline;
}

void Pipeline::build()
{
    if (!mRenderTarget || !mMaterial) {
        return;
    }

    if (mType != Gfx::PIPELINE_GRAPHICS) {
        return;
    }

    // first, make a temporary render pass because Vulkan needs one
    std::unique_ptr<RenderPass> renderPass(new RenderPass(mDevice));
    renderPass->attachOutput(mRenderTarget);

    VkShaderModule vertexShaderModule = buildVertexShader();
    VkShaderModule fragmentShaderModule = buildFragmentShader();

    // first, vertex shader stage
    VkPipelineShaderStageCreateInfo vertexStage = {};
    vertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexStage.module = vertexShaderModule;
    vertexStage.pName = "main";

    // then, fragment shader stage
    VkPipelineShaderStageCreateInfo fragmentStage = {};
    fragmentStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentStage.module = fragmentShaderModule;
    fragmentStage.pName = "main";

    VkPipelineShaderStageCreateInfo stages[] = {
        vertexStage,
        fragmentStage,
    };
    uint32_t nStages = sizeof(stages) / sizeof(stages[0]);

    // describe the vertex data layout for the geometry that was supplied
    VkVertexInputBindingDescription* vertexBindingDescriptions = nullptr;
    VkVertexInputAttributeDescription* vertexAttributeDescriptions = nullptr;
    uint32_t bindingCount = 0;
    uint32_t attributeCount = 0;

    if (!mGeometryStreams.empty()) {
        size_t nLocations = 0;
        for (auto const& stream : mGeometryStreams) {
            bindingCount++;
            nLocations += stream.size();
        }

        attributeCount = (uint32_t)nLocations;

        if (nLocations) {
            vertexBindingDescriptions = (VkVertexInputBindingDescription*)alloca(sizeof(VkVertexInputBindingDescription) * mGeometryStreams.size());
            vertexAttributeDescriptions = (VkVertexInputAttributeDescription*)alloca(sizeof(VkVertexInputAttributeDescription) * nLocations);
        }

        // and then, finally, fill them out
        uint32_t binding = 0;
        for (auto const& stream : mGeometryStreams) {
            vertexBindingDescriptions->stride = (uint32_t)stream[binding].size();
            vertexBindingDescriptions->binding = binding;
            vertexBindingDescriptions->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            uint32_t location = 0;
            uint32_t offset = 0;
            for (auto const& element : stream) {
                vertexAttributeDescriptions[location].format = getVertexElementFormat(element);
                vertexAttributeDescriptions[location].binding = binding;
                vertexAttributeDescriptions[location].location = location;
                vertexAttributeDescriptions[location].offset = offset;

                offset += element.size();
                location++;
            }
        }
    }

    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.renderPass = renderPass->renderPass();
    info.subpass = 0;
    info.stageCount = nStages;
    info.pStages = stages;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (vertexBindingDescriptions && vertexAttributeDescriptions) {
        vertexInputInfo.pVertexBindingDescriptions = vertexBindingDescriptions;
        vertexInputInfo.vertexBindingDescriptionCount = bindingCount;
        vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions;
        vertexInputInfo.vertexAttributeDescriptionCount = attributeCount;
        info.pVertexInputState = &vertexInputInfo;
    }

    vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &info, getAllocationCallbacks(), &mPipeline);
}

VkShaderModule Pipeline::buildVertexShader()
{
    return VK_NULL_HANDLE;
}

VkShaderModule Pipeline::buildFragmentShader()
{
    // generate fragment shader source code
    std::vector<std::string> lines;
    generateSourceCode(mMaterial, lines);

    std::cout << "Generated source:" << std::endl;
    for (auto const& line : lines) {
        std::cout << "    " << line << std::endl;
    }

    return VK_NULL_HANDLE;
}

} // namespace Vulkan
} // namespace Gfx
} // namespace Teardrop
