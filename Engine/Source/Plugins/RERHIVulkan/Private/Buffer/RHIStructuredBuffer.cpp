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
#include "RERHIVulkan/Buffer/RHIStructuredBuffer.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


StructuredBuffer::StructuredBuffer(RHIDynamicRHI& vulkanRhi, RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIStructuredBuffer(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mVkBuffer(VK_NULL_HANDLE),
mVkDeviceMemory(VK_NULL_HANDLE)
{
  // Sanity checks
  RHI_ASSERT((numberOfBytes % numberOfStructureBytes) == 0, "The Vulkan structured buffer size must be a multiple of the given number of structure bytes")
  RHI_ASSERT((numberOfBytes % (sizeof(float) * 4)) == 0, "Performance: The Vulkan structured buffer should be aligned to a 128-bit stride, see \"Understanding Structured Buffer Performance\" by Evan Hart, posted Apr 17 2015 at 11:33AM - https://developer.nvidia.com/content/understanding-structured-buffer-performance")

  // Create the structured buffer
  Helper::createAndAllocateVkBuffer(vulkanRhi, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, mVkBuffer, mVkDeviceMemory);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (nullptr != vkDebugMarkerSetObjectNameEXT)
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "SBO", 6)	// 6 = "SBO: " including terminating zero
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
StructuredBuffer::~StructuredBuffer()
{
Helper::destroyAndFreeVkBuffer(static_cast<const RHIDynamicRHI&>(getRhi()), mVkBuffer, mVkDeviceMemory);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
