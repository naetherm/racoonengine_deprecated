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
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


VkCommandBuffer Helper::beginSingleTimeCommands(const RHIDynamicRHI& vulkanRhi)
{
  // Create and begin Vulkan command buffer
  VkCommandBuffer vkCommandBuffer = vulkanRhi.getVulkanContext().createVkCommandBuffer();
  static constexpr VkCommandBufferBeginInfo vkCommandBufferBeginInfo =
    {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// sType (VkStructureType)
      nullptr,										// pNext (const void*)
      0,												// flags (VkCommandBufferUsageFlags)
      nullptr											// pInheritanceInfo (const VkCommandBufferInheritanceInfo*)
    };
  if (vkBeginCommandBuffer(vkCommandBuffer, &vkCommandBufferBeginInfo) == VK_SUCCESS)
  {
    // Done
    return vkCommandBuffer;
  }
  else
  {
    // Error!
    RE_LOG(Critical, "Failed to begin Vulkan command buffer instance")
    return VK_NULL_HANDLE;
  }
}

void Helper::endSingleTimeCommands(const RHIDynamicRHI& vulkanRhi, VkCommandBuffer vkCommandBuffer)
{
  const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
  const VkQueue vkQueue = vulkanContext.getGraphicsVkQueue();

  // End Vulkan command buffer
  vkEndCommandBuffer(vkCommandBuffer);

  // Submit Vulkan command buffer
  const VkSubmitInfo vkSubmitInfo =
    {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,	// sType (VkStructureType)
      nullptr,						// pNext (const void*)
      0,								// waitSemaphoreCount (RECore::uint32)
      nullptr,						// pWaitSemaphores (const VkSemaphore*)
      nullptr,						// pWaitDstStageMask (const VkPipelineStageFlags*)
      1,								// commandBufferCount (RECore::uint32)
      &vkCommandBuffer,				// pCommandBuffers (const VkCommandBuffer*)
      0,								// signalSemaphoreCount (RECore::uint32)
      nullptr							// pSignalSemaphores (const VkSemaphore*)
    };
  if (vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
  {
    // Error!
    RE_LOG(Critical, "Vulkan queue submit failed")
    return;
  }
  if (vkQueueWaitIdle(vkQueue) != VK_SUCCESS)
  {
    // Error!
    RE_LOG(Critical, "Vulkan Queue wait idle failed")
    return;
  }

  // Destroy Vulkan command buffer
  vulkanContext.destroyVkCommandBuffer(vkCommandBuffer);
}

//[-------------------------------------------------------]
//[ Transition                                            ]
//[-------------------------------------------------------]
void Helper::transitionVkImageLayout(const RHIDynamicRHI& vulkanRhi, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout)
{
  // Create and begin Vulkan command buffer
  VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);

  // Vulkan image memory barrier
  transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, 1, 1, oldVkImageLayout, newVkImageLayout);

  // End and destroy Vulkan command buffer
  endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
}

void Helper::transitionVkImageLayout(const RHIDynamicRHI& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageAspectFlags vkImageAspectFlags, RECore::uint32 levelCount, RECore::uint32 layerCount, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout)
{
  VkImageMemoryBarrier vkImageMemoryBarrier =
    {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
      nullptr,								// pNext (const void*)
      0,										// srcAccessMask (VkAccessFlags)
      0,										// dstAccessMask (VkAccessFlags)
      oldVkImageLayout,						// oldLayout (VkImageLayout)
      newVkImageLayout,						// newLayout (VkImageLayout)
      VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (RECore::uint32)
      VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (RECore::uint32)
      vkImage,								// image (VkImage)
      { // subresourceRange (VkImageSubresourceRange)
        vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
        0,					// baseMipLevel (RECore::uint32)
        levelCount,			// levelCount (RECore::uint32)
        0,					// baseArrayLayer (RECore::uint32)
        layerCount			// layerCount (RECore::uint32)
      }
    };

  // "srcAccessMask" and "dstAccessMask" configuration
  VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  if (VK_IMAGE_LAYOUT_PREINITIALIZED == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == newVkImageLayout)
  {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL == newVkImageLayout)
  {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  else if (VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL == oldVkImageLayout && VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL == newVkImageLayout)
  {
    vkImageMemoryBarrier.srcAccessMask = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    vkImageMemoryBarrier.dstAccessMask = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dstStageMask = VK_PIPELINE_STAGE_HOST_BIT;
  }
  else if (VK_IMAGE_LAYOUT_UNDEFINED == oldVkImageLayout && VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL == newVkImageLayout)
  {
    vkImageMemoryBarrier.srcAccessMask = 0;
    vkImageMemoryBarrier.dstAccessMask = (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
  }
  else
  {
    RE_LOG(Critical, "Unsupported Vulkan image layout transition")
  }

  // Create Vulkan pipeline barrier command
  vkCmdPipelineBarrier(vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
}

void Helper::transitionVkImageLayout(const RHIDynamicRHI& vulkanRhi, VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldVkImageLayout, VkImageLayout newVkImageLayout, VkImageSubresourceRange vkImageSubresourceRange, VkPipelineStageFlags sourceVkPipelineStageFlags, VkPipelineStageFlags destinationVkPipelineStageFlags)
{
  // Basing on https://github.com/SaschaWillems/Vulkan/tree/master

  // Create an image barrier object
  VkImageMemoryBarrier vkImageMemoryBarrier =
    {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,	// sType (VkStructureType)
      nullptr,								// pNext (const void*)
      0,										// srcAccessMask (VkAccessFlags)
      0,										// dstAccessMask (VkAccessFlags)
      oldVkImageLayout,						// oldLayout (VkImageLayout)
      newVkImageLayout,						// newLayout (VkImageLayout)
      VK_QUEUE_FAMILY_IGNORED,				// srcQueueFamilyIndex (RECore::uint32)
      VK_QUEUE_FAMILY_IGNORED,				// dstQueueFamilyIndex (RECore::uint32)
      vkImage,								// image (VkImage)
      vkImageSubresourceRange					// subresourceRange (VkImageSubresourceRange)
    };

  // Source layouts (old)
  // -> Source access mask controls actions that have to be finished on the old layout before it will be transitioned to the new layout
  switch (oldVkImageLayout)
  {
    case VK_IMAGE_LAYOUT_UNDEFINED:
      // Image layout is undefined (or does not matter)
      // Only valid as initial layout
      // No flags required, listed only for completeness
      vkImageMemoryBarrier.srcAccessMask = 0;
      break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      // Image is preinitialized
      // Only valid as initial layout for linear images, preserves memory contents
      // Make sure host writes have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image is a color attachment
      // Make sure any writes to the color buffer have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image is a depth/stencil attachment
      // Make sure any writes to the depth/stencil buffer have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image is a transfer source
      // Make sure any reads from the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image is a transfer destination
      // Make sure any writes to the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image is read by a shader
      // Make sure any shader reads from the image have been finished
      vkImageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_GENERAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
      // case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
      // case VK_IMAGE_LAYOUT_END_RANGE:		not possible
    case VK_IMAGE_LAYOUT_MAX_ENUM:
    default:
      // Other source layouts aren't handled (yet)
    RE_LOG(Critical, "Unsupported Vulkan image old layout transition")
      break;
  }

  // Target layouts (new)
  // -> Destination access mask controls the dependency for the new image layout
  switch (newVkImageLayout)
  {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      // Image will be used as a transfer destination
      // Make sure any writes to the image have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
      // Image will be used as a transfer source
      // Make sure any reads from the image have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
      // Image will be used as a color attachment
      // Make sure any writes to the color buffer have been finished
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      // Image layout will be used as a depth/stencil attachment
      // Make sure any writes to depth/stencil buffer have been finished
      vkImageMemoryBarrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      // Image will be read in a shader (sampler, input attachment)
      // Make sure any writes to the image have been finished
      if (vkImageMemoryBarrier.srcAccessMask == 0)
      {
        vkImageMemoryBarrier.srcAccessMask = (VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT);
      }
      vkImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      break;

    case VK_IMAGE_LAYOUT_UNDEFINED:
    case VK_IMAGE_LAYOUT_GENERAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
    case VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV:
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:
      // case VK_IMAGE_LAYOUT_BEGIN_RANGE:	not possible
      // case VK_IMAGE_LAYOUT_END_RANGE:		not possible
    case VK_IMAGE_LAYOUT_MAX_ENUM:
    default:
      // Other source layouts aren't handled (yet)
    RE_LOG(Critical, "Unsupported Vulkan image new layout transition")
      break;
  }

  // Put barrier inside setup command buffer
  // -> "Table 4. Supported access types": https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#synchronization-access-types-supported
  vkCmdPipelineBarrier(vkCommandBuffer, sourceVkPipelineStageFlags, destinationVkPipelineStageFlags, 0, 0, nullptr, 0, nullptr, 1, &vkImageMemoryBarrier);
}

//[-------------------------------------------------------]
//[ Buffer                                                ]
//[-------------------------------------------------------]
// TODO(naetherm) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
void Helper::createAndAllocateVkBuffer(const RHIDynamicRHI& vulkanRhi, VkBufferUsageFlagBits vkBufferUsageFlagBits, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkDeviceSize numberOfBytes, const void* data, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory)
{
  const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
  const VkDevice vkDevice = vulkanContext.getVkDevice();

  // Create the Vulkan buffer
  const VkBufferCreateInfo vkBufferCreateInfo =
    {
      VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,					// sType (VkStructureType)
      nullptr,												// pNext (const void*)
      0,														// flags (VkBufferCreateFlags)
      numberOfBytes,											// size (VkDeviceSize)
      static_cast<VkBufferUsageFlags>(vkBufferUsageFlagBits),	// usage (VkBufferUsageFlags)
      VK_SHARING_MODE_EXCLUSIVE,								// sharingMode (VkSharingMode)
      0,														// queueFamilyIndexCount (RECore::uint32)
      nullptr													// pQueueFamilyIndices (const RECore::uint32*)
    };
  if (vkCreateBuffer(vkDevice, &vkBufferCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkBuffer) != VK_SUCCESS)
  {
    RE_LOG(Critical, "Failed to create the Vulkan buffer")
  }

  // Allocate memory for the Vulkan buffer
  VkMemoryRequirements vkMemoryRequirements = {};
  vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &vkMemoryRequirements);
  const VkMemoryAllocateInfo vkMemoryAllocateInfo =
    {
      VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
      nullptr,																						// pNext (const void*)
      vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
      vulkanContext.findMemoryTypeIndex(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (RECore::uint32)
    };
  if (vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkDeviceMemory) != VK_SUCCESS)
  {
    RE_LOG(Critical, "Failed to allocate the Vulkan buffer memory")
  }

  // Bind and fill memory
  vkBindBufferMemory(vkDevice, vkBuffer, vkDeviceMemory, 0);
  if (nullptr != data)
  {
    void* mappedData = nullptr;
    if (vkMapMemory(vkDevice, vkDeviceMemory, 0, vkBufferCreateInfo.size, 0, &mappedData) == VK_SUCCESS)
    {
      memcpy(mappedData, data, static_cast<size_t>(vkBufferCreateInfo.size));
      vkUnmapMemory(vkDevice, vkDeviceMemory);
    }
    else
    {
      RE_LOG(Critical, "Failed to map the Vulkan memory")
    }
  }
}

void Helper::destroyAndFreeVkBuffer(const RHIDynamicRHI& vulkanRhi, VkBuffer& vkBuffer, VkDeviceMemory& vkDeviceMemory)
{
  if (VK_NULL_HANDLE != vkBuffer)
  {
    const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
    vkDestroyBuffer(vkDevice, vkBuffer, vulkanRhi.getVkAllocationCallbacks());
    if (VK_NULL_HANDLE != vkDeviceMemory)
    {
      vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.getVkAllocationCallbacks());
    }
  }
}

//[-------------------------------------------------------]
//[ Image                                                 ]
//[-------------------------------------------------------]
VkImageLayout Helper::getVkImageLayoutByTextureFlags(RECore::uint32 textureFlags)
{
  if (textureFlags & RERHI::TextureFlag::RENDER_TARGET)
  {
    return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  else if (textureFlags & RERHI::TextureFlag::UNORDERED_ACCESS)
  {
    return VK_IMAGE_LAYOUT_GENERAL;
  }
  return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

// TODO(naetherm) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
VkFormat Helper::createAndFillVkImage(const RHIDynamicRHI& vulkanRhi, VkImageType vkImageType, VkImageViewType vkImageViewType, const VkExtent3D& vkExtent3D, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, RECore::uint8 numberOfMultisamples, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView)
{
  // Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS));
  RECore::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? RERHI::RHITexture::getNumberOfMipmaps(vkExtent3D.width, vkExtent3D.height) : 1;

  // Get Vulkan image usage flags
  RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "Vulkan render target textures can't be filled using provided data")
  const bool isDepthTextureFormat = RERHI::TextureFormat::isDepth(textureFormat);
  VkImageUsageFlags vkImageUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if (textureFlags & RERHI::TextureFlag::SHADER_RESOURCE)
  {
    vkImageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;
  }
  if (textureFlags & RERHI::TextureFlag::UNORDERED_ACCESS)
  {
    vkImageUsageFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
  }
  if (textureFlags & RERHI::TextureFlag::RENDER_TARGET)
  {
    if (isDepthTextureFormat)
    {
      vkImageUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    else
    {
      vkImageUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
  }
  if (generateMipmaps)
  {
    vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  }

  // Get Vulkan format
  const VkFormat vkFormat   = Mapping::getVulkanFormat(textureFormat);
  const bool     layered    = (VK_IMAGE_VIEW_TYPE_1D_ARRAY == vkImageViewType || VK_IMAGE_VIEW_TYPE_2D_ARRAY == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType);
  const RECore::uint32 layerCount = layered ? vkExtent3D.depth : 1;
  const RECore::uint32 depth	  = layered ? 1 : vkExtent3D.depth;
  const VkSampleCountFlagBits vkSampleCountFlagBits = Mapping::getVulkanSampleCountFlagBits(vulkanRhi.getContext(), numberOfMultisamples);
  VkImageAspectFlags vkImageAspectFlags = isDepthTextureFormat ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
  if (::detail::hasVkFormatStencilComponent(vkFormat))
  {
    vkImageAspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }

  // Calculate the number of bytes
  RECore::uint32 numberOfBytes = 0;
  if (dataContainsMipmaps)
  {
    RECore::uint32 currentWidth  = vkExtent3D.width;
    RECore::uint32 currentHeight = vkExtent3D.height;
    RECore::uint32 currentDepth  = depth;
    for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap)
    {
      numberOfBytes += RERHI::TextureFormat::getNumberOfBytesPerSlice(static_cast<RERHI::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
      currentWidth = RERHI::RHITexture::getHalfSize(currentWidth);
      currentHeight = RERHI::RHITexture::getHalfSize(currentHeight);
      currentDepth = RERHI::RHITexture::getHalfSize(currentDepth);
    }
    numberOfBytes *= vkExtent3D.depth;
  }
  else
  {
    numberOfBytes = RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, vkExtent3D.width, vkExtent3D.height) * vkExtent3D.depth;
  }

  { // Create and fill Vulkan image
    const VkImageCreateFlags vkImageCreateFlags = (VK_IMAGE_VIEW_TYPE_CUBE == vkImageViewType || VK_IMAGE_VIEW_TYPE_CUBE_ARRAY == vkImageViewType) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;
    createAndAllocateVkImage(vulkanRhi, vkImageCreateFlags, vkImageType, VkExtent3D{vkExtent3D.width, vkExtent3D.height, depth}, numberOfMipmaps, layerCount, vkFormat, vkSampleCountFlagBits, VK_IMAGE_TILING_OPTIMAL, vkImageUsageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkImage, vkDeviceMemory);
  }

  // Create the Vulkan image view
  if ((textureFlags & RERHI::TextureFlag::SHADER_RESOURCE) != 0 || (textureFlags & RERHI::TextureFlag::RENDER_TARGET) != 0 || (textureFlags & RERHI::TextureFlag::UNORDERED_ACCESS) != 0)
  {
    createVkImageView(vulkanRhi, vkImage, vkImageViewType, numberOfMipmaps, layerCount, vkFormat, vkImageAspectFlags, vkImageView);
  }

  // Upload all mipmaps
  if (nullptr != data)
  {
    // Create Vulkan staging buffer
    VkBuffer stagingVkBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingVkDeviceMemory = VK_NULL_HANDLE;
    createAndAllocateVkBuffer(vulkanRhi, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, numberOfBytes, data, stagingVkBuffer, stagingVkDeviceMemory);

    { // Upload all mipmaps
      const RECore::uint32 numberOfUploadedMipmaps = generateMipmaps ? 1 : numberOfMipmaps;

      // Create and begin Vulkan command buffer
      VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);
      transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps, layerCount, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

      // Upload all mipmaps
      RECore::uint32 bufferOffset  = 0;
      RECore::uint32 currentWidth  = vkExtent3D.width;
      RECore::uint32 currentHeight = vkExtent3D.height;
      RECore::uint32 currentDepth  = depth;

      // Allocate list of VkBufferImageCopy and setup VkBufferImageCopy data for each mipmap level
      std::vector<VkBufferImageCopy> vkBufferImageCopyList;
      vkBufferImageCopyList.reserve(numberOfUploadedMipmaps);
      for (RECore::uint32 mipmap = 0; mipmap < numberOfUploadedMipmaps; ++mipmap)
      {
        vkBufferImageCopyList.push_back({
                                          bufferOffset,									// bufferOffset (VkDeviceSize)
                                          0,												// bufferRowLength (RECore::uint32)
                                          0,												// bufferImageHeight (RECore::uint32)
                                          { // imageSubresource (VkImageSubresourceLayers)
                                            vkImageAspectFlags,							// aspectMask (VkImageAspectFlags)
                                            mipmap,										// mipLevel (RECore::uint32)
                                            0,											// baseArrayLayer (RECore::uint32)
                                            layerCount									// layerCount (RECore::uint32)
                                          },
                                          { 0, 0, 0 },									// imageOffset (VkOffset3D)
                                          { currentWidth, currentHeight, currentDepth }	// imageExtent (VkExtent3D)
                                        });

        // Move on to the next mipmap
        bufferOffset += RERHI::TextureFormat::getNumberOfBytesPerSlice(static_cast<RERHI::TextureFormat::Enum>(textureFormat), currentWidth, currentHeight) * currentDepth;
        currentWidth = RERHI::RHITexture::getHalfSize(currentWidth);
        currentHeight = RERHI::RHITexture::getHalfSize(currentHeight);
        currentDepth = RERHI::RHITexture::getHalfSize(currentDepth);
      }

      // Copy Vulkan buffer to Vulkan image
      vkCmdCopyBufferToImage(vkCommandBuffer, stagingVkBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<RECore::uint32>(vkBufferImageCopyList.size()), vkBufferImageCopyList.data());

      // End and destroy Vulkan command buffer
      if (generateMipmaps)
      {
        const VkImageSubresourceRange vkImageSubresourceRange =
          {
            vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
            0,					// baseMipLevel (RECore::uint32)
            1,					// levelCount (RECore::uint32)
            0,					// baseArrayLayer (RECore::uint32)
            layerCount			// layerCount (RECore::uint32)
          };
        transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
      }
      else
      {
        transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, vkImageAspectFlags, numberOfUploadedMipmaps, layerCount, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
      }
      endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
    }

    // Destroy Vulkan staging buffer
    destroyAndFreeVkBuffer(vulkanRhi, stagingVkBuffer, stagingVkDeviceMemory);

    // Generate a complete texture mip-chain at runtime from a base image using image blits and proper image barriers
    // -> Basing on https://github.com/SaschaWillems/Vulkan/tree/master/examples/texturemipmapgen and "Mipmap generation : Transfers, transition layout" by Antoine MORRIER published January 12, 2017 at http://cpp-rendering.io/mipmap-generation/
    // -> We copy down the whole mip chain doing a blit from mip-1 to mip. An alternative way would be to always blit from the first mip level and sample that one down.
    // TODO(naetherm) Some GPUs also offer "asynchronous transfer queues" (check for queue families with only the "VK_QUEUE_TRANSFER_BIT" set) that may be used to speed up such operations
    if (generateMipmaps)
    {
#ifdef DEBUG
      {
						// Get device properties for the requested Vulkan texture format
						VkFormatProperties vkFormatProperties;
						vkGetPhysicalDeviceFormatProperties(vulkanRhi.getVulkanContext().getVkPhysicalDevice(), vkFormat, &vkFormatProperties);

						// Mip-chain generation requires support for blit source and destination
						RHI_ASSERT(vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT, "Invalid Vulkan optimal tiling features")
						RHI_ASSERT(vkFormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT, "Invalid Vulkan optimal tiling features")
					}
#endif

      // Create and begin Vulkan command buffer
      VkCommandBuffer vkCommandBuffer = beginSingleTimeCommands(vulkanRhi);

      // Copy down mips from n-1 to n
      for (RECore::uint32 i = 1; i < numberOfMipmaps; ++i)
      {
        const VkImageBlit VkImageBlit =
          {
            { // srcSubresource (VkImageSubresourceLayers)
              vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
              i - 1,				// mipLevel (RECore::uint32)
              0,					// baseArrayLayer (RECore::uint32)
              layerCount			// layerCount (RECore::uint32)
            },
            { // srcOffsets[2] (VkOffset3D)
              { 0, 0, 0 },
              { std::max(int32_t(vkExtent3D.width >> (i - 1)), 1), std::max(int32_t(vkExtent3D.height >> (i - 1)), 1), 1 }
            },
            { // dstSubresource (VkImageSubresourceLayers)
              vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
              i,					// mipLevel (RECore::uint32)
              0,					// baseArrayLayer (RECore::uint32)
              layerCount			// layerCount (RECore::uint32)
            },
            { // dstOffsets[2] (VkOffset3D)
              { 0, 0, 0 },
              { std::max(int32_t(vkExtent3D.width >> i), 1), std::max(int32_t(vkExtent3D.height >> i), 1), 1 }
            }
          };
        const VkImageSubresourceRange vkImageSubresourceRange =
          {
            vkImageAspectFlags,	// aspectMask (VkImageAspectFlags)
            i,					// baseMipLevel (RECore::uint32)
            1,					// levelCount (RECore::uint32)
            0,					// baseArrayLayer (RECore::uint32)
            layerCount			// layerCount (RECore::uint32)
          };

        // Transition current mip level to transfer destination
        transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        // Blit from previous level
        vkCmdBlitImage(vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &VkImageBlit, VK_FILTER_LINEAR);

        // Transition current mip level to transfer source for read in next iteration
        transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);
      }

      { // After the loop, all mip layers are in "VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL"-layout, so transition all to "VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL"-layout
        const VkImageSubresourceRange vkImageSubresourceRange =
          {
            vkImageAspectFlags,		// aspectMask (VkImageAspectFlags)
            0,						// baseMipLevel (RECore::uint32)
            numberOfMipmaps,		// levelCount (RECore::uint32)
            0,						// baseArrayLayer (RECore::uint32)
            layerCount				// layerCount (RECore::uint32)
          };
        transitionVkImageLayout(vulkanRhi, vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vkImageSubresourceRange, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
      }

      // End and destroy Vulkan command buffer
      endSingleTimeCommands(vulkanRhi, vkCommandBuffer);
    }
  }

  // Done
  return vkFormat;
}

void Helper::createAndAllocateVkImage(const RHIDynamicRHI& vulkanRhi, VkImageCreateFlags vkImageCreateFlags, VkImageType vkImageType, const VkExtent3D& vkExtent3D, RECore::uint32 mipLevels, RECore::uint32 arrayLayers, VkFormat vkFormat, VkSampleCountFlagBits vkSampleCountFlagBits, VkImageTiling vkImageTiling, VkImageUsageFlags vkImageUsageFlags, VkMemoryPropertyFlags vkMemoryPropertyFlags, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory)
{
  const VulkanContext& vulkanContext = vulkanRhi.getVulkanContext();
  const VkDevice vkDevice = vulkanContext.getVkDevice();

  { // Create Vulkan image
    const VkImageCreateInfo vkImageCreateInfo =
      {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	// sType (VkStructureType)
        nullptr,								// pNext (const void*)
        vkImageCreateFlags,						// flags (VkImageCreateFlags)
        vkImageType,							// imageType (VkImageType)
        vkFormat,								// format (VkFormat)
        vkExtent3D,								// extent (VkExtent3D)
        mipLevels,								// mipLevels (RECore::uint32)
        arrayLayers,							// arrayLayers (RECore::uint32)
        vkSampleCountFlagBits,					// samples (VkSampleCountFlagBits)
        vkImageTiling,							// tiling (VkImageTiling)
        vkImageUsageFlags,						// usage (VkImageUsageFlags)
        VK_SHARING_MODE_EXCLUSIVE,				// sharingMode (VkSharingMode)
        0,										// queueFamilyIndexCount (RECore::uint32)
        nullptr,								// pQueueFamilyIndices (const RECore::uint32*)
        VK_IMAGE_LAYOUT_PREINITIALIZED			// initialLayout (VkImageLayout)
      };
    if (vkCreateImage(vkDevice, &vkImageCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkImage) != VK_SUCCESS)
    {
      RE_LOG(Critical, "Failed to create the Vulkan image")
    }
  }

  { // Allocate Vulkan memory
    VkMemoryRequirements vkMemoryRequirements = {};
    vkGetImageMemoryRequirements(vkDevice, vkImage, &vkMemoryRequirements);
    const VkMemoryAllocateInfo vkMemoryAllocateInfo =
      {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,															// sType (VkStructureType)
        nullptr,																						// pNext (const void*)
        vkMemoryRequirements.size,																		// allocationSize (VkDeviceSize)
        vulkanContext.findMemoryTypeIndex(vkMemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags)	// memoryTypeIndex (RECore::uint32)
      };
    if (vkAllocateMemory(vkDevice, &vkMemoryAllocateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkDeviceMemory) != VK_SUCCESS)
    {
      RE_LOG(Critical, "Failed to allocate the Vulkan memory")
    }
    if (vkBindImageMemory(vkDevice, vkImage, vkDeviceMemory, 0) != VK_SUCCESS)
    {
      RE_LOG(Critical, "Failed to bind the Vulkan image memory")
    }
  }
}

void Helper::destroyAndFreeVkImage(const RHIDynamicRHI& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory)
{
  if (VK_NULL_HANDLE != vkImage)
  {
    const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
    vkDestroyImage(vkDevice, vkImage, vulkanRhi.getVkAllocationCallbacks());
    vkImage = VK_NULL_HANDLE;
    if (VK_NULL_HANDLE != vkDeviceMemory)
    {
      vkFreeMemory(vkDevice, vkDeviceMemory, vulkanRhi.getVkAllocationCallbacks());
      vkDeviceMemory = VK_NULL_HANDLE;
    }
  }
}

void Helper::destroyAndFreeVkImage(const RHIDynamicRHI& vulkanRhi, VkImage& vkImage, VkDeviceMemory& vkDeviceMemory, VkImageView& vkImageView)
{
  if (VK_NULL_HANDLE != vkImageView)
  {
    vkDestroyImageView(vulkanRhi.getVulkanContext().getVkDevice(), vkImageView, vulkanRhi.getVkAllocationCallbacks());
    vkImageView = VK_NULL_HANDLE;
  }
  destroyAndFreeVkImage(vulkanRhi, vkImage, vkDeviceMemory);
}

void Helper::createVkImageView(const RHIDynamicRHI& vulkanRhi, VkImage vkImage, VkImageViewType vkImageViewType, RECore::uint32 levelCount, RECore::uint32 layerCount, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags, VkImageView& vkImageView)
{
  const VkImageViewCreateInfo vkImageViewCreateInfo =
    {
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	// sType (VkStructureType)
      nullptr,									// pNext (const void*)
      0,											// flags (VkImageViewCreateFlags)
      vkImage,									// image (VkImage)
      vkImageViewType,							// viewType (VkImageViewType)
      vkFormat,									// format (VkFormat)
      { // components (VkComponentMapping)
        VK_COMPONENT_SWIZZLE_IDENTITY,			// r (VkComponentSwizzle)
        VK_COMPONENT_SWIZZLE_IDENTITY,			// g (VkComponentSwizzle)
        VK_COMPONENT_SWIZZLE_IDENTITY,			// b (VkComponentSwizzle)
        VK_COMPONENT_SWIZZLE_IDENTITY			// a (VkComponentSwizzle)
      },
      { // subresourceRange (VkImageSubresourceRange)
        vkImageAspectFlags,						// aspectMask (VkImageAspectFlags)
        0,										// baseMipLevel (RECore::uint32)
        levelCount,								// levelCount (RECore::uint32)
        0,										// baseArrayLayer (RECore::uint32)
        layerCount								// layerCount (RECore::uint32)
      }
    };
  if (vkCreateImageView(vulkanRhi.getVulkanContext().getVkDevice(), &vkImageViewCreateInfo, vulkanRhi.getVkAllocationCallbacks(), &vkImageView) != VK_SUCCESS)
  {
    RE_LOG(Critical, "Failed to create Vulkan image view")
  }
}

//[-------------------------------------------------------]
//[ Debug                                                 ]
//[-------------------------------------------------------]
#ifdef DEBUG
void Helper::setDebugObjectName(VkDevice vkDevice, VkDebugReportObjectTypeEXT vkDebugReportObjectTypeEXT, RECore::uint64 object, const char* objectName)
			{
				if (nullptr != vkDebugMarkerSetObjectNameEXT)
				{
					const VkDebugMarkerObjectNameInfoEXT vkDebugMarkerObjectNameInfoEXT =
					{
						VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT,	// sType (VkStructureType)
						nullptr,												// pNext (const void*)
						vkDebugReportObjectTypeEXT,								// objectType (VkDebugReportObjectTypeEXT)
						object,													// object (RECore::uint64)
						objectName												// pObjectName (const char*)
					};
					vkDebugMarkerSetObjectNameEXT(vkDevice, &vkDebugMarkerObjectNameInfoEXT);
				}
			}
#endif



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
