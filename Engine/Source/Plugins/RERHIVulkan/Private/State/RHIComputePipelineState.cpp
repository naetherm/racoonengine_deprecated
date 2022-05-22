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
#include "RERHIVulkan/State/RHIComputePipelineState.h"
#include "RERHIVulkan/Shader/RHIComputeShaderGLSL.h"
#include "RERHIVulkan/RHIRootSignature.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

ComputePipelineState::ComputePipelineState(RHIDynamicRHI &vulkanRhi, RERHI::RHIRootSignature &rootSignature,
                                           RERHI::RHIComputeShader &computeShader, RECore::uint16 id
                                           RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIComputePipelineState(vulkanRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(rootSignature),
  mComputeShader(computeShader),
  mVkPipeline(VK_NULL_HANDLE) {
  // Add a reference to the given root signature and compute shader
  rootSignature.AddReference();
  computeShader.AddReference();

  // Create the Vulkan compute pipeline
  const VkComputePipelineCreateInfo vkComputePipelineCreateInfo =
    {
      VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,              // sType (VkStructureType)
      nullptr,                                // pNext (const void*)
      0,                                    // flags (VkPipelineCreateFlags)
      {                                    // stage (VkPipelineShaderStageCreateInfo)
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // sType (VkStructureType)
        nullptr,                              // pNext (const void*)
        0,                                  // flags (VkPipelineShaderStageCreateFlags)
        VK_SHADER_STAGE_COMPUTE_BIT,                    // stage (VkShaderStageFlagBits)
        static_cast<ComputeShaderGlsl &>(computeShader).getVkShaderModule(),  // module (VkShaderModule)
        "main",                                // pName (const char*)
        nullptr                                // pSpecializationInfo (const VkSpecializationInfo*)
      },
      static_cast<RootSignature &>(rootSignature).getVkPipelineLayout(),    // layout (VkPipelineLayout)
      VK_NULL_HANDLE,                              // basePipelineHandle (VkPipeline)
      0                                    // basePipelineIndex (int32_t)
    };
  if (vkCreateComputePipelines(vulkanRhi.getVulkanContext().getVkDevice(), VK_NULL_HANDLE, 1,
                               &vkComputePipelineCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &mVkPipeline) ==
      VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Compute PSO", 14)	// 14 = "Compute PSO: " including terminating zero
            Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, (RECore::uint64)mVkPipeline, detailedDebugName);
          }
#endif
  } else {
    RE_LOG(Critical, "Failed to create the Vulkan compute pipeline")
  }
}

/**
*  @brief
*    Destructor
*/
ComputePipelineState::~ComputePipelineState() {
// Destroy the Vulkan compute pipeline
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  if (VK_NULL_HANDLE != mVkPipeline) {
    vkDestroyPipeline(vulkanRhi.getVulkanContext().getVkDevice(), mVkPipeline, vulkanRhi.getVkAllocationCallbacks());
  }

// Release the root signature and compute shader reference
  mRootSignature.Release();
  mComputeShader.Release();

// Free the unique compact compute pipeline state ID
  vulkanRhi.ComputePipelineStateMakeId.destroyID(getId());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
