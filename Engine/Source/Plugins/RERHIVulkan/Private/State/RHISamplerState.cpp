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
#include "RERHIVulkan/State/RHISamplerState.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

SamplerState::SamplerState(RHIDynamicRHI &vulkanRhi, const RERHI::SamplerState &samplerState
                           RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHISamplerState(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVkSampler(VK_NULL_HANDLE) {
  // Sanity checks
  RHI_ASSERT(samplerState.filter != RERHI::FilterMode::UNKNOWN, "Vulkan filter mode must not be unknown")
  RHI_ASSERT(samplerState.maxAnisotropy <= vulkanRhi.getCapabilities().maximumAnisotropy,
             "Maximum Vulkan anisotropy value violated")

  // TODO(naetherm) Map "RERHI::SamplerState" to VkSamplerCreateInfo
  const bool anisotropyEnable = (RERHI::FilterMode::ANISOTROPIC == samplerState.filter ||
                                 RERHI::FilterMode::COMPARISON_ANISOTROPIC == samplerState.filter);
  const VkSamplerCreateInfo vkSamplerCreateInfo =
    {
      VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,                      // sType (VkStructureType)
      nullptr,                                    // pNext (const void*)
      0,                                        // flags (VkSamplerCreateFlags)
      Mapping::getVulkanMagFilterMode(vulkanRhi.getContext(), samplerState.filter),  // magFilter (VkFilter)
      Mapping::getVulkanMinFilterMode(vulkanRhi.getContext(), samplerState.filter),  // minFilter (VkFilter)
      Mapping::getVulkanMipmapMode(vulkanRhi.getContext(), samplerState.filter),    // mipmapMode (VkSamplerMipmapMode)
      Mapping::getVulkanTextureAddressMode(samplerState.addressU),          // addressModeU (VkSamplerAddressMode)
      Mapping::getVulkanTextureAddressMode(samplerState.addressV),          // addressModeV (VkSamplerAddressMode)
      Mapping::getVulkanTextureAddressMode(samplerState.addressW),          // addressModeW (VkSamplerAddressMode)
      samplerState.mipLodBias,                            // mipLodBias (float)
      static_cast<VkBool32>(anisotropyEnable),                    // anisotropyEnable (VkBool32)
      static_cast<float>(samplerState.maxAnisotropy),                  // maxAnisotropy (float)
      VK_FALSE,                                    // compareEnable (VkBool32)
      VK_COMPARE_OP_ALWAYS,                              // compareOp (VkCompareOp)
      samplerState.minLod,                              // minLod (float)
      samplerState.maxLod,                              // maxLod (float)
      VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,                    // borderColor (VkBorderColor)
      VK_FALSE                                    // unnormalizedCoordinates (VkBool32)
    };
  if (vkCreateSampler(vulkanRhi.getVulkanContext().getVkDevice(), &vkSamplerCreateInfo,
                      vulkanRhi.getVkAllocationCallbacks(), &mVkSampler) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
            Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT, (RECore::uint64)mVkSampler, detailedDebugName);
          }
#endif
  } else {
    RE_LOG(Critical, "Failed to create Vulkan sampler instance")
  }
}

/**
*  @brief
*    Destructor
*/
SamplerState::~SamplerState() {
  if (VK_NULL_HANDLE != mVkSampler) {
    const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    vkDestroySampler(vulkanRhi.getVulkanContext().getVkDevice(), mVkSampler, vulkanRhi.getVkAllocationCallbacks());
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
