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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHINull/RenderTarget/RHIRenderPass.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

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
RenderPass::RenderPass(RERHI::RHIDynamicRHI& rhi, RECore::uint32 numberOfColorAttachments, const RERHI::TextureFormat::Enum* colorAttachmentTextureFormats, RERHI::TextureFormat::Enum depthStencilAttachmentTextureFormat, RECore::uint8 numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIRenderPass(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mNumberOfColorAttachments(numberOfColorAttachments),
mDepthStencilAttachmentTextureFormat(depthStencilAttachmentTextureFormat),
mNumberOfMultisamples(numberOfMultisamples)
{
  RHI_ASSERT(mNumberOfColorAttachments < 8, "Invalid number of null color attachments")
  memcpy(mColorAttachmentTextureFormats, colorAttachmentTextureFormats, sizeof(RERHI::TextureFormat::Enum) * mNumberOfColorAttachments);
}

/**
*  @brief
*    Destructor
*/
RenderPass::~RenderPass() {}

/**
*  @brief
*    Return the number of color render target textures
*
*  @return
*    The number of color render target textures
*/
RECore::uint32 RenderPass::getNumberOfColorAttachments() const
{
  return mNumberOfColorAttachments;
}


void RenderPass::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), RenderPass, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
