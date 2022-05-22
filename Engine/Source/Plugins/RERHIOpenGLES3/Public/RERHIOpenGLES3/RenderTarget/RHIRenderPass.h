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
*    OpenGL ES 3 render pass interface
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
  *  @param[in] rhi
  *    Owner RHI instance
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
  RenderPass(RERHI::RHIDynamicRHI& rhi, RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RenderPass() override
  {}

  /**
  *  @brief
  *    Return the number of color render target textures
  *
  *  @return
  *    The number of color render target textures
  */
  [[nodiscard]] inline RECore::uint32 getNumberOfColorAttachments() const
  {
    return mNumberOfColorAttachments;
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
  RECore::uint32				 mNumberOfColorAttachments;
  RERHI::TextureFormat::Enum mColorAttachmentTextureFormats[8];
  RERHI::TextureFormat::Enum mDepthStencilAttachmentTextureFormat;
  RECore::uint8					 mNumberOfMultisamples;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
