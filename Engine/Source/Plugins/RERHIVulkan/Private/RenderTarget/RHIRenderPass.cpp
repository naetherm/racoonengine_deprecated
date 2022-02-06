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
#include "RERHIVulkan/RenderTarget/RHIRenderPass.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

RenderPass::RenderPass(RHIDynamicRHI &vulkanRhi, uint32_t numberOfColorAttachments,
                       const RERHI::TextureFormat::Enum *colorAttachmentTextureFormats,
                       RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples
                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIRenderPass(vulkanRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVkRenderPass(VK_NULL_HANDLE),
  mNumberOfColorAttachments(numberOfColorAttachments),
  mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat),
  mVkSampleCountFlagBits(Mapping::getVulkanSampleCountFlagBits(vulkanRhi.getContext(), numberOfMultisamples)) {
  const bool hasDepthStencilAttachment = (RERHI::TextureFormat::Enum::UNKNOWN != depthStencilAttachmentTextureFormat);

  // Vulkan attachment descriptions
  std::vector<VkAttachmentDescription> vkAttachmentDescriptions;
  vkAttachmentDescriptions.resize(mNumberOfColorAttachments + (hasDepthStencilAttachment ? 1u : 0u));
  uint32_t currentVkAttachmentDescriptionIndex = 0;

  // Handle color attachments
  typedef std::vector<VkAttachmentReference> VkAttachmentReferences;
  VkAttachmentReferences colorVkAttachmentReferences;
  if (mNumberOfColorAttachments > 0) {
    colorVkAttachmentReferences.resize(mNumberOfColorAttachments);
    for (uint32_t i = 0; i < mNumberOfColorAttachments; ++i) {
      { // Setup Vulkan color attachment references
        VkAttachmentReference &vkAttachmentReference = colorVkAttachmentReferences[currentVkAttachmentDescriptionIndex];
        vkAttachmentReference.attachment = currentVkAttachmentDescriptionIndex;      // attachment (uint32_t)
        vkAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // layout (VkImageLayout)
      }

      { // Setup Vulkan color attachment description
        VkAttachmentDescription &vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
        vkAttachmentDescription.flags = 0;                                // flags (VkAttachmentDescriptionFlags)
        vkAttachmentDescription.format = Mapping::getVulkanFormat(
          colorAttachmentTextureFormats[i]);  // format (VkFormat)
        vkAttachmentDescription.samples = mVkSampleCountFlagBits;                    // samples (VkSampleCountFlagBits)
        vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                  // loadOp (VkAttachmentLoadOp)
        vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;                  // storeOp (VkAttachmentStoreOp)
        vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // stencilLoadOp (VkAttachmentLoadOp)
        vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                // stencilStoreOp (VkAttachmentStoreOp)
        vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                    // initialLayout (VkImageLayout)
        vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;            // finalLayout (VkImageLayout)
      }

      // Advance current Vulkan attachment description index
      ++currentVkAttachmentDescriptionIndex;
    }
  }

  // Handle depth stencil attachments
  const VkAttachmentReference depthVkAttachmentReference =
    {
      currentVkAttachmentDescriptionIndex,        // attachment (uint32_t)
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL  // layout (VkImageLayout)
    };
  if (hasDepthStencilAttachment) {
    // Setup Vulkan depth attachment description
    VkAttachmentDescription &vkAttachmentDescription = vkAttachmentDescriptions[currentVkAttachmentDescriptionIndex];
    vkAttachmentDescription.flags = 0;                                // flags (VkAttachmentDescriptionFlags)
    vkAttachmentDescription.format = Mapping::getVulkanFormat(
      depthStencilAttachmentTextureFormat);  // format (VkFormat)
    vkAttachmentDescription.samples = mVkSampleCountFlagBits;                    // samples (VkSampleCountFlagBits)
    vkAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;                  // loadOp (VkAttachmentLoadOp)
    vkAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;                  // storeOp (VkAttachmentStoreOp)
    vkAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;                // stencilLoadOp (VkAttachmentLoadOp)
    vkAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;                // stencilStoreOp (VkAttachmentStoreOp)
    vkAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;                    // initialLayout (VkImageLayout)
    vkAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;        // finalLayout (VkImageLayout)
    // ++currentVkAttachmentDescriptionIndex;	// Not needed since we're the last
  }

  // Create Vulkan create render pass
  const VkSubpassDescription vkSubpassDescription =
    {
      0,                                        // flags (VkSubpassDescriptionFlags)
      VK_PIPELINE_BIND_POINT_GRAPHICS,                        // pipelineBindPoint (VkPipelineBindPoint)
      0,                                        // inputAttachmentCount (uint32_t)
      nullptr,                                    // pInputAttachments (const VkAttachmentReference*)
      mNumberOfColorAttachments,                            // colorAttachmentCount (uint32_t)
      (mNumberOfColorAttachments > 0) ? colorVkAttachmentReferences.data()
                                      : nullptr,  // pColorAttachments (const VkAttachmentReference*)
      nullptr,                                    // pResolveAttachments (const VkAttachmentReference*)
      hasDepthStencilAttachment ? &depthVkAttachmentReference
                                : nullptr,        // pDepthStencilAttachment (const VkAttachmentReference*)
      0,                                        // preserveAttachmentCount (uint32_t)
      nullptr                                      // pPreserveAttachments (const uint32_t*)
    };
  static constexpr std::array<VkSubpassDependency, 2> vkSubpassDependencies =
    {{
       {
         VK_SUBPASS_EXTERNAL,                            // srcSubpass (uint32_t)
         0,                                      // dstSubpass (uint32_t)
         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,                    // srcStageMask (VkPipelineStageFlags)
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                // dstStageMask (VkPipelineStageFlags)
         VK_ACCESS_MEMORY_READ_BIT,                          // srcAccessMask (VkAccessFlags)
         VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,  // dstAccessMask (VkAccessFlags)
         VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags (VkDependencyFlags)
       },
       {
         0,                                      // srcSubpass (uint32_t)
         VK_SUBPASS_EXTERNAL,                            // dstSubpass (uint32_t)
         VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,                // srcStageMask (VkPipelineStageFlags)
         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,                    // dstStageMask (VkPipelineStageFlags)
         VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,  // srcAccessMask (VkAccessFlags)
         VK_ACCESS_MEMORY_READ_BIT,                          // dstAccessMask (VkAccessFlags)
         VK_DEPENDENCY_BY_REGION_BIT                          // dependencyFlags (VkDependencyFlags)
       }
     }};
  const VkRenderPassCreateInfo vkRenderPassCreateInfo =
    {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,        // sType (VkStructureType)
      nullptr,                        // pNext (const void*)
      0,                            // flags (VkRenderPassCreateFlags)
      static_cast<uint32_t>(vkAttachmentDescriptions.size()),  // attachmentCount (uint32_t)
      vkAttachmentDescriptions.data(),            // pAttachments (const VkAttachmentDescription*)
      1,                            // subpassCount (uint32_t)
      &vkSubpassDescription,                  // pSubpasses (const VkSubpassDescription*)
      static_cast<uint32_t>(vkSubpassDependencies.size()),  // dependencyCount (uint32_t)
      vkSubpassDependencies.data()              // pDependencies (const VkSubpassDependency*)
    };
  if (vkCreateRenderPass(vulkanRhi.getVulkanContext().getVkDevice(), &vkRenderPassCreateInfo,
                         vulkanRhi.getVkAllocationCallbacks(), &mVkRenderPass) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Render pass", 14)	// 14 = "Render pass: " including terminating zero
            Helper::setDebugObjectName(vulkanRhi.getVulkanContext().getVkDevice(), VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (uint64_t)mVkRenderPass, detailedDebugName);
          }
#endif
  } else {
    RE_LOG(Critical, "Failed to create Vulkan render pass")
  }
}

/**
*  @brief
*    Destructor
*/
RenderPass::~RenderPass() {
// Destroy Vulkan render pass instance
  if (VK_NULL_HANDLE != mVkRenderPass) {
    const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    vkDestroyRenderPass(vulkanRhi.getVulkanContext().getVkDevice(), mVkRenderPass,
                        vulkanRhi.getVkAllocationCallbacks());
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
