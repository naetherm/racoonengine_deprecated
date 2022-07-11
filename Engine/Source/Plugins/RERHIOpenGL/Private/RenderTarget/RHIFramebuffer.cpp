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
#include "RERHIOpenGL/RenderTarget/RHIFramebuffer.h"
#include "RERHIOpenGL/RenderTarget/RHIRenderPass.h"
#include "RERHIOpenGL/Texture/RHITexture2D.h"
#include "RERHIOpenGL/Texture/RHITexture2DArray.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Helper.h"


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
Framebuffer::~Framebuffer()
{
  // Destroy the OpenGL framebuffer
  // -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteFramebuffers(1, &mOpenGLFramebuffer);

  // Release the reference to the used color textures
  if (nullptr != mColorTextures)
  {
    // Release references
    RERHI::RHITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (RERHI::RHITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture)
    {
      (*colorTexture)->Release();
    }

    // Cleanup
    RHI_FREE(getRhi().getContext(), mColorTextures);
  }

  // Release the reference to the used depth stencil texture
  if (nullptr != mDepthStencilTexture)
  {
    // Release reference
    mDepthStencilTexture->Release();
  }
}

Framebuffer::Framebuffer(RERHI::RHIRenderPass& renderPass, const RERHI::FramebufferAttachment* colorFramebufferAttachments, const RERHI::FramebufferAttachment* depthStencilFramebufferAttachment RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLFramebuffer(0),
mNumberOfColorTextures(static_cast<RenderPass&>(renderPass).getNumberOfColorAttachments()),
mColorTextures(nullptr),	// Set below
mDepthStencilTexture(nullptr),
mWidth(UINT_MAX),
mHeight(UINT_MAX),
mMultisampleRenderTarget(false)
{
  // The "GL_ARB_framebuffer_object"-extension documentation says the following about the framebuffer width and height
  //   "If the attachment sizes are not all identical, rendering will be limited to the largest area that can fit in
  //    all of the attachments (i.e. an intersection of rectangles having a lower left of (0,0) and an upper right of
  //    (width,height) for each attachment)"

  // Add a reference to the used color textures
  if (mNumberOfColorTextures > 0)
  {
    mColorTextures = RHI_MALLOC_TYPED(renderPass.getRhi().getContext(), RERHI::RHITexture*, mNumberOfColorTextures);

    // Loop through all color textures
    RERHI::RHITexture** colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (RERHI::RHITexture** colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture, ++colorFramebufferAttachments)
    {
      // Sanity check
      RHI_ASSERT(nullptr != colorFramebufferAttachments->texture, "Invalid OpenGL color framebuffer attachment texture")

      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *colorTexture = colorFramebufferAttachments->texture;
      (*colorTexture)->AddReference();

      // Evaluate the color texture type
      switch ((*colorTexture)->getResourceType())
      {
        case RERHI::ResourceType::TEXTURE_2D:
        {
          const Texture2D* texture2D = static_cast<Texture2D*>(*colorTexture);

          // Sanity checks
          RHI_ASSERT(colorFramebufferAttachments->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL color framebuffer attachment mipmap index")
          RHI_ASSERT(0 == colorFramebufferAttachments->layerIndex, "Invalid OpenGL color framebuffer attachment layer index")

          // Update the framebuffer width and height if required
          Helper::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
          break;
        }

        case RERHI::ResourceType::TEXTURE_2D_ARRAY:
        {
          // Update the framebuffer width and height if required
          const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(*colorTexture);
          Helper::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
          break;
        }

        case RERHI::ResourceType::ROOT_SIGNATURE:
        case RERHI::ResourceType::RESOURCE_GROUP:
        case RERHI::ResourceType::GRAPHICS_PROGRAM:
        case RERHI::ResourceType::VERTEX_ARRAY:
        case RERHI::ResourceType::RENDER_PASS:
        case RERHI::ResourceType::QUERY_POOL:
        case RERHI::ResourceType::SWAP_CHAIN:
        case RERHI::ResourceType::FRAMEBUFFER:
        case RERHI::ResourceType::VERTEX_BUFFER:
        case RERHI::ResourceType::INDEX_BUFFER:
        case RERHI::ResourceType::TEXTURE_BUFFER:
        case RERHI::ResourceType::STRUCTURED_BUFFER:
        case RERHI::ResourceType::INDIRECT_BUFFER:
        case RERHI::ResourceType::UNIFORM_BUFFER:
        case RERHI::ResourceType::TEXTURE_1D:
        case RERHI::ResourceType::TEXTURE_1D_ARRAY:
        case RERHI::ResourceType::TEXTURE_3D:
        case RERHI::ResourceType::TEXTURE_CUBE:
        case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
        case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
        case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
        case RERHI::ResourceType::SAMPLER_STATE:
        case RERHI::ResourceType::VERTEX_SHADER:
        case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
        case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
        case RERHI::ResourceType::GEOMETRY_SHADER:
        case RERHI::ResourceType::FRAGMENT_SHADER:
        case RERHI::ResourceType::TASK_SHADER:
        case RERHI::ResourceType::MESH_SHADER:
        case RERHI::ResourceType::COMPUTE_SHADER:
        default:
          // Nothing here
          break;
      }
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment)
  {
    mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
    RHI_ASSERT(nullptr != mDepthStencilTexture, "Invalid OpenGL depth stencil framebuffer attachment texture")
    mDepthStencilTexture->AddReference();

    // Evaluate the depth stencil texture type
    switch (mDepthStencilTexture->getResourceType())
    {
      case RERHI::ResourceType::TEXTURE_2D:
      {
        const Texture2D* texture2D = static_cast<Texture2D*>(mDepthStencilTexture);

        // Sanity checks
        RHI_ASSERT(depthStencilFramebufferAttachment->mipmapIndex < Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()), "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
        RHI_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex, "Invalid OpenGL depth stencil framebuffer attachment layer index")

        // Update the framebuffer width and height if required
        Helper::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(), texture2D->getHeight(), mWidth, mHeight);
        break;
      }

      case RERHI::ResourceType::TEXTURE_2D_ARRAY:
      {
        // Update the framebuffer width and height if required
        const Texture2DArray* texture2DArray = static_cast<Texture2DArray*>(mDepthStencilTexture);
        Helper::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(), texture2DArray->getHeight(), mWidth, mHeight);
        break;
      }

      case RERHI::ResourceType::ROOT_SIGNATURE:
      case RERHI::ResourceType::RESOURCE_GROUP:
      case RERHI::ResourceType::GRAPHICS_PROGRAM:
      case RERHI::ResourceType::VERTEX_ARRAY:
      case RERHI::ResourceType::RENDER_PASS:
      case RERHI::ResourceType::QUERY_POOL:
      case RERHI::ResourceType::SWAP_CHAIN:
      case RERHI::ResourceType::FRAMEBUFFER:
      case RERHI::ResourceType::VERTEX_BUFFER:
      case RERHI::ResourceType::INDEX_BUFFER:
      case RERHI::ResourceType::TEXTURE_BUFFER:
      case RERHI::ResourceType::STRUCTURED_BUFFER:
      case RERHI::ResourceType::INDIRECT_BUFFER:
      case RERHI::ResourceType::UNIFORM_BUFFER:
      case RERHI::ResourceType::TEXTURE_1D:
      case RERHI::ResourceType::TEXTURE_1D_ARRAY:
      case RERHI::ResourceType::TEXTURE_3D:
      case RERHI::ResourceType::TEXTURE_CUBE:
      case RERHI::ResourceType::TEXTURE_CUBE_ARRAY:
      case RERHI::ResourceType::GRAPHICS_PIPELINE_STATE:
      case RERHI::ResourceType::COMPUTE_PIPELINE_STATE:
      case RERHI::ResourceType::SAMPLER_STATE:
      case RERHI::ResourceType::VERTEX_SHADER:
      case RERHI::ResourceType::TESSELLATION_CONTROL_SHADER:
      case RERHI::ResourceType::TESSELLATION_EVALUATION_SHADER:
      case RERHI::ResourceType::GEOMETRY_SHADER:
      case RERHI::ResourceType::FRAGMENT_SHADER:
      case RERHI::ResourceType::TASK_SHADER:
      case RERHI::ResourceType::MESH_SHADER:
      case RERHI::ResourceType::COMPUTE_SHADER:
      default:
        // Nothing here
        break;
    }
  }

  // Validate the framebuffer width and height
  if (0 == mWidth || UINT_MAX == mWidth)
  {
    RHI_ASSERT(false, "Invalid OpenGL framebuffer width")
    mWidth = 1;
  }
  if (0 == mHeight || UINT_MAX == mHeight)
  {
    RHI_ASSERT(false, "Invalid OpenGL framebuffer height")
    mHeight = 1;
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
