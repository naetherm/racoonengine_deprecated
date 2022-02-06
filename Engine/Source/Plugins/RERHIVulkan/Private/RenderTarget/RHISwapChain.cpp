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
#include "RERHIVulkan/RenderTarget/RHISwapChain.h"
#include "RERHIVulkan/RenderTarget/RHIRenderPass.h"
#include "RERHIVulkan/VulkanContext.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

VkFormat SwapChain::findColorVkFormat(const RERHI::RHIContext &context, VkInstance vkInstance,
                                      const VulkanContext &vulkanContext) {
  const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
  const VkSurfaceKHR vkSurfaceKHR = detail::createPresentationSurface(context,
                                                                      vulkanContext.getRHIDynamicRHI().getVkAllocationCallbacks(),
                                                                      vkInstance, vkPhysicalDevice,
                                                                      vulkanContext.getGraphicsQueueFamilyIndex(),
                                                                      RERHI::WindowHandle{
                                                                        context.getNativeWindowHandle(), nullptr,
                                                                        nullptr});
  const VkSurfaceFormatKHR desiredVkSurfaceFormatKHR = ::detail::getSwapChainFormat(context, vkPhysicalDevice,
                                                                                    vkSurfaceKHR);
  vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, vulkanContext.getRHIDynamicRHI().getVkAllocationCallbacks());
  return desiredVkSurfaceFormatKHR.format;
}

VkFormat SwapChain::findDepthVkFormat(VkPhysicalDevice vkPhysicalDevice) {
  return detail::findSupportedVkFormat(vkPhysicalDevice, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                          VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL,
                                       VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


SwapChain::SwapChain(RERHI::RHIRenderPass &renderPass, RERHI::WindowHandle windowHandle
                     RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHISwapChain(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
// Operation system window
  mNativeWindowHandle(windowHandle.nativeWindowHandle),
  mRenderWindow(windowHandle.renderWindow),
// Vulkan presentation surface
  mVkSurfaceKHR(VK_NULL_HANDLE),
// Vulkan swap chain and color render target related
  mVkSwapchainKHR(VK_NULL_HANDLE),
  mVkRenderPass(VK_NULL_HANDLE),
  mImageAvailableVkSemaphore(VK_NULL_HANDLE),
  mRenderingFinishedVkSemaphore(VK_NULL_HANDLE),
  mCurrentImageIndex(~0u),
// Depth render target related
  mDepthVkFormat(
    Mapping::getVulkanFormat(static_cast<RenderPass &>(renderPass).getDepthStencilAttachmentTextureFormat())),
  mDepthVkImage(VK_NULL_HANDLE),
  mDepthVkDeviceMemory(VK_NULL_HANDLE),
  mDepthVkImageView(VK_NULL_HANDLE) {
  // Create the Vulkan presentation surface instance depending on the operation system
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(renderPass.getRhi());
  const VulkanContext &vulkanContext = vulkanRhi.getVulkanContext();
  const VkInstance vkInstance = vulkanRhi.getVulkanRuntimeLinking().getVkInstance();
  const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
  mVkSurfaceKHR = detail::createPresentationSurface(vulkanRhi.getContext(), vulkanRhi.getVkAllocationCallbacks(),
                                                    vkInstance, vkPhysicalDevice,
                                                    vulkanContext.getGraphicsQueueFamilyIndex(), windowHandle);
  if (VK_NULL_HANDLE != mVkSurfaceKHR) {
    // Create the Vulkan swap chain
    createVulkanSwapChain();
  } else {
    // Error!
    RE_LOG(Critical, "The swap chain failed to create the Vulkan presentation surface")
  }
}

/**
*  @brief
*    Destructor
*/
SwapChain::~SwapChain() {
  if (VK_NULL_HANDLE != mVkSurfaceKHR) {
    destroyVulkanSwapChain();
    const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    vkDestroySurfaceKHR(vulkanRhi.getVulkanRuntimeLinking().getVkInstance(), mVkSurfaceKHR,
                        vulkanRhi.getVkAllocationCallbacks());
  }
}

void SwapChain::getWidthAndHeight(uint32_t &width, uint32_t &height) const {
// Return stored width and height when both valid
  if (nullptr != mRenderWindow) {
    mRenderWindow->getWidthAndHeight(width, height);
    return;
  }
#ifdef _WIN32
  // Is there a valid native OS window?
          if (NULL_HANDLE != mNativeWindowHandle)
          {
            // Get the width and height
            long swapChainWidth  = 1;
            long swapChainHeight = 1;
            {
              // Get the client rectangle of the native output window
              // -> Don't use the width and height stored in "DXGI_SWAP_CHAIN_DESC" -> "DXGI_MODE_DESC"
              //    because it might have been modified in order to avoid zero values
              RECT rect;
              ::GetClientRect(reinterpret_cast<HWND>(mNativeWindowHandle), &rect);

              // Get the width and height...
              swapChainWidth  = rect.right  - rect.left;
              swapChainHeight = rect.bottom - rect.top;

              // ... and ensure that none of them is ever zero
              if (swapChainWidth < 1)
              {
                swapChainWidth = 1;
              }
              if (swapChainHeight < 1)
              {
                swapChainHeight = 1;
              }
            }

            // Write out the width and height
            width  = static_cast<UINT>(swapChainWidth);
            height = static_cast<UINT>(swapChainHeight);
          }
          else
#elif defined(__ANDROID__)
  if (NULL_HANDLE != mNativeWindowHandle)
          {
            // TODO(naetherm) Get size on Android
            width = height = 1;
          }
          else
#elif defined LINUX
  if (NULL_HANDLE != mNativeWindowHandle) {
    RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    const RERHI::RHIContext &context = vulkanRhi.getContext();
    RHI_ASSERT(context.getType() == RERHI::RHIContext::ContextType::X11, "Invalid Vulkan context type")

// If the given RHI context is an X11 context use the display connection object provided by the context
    if (context.getType() == RERHI::RHIContext::ContextType::X11) {
      const RERHI::X11Context &x11Context = static_cast<const RERHI::X11Context &>(context);
      Display *display = x11Context.getDisplay();

// Get the width and height...
      ::Window rootWindow = 0;
      int positionX = 0, positionY = 0;
      unsigned int unsignedWidth = 0, unsignedHeight = 0, border = 0, depth = 0;
      if (nullptr != display) {
        XGetGeometry(display, mNativeWindowHandle, &rootWindow, &positionX, &positionY, &unsignedWidth, &unsignedHeight,
                     &border, &depth);
      }

// ... and ensure that none of them is ever zero
      if (unsignedWidth < 1) {
        unsignedWidth = 1;
      }
      if (unsignedHeight < 1) {
        unsignedHeight = 1;
      }

// Done
      width = unsignedWidth;
      height = unsignedHeight;
    }
  } else
#else
#error "Unsupported platform"
#endif
  {
// Set known default return values
    width = 1;
    height = 1;
  }
}

void SwapChain::present() {
// TODO(naetherm) "RERHI::RHIRenderWindow::present()" support
/*
if (nullptr != mRenderWindow)
{
  mRenderWindow->present();
  return;
}
*/

// Get the Vulkan context
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const VulkanContext &vulkanContext = vulkanRhi.getVulkanContext();

  { // Queue submit
    const VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    const VkCommandBuffer vkCommandBuffer = vulkanContext.getVkCommandBuffer();
    const VkSubmitInfo vkSubmitInfo =
      {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,  // sType (VkStructureType)
        nullptr,            // pNext (const void*)
        1,                // waitSemaphoreCount (uint32_t)
        &mImageAvailableVkSemaphore,  // pWaitSemaphores (const VkSemaphore*)
        &waitDstStageMask,        // pWaitDstStageMask (const VkPipelineStageFlags*)
        1,                // commandBufferCount (uint32_t)
        &vkCommandBuffer,        // pCommandBuffers (const VkCommandBuffer*)
        1,                // signalSemaphoreCount (uint32_t)
        &mRenderingFinishedVkSemaphore  // pSignalSemaphores (const VkSemaphore*)
      };
    if (vkQueueSubmit(vulkanContext.getGraphicsVkQueue(), 1, &vkSubmitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
// Error!
      RE_LOG(Critical, "Vulkan queue submit failed")
      return;
    }
  }

  { // Queue present
    const VkPresentInfoKHR vkPresentInfoKHR =
      {
        VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,  // sType (VkStructureType)
        nullptr,              // pNext (const void*)
        1,                  // waitSemaphoreCount (uint32_t)
        &mRenderingFinishedVkSemaphore,    // pWaitSemaphores (const VkSemaphore*)
        1,                  // swapchainCount (uint32_t)
        &mVkSwapchainKHR,          // pSwapchains (const VkSwapchainKHR*)
        &mCurrentImageIndex,        // pImageIndices (const uint32_t*)
        nullptr                // pResults (VkResult*)
      };
    const VkResult vkResult = vkQueuePresentKHR(vulkanContext.getPresentVkQueue(), &vkPresentInfoKHR);
    if (VK_SUCCESS != vkResult) {
      if (VK_ERROR_OUT_OF_DATE_KHR == vkResult || VK_SUBOPTIMAL_KHR == vkResult) {
// Recreate the Vulkan swap chain
        createVulkanSwapChain();
        return;
      } else {
// Error!
        RE_LOG(Critical, "Failed to present Vulkan queue")
        return;
      }
    }
    vkQueueWaitIdle(vulkanContext.getPresentVkQueue());
  }

// Acquire next image
  acquireNextImage(true);
}


void SwapChain::createVulkanSwapChain() {
  // Get the Vulkan physical device
  const RHIDynamicRHI &vulkanRhi = static_cast<const RHIDynamicRHI &>(getRhi());
  const RERHI::RHIContext &context = vulkanRhi.getContext();
  const VulkanContext &vulkanContext = vulkanRhi.getVulkanContext();
  const VkPhysicalDevice vkPhysicalDevice = vulkanContext.getVkPhysicalDevice();
  const VkDevice vkDevice = vulkanContext.getVkDevice();

  // Sanity checks
  RHI_ASSERT(VK_NULL_HANDLE != vkPhysicalDevice, "Invalid physical Vulkan device")
  RHI_ASSERT(VK_NULL_HANDLE != vkDevice, "Invalid Vulkan device")

  // Wait for the Vulkan device to become idle
  vkDeviceWaitIdle(vkDevice);

  // Get Vulkan surface capabilities
  VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, mVkSurfaceKHR, &vkSurfaceCapabilitiesKHR) !=
      VK_SUCCESS) {
    RE_LOG(Critical, "Failed to get physical Vulkan device surface capabilities")
    return;
  }

  // Get Vulkan swap chain settings
  const uint32_t desiredNumberOfImages = ::detail::getNumberOfSwapChainImages(vkSurfaceCapabilitiesKHR);
  const VkSurfaceFormatKHR desiredVkSurfaceFormatKHR = ::detail::getSwapChainFormat(context, vkPhysicalDevice,
                                                                                    mVkSurfaceKHR);
  const VkExtent2D desiredVkExtent2D = ::detail::getSwapChainExtent(vkSurfaceCapabilitiesKHR);
  const VkImageUsageFlags desiredVkImageUsageFlags = ::detail::getSwapChainUsageFlags(context,
                                                                                      vkSurfaceCapabilitiesKHR);
  const VkSurfaceTransformFlagBitsKHR desiredVkSurfaceTransformFlagBitsKHR = ::detail::getSwapChainTransform(
    vkSurfaceCapabilitiesKHR);
  const VkPresentModeKHR desiredVkPresentModeKHR = ::detail::getSwapChainPresentMode(context, vkPhysicalDevice,
                                                                                     mVkSurfaceKHR);

  // Validate Vulkan swap chain settings
  if (-1 == static_cast<int>(desiredVkImageUsageFlags)) {
    RE_LOG(Critical, "Invalid desired Vulkan image usage flags")
    return;
  }
  if (VK_PRESENT_MODE_MAX_ENUM_KHR == desiredVkPresentModeKHR) {
    RE_LOG(Critical, "Invalid desired Vulkan presentation mode")
    return;
  }
  if ((0 == desiredVkExtent2D.width) || (0 == desiredVkExtent2D.height)) {
    // Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
    // But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
    destroyVulkanSwapChain();
    return;
  }

  { // Create Vulkan swap chain
    VkSwapchainKHR newVkSwapchainKHR = VK_NULL_HANDLE;
    const VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR =
      {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // sType (VkStructureType)
        nullptr,                    // pNext (const void*)
        0,                        // flags (VkSwapchainCreateFlagsKHR)
        mVkSurfaceKHR,                  // surface (VkSurfaceKHR)
        desiredNumberOfImages,              // minImageCount (uint32_t)
        desiredVkSurfaceFormatKHR.format,        // imageFormat (VkFormat)
        desiredVkSurfaceFormatKHR.colorSpace,      // imageColorSpace (VkColorSpaceKHR)
        desiredVkExtent2D,                // imageExtent (VkExtent2D)
        1,                        // imageArrayLayers (uint32_t)
        desiredVkImageUsageFlags,            // imageUsage (VkImageUsageFlags)
        VK_SHARING_MODE_EXCLUSIVE,            // imageSharingMode (VkSharingMode)
        0,                        // queueFamilyIndexCount (uint32_t)
        nullptr,                    // pQueueFamilyIndices (const uint32_t*)
        desiredVkSurfaceTransformFlagBitsKHR,      // preTransform (VkSurfaceTransformFlagBitsKHR)
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,        // compositeAlpha (VkCompositeAlphaFlagBitsKHR)
        desiredVkPresentModeKHR,            // presentMode (VkPresentModeKHR)
        VK_TRUE,                    // clipped (VkBool32)
        mVkSwapchainKHR                  // oldSwapchain (VkSwapchainKHR)
      };
    if (vkCreateSwapchainKHR(vkDevice, &vkSwapchainCreateInfoKHR, vulkanRhi.getVkAllocationCallbacks(),
                             &newVkSwapchainKHR) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to create Vulkan swap chain")
      return;
    }
    destroyVulkanSwapChain();
    mVkSwapchainKHR = newVkSwapchainKHR;
  }

  // Create depth render target
  createDepthRenderTarget(desiredVkExtent2D);

  // Create render pass
  mVkRenderPass = ::detail::createRenderPass(context, vulkanRhi.getVkAllocationCallbacks(), vkDevice,
                                             desiredVkSurfaceFormatKHR.format, mDepthVkFormat,
                                             static_cast<RenderPass &>(getRenderPass()).getVkSampleCountFlagBits());

  // Vulkan swap chain image handling
  if (VK_NULL_HANDLE != mVkRenderPass) {
    // Get the swap chain images
    uint32_t swapchainImageCount = 0;
    if (vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, nullptr) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to get Vulkan swap chain images")
      return;
    }
    std::vector<VkImage> vkImages(swapchainImageCount);
    if (vkGetSwapchainImagesKHR(vkDevice, mVkSwapchainKHR, &swapchainImageCount, vkImages.data()) != VK_SUCCESS) {
      RE_LOG(Critical, "Failed to get Vulkan swap chain images")
      return;
    }

    // Get the swap chain buffers containing the image and image view
    mSwapChainBuffer.resize(swapchainImageCount);
    const bool hasDepthStencilAttachment = (VK_FORMAT_UNDEFINED != mDepthVkFormat);
    for (uint32_t i = 0; i < swapchainImageCount; ++i) {
      SwapChainBuffer &swapChainBuffer = mSwapChainBuffer[i];
      swapChainBuffer.vkImage = vkImages[i];

      // Create the Vulkan image view
      Helper::createVkImageView(vulkanRhi, swapChainBuffer.vkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1,
                                desiredVkSurfaceFormatKHR.format, VK_IMAGE_ASPECT_COLOR_BIT,
                                swapChainBuffer.vkImageView);

      { // Create the Vulkan framebuffer
        const std::array<VkImageView, 2> vkImageViews =
          {
            swapChainBuffer.vkImageView,
            mDepthVkImageView
          };
        const VkFramebufferCreateInfo vkFramebufferCreateInfo =
          {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // sType (VkStructureType)
            nullptr,                  // pNext (const void*)
            0,                      // flags (VkFramebufferCreateFlags)
            mVkRenderPass,                // renderPass (VkRenderPass)
            hasDepthStencilAttachment ? 2u : 1u,    // attachmentCount (uint32_t)
            vkImageViews.data(),            // pAttachments (const VkImageView*)
            desiredVkExtent2D.width,          // width (uint32_t)
            desiredVkExtent2D.height,          // height (uint32_t)
            1                      // layers (uint32_t)
          };
        if (vkCreateFramebuffer(vkDevice, &vkFramebufferCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                                &swapChainBuffer.vkFramebuffer) != VK_SUCCESS) {
          RE_LOG(Critical, "Failed to create Vulkan framebuffer")
        }
      }
    }
  }

  { // Create the Vulkan semaphores
    static constexpr VkSemaphoreCreateInfo vkSemaphoreCreateInfo =
      {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,  // sType (VkStructureType)
        nullptr,                  // pNext (const void*)
        0                      // flags (VkSemaphoreCreateFlags)
      };
    if ((vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                           &mImageAvailableVkSemaphore) != VK_SUCCESS) ||
        (vkCreateSemaphore(vkDevice, &vkSemaphoreCreateInfo, vulkanRhi.getVkAllocationCallbacks(),
                           &mRenderingFinishedVkSemaphore) != VK_SUCCESS)) {
      RE_LOG(Critical, "Failed to create Vulkan semaphore")
    }
  }

  // Acquire next image
  acquireNextImage(false);
}

void SwapChain::destroyVulkanSwapChain() {
  // Destroy Vulkan swap chain
  if (VK_NULL_HANDLE != mVkRenderPass || !mSwapChainBuffer.empty() || VK_NULL_HANDLE != mVkSwapchainKHR) {
    const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    const VkDevice vkDevice = vulkanRhi.getVulkanContext().getVkDevice();
    vkDeviceWaitIdle(vkDevice);
    if (VK_NULL_HANDLE != mVkRenderPass) {
      vkDestroyRenderPass(vkDevice, mVkRenderPass, vulkanRhi.getVkAllocationCallbacks());
      mVkRenderPass = VK_NULL_HANDLE;
    }
    if (!mSwapChainBuffer.empty()) {
      for (const SwapChainBuffer &swapChainBuffer: mSwapChainBuffer) {
        vkDestroyFramebuffer(vkDevice, swapChainBuffer.vkFramebuffer, vulkanRhi.getVkAllocationCallbacks());
        vkDestroyImageView(vkDevice, swapChainBuffer.vkImageView, vulkanRhi.getVkAllocationCallbacks());
      }
      mSwapChainBuffer.clear();
    }
    if (VK_NULL_HANDLE != mVkSwapchainKHR) {
      vkDestroySwapchainKHR(vkDevice, mVkSwapchainKHR, vulkanRhi.getVkAllocationCallbacks());
      mVkSwapchainKHR = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != mImageAvailableVkSemaphore) {
      vkDestroySemaphore(vulkanRhi.getVulkanContext().getVkDevice(), mImageAvailableVkSemaphore,
                         vulkanRhi.getVkAllocationCallbacks());
      mImageAvailableVkSemaphore = VK_NULL_HANDLE;
    }
    if (VK_NULL_HANDLE != mRenderingFinishedVkSemaphore) {
      vkDestroySemaphore(vulkanRhi.getVulkanContext().getVkDevice(), mRenderingFinishedVkSemaphore,
                         vulkanRhi.getVkAllocationCallbacks());
      mRenderingFinishedVkSemaphore = VK_NULL_HANDLE;
    }
  }

  // Destroy depth render target
  destroyDepthRenderTarget();
}

void SwapChain::acquireNextImage(bool recreateSwapChainIfNeeded) {
  const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
  const VkResult vkResult = vkAcquireNextImageKHR(vulkanRhi.getVulkanContext().getVkDevice(), mVkSwapchainKHR,
                                                  UINT64_MAX, mImageAvailableVkSemaphore, VK_NULL_HANDLE,
                                                  &mCurrentImageIndex);
  if (VK_SUCCESS != vkResult && VK_SUBOPTIMAL_KHR != vkResult) {
    if (VK_ERROR_OUT_OF_DATE_KHR == vkResult) {
      // Recreate the Vulkan swap chain
      if (recreateSwapChainIfNeeded) {
        createVulkanSwapChain();
      }
    } else {
      // Error!
      RE_LOG(Critical, "Failed to acquire next Vulkan image from swap chain")
    }
  }
}

void SwapChain::createDepthRenderTarget(const VkExtent2D &vkExtent2D) {
  if (VK_FORMAT_UNDEFINED != mDepthVkFormat) {
    const RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());
    Helper::createAndAllocateVkImage(vulkanRhi, 0, VK_IMAGE_TYPE_2D, {vkExtent2D.width, vkExtent2D.height, 1}, 1, 1,
                                     mDepthVkFormat,
                                     static_cast<RenderPass &>(getRenderPass()).getVkSampleCountFlagBits(),
                                     VK_IMAGE_TILING_OPTIMAL,
                                     VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mDepthVkImage, mDepthVkDeviceMemory);
    Helper::createVkImageView(vulkanRhi, mDepthVkImage, VK_IMAGE_VIEW_TYPE_2D, 1, 1, mDepthVkFormat,
                              VK_IMAGE_ASPECT_DEPTH_BIT, mDepthVkImageView);
    // TODO(naetherm) File "racoonengine\source\rhi\private\vulkanrhi\vulkanrhi.cpp" | Line 1036 | Critical: Vulkan debug report callback: Object type: "VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT" Object: "103612336" Location: "0" Message code: "461375810" Layer prefix: "Validation" Message: " [ VUID-vkCmdPipelineBarrier-pMemoryBarriers-01185 ] Object: 0x62cffb0 (Type = 6) | vkCmdPipelineBarrier(): pImageMemBarriers[0].dstAccessMask (0x600) is not supported by dstStageMask (0x1). The spec valid usage text states 'Each element of pMemoryBarriers, pBufferMemoryBarriers and pImageMemoryBarriers must not have any access flag included in its dstAccessMask member if that bit is not supported by any of the pipeline stages in dstStageMask, as specified in the table of supported access types.' (https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VUID-vkCmdPipelineBarrier-pMemoryBarriers-01185)"
    //Helper::transitionVkImageLayout(vulkanRhi, mDepthVkImage, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  }
}

void SwapChain::destroyDepthRenderTarget() {
  if (VK_NULL_HANDLE != mDepthVkImage) {
    RHI_ASSERT(VK_NULL_HANDLE != mDepthVkDeviceMemory, "Invalid Vulkan depth device memory")
    RHI_ASSERT(VK_NULL_HANDLE != mDepthVkImageView, "Invalid Vulkan depth image view")
    Helper::destroyAndFreeVkImage(static_cast<RHIDynamicRHI &>(getRhi()), mDepthVkImage, mDepthVkDeviceMemory,
                                  mDepthVkImageView);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
