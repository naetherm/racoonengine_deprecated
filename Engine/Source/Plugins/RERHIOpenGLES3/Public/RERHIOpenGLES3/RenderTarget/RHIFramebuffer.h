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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    OpenGL ES 3 framebuffer class
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
  *    Return the OpenGL ES 3 framebuffer
  *
  *  @return
  *    The OpenGL ES 3 framebuffer, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3Framebuffer() const
  {
    return mOpenGLES3Framebuffer;
  }

  /**
  *  @brief
  *    Return the number of color render target textures
  *
  *  @return
  *    The number of color render target textures
  */
  [[nodiscard]] inline RECore::uint32 getNumberOfColorTextures() const
  {
    return mNumberOfColorTextures;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void* getInternalResourceHandle() const override
  {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLES3Framebuffer));
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
  GLuint			mOpenGLES3Framebuffer;	///< OpenGL ES 3 framebuffer, can be zero if no resource is allocated
  GLuint			mDepthRenderbuffer;		///< OpenGL ES render buffer for the depth component, can be zero if no resource is allocated
  RECore::uint32		mNumberOfColorTextures;	///< Number of color render target textures
  RERHI::RHITexture** mColorTextures;			///< The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
  RERHI::RHITexture*  mDepthStencilTexture;	///< The depth stencil render target texture (we keep a reference to it), can be a null pointer
  RECore::uint32		mWidth;					///< The framebuffer width
  RECore::uint32		mHeight;				///< The framebuffer height


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
