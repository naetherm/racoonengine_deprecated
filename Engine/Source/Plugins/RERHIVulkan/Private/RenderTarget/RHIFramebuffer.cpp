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
#include "RERHIVulkan/RenderTarget/RHIFramebuffer.h"
#include "RERHIVulkan/RenderTarget/RHIRenderPass.h"
#include "RERHIVulkan/Texture/RHITexture2D.h"
#include "RERHIVulkan/Texture/RHITexture2DArray.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

Framebuffer::Framebuffer(RERHI::RHIRenderPass &renderPass,
                         const RERHI::FramebufferAttachment *colorFramebufferAttachments,
                         const RERHI::FramebufferAttachment *depthStencilFramebufferAttachment
                         RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mNumberOfColorTextures(static_cast<RenderPass &>(renderPass).getNumberOfColorAttachments()),
  mColorTextures(nullptr),  // Set below
  mDepthStencilTexture(nullptr),
  mWidth(UINT_MAX),
  mHeight(UINT_MAX),
  mVkRenderPass(static_cast<RenderPass &>(renderPass).getVkRenderPass()),
  mVkFramebuffer(VK_NULL_HANDLE) {
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(renderPass.getRhi());

  // Vulkan attachment descriptions and views to fill
  std::vector<VkImageView> vkImageViews;
  vkImageViews.resize(mNumberOfColorTextures + ((nullptr != depthStencilFramebufferAttachment) ? 1u : 0u));
  uint32_t currentVkAttachmentDescriptionIndex = 0;

  // Add a reference to the used color textures
  if (mNumberOfColorTextures > 0) {
    mColorTextures = RHI_MALLOC_TYPED(vulkanRhi.getContext(), RERHI::RHITexture*, mNumberOfColorTextures);

    // Loop through all color textures
    RERHI::RHITexture **colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (RERHI::RHITexture **colorTexture = mColorTextures;
         colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments) {
      // Sanity check
      RHI_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid Vulkan color framebuffer attachment texture")

      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *colorTexture = colorFramebufferAttachments->texture;
      (*colorTexture)->AddReference();

      // Evaluate the color texture type
      VkImageView vkImageView = VK_NULL_HANDLE;
      switch ((*colorTexture)->getResourceType()) {
        case RERHI::ResourceType::TEXTURE_2D: {
          const Texture2D *texture2D = static_cast<Texture2D *>(*colorTexture);

          // Sanity checks
          RHI_ASSERT(colorFramebufferAttachments->mipmapIndex <
                     Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()),
                     "Invalid Vulkan color framebuffer attachment mipmap index")
          RHI_ASSERT(0 == colorFramebufferAttachments->layerIndex,
                     "Invalid Vulkan color framebuffer attachment layer index")

          // Update the framebuffer width and height if required
          vkImageView = texture2D->getVkImageView();
          ::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(),
                                      texture2D->getHeight(), mWidth, mHeight);
          break;
        }

        case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
          // Update the framebuffer width and height if required
          const Texture2DArray *texture2DArray = static_cast<Texture2DArray *>(*colorTexture);
          vkImageView = texture2DArray->getVkImageView();
          ::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(),
                                      texture2DArray->getHeight(), mWidth, mHeight);
          break;
        }

        case RERHI::ResourceType::ROOT_SIGNATURE:
        case RERHI::ResourceType::RESOURCE_GROUP:
        case RERHI::ResourceType::GRAPHICS_PROGRAM:
        case RERHI::ResourceType::VERTEX_ARRAY:
        case RERHI::ResourceType::RENDER_PASS:
        case RERHI::ResourceType::QUERY_POOL:
        case RERHI::ResourceType::SWAP_CHAIN:
        case RERHI::ResourceType::FRAMEBUFFER:
        case RERHI::ResourceType::VERTEX_BUFFER:
        case RERHI::ResourceType::INDEX_BUFFER:
        case RERHI::ResourceType::TEXTURE_BUFFER:
        case RERHI::ResourceType::STRUCTURED_BUFFER:
        case RERHI::ResourceType::INDIRECT_BUFFER:
        case RERHI::ResourceType::UNIFORM_BUFFER:
        case RERHI::ResourceType::TEXTURE_1D:
        case RERHI::ResourceType::TEXTURE_1D_ARRAY:
        case RERHI::ResourceType::TEXTURE_3D:
        case RERHI::ResourceType::TEXTURE_CUBE:
        case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
        case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
        case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
        case RERHI::ResourceType::SAMPLER_STATE:
        case RERHI::ResourceType::VERTEX_SHADER:
        case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
        case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case RERHI::ResourceType::GEOMETRY_SHADER:
        case RERHI::ResourceType::FRAGMENT_SHADER:
        case RERHI::ResourceType::TASK_SHADER:
        case RERHI::ResourceType::MESH_SHADER:
        case RERHI::ResourceType::COMPUTE_SHADER:
        default:
          // Nothing here
          break;
      }

      // Remember the Vulkan image view
      vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;

      // Advance current Vulkan attachment description index
      ++currentVkAttachmentDescriptionIndex;
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment) {
    mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
    RHI_ASSERT(nullptr != mDepthStencilTexture, "Invalid Vulkan depth stencil framebuffer attachment texture")
    mDepthStencilTexture->AddReference();

    // Evaluate the depth stencil texture type
    VkImageView vkImageView = VK_NULL_HANDLE;
    switch (mDepthStencilTexture->getResourceType()) {
      case RERHI::ResourceType::TEXTURE_2D: {
        const Texture2D *texture2D = static_cast<Texture2D *>(mDepthStencilTexture);

        // Sanity checks
        RHI_ASSERT(depthStencilFramebufferAttachment->mipmapIndex <
                   Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()),
                   "Invalid Vulkan depth stencil framebuffer attachment mipmap index")
        RHI_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex,
                   "Invalid Vulkan depth stencil framebuffer attachment layer index")

        // Update the framebuffer width and height if required
        vkImageView = texture2D->getVkImageView();
        ::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(),
                                    texture2D->getHeight(), mWidth, mHeight);
        break;
      }

      case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
        // Update the framebuffer width and height if required
        const Texture2DArray *texture2DArray = static_cast<Texture2DArray *>(mDepthStencilTexture);
        vkImageView = texture2DArray->getVkImageView();
        ::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(),
                                    texture2DArray->getHeight(), mWidth, mHeight);
        break;
      }

      case RERHI::ResourceType::ROOT_SIGNATURE:
      case RERHI::ResourceType::RESOURCE_GROUP:
      case RERHI::ResourceType::GRAPHICS_PROGRAM:
      case RERHI::ResourceType::VERTEX_ARRAY:
      case RERHI::ResourceType::RENDER_PASS:
      case RERHI::ResourceType::QUERY_POOL:
      case RERHI::ResourceType::SWAP_CHAIN:
      case RERHI::ResourceType::FRAMEBUFFER:
      case RERHI::ResourceType::VERTEX_BUFFER:
      case RERHI::ResourceType::INDEX_BUFFER:
      case RERHI::ResourceType::TEXTURE_BUFFER:
      case RERHI::ResourceType::STRUCTURED_BUFFER:
      case RERHI::ResourceType::INDIRECT_BUFFER:
      case RERHI::ResourceType::UNIFORM_BUFFER:
      case RERHI::ResourceType::TEXTURE_1D:
      case RERHI::ResourceType::TEXTURE_1D_ARRAY:
      case RERHI::ResourceType::TEXTURE_3D:
      case RERHI::ResourceType::TEXTURE_CUBE:
      case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
      case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
      case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
      case RERHI::ResourceType::SAMPLER_STATE:
      case RERHI::ResourceType::VERTEX_SHADER:
      case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
      case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case RERHI::ResourceType::GEOMETRY_SHADER:
      case RERHI::ResourceType::FRAGMENT_SHADER:
      case RERHI::ResourceType::TASK_SHADER:
      case RERHI::ResourceType::MESH_SHADER:
      case RERHI::ResourceType::COMPUTE_SHADER:
      default:
        // Nothing here
        break;
    }

    // Remember the Vulkan image view
    vkImageViews[currentVkAttachmentDescriptionIndex] = vkImageView;
  }

  // Validate the framebuffer width and height
  if (0 == mWidth || UINT_MAX == mWidth) {
    RHI_ASSERT(false, "Invalid Vulkan framebuffer width")
    mWidth = 1;
  }
  if (0 == mHeight || UINT_MAX == mHeight) {
    RHI_ASSERT(false, "Invalid Vulkan framebuffer height")
    mHeight = 1;
  }

  // Create Vulkan framebuffer
  const VkFramebufferCreateInfo vkFramebufferCreateInfo =
    {
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // sType (VkStructureType)
      nullptr,                  // pNext (const void*)
      0,                      // flags (VkFramebufferCreateFlags)
      mVkRenderPass,                // renderPass (VkRenderPass)
      static_cast<uint32_t>(vkImageViews.size()),  // attachmentCount (uint32_t)
      vkImageViews.data(),            // pAttachments (const VkImageView*)
      mWidth,                    // width (uint32_t)
      mHeight,                  // height (uint32_t
      1                      // layers (uint32_t)
    };
  if (vkCreateFramebuffer(vulkanRhi.getVulkanContext().getVkDevice(), &vkFramebufferCreateInfo,
                          vulkanRhi.getVkAllocationCallbacks(), &mVkFramebuffer) == VK_SUCCESS) {
    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (nullptr != vkDebugMarkerSetObjectNameEXT)
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
            const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
            Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT, (uint64_t)mVkRenderPass, detailedDebugName);
            Helper::setDebugObjectName(vkDevice, VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT, (uint64_t)mVkFramebuffer, detailedDebugName);
          }
#endif
  } else {
    RE_LOG(Critical, "Failed to create Vulkan framebuffer")
  }
}

/**
*  @brief
*    Destructor
*/
Framebuffer::~Framebuffer() {
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();

// Destroy Vulkan framebuffer instance
  if (VK_NULL_HANDLE != mVkFramebuffer) {
    vkDestroyFramebuffer(vkDevice, mVkFramebuffer, vulkanRhi.getVkAllocationCallbacks());
  }

// Release the reference to the used color textures
  if (nullptr != mColorTextures) {
// Release references
    RERHI::RHITexture **colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (RERHI::RHITexture **colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture) {
      (*colorTexture)->Release();
    }

// Cleanup
    RHI_FREE(vulkanRhi.getContext(), mColorTextures);
  }

// Release the reference to the used depth stencil texture
  if (nullptr != mDepthStencilTexture) {
// Release reference
    mDepthStencilTexture->Release();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
