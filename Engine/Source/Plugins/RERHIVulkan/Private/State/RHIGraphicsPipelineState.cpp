////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/State/RHIGraphicsPipelineState.h"
#include "RERHIVulkan/Shader/RHIGraphicsProgramGLSL.h"
#include "RERHIVulkan/RenderTarget/RHIRenderPass.h"
#include "RERHIVulkan/RHIRootSignature.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

GraphicsPipelineState::GraphicsPipelineState(RHIDynamicRHI &vulkanRhi,
                                             const RERHI::GraphicsPipelineState &graphicsPipelineState, uint16_t id
                                             RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIGraphicsPipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(graphicsPipelineState.rootSignature),
  mGraphicsProgram(graphicsPipelineState.graphicsProgram),
  mRenderPass(graphicsPipelineState.renderPass),
  mVkPipeline(VK_NULL_HANDLE) {
// Add a reference to the referenced RHI resources
  mRootSignature->AddReference();
  mGraphicsProgram->AddReference();
  mRenderPass->AddReference();

// Our pipeline state needs to be independent of concrete render targets, so we're using dynamic viewport ("VK_DYNAMIC_STATE_VIEWPORT") and scissor ("VK_DYNAMIC_STATE_SCISSOR") states
  static constexpr uint32_t WIDTH = 42;
  static constexpr uint32_t HEIGHT = 42;

// Shaders
  GraphicsProgramGlsl *graphicsProgramGlsl = static_cast<GraphicsProgramGlsl *>(mGraphicsProgram);
  const bool hasMeshShader = (nullptr != graphicsProgramGlsl->getMeshShaderGlsl());
  uint32_t stageCount = 0;
  ::detail::VkPipelineShaderStageCreateInfos vkPipelineShaderStageCreateInfos;
  {
// Define helper macro
#define SHADER_STAGE(vkShaderStageFlagBits, shaderGlsl) \
          if (nullptr != shaderGlsl) \
          { \
            ::detail::addVkPipelineShaderStageCreateInfo(vkShaderStageFlagBits, shaderGlsl->getVkShaderModule(), vkPipelineShaderStageCreateInfos, stageCount); \
            ++stageCount; \
          }

// Shader stages
    SHADER_STAGE(VK_SHADER_STAGE_TASK_BIT_NV, graphicsProgramGlsl->getTaskShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_MESH_BIT_NV, graphicsProgramGlsl->getMeshShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_VERTEX_BIT, graphicsProgramGlsl->getVertexShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, graphicsProgramGlsl->getTessellationControlShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
                 graphicsProgramGlsl->getTessellationEvaluationShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_GEOMETRY_BIT, graphicsProgramGlsl->getGeometryShaderGlsl())
    SHADER_STAGE(VK_SHADER_STAGE_FRAGMENT_BIT, graphicsProgramGlsl->getFragmentShaderGlsl())

// Undefine helper macro
#undef SHADER_STAGE
  }

// Vertex attributes
  const uint32_t numberOfAttributes = graphicsPipelineState.vertexAttributes.numberOfAttributes;
  std::vector<VkVertexInputBindingDescription> vkVertexInputBindingDescriptions;
  std::vector<VkVertexInputAttributeDescription> vkVertexInputAttributeDescriptions(numberOfAttributes);
  for (uint32_t attribute = 0; attribute < numberOfAttributes; ++attribute) {
    const RERHI::VertexAttribute *attributes = &graphicsPipelineState.vertexAttributes.attributes[attribute];
    const uint32_t inputSlot = attributes->inputSlot;

    { // Map to Vulkan vertex input binding description
      if (vkVertexInputBindingDescriptions.size() <= inputSlot) {
        vkVertexInputBindingDescriptions.resize(inputSlot + 1);
      }
      VkVertexInputBindingDescription &vkVertexInputBindingDescription = vkVertexInputBindingDescriptions[inputSlot];
      vkVertexInputBindingDescription.binding = inputSlot;
      vkVertexInputBindingDescription.stride = attributes->strideInBytes;
      vkVertexInputBindingDescription.inputRate = (attributes->instancesPerElement > 0) ? VK_VERTEX_INPUT_RATE_INSTANCE
                                                                                        : VK_VERTEX_INPUT_RATE_VERTEX;
    }

    { // Map to Vulkan vertex input attribute description
      VkVertexInputAttributeDescription &vkVertexInputAttributeDescription = vkVertexInputAttributeDescriptions[attribute];
      vkVertexInputAttributeDescription.location = attribute;
      vkVertexInputAttributeDescription.binding = inputSlot;
      vkVertexInputAttributeDescription.format = Mapping::getVulkanFormat(attributes->vertexAttributeFormat);
      vkVertexInputAttributeDescription.offset = attributes->alignedByteOffset;
    }
  }

// Create the Vulkan graphics pipeline
// TODO(naetherm) Implement the rest of the value mappings
  const VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,      // sType (VkStructureType)
      nullptr,                              // pNext (const void*)
      0,                                  // flags (VkPipelineVertexInputStateCreateFlags)
      static_cast<uint32_t>(vkVertexInputBindingDescriptions.size()),    // vertexBindingDescriptionCount (uint32_t)
      vkVertexInputBindingDescriptions.data(),              // pVertexBindingDescriptions (const VkVertexInputBindingDescription*)
      static_cast<uint32_t>(vkVertexInputAttributeDescriptions.size()),  // vertexAttributeDescriptionCount (uint32_t)
      vkVertexInputAttributeDescriptions.data()              // pVertexAttributeDescriptions (const VkVertexInputAttributeDescription*)
    };
  const VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,    // sType (VkStructureType)
      nullptr,                              // pNext (const void*)
      0,                                  // flags (VkPipelineInputAssemblyStateCreateFlags)
      Mapping::getVulkanType(graphicsPipelineState.primitiveTopology),  // topology (VkPrimitiveTopology)
      VK_FALSE                              // primitiveRestartEnable (VkBool32)
    };
  const VkViewport vkViewport =
    {
      0.0f,            // x (float)
      0.0f,            // y (float)
      static_cast<float>(WIDTH),  // width (float)
      static_cast<float>(HEIGHT),  // height (float)
      0.0f,            // minDepth (float)
      1.0f            // maxDepth (float)
    };
  const VkRect2D scissorVkRect2D =
    {
      { // offset (VkOffset2D)
        0,  // x (int32_t)
        0  // y (int32_t)
      },
      { // extent (VkExtent2D)
        WIDTH,  // width (uint32_t)
        HEIGHT  // height (uint32_t)
      }
    };
  const VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,                                                                                  // sType (VkStructureType)
      nullptr,                                                                                                          // pNext (const void*)
      0,                                                                                                              // flags (VkPipelineTessellationStateCreateFlags)
      (graphicsPipelineState.primitiveTopology >= RERHI::PrimitiveTopology::PATCH_LIST_1) ?
      static_cast<uint32_t>(graphicsPipelineState.primitiveTopology) -
      static_cast<uint32_t>(RERHI::PrimitiveTopology::PATCH_LIST_1) + 1 : 1  // patchControlPoints (uint32_t)
    };
  const VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                        // pNext (const void*)
      0,                            // flags (VkPipelineViewportStateCreateFlags)
      1,                            // viewportCount (uint32_t)
      &vkViewport,                      // pViewports (const VkViewport*)
      1,                            // scissorCount (uint32_t)
      &scissorVkRect2D                    // pScissors (const VkRect2D*)
    };
  const float depthBias = static_cast<float>(graphicsPipelineState.rasterizerState.depthBias);
  const float depthBiasClamp = graphicsPipelineState.rasterizerState.depthBiasClamp;
  const float slopeScaledDepthBias = graphicsPipelineState.rasterizerState.slopeScaledDepthBias;
  const VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,                                    // sType (VkStructureType)
      nullptr,                                                            // pNext (const void*)
      0,                                                                // flags (VkPipelineRasterizationStateCreateFlags)
      static_cast<VkBool32>(graphicsPipelineState.rasterizerState.depthClipEnable),                          // depthClampEnable (VkBool32)
      VK_FALSE,                                                            // rasterizerDiscardEnable (VkBool32)
      (RERHI::FillMode::WIREFRAME == graphicsPipelineState.rasterizerState.fillMode) ? VK_POLYGON_MODE_LINE
                                                                                     : VK_POLYGON_MODE_FILL,    // polygonMode (VkPolygonMode)
      static_cast<VkCullModeFlags>(static_cast<int>(graphicsPipelineState.rasterizerState.cullMode) -
                                   1),                // cullMode (VkCullModeFlags)
      (1 == graphicsPipelineState.rasterizerState.frontCounterClockwise) ? VK_FRONT_FACE_COUNTER_CLOCKWISE
                                                                         : VK_FRONT_FACE_CLOCKWISE,  // frontFace (VkFrontFace)
      static_cast<VkBool32>(0.0f != depthBias || 0.0f != depthBiasClamp ||
                            0.0f != slopeScaledDepthBias),                // depthBiasEnable (VkBool32)
      depthBias,                                                            // depthBiasConstantFactor (float)
      depthBiasClamp,                                                          // depthBiasClamp (float)
      slopeScaledDepthBias,                                                      // depthBiasSlopeFactor (float)
      1.0f                                                              // lineWidth (float)
    };
  const RenderPass *renderPass = static_cast<const RenderPass *>(mRenderPass);
  const VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                          // pNext (const void*)
      0,                              // flags (VkPipelineMultisampleStateCreateFlags)
      renderPass->getVkSampleCountFlagBits(),            // rasterizationSamples (VkSampleCountFlagBits)
      VK_FALSE,                          // sampleShadingEnable (VkBool32)
      0.0f,                            // minSampleShading (float)
      nullptr,                          // pSampleMask (const VkSampleMask*)
      VK_FALSE,                          // alphaToCoverageEnable (VkBool32)
      VK_FALSE                          // alphaToOneEnable (VkBool32)
    };
  const VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,                          // sType (VkStructureType)
      nullptr,                                                  // pNext (const void*)
      0,                                                      // flags (VkPipelineDepthStencilStateCreateFlags)
      static_cast<VkBool32>(0 !=
                            graphicsPipelineState.depthStencilState.depthEnable),              // depthTestEnable (VkBool32)
      static_cast<VkBool32>(RERHI::DepthWriteMask::ALL ==
                            graphicsPipelineState.depthStencilState.depthWriteMask),  // depthWriteEnable (VkBool32)
      Mapping::getVulkanComparisonFunc(
        graphicsPipelineState.depthStencilState.depthFunc),            // depthCompareOp (VkCompareOp)
      VK_FALSE,                                                  // depthBoundsTestEnable (VkBool32)
      static_cast<VkBool32>(0 !=
                            graphicsPipelineState.depthStencilState.stencilEnable),              // stencilTestEnable (VkBool32)
      { // front (VkStencilOpState)
        VK_STENCIL_OP_KEEP,                                            // failOp (VkStencilOp)
        VK_STENCIL_OP_KEEP,                                            // passOp (VkStencilOp)
        VK_STENCIL_OP_KEEP,                                            // depthFailOp (VkStencilOp)
        VK_COMPARE_OP_NEVER,                                          // compareOp (VkCompareOp)
        0,                                                    // compareMask (uint32_t)
        0,                                                    // writeMask (uint32_t)
        0                                                    // reference (uint32_t)
      },
      { // back (VkStencilOpState)
        VK_STENCIL_OP_KEEP,                                            // failOp (VkStencilOp)
        VK_STENCIL_OP_KEEP,                                            // passOp (VkStencilOp)
        VK_STENCIL_OP_KEEP,                                            // depthFailOp (VkStencilOp)
        VK_COMPARE_OP_NEVER,                                          // compareOp (VkCompareOp)
        0,                                                    // compareMask (uint32_t)
        0,                                                    // writeMask (uint32_t)
        0                                                    // reference (uint32_t)
      },
      0.0f,                                                    // minDepthBounds (float)
      1.0f                                                    // maxDepthBounds (float)
    };
  const uint32_t numberOfColorAttachments = renderPass->getNumberOfColorAttachments();
  RHI_ASSERT(numberOfColorAttachments < 8, "Invalid number of Vulkan color attachments")
  RHI_ASSERT(numberOfColorAttachments == graphicsPipelineState.numberOfRenderTargets,
             "Invalid number of Vulkan color attachments")
  std::array<VkPipelineColorBlendAttachmentState, 8> vkPipelineColorBlendAttachmentStates;
  for (uint8_t i = 0; i < numberOfColorAttachments; ++i) {
    const RERHI::RenderTargetBlendDesc &renderTargetBlendDesc = graphicsPipelineState.blendState.renderTarget[i];
    VkPipelineColorBlendAttachmentState &vkPipelineColorBlendAttachmentState = vkPipelineColorBlendAttachmentStates[i];
    vkPipelineColorBlendAttachmentState.blendEnable = static_cast<VkBool32>(renderTargetBlendDesc.blendEnable);        // blendEnable (VkBool32)
    vkPipelineColorBlendAttachmentState.srcColorBlendFactor = Mapping::getVulkanBlendFactor(
      renderTargetBlendDesc.srcBlend);    // srcColorBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.dstColorBlendFactor = Mapping::getVulkanBlendFactor(
      renderTargetBlendDesc.destBlend);    // dstColorBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.colorBlendOp = Mapping::getVulkanBlendOp(
      renderTargetBlendDesc.blendOp);        // colorBlendOp (VkBlendOp)
    vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = Mapping::getVulkanBlendFactor(
      renderTargetBlendDesc.srcBlendAlpha);  // srcAlphaBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = Mapping::getVulkanBlendFactor(
      renderTargetBlendDesc.destBlendAlpha);  // dstAlphaBlendFactor (VkBlendFactor)
    vkPipelineColorBlendAttachmentState.alphaBlendOp = Mapping::getVulkanBlendOp(
      renderTargetBlendDesc.blendOpAlpha);    // alphaBlendOp (VkBlendOp)
    vkPipelineColorBlendAttachmentState.colorWriteMask = renderTargetBlendDesc.renderTargetWriteMask;              // colorWriteMask (VkColorComponentFlags)
  }
  const VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                          // pNext (const void*)
      0,                              // flags (VkPipelineColorBlendStateCreateFlags)
      VK_FALSE,                          // logicOpEnable (VkBool32)
      VK_LOGIC_OP_COPY,                      // logicOp (VkLogicOp)
      numberOfColorAttachments,                  // attachmentCount (uint32_t)
      vkPipelineColorBlendAttachmentStates.data(),        // pAttachments (const VkPipelineColorBlendAttachmentState*)
      {0.0f, 0.0f, 0.0f, 0.0f}                  // blendConstants[4] (float)
    };
  static constexpr std::array<VkDynamicState, 2> vkDynamicStates =
    {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };
  static const VkPipelineDynamicStateCreateInfo vkPipelineDynamicStateCreateInfo =
    {
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                        // pNext (const void*)
      0,                            // flags (VkPipelineDynamicStateCreateFlags)
      static_cast<uint32_t>(vkDynamicStates.size()),      // dynamicStateCount (uint32_t)
      vkDynamicStates.data()                  // pDynamicStates (const VkDynamicState*)
    };
  const VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo =
    {
      VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,          // sType (VkStructureType)
      nullptr,                              // pNext (const void*)
      0,                                  // flags (VkPipelineCreateFlags)
      stageCount,                              // stageCount (uint32_t)
      vkPipelineShaderStageCreateInfos.data(),              // pStages (const VkPipelineShaderStageCreateInfo*)
      &vkPipelineVertexInputStateCreateInfo,                // pVertexInputState (const VkPipelineVertexInputStateCreateInfo*)
      hasMeshShader ? nullptr
                    : &vkPipelineInputAssemblyStateCreateInfo,  // pInputAssemblyState (const VkPipelineInputAssemblyStateCreateInfo*)
      hasMeshShader ? nullptr
                    : &vkPipelineTessellationStateCreateInfo,  // pTessellationState (const VkPipelineTessellationStateCreateInfo*)
      &vkPipelineViewportStateCreateInfo,                  // pViewportState (const VkPipelineViewportStateCreateInfo*)
      &vkPipelineRasterizationStateCreateInfo,              // pRasterizationState (const VkPipelineRasterizationStateCreateInfo*)
      &vkPipelineMultisampleStateCreateInfo,                // pMultisampleState (const VkPipelineMultisampleStateCreateInfo*)
      &vkPipelineDepthStencilStateCreateInfo,                // pDepthStencilState (const VkPipelineDepthStencilStateCreateInfo*)
      &vkPipelineColorBlendStateCreateInfo,                // pColorBlendState (const VkPipelineColorBlendStateCreateInfo*)
      &vkPipelineDynamicStateCreateInfo,                  // pDynamicState (const VkPipelineDynamicStateCreateInfo*)
      static_cast<RootSignature *>(mRootSignature)->getVkPipelineLayout(),  // layout (VkPipelineLayout)
      renderPass->getVkRenderPass(),                    // renderPass (VkRenderPass)
      0,                                  // subpass (uint32_t)
      VK_NULL_HANDLE,                            // basePipelineHandle (VkPipeline)
      0                                  // basePipelineIndex (int32_t)
    };
  if (vkCreateGraphicsPipelines(vulkanRhi.getVulkanContext().getVkDevice(), VK_NULL_HANDLE, 1,
                                &vkGraphicsPipelineCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkPipeline) ==
      VK_SUCCESS) {
// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
              {
                RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Graphics PSO", 15)	// 15 = "Graphics PSO: " including terminating zero
                Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (uint64_t)mVkPipeline, detailedDebugName);
              }
#endif
  } else {
    RE_LOG(Critical, "Failed to create the Vulkan graphics pipeline")
  }
}

/**
*  @brief
*    Destructor
*/
GraphicsPipelineState::~GraphicsPipelineState() {
// Destroy the Vulkan graphics pipeline
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  if (VK_NULL_HANDLE != mVkPipeline) {
    vkDestroyPipeline(vulkanRhi.getVulkanContext().getVkDevice(), mVkPipeline, vulkanRhi.getVkAllocationCallbacks());
  }

// Release referenced RHI resources
  mRootSignature->Release();
  mGraphicsProgram->Release();
  mRenderPass->Release();

// Free the unique compact graphics pipeline state ID
  vulkanRhi.GraphicsPipelineStateMakeId.destroyID(getId());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
