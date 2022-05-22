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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/RERHIVulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class VulkanContext;


/**
*  @brief
*    Vulkan swap chain class
*
*  @todo
*    - TODO(naetherm) Add support for debug name (not that important while at the same time more complex to implement here, but lets keep the TODO here to know there's room for improvement)
*/
class SwapChain final : public RERHI::RHISwapChain
{


  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] static VkFormat findColorVkFormat(const RERHI::RHIContext& context, VkInstance vkInstance, const VulkanContext& vulkanContext);

  [[nodiscard]] static VkFormat findDepthVkFormat(VkPhysicalDevice vkPhysicalDevice);


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] renderPass
  *    Render pass to use, the swap chain keeps a reference to the render pass
  *  @param[in] windowHandle
  *    Information about the window to render into
  */
  SwapChain(RERHI::RHIRenderPass& renderPass, RERHI::WindowHandle windowHandle RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~SwapChain() override;

  /**
  *  @brief
  *    Return the Vulkan render pass
  *
  *  @return
  *    The Vulkan render pass
  */
  [[nodiscard]] inline VkRenderPass getVkRenderPass() const
  {
    return mVkRenderPass;
  }

  /**
  *  @brief
  *    Return the current Vulkan image to render color into
  *
  *  @return
  *    The current Vulkan image to render color into
  */
  [[nodiscard]] inline VkImage getColorCurrentVkImage() const
  {
    RHI_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
    RHI_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
    return mSwapChainBuffer[mCurrentImageIndex].vkImage;
  }

  /**
  *  @brief
  *    Return the Vulkan image to render depth into
  *
  *  @return
  *    The Vulkan image to render depth into
  */
  [[nodiscard]] inline VkImage getDepthVkImage() const
  {
    return mDepthVkImage;
  }

  /**
  *  @brief
  *    Return the current Vulkan framebuffer to render into
  *
  *  @return
  *    The current Vulkan framebuffer to render into
  */
  [[nodiscard]] inline VkFramebuffer getCurrentVkFramebuffer() const
  {
    RHI_ASSERT(~0u != mCurrentImageIndex, "Invalid index of the current Vulkan swap chain image to render into (Vulkan swap chain creation failed?)")
    RHI_ASSERT(mCurrentImageIndex < mSwapChainBuffer.size(), "Out-of-bounds index of the current Vulkan swap chain image to render into")
    return mSwapChainBuffer[mCurrentImageIndex].vkFramebuffer;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  virtual void getWidthAndHeight(RECore::uint32& width, RECore::uint32& height) const override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHISwapChain methods                ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual RECore::handle getNativeWindowHandle() const override
  {
    return mNativeWindowHandle;
  }

  inline virtual void setVerticalSynchronizationInterval(RECore::uint32) override
  {
    // TODO(naetherm) Implement usage of "synchronizationInterval"
  }

  virtual void present() override;

  inline virtual void resizeBuffers() override
  {
    // Recreate the Vulkan swap chain
    createVulkanSwapChain();
  }

  [[nodiscard]] inline virtual bool getFullscreenState() const override
  {
    // TODO(naetherm) Implement me
    return false;
  }

  inline virtual void setFullscreenState(bool) override
  {
    // TODO(naetherm) Implement me
  }

  inline virtual void setRenderWindow(RERHI::IRenderWindow* renderWindow) override
  {
    mRenderWindow = renderWindow;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), SwapChain, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit SwapChain(const SwapChain& source) = delete;
  SwapChain& operator =(const SwapChain& source) = delete;

  void createVulkanSwapChain();

  void destroyVulkanSwapChain();

  void acquireNextImage(bool recreateSwapChainIfNeeded);

  void createDepthRenderTarget(const VkExtent2D& vkExtent2D);

  void destroyDepthRenderTarget();


  //[-------------------------------------------------------]
  //[ Private definitions                                   ]
  //[-------------------------------------------------------]
private:
  struct SwapChainBuffer
  {
    VkImage		  vkImage		= VK_NULL_HANDLE;	///< Vulkan image, don't destroy since we don't own it
    VkImageView   vkImageView	= VK_NULL_HANDLE;	///< Vulkan image view, destroy if no longer needed
    VkFramebuffer vkFramebuffer	= VK_NULL_HANDLE;	///< Vulkan framebuffer, destroy if no longer needed
  };
  typedef std::vector<SwapChainBuffer> SwapChainBuffers;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // Operation system window
  RECore::handle			mNativeWindowHandle;	///< Native window handle window, can be a null handle
  RERHI::IRenderWindow* mRenderWindow;			///< Render window instance, can be a null pointer, don't destroy the instance since we don't own it
  // Vulkan presentation surface
  VkSurfaceKHR mVkSurfaceKHR;	///< Vulkan presentation surface, destroy if no longer needed
  // Vulkan swap chain and color render target related
  VkSwapchainKHR	 mVkSwapchainKHR;				///< Vulkan swap chain, destroy if no longer needed
  VkRenderPass	 mVkRenderPass;					///< Vulkan render pass, destroy if no longer needed (due to "VK_IMAGE_LAYOUT_PRESENT_SRC_KHR" we need an own Vulkan render pass instance)
  SwapChainBuffers mSwapChainBuffer;				///< Swap chain buffer for managing the color render targets
  VkSemaphore		 mImageAvailableVkSemaphore;	///< Vulkan semaphore, destroy if no longer needed
  VkSemaphore		 mRenderingFinishedVkSemaphore;	///< Vulkan semaphore, destroy if no longer needed
  RECore::uint32		 mCurrentImageIndex;			///< The index of the current Vulkan swap chain image to render into, ~0 if invalid
  // Depth render target related
  VkFormat		mDepthVkFormat;	///< Can be "VK_FORMAT_UNDEFINED" if no depth stencil buffer is needed
  VkImage			mDepthVkImage;
  VkDeviceMemory  mDepthVkDeviceMemory;
  VkImageView		mDepthVkImageView;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
