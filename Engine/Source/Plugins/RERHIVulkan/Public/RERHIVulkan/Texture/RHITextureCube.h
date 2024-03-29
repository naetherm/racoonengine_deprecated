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
*    Vulkan cube texture interface
*/
class TextureCube final : public RERHI::RHITextureCube
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
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer
  *  @param[in] textureFlags
  *    Texture flags, see "RERHI::TextureFlag::Enum"
  */
  TextureCube(RHIDynamicRHI& vulkanRhi, RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~TextureCube() override;

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


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), TextureCube, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TextureCube(const TextureCube& source) = delete;
  TextureCube& operator =(const TextureCube& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VkImage		   mVkImage;
  VkImageLayout  mVkImageLayout;
  VkDeviceMemory mVkDeviceMemory;
  VkImageView	   mVkImageView;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
