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
#include "RERHIVulkan/Buffer/RHITextureBuffer.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


TextureBuffer::TextureBuffer(RHIDynamicRHI &vulkanRhi, uint32_t numberOfBytes, const void *data, uint32_t bufferFlags,
                             [[maybe_unused]] RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat
                             RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHITextureBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVkBuffer(VK_NULL_HANDLE),
  mVkDeviceMemory(VK_NULL_HANDLE),
  mVkBufferView(VK_NULL_HANDLE) {
  // Sanity check
  RHI_ASSERT((numberOfBytes % RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat)) == 0,
             "The Vulkan texture buffer size must be a multiple of the selected texture format bytes per texel")

  // Create the texture buffer
  uint32_t vkBufferUsageFlagBits = 0;
  if (bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) {
    vkBufferUsageFlagBits |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
  }
  if (bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) {
    vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
  }
  Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits),
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                    numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

  // Create Vulkan buffer view
  if ((bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0 ||
      (bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) != 0) {
    const VkBufferViewCreateInfo vkBufferViewCreateInfo =
      {
        VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,  // sType (VkStructureType)
        nullptr,                  // pNext (const void*)
        0,                      // flags (VkBufferViewCreateFlags)
        mVkBuffer,                  // buffer (VkBuffer)
        Mapping::getVulkanFormat(textureFormat),  // format (VkFormat)
        0,                      // offset (VkDeviceSize)
        VK_WHOLE_SIZE                // range (VkDeviceSize)
      };
    if (vkCreateBufferView(vulkanRhi.getVulkanContext().getVkDevice(), &vkBufferViewCreateInfo,
                           vulkanRhi.getVkAllocationCallbacks(), &mVkBufferView) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to create the Vulkan buffer view")
    }
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "TBO", 6)	// 6 = "TBO: " including terminating zero
          const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (uint64_t)mVkBuffer, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (uint64_t)mVkDeviceMemory, detailedDebugName);
          Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT, (uint64_t)mVkBufferView, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
TextureBuffer::~TextureBuffer() {
  const RHIDynamicRHI &vulkanRhi = static_cast<const RHIDynamicRHI &>(getRhi());
  if (VK_NULL_HANDLE != mVkBufferView) {
    vkDestroyBufferView(vulkanRhi.getVulkanContext().getVkDevice(), mVkBufferView,
                        vulkanRhi.getVkAllocationCallbacks());
  }
  Helper::destroyAndFreeVkBuffer(vulkanRhi, mVkBuffer, mVkDeviceMemory);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
