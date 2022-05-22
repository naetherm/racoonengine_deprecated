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
#include "RERHIVulkan/Texture/RHITexture1D.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

Texture1D::Texture1D(RHIDynamicRHI &vulkanRhi, RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat,
                     const void *data, RECore::uint32 textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHITexture1D(vulkanRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVkImage(VK_NULL_HANDLE),
  mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
  mVkDeviceMemory(VK_NULL_HANDLE),
  mVkImageView(VK_NULL_HANDLE) {
  Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_1D, VK_IMAGE_VIEW_TYPE_1D, {width, 1, 1}, textureFormat, data,
                               textureFlags, 1, mVkImage, mVkDeviceMemory, mVkImageView);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
          const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (RECore::uint64)mVkImage, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (RECore::uint64)mVkDeviceMemory, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (RECore::uint64)mVkImageView, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
Texture1D::~Texture1D() {
  Helper::destroyAndFreeVkImage(static_cast<RHIDynamicRHI &>(getRhi()), mVkImage, mVkDeviceMemory, mVkImageView);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
