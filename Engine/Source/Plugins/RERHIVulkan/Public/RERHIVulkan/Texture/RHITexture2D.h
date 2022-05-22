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
*    OpenVR-support: Data required for passing Vulkan textures to IVRCompositor::Submit; Be sure to call OpenVR_Shutdown before destroying these resources
*
*  @note
*    - From OpenVR SDK 1.0.7 "openvr.h"-header
*/
struct VRVulkanTextureData_t
{
  VkImage			 m_nImage;
  VkDevice		 m_pDevice;
  VkPhysicalDevice m_pPhysicalDevice;
  VkInstance		 m_pInstance;
  VkQueue			 m_pQueue;
  RECore::uint32		 m_nQueueFamilyIndex;
  RECore::uint32		 m_nWidth;
  RECore::uint32		 m_nHeight;
  VkFormat		 m_nFormat;
  RECore::uint32		 m_nSampleCount;
};

/**
*  @brief
*    Vulkan 2D texture interface
*/
class Texture2D final : public RERHI::RHITexture2D
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
  *  @param[in] width
  *    Texture width, must be >0
  *  @param[in] height
  *    Texture height, must be >0
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer
  *  @param[in] textureFlags
  *    Texture flags, see "RERHI::TextureFlag::Enum"
  *  @param[in] numberOfMultisamples
  *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  */
  Texture2D(RHIDynamicRHI& vulkanRhi, RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture2D() override;

  /**
  *  @brief
  *    Return the Vulkan image view
  *
  *  @return
  *    The Vulkan image view
  */
  [[nodiscard]] inline VkImageView getVkImageView() const
  {
    return mVkImageView;
  }

  /**
  *  @brief
  *    Return the Vulkan image layout
  *
  *  @return
  *    The Vulkan image layout
  */
  [[nodiscard]] inline VkImageLayout getVkImageLayout() const
  {
    return mVkImageLayout;
  }

  /**
  *  @brief
  *    Return the Vulkan format
  *
  *  @return
  *    The Vulkan format
  */
  [[nodiscard]] inline VkFormat getVkFormat() const
  {
    return mVrVulkanTextureData.m_nFormat;
  }

  /**
  *  @brief
  *    Set minimum maximum mipmap index
  *
  *  @param[in] minimumMipmapIndex
  *    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
  *  @param[in] maximumMipmapIndex
  *    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
  */
  inline void setMinimumMaximumMipmapIndex([[maybe_unused]] RECore::uint32 minimumMipmapIndex, [[maybe_unused]] RECore::uint32 maximumMipmapIndex)
  {
    // TODO(naetherm) Implement me
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void* getInternalResourceHandle() const override
  {
    return reinterpret_cast<void*>(const_cast<VRVulkanTextureData_t*>(&mVrVulkanTextureData));
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Texture2D, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture2D(const Texture2D& source) = delete;
  Texture2D& operator =(const Texture2D& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VRVulkanTextureData_t mVrVulkanTextureData;
  VkImageLayout		  mVkImageLayout;
  VkDeviceMemory		  mVkDeviceMemory;
  VkImageView			  mVkImageView;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
