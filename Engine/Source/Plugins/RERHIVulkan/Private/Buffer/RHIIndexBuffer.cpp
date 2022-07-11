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
#include "RERHIVulkan/Buffer/RHIIndexBuffer.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


IndexBuffer::IndexBuffer(RHIDynamicRHI& vulkanRhi, RECore::uint32 numberOfBytes, const void* data, RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIIndexBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mVkIndexType(Mapping::getVulkanType(vulkanRhi.getContext(), indexBufferFormat)),
mVkBuffer(VK_NULL_HANDLE),
mVkDeviceMemory(VK_NULL_HANDLE)
{
  int vkBufferUsageFlagBits = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  if ((bufferFlags & RERHI::BufferFlag::UNORDERED_ACCESS) != 0 || (bufferFlags & RERHI::BufferFlag::SHADER_RESOURCE) != 0)
  {
    vkBufferUsageFlagBits |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  }
  Helper::createAndAllocateVkBuffer(vulkanRhi, static_cast<VkBufferUsageFlagBits>(vkBufferUsageFlagBits), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
					const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
					Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (RECore::uint64)mVkBuffer, detailedDebugName);
					Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, (RECore::uint64)mVkDeviceMemory, detailedDebugName);
				}
#endif
}

/**
*  @brief
*    Destructor
*/
IndexBuffer::~IndexBuffer()
{
Helper::destroyAndFreeVkBuffer(static_cast<const RHIDynamicRHI&>(getRhi()), mVkBuffer, mVkDeviceMemory);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
