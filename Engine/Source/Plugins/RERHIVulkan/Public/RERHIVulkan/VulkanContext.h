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
*    Vulkan context class
*/
class VulkanContext final
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
  */
  explicit VulkanContext(RHIDynamicRHI& vulkanRhi);

  /**
  *  @brief
  *    Destructor
  */
  ~VulkanContext();

  /**
  *  @brief
  *    Return whether or not the content is initialized
  *
  *  @return
  *    "true" if the context is initialized, else "false"
  */
  [[nodiscard]] inline bool isInitialized() const
  {
    return (VK_NULL_HANDLE != mVkCommandBuffer);
  }

  /**
  *  @brief
  *    Return the owner Vulkan RHI instance
  *
  *  @return
  *    Owner Vulkan RHI instance
  */
  [[nodiscard]] inline RHIDynamicRHI& getRHIDynamicRHI() const
  {
    return mRHIDynamicRHI;
  }

  /**
  *  @brief
  *    Return the Vulkan physical device this context is using
  *
  *  @return
  *    The Vulkan physical device this context is using
  */
  [[nodiscard]] inline VkPhysicalDevice getVkPhysicalDevice() const
  {
    return mVkPhysicalDevice;
  }

  /**
  *  @brief
  *    Return the Vulkan device this context is using
  *
  *  @return
  *    The Vulkan device this context is using
  */
  [[nodiscard]] inline VkDevice getVkDevice() const
  {
    return mVkDevice;
  }

  /**
  *  @brief
  *    Return the used graphics queue family index
  *
  *  @return
  *    Graphics queue family index, ~0u if invalid
  */
  [[nodiscard]] inline RECore::uint32 getGraphicsQueueFamilyIndex() const
  {
    return mGraphicsQueueFamilyIndex;
  }

  /**
  *  @brief
  *    Return the used present queue family index
  *
  *  @return
  *    Present queue family index, ~0u if invalid
  */
  [[nodiscard]] inline RECore::uint32 getPresentQueueFamilyIndex() const
  {
    return mPresentQueueFamilyIndex;
  }

  /**
  *  @brief
  *    Return the handle to the Vulkan device graphics queue that command buffers are submitted to
  *
  *  @return
  *    Handle to the Vulkan device graphics queue that command buffers are submitted to
  */
  [[nodiscard]] inline VkQueue getGraphicsVkQueue() const
  {
    return mGraphicsVkQueue;
  }

  /**
  *  @brief
  *    Return the handle to the Vulkan device present queue
  *
  *  @return
  *    Handle to the Vulkan device present queue
  */
  [[nodiscard]] inline VkQueue getPresentVkQueue() const
  {
    return mPresentVkQueue;
  }

  /**
  *  @brief
  *    Return the used Vulkan command buffer pool instance
  *
  *  @return
  *    The used Vulkan command buffer pool instance
  */
  [[nodiscard]] inline VkCommandPool getVkCommandPool() const
  {
    return mVkCommandPool;
  }

  /**
  *  @brief
  *    Return the Vulkan command buffer instance
  *
  *  @return
  *    The Vulkan command buffer instance
  */
  [[nodiscard]] inline VkCommandBuffer getVkCommandBuffer() const
  {
    return mVkCommandBuffer;
  }

  // TODO(naetherm) Trivial implementation to have something to start with. Need to use more clever memory management and stating buffers later on.
  [[nodiscard]] RECore::uint32 findMemoryTypeIndex(RECore::uint32 typeFilter, VkMemoryPropertyFlags vkMemoryPropertyFlags) const;

  [[nodiscard]] VkCommandBuffer createVkCommandBuffer() const;

  void destroyVkCommandBuffer(VkCommandBuffer vkCommandBuffer) const;


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  explicit VulkanContext(const VulkanContext& source) = delete;
  VulkanContext& operator =(const VulkanContext& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RHIDynamicRHI&		 mRHIDynamicRHI;				///< Owner Vulkan RHI instance
  VkPhysicalDevice mVkPhysicalDevice;			///< Vulkan physical device this context is using
  VkDevice		 mVkDevice;					///< Vulkan device instance this context is using (equivalent of a OpenGL context or Direct3D 11 device)
  RECore::uint32		 mGraphicsQueueFamilyIndex;	///< Graphics queue family index, ~0u if invalid
  RECore::uint32		 mPresentQueueFamilyIndex;	///< Present queue family index, ~0u if invalid
  VkQueue			 mGraphicsVkQueue;			///< Handle to the Vulkan device graphics queue that command buffers are submitted to
  VkQueue			 mPresentVkQueue;			///< Handle to the Vulkan device present queue
  VkCommandPool	 mVkCommandPool;			///< Vulkan command buffer pool instance
  VkCommandBuffer  mVkCommandBuffer;			///< Vulkan command buffer instance


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
