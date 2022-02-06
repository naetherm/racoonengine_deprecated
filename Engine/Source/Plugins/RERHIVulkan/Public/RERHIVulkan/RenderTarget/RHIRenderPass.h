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
*    Vulkan render pass interface
*/
class RenderPass final : public RERHI::RHIRenderPass
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
  *  @param[in] numberOfColorAttachments
  *    Number of color render target textures, must be <="RERHI::Capabilities::maximumNumberOfSimultaneousRenderTargets"
  *  @param[in] colorAttachmentTextureFormats
  *    The color render target texture formats, can be a null pointer or can contain null pointers, if not a null pointer there must be at
  *    least "numberOfColorAttachments" textures in the provided C-array of pointers
  *  @param[in] depthStencilAttachmentTextureFormat
  *    The optional depth stencil render target texture format, can be a "RERHI::TextureFormat::UNKNOWN" if there should be no depth buffer
  *  @param[in] numberOfMultisamples
  *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  */
  RenderPass(RHIDynamicRHI& vulkanRhi, uint32_t numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~RenderPass() override;
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
  *    Return the number of color render target textures
  *
  *  @return
  *    The number of color render target textures
  */
  [[nodiscard]] inline uint32_t getNumberOfColorAttachments() const
  {
    return mNumberOfColorAttachments;
  }

  /**
  *  @brief
  *    Return the number of render target textures (color and depth stencil)
  *
  *  @return
  *    The number of render target textures (color and depth stencil)
  */
  [[nodiscard]] inline uint32_t getNumberOfAttachments() const
  {
    return (mDepthStencilAttachmentTextureFormat != RERHI::TextureFormat::Enum::UNKNOWN) ? (mNumberOfColorAttachments + 1) : mNumberOfColorAttachments;
  }

  /**
  *  @brief
  *    Return the depth stencil attachment texture format
  *
  *  @return
  *    The depth stencil attachment texture format
  */
  [[nodiscard]] inline RERHI::TextureFormat::Enum getDepthStencilAttachmentTextureFormat() const
  {
    return mDepthStencilAttachmentTextureFormat;
  }

  /**
  *  @brief
  *    Return the Vulkan sample count flag bits
  *
  *  @return
  *    The Vulkan sample count flag bits
  */
  [[nodiscard]] inline VkSampleCountFlagBits getVkSampleCountFlagBits() const
  {
    return mVkSampleCountFlagBits;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), RenderPass, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RenderPass(const RenderPass& source) = delete;
  RenderPass& operator =(const RenderPass& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VkRenderPass			 mVkRenderPass;							///< Vulkan render pass instance, can be a null handle
  uint32_t				 mNumberOfColorAttachments;				///< Number of color render target textures
  RERHI::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;	///< The depth stencil attachment texture format
  VkSampleCountFlagBits	 mVkSampleCountFlagBits;				///< Vulkan sample count flag bits


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
