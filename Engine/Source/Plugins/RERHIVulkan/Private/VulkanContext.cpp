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
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

VulkanContext::VulkanContext(RHIDynamicRHI& vulkanRhi) :
mRHIDynamicRHI(vulkanRhi),
mVkPhysicalDevice(VK_NULL_HANDLE),
mVkDevice(VK_NULL_HANDLE),
mGraphicsQueueFamilyIndex(~0u),
mPresentQueueFamilyIndex(~0u),
mGraphicsVkQueue(VK_NULL_HANDLE),
mPresentVkQueue(VK_NULL_HANDLE),
mVkCommandPool(VK_NULL_HANDLE),
mVkCommandBuffer(VK_NULL_HANDLE)
{
  const VulkanRuntimeLinking& vulkanRuntimeLinking = mRHIDynamicRHI.getVulkanRuntimeLinking();

  // Get the physical Vulkan device this context should use
  bool enableDebugMarker = true;	// TODO(naetherm) Make it possible to setup from the outside whether or not the "VK_EXT_debug_marker"-extension should be used (e.g. retail shipped games might not want to have this enabled)
  bool hasMeshShaderSupport = false;
  {
    detail::VkPhysicalDevices vkPhysicalDevices;
    ::detail::enumeratePhysicalDevices(vulkanRhi.getContext(), vulkanRuntimeLinking.getVkInstance(), vkPhysicalDevices);
    if (!vkPhysicalDevices.empty())
    {
      mVkPhysicalDevice = ::detail::selectPhysicalDevice(vulkanRhi.getContext(), vkPhysicalDevices, vulkanRhi.getVulkanRuntimeLinking().isValidationEnabled(), enableDebugMarker, hasMeshShaderSupport);
    }
  }

  // Create the logical Vulkan device instance
  if (VK_NULL_HANDLE != mVkPhysicalDevice)
  {
    mVkDevice = ::detail::createVkDevice(mRHIDynamicRHI.getContext(), mRHIDynamicRHI.getVkAllocationCallbacks(), mVkPhysicalDevice, vulkanRuntimeLinking.isValidationEnabled(), enableDebugMarker, hasMeshShaderSupport, mGraphicsQueueFamilyIndex, mPresentQueueFamilyIndex);
    if (VK_NULL_HANDLE != mVkDevice)
    {
      // Load device based instance level Vulkan function pointers
      if (mRHIDynamicRHI.getVulkanRuntimeLinking().loadDeviceLevelVulkanEntryPoints(mVkDevice, hasMeshShaderSupport))
      {
        // Get the Vulkan device graphics queue that command buffers are submitted to
        vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamilyIndex, 0, &mGraphicsVkQueue);
        if (VK_NULL_HANDLE != mGraphicsVkQueue)
        {
          // Get the Vulkan device present queue
          vkGetDeviceQueue(mVkDevice, mPresentQueueFamilyIndex, 0, &mPresentVkQueue);
          if (VK_NULL_HANDLE != mPresentVkQueue)
          {
            // Create Vulkan command pool instance
            mVkCommandPool = ::detail::createVkCommandPool(mRHIDynamicRHI.getContext(), mRHIDynamicRHI.getVkAllocationCallbacks(), mVkDevice, mGraphicsQueueFamilyIndex);
            if (VK_NULL_HANDLE != mVkCommandPool)
            {
              // Create Vulkan command buffer instance
              mVkCommandBuffer = ::detail::createVkCommandBuffer(mRHIDynamicRHI.getContext(), mVkDevice, mVkCommandPool);
            }
            else
            {
              // Error!
              RE_LOG(Critical, "Failed to create Vulkan command pool instance")
            }
          }
        }
        else
        {
          // Error!
          RE_LOG(Critical, "Failed to get the Vulkan device graphics queue that command buffers are submitted to")
        }
      }
    }
  }
}

/**
*  @brief
*    Destructor
*/
VulkanContext::~VulkanContext()
{
  if (VK_NULL_HANDLE != mVkDevice)
  {
    if (VK_NULL_HANDLE != mVkCommandPool)
    {
      if (VK_NULL_HANDLE != mVkCommandBuffer)
      {
        vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &mVkCommandBuffer);
      }
      vkDestroyCommandPool(mVkDevice, mVkCommandPool, mRHIDynamicRHI.getVkAllocationCallbacks());
    }
    vkDeviceWaitIdle(mVkDevice);
    vkDestroyDevice(mVkDevice, mRHIDynamicRHI.getVkAllocationCallbacks());
  }
}


RECore::uint32 VulkanContext::findMemoryTypeIndex(RECore::uint32 typeFilter, VkMemoryPropertyFlags vkMemoryPropertyFlags) const
{
  VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
  vkGetPhysicalDeviceMemoryProperties(mVkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);
  for (RECore::uint32 i = 0; i < vkPhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
  {
    if ((typeFilter & (1 << i)) && (vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags)
    {
      return i;
    }
  }

  // Error!
  RE_LOG(Critical, "Failed to find suitable Vulkan memory type")
  return ~0u;
}

VkCommandBuffer VulkanContext::createVkCommandBuffer() const
{
  return ::detail::createVkCommandBuffer(mRHIDynamicRHI.getContext(), mVkDevice, mVkCommandPool);
}

void VulkanContext::destroyVkCommandBuffer(VkCommandBuffer vkCommandBuffer) const
{
  if (VK_NULL_HANDLE != mVkCommandBuffer)
  {
    vkFreeCommandBuffers(mVkDevice, mVkCommandPool, 1, &vkCommandBuffer);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
