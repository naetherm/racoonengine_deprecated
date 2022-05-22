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
*    Vulkan framebuffer interface
*/
class Framebuffer final : public RERHI::RHIFramebuffer
{


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
  *  @param[in] colorFramebufferAttachments
  *    The color render target textures, can be a null pointer or can contain null pointers, if not a null pointer there must be at
  *    least "RERHI::RHIRenderPass::getNumberOfColorAttachments()" textures in the provided C-array of pointers
  *  @param[in] depthStencilFramebufferAttachment
  *    The depth stencil render target texture, can be a null pointer
  *
  *  @note
  *    - The framebuffer keeps a reference to the provided texture instances
  */
  Framebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~Framebuffer() override;

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
  *    Return the Vulkan framebuffer to render into
  *
  *  @return
  *    The Vulkan framebuffer to render into
  */
  [[nodiscard]] inline VkFramebuffer getVkFramebuffer() const
  {
    return mVkFramebuffer;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  inline virtual void getWidthAndHeight(RECore::uint32& width, RECore::uint32& height) const override
  {
    // No fancy implementation in here, just copy over the internal information
    width  = mWidth;
    height = mHeight;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Framebuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Framebuffer(const Framebuffer& source) = delete;
  Framebuffer& operator =(const Framebuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RECore::uint32		mNumberOfColorTextures;	///< Number of color render target textures
  RERHI::RHITexture** mColorTextures;			///< The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
  RERHI::RHITexture*  mDepthStencilTexture;	///< The depth stencil render target texture (we keep a reference to it), can be a null pointer
  RECore::uint32		mWidth;					///< The framebuffer width
  RECore::uint32		mHeight;				///< The framebuffer height
  VkRenderPass	mVkRenderPass;			///< Vulkan render pass instance, can be a null handle, we don't own it
  VkFramebuffer	mVkFramebuffer;			///< Vulkan framebuffer instance, can be a null handle


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
