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


/**
*  @brief
*    Vulkan runtime linking for creating and managing the Vulkan instance ("VkInstance")
*/
class VulkanRuntimeLinking final
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] enableValidation
  *    Enable validation?
  */
  VulkanRuntimeLinking(RHIDynamicRHI& vulkanRhi, bool enableValidation);

  /**
  *  @brief
  *    Destructor
  */
  ~VulkanRuntimeLinking();

  /**
  *  @brief
  *    Return whether or not validation is enabled
  *
  *  @return
  *    "true" if validation is enabled, else "false"
  */
  [[nodiscard]] inline bool isValidationEnabled() const
  {
    return mValidationEnabled;
  }

  /**
  *  @brief
  *    Return whether or not Vulkan is available
  *
  *  @return
  *    "true" if Vulkan is available, else "false"
  */
  [[nodiscard]] bool isVulkanAvailable();

  /**
  *  @brief
  *    Return the Vulkan instance
  *
  *  @return
  *    Vulkan instance
  */
  [[nodiscard]] inline VkInstance getVkInstance() const
  {
    return mVkInstance;
  }

  /**
  *  @brief
  *    Load the device level Vulkan function entry points
  *
  *  @param[in] vkDevice
  *    Vulkan device instance to load the function entry pointers for
  *  @param[in] hasMeshShaderSupport
  *    Has mesh shader support?
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadDeviceLevelVulkanEntryPoints(VkDevice vkDevice, bool hasMeshShaderSupport) const;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit VulkanRuntimeLinking(const VulkanRuntimeLinking& source) = delete;
  VulkanRuntimeLinking& operator =(const VulkanRuntimeLinking& source) = delete;

  /**
  *  @brief
  *    Load the shared libraries
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadSharedLibraries()
  {
    // Load the shared library
#ifdef _WIN32
    mVulkanSharedLibrary = ::LoadLibraryExA("vulkan-1.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
				if (nullptr == mVulkanSharedLibrary)
				{
					RE_LOG(Critical, "Failed to load in the shared Vulkan library \"vulkan-1.dll\"")
				}
#elif defined LINUX
    mVulkanSharedLibrary = ::dlopen("libvulkan.so", RTLD_NOW);
    if (nullptr == mVulkanSharedLibrary)
    {
      RE_LOG(Critical, "Failed to load in the shared Vulkan library \"libvulkan-1.so\"")
    }
#else
#error "Unsupported platform"
#endif

    // Done
    return (nullptr != mVulkanSharedLibrary);
  }

  /**
  *  @brief
  *    Load the global level Vulkan function entry points
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadGlobalLevelVulkanEntryPoints() const;

  /**
  *  @brief
  *    Create the Vulkan instance
  *
  *  @param[in] enableValidation
  *    Enable validation layer? (don't do this for shipped products)
  *
  *  @return
  *    Vulkan instance creation result
  */
  [[nodiscard]] VkResult createVulkanInstance(bool enableValidation);

  /**
  *  @brief
  *    Load the instance level Vulkan function entry points
  *
  *  @return
  *    "true" if all went fine, else "false"
  */
  [[nodiscard]] bool loadInstanceLevelVulkanEntryPoints() const;

  /**
  *  @brief
  *    Setup debug callback
  */
  void setupDebugCallback();


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI&				 mRHIDynamicRHI;						///< Owner Vulkan RHI instance
  bool					 mValidationEnabled;				///< Validation enabled?
  void*					 mVulkanSharedLibrary;				///< Vulkan shared library, can be a null pointer
  bool					 mEntryPointsRegistered;			///< Entry points successfully registered?
  VkInstance				 mVkInstance;						///< Vulkan instance, stores all per-application states
  VkDebugReportCallbackEXT mVkDebugReportCallbackEXT;			///< Vulkan debug report callback, can be a null handle
  bool					 mInstanceLevelFunctionsRegistered;	///< Instance level Vulkan function pointers registered?
  bool					 mInitialized;						///< Already initialized?


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan


#ifdef VULKAN_DEFINE_RUNTIMELINKING
#define FNPTR(name) PFN_##name name;
#else
#define FNPTR(name) extern PFN_##name name;
#endif

// Global Vulkan function pointers
FNPTR(vkGetInstanceProcAddr)
FNPTR(vkGetDeviceProcAddr)
FNPTR(vkEnumerateInstanceExtensionProperties)
FNPTR(vkEnumerateInstanceLayerProperties)
FNPTR(vkCreateInstance)

// Instance based Vulkan function pointers
FNPTR(vkDestroyInstance)
FNPTR(vkEnumeratePhysicalDevices)
FNPTR(vkEnumerateDeviceLayerProperties)
FNPTR(vkEnumerateDeviceExtensionProperties)
FNPTR(vkGetPhysicalDeviceQueueFamilyProperties)
FNPTR(vkGetPhysicalDeviceFeatures)
FNPTR(vkGetPhysicalDeviceFormatProperties)
FNPTR(vkGetPhysicalDeviceMemoryProperties)
FNPTR(vkGetPhysicalDeviceProperties)
FNPTR(vkCreateDevice)
// "VK_EXT_debug_report"-extension
FNPTR(vkCreateDebugReportCallbackEXT)
FNPTR(vkDestroyDebugReportCallbackEXT)
// "VK_KHR_surface"-extension
FNPTR(vkDestroySurfaceKHR)
FNPTR(vkGetPhysicalDeviceSurfaceSupportKHR)
FNPTR(vkGetPhysicalDeviceSurfaceFormatsKHR)
FNPTR(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
FNPTR(vkGetPhysicalDeviceSurfacePresentModesKHR)
#ifdef VK_USE_PLATFORM_WIN32_KHR
// "VK_KHR_win32_surface"-extension
	FNPTR(vkCreateWin32SurfaceKHR)
#elif defined VK_USE_PLATFORM_ANDROID_KHR
// "VK_KHR_android_surface"-extension
	#warning "TODO(naetherm) Not tested"
	FNPTR(vkCreateAndroidSurfaceKHR)
#elif defined VK_USE_PLATFORM_XLIB_KHR || defined VK_USE_PLATFORM_WAYLAND_KHR
#if defined VK_USE_PLATFORM_XLIB_KHR
// "VK_KHR_xlib_surface"-extension
FNPTR(vkCreateXlibSurfaceKHR)
#endif
#if defined VK_USE_PLATFORM_WAYLAND_KHR
// "VK_KHR_wayland_surface"-extension
FNPTR(vkCreateWaylandSurfaceKHR)
#endif
#elif defined VK_USE_PLATFORM_XCB_KHR
// "VK_KHR_xcb_surface"-extension
	#warning "TODO(naetherm) Not tested"
	FNPTR(vkCreateXcbSurfaceKHR)
#else
	#error "Unsupported platform"
#endif

// Device based Vulkan function pointers
FNPTR(vkDestroyDevice)
FNPTR(vkCreateShaderModule)
FNPTR(vkDestroyShaderModule)
FNPTR(vkCreateBuffer)
FNPTR(vkDestroyBuffer)
FNPTR(vkMapMemory)
FNPTR(vkUnmapMemory)
FNPTR(vkCreateBufferView)
FNPTR(vkDestroyBufferView)
FNPTR(vkAllocateMemory)
FNPTR(vkFreeMemory)
FNPTR(vkGetBufferMemoryRequirements)
FNPTR(vkBindBufferMemory)
FNPTR(vkCreateRenderPass)
FNPTR(vkDestroyRenderPass)
FNPTR(vkCreateImage)
FNPTR(vkDestroyImage)
FNPTR(vkGetImageSubresourceLayout)
FNPTR(vkGetImageMemoryRequirements)
FNPTR(vkBindImageMemory)
FNPTR(vkCreateImageView)
FNPTR(vkDestroyImageView)
FNPTR(vkCreateSampler)
FNPTR(vkDestroySampler)
FNPTR(vkCreateSemaphore)
FNPTR(vkDestroySemaphore)
FNPTR(vkCreateFence)
FNPTR(vkDestroyFence)
FNPTR(vkWaitForFences)
FNPTR(vkCreateCommandPool)
FNPTR(vkDestroyCommandPool)
FNPTR(vkAllocateCommandBuffers)
FNPTR(vkFreeCommandBuffers)
FNPTR(vkBeginCommandBuffer)
FNPTR(vkEndCommandBuffer)
FNPTR(vkGetDeviceQueue)
FNPTR(vkQueueSubmit)
FNPTR(vkQueueWaitIdle)
FNPTR(vkDeviceWaitIdle)
FNPTR(vkCreateFramebuffer)
FNPTR(vkDestroyFramebuffer)
FNPTR(vkCreatePipelineCache)
FNPTR(vkDestroyPipelineCache)
FNPTR(vkCreatePipelineLayout)
FNPTR(vkDestroyPipelineLayout)
FNPTR(vkCreateGraphicsPipelines)
FNPTR(vkCreateComputePipelines)
FNPTR(vkDestroyPipeline)
FNPTR(vkCreateDescriptorPool)
FNPTR(vkDestroyDescriptorPool)
FNPTR(vkCreateDescriptorSetLayout)
FNPTR(vkDestroyDescriptorSetLayout)
FNPTR(vkAllocateDescriptorSets)
FNPTR(vkFreeDescriptorSets)
FNPTR(vkUpdateDescriptorSets)
FNPTR(vkCreateQueryPool)
FNPTR(vkDestroyQueryPool)
FNPTR(vkGetQueryPoolResults)
FNPTR(vkCmdBeginQuery)
FNPTR(vkCmdEndQuery)
FNPTR(vkCmdResetQueryPool)
FNPTR(vkCmdWriteTimestamp)
FNPTR(vkCmdCopyQueryPoolResults)
FNPTR(vkCmdPipelineBarrier)
FNPTR(vkCmdBeginRenderPass)
FNPTR(vkCmdEndRenderPass)
FNPTR(vkCmdExecuteCommands)
FNPTR(vkCmdCopyImage)
FNPTR(vkCmdBlitImage)
FNPTR(vkCmdCopyBufferToImage)
FNPTR(vkCmdClearAttachments)
FNPTR(vkCmdCopyBuffer)
FNPTR(vkCmdBindDescriptorSets)
FNPTR(vkCmdBindPipeline)
FNPTR(vkCmdSetViewport)
FNPTR(vkCmdSetScissor)
FNPTR(vkCmdSetLineWidth)
FNPTR(vkCmdSetDepthBias)
FNPTR(vkCmdPushConstants)
FNPTR(vkCmdBindIndexBuffer)
FNPTR(vkCmdBindVertexBuffers)
FNPTR(vkCmdDraw)
FNPTR(vkCmdDrawIndexed)
FNPTR(vkCmdDrawIndirect)
FNPTR(vkCmdDrawIndexedIndirect)
FNPTR(vkCmdDispatch)
FNPTR(vkCmdClearColorImage)
FNPTR(vkCmdClearDepthStencilImage)
// "VK_EXT_debug_marker"-extension
FNPTR(vkDebugMarkerSetObjectTagEXT)
FNPTR(vkDebugMarkerSetObjectNameEXT)
FNPTR(vkCmdDebugMarkerBeginEXT)
FNPTR(vkCmdDebugMarkerEndEXT)
FNPTR(vkCmdDebugMarkerInsertEXT)
// "VK_KHR_swapchain"-extension
FNPTR(vkCreateSwapchainKHR)
FNPTR(vkDestroySwapchainKHR)
FNPTR(vkGetSwapchainImagesKHR)
FNPTR(vkAcquireNextImageKHR)
FNPTR(vkQueuePresentKHR)
// "VK_NV_mesh_shader"-extension
FNPTR(vkCmdDrawMeshTasksNV)

#undef FNPTR