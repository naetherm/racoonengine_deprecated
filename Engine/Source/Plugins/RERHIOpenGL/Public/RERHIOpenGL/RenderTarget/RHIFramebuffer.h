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
#include "RERHIOpenGL/RERHIOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OpenGL framebuffer interface
*/
class Framebuffer : public RERHI::RHIFramebuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  virtual ~Framebuffer() override;

  /**
  *  @brief
  *    Return the OpenGL framebuffer
  *
  *  @return
  *    The OpenGL framebuffer, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLFramebuffer() const
  {
    return mOpenGLFramebuffer;
  }

  /**
  *  @brief
  *    Return the number of color render target textures
  *
  *  @return
  *    The number of color render target textures
  */
  [[nodiscard]] inline uint32_t getNumberOfColorTextures() const
  {
    return mNumberOfColorTextures;
  }

  /**
  *  @brief
  *    Return whether or not the framebuffer is a multisample render target
  *
  *  @return
  *    "true" if the framebuffer is a multisample render target, else "false"
  */
  [[nodiscard]] inline bool isMultisampleRenderTarget() const
  {
    return mMultisampleRenderTarget;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void* getInternalResourceHandle() const override
  {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLFramebuffer));
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRenderTarget methods             ]
  //[-------------------------------------------------------]
public:
  inline virtual void getWidthAndHeight(uint32_t& width, uint32_t& height) const override
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
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
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
  Framebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint			mOpenGLFramebuffer;			///< OpenGL framebuffer ("container" object, not shared between OpenGL contexts), can be zero if no resource is allocated
  uint32_t		mNumberOfColorTextures;		///< Number of color render target textures
  RERHI::RHITexture** mColorTextures;				///< The color render target textures (we keep a reference to it), can be a null pointer or can contain null pointers, if not a null pointer there must be at least "mNumberOfColorTextures" textures in the provided C-array of pointers
  RERHI::RHITexture*  mDepthStencilTexture;		///< The depth stencil render target texture (we keep a reference to it), can be a null pointer
  uint32_t		mWidth;						///< The framebuffer width
  uint32_t		mHeight;					///< The framebuffer height
  bool			mMultisampleRenderTarget;	///< Multisample render target?


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Framebuffer(const Framebuffer& source) = delete;
  Framebuffer& operator =(const Framebuffer& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
