/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/Texture/RHITexture2D.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

Texture2D::Texture2D(RHIDynamicRHI &vulkanRhi, RECore::uint32 width, RECore::uint32 height,
                     RERHI::TextureFormat::Enum textureFormat, const void *data, RECore::uint32 textureFlags,
                     RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHITexture2D(vulkanRhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVrVulkanTextureData{},
  mVkImageLayout(Helper::getVkImageLayoutByTextureFlags(textureFlags)),
  mVkDeviceMemory(VK_NULL_HANDLE),
  mVkImageView(VK_NULL_HANDLE) {
  mVrVulkanTextureData.m_nFormat = Helper::createAndFillVkImage(vulkanRhi, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D,
                                                                {width, height, 1}, textureFormat, data, textureFlags,
                                                                numberOfMultisamples, mVrVulkanTextureData.m_nImage,
                                                                mVkDeviceMemory, mVkImageView);

  // Fill the rest of the "VRVulkanTextureData_t"-structure
  const VulkanContext &vulkanContext = vulkanRhi.getVulkanContext();
  const VulkanRuntimeLinking &vulkanRuntimeLinking = vulkanRhi.getVulkanRuntimeLinking();
  // m_nImage (VkImage) was set by "RHIDynamicRHI::Helper::createAndFillVkImage()" above
  mVrVulkanTextureData.m_pDevice = vulkanContext.getVkDevice();          // m_pDevice (VkDevice)
  mVrVulkanTextureData.m_pPhysicalDevice = vulkanContext.getVkPhysicalDevice();      // m_pPhysicalDevice (VkPhysicalDevice)
  mVrVulkanTextureData.m_pInstance = vulkanRuntimeLinking.getVkInstance();    // m_pInstance (VkInstance)
  mVrVulkanTextureData.m_pQueue = vulkanContext.getGraphicsVkQueue();      // m_pQueue (VkQueue)
  mVrVulkanTextureData.m_nQueueFamilyIndex = vulkanContext.getGraphicsQueueFamilyIndex();  // m_nQueueFamilyIndex (RECore::uint32)
  mVrVulkanTextureData.m_nWidth = width;                    // m_nWidth (RECore::uint32)
  mVrVulkanTextureData.m_nHeight = height;                    // m_nHeight (RECore::uint32)
  // m_nFormat (VkFormat)  was set by "RHIDynamicRHI::Helper::createAndFillVkImage()" above
  mVrVulkanTextureData.m_nSampleCount = numberOfMultisamples;            // m_nSampleCount (RECore::uint32)

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
          const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT, (RECore::uint64)mVrVulkanTextureData.m_nImage, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (RECore::uint64)mVkDeviceMemory, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (RECore::uint64)mVkImageView, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
Texture2D::~Texture2D() {
  Helper::destroyAndFreeVkImage(static_cast<RHIDynamicRHI &>(getRhi()), mVrVulkanTextureData.m_nImage, mVkDeviceMemory,
                                mVkImageView);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
