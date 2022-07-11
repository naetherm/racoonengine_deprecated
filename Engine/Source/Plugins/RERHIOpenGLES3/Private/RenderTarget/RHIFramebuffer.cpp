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
#include "RERHIOpenGLES3/RenderTarget/RHIFramebuffer.h"
#include "RERHIOpenGLES3/RenderTarget/RHIRenderPass.h"
#include "RERHIOpenGLES3/Texture/RHITexture2D.h"
#include "RERHIOpenGLES3/Texture/RHITexture2DArray.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Helper.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


Framebuffer::Framebuffer(RERHI::RHIRenderPass &renderPass,
                         const RERHI::FramebufferAttachment *colorFramebufferAttachments,
                         const RERHI::FramebufferAttachment *depthStencilFramebufferAttachment
                         RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3Framebuffer(0),
  mDepthRenderbuffer(0),
  mNumberOfColorTextures(static_cast<RenderPass &>(renderPass).getNumberOfColorAttachments()),
  mColorTextures(nullptr),  // Set below
  mDepthStencilTexture(nullptr),
  mWidth(1),
  mHeight(1) {
  // Unlike the "GL_ARB_framebuffer_object"-extension of OpenGL, in OpenGL ES 3 all
  // textures attached to the framebuffer must have the same width and height

  // Create the OpenGL ES 3 framebuffer
  glGenFramebuffers(1, &mOpenGLES3Framebuffer);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 framebuffer
        GLint openGLES3FramebufferBackup = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLES3FramebufferBackup);
#endif

  // Bind this OpenGL ES 3 framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLES3Framebuffer);

  // Are there any color textures? (usually there are, so we just keep the "glBindFramebuffer()" above without trying to make this method implementation more complex)
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(renderPass.getRhi());
  if (mNumberOfColorTextures > 0) {
    mColorTextures = RHI_MALLOC_TYPED(openGLES3Rhi.getContext(), RERHI::RHITexture*, mNumberOfColorTextures);

    // Loop through all framebuffer color attachments
    // -> "GL_COLOR_ATTACHMENT0" and "GL_COLOR_ATTACHMENT0_NV" have the same value
    RERHI::RHITexture **colorTexture = mColorTextures;
    const RERHI::FramebufferAttachment *colorFramebufferAttachment = colorFramebufferAttachments;
    const RERHI::FramebufferAttachment *colorFramebufferAttachmentEnd =
      colorFramebufferAttachments + mNumberOfColorTextures;
    for (GLenum openGLES3Attachment = GL_COLOR_ATTACHMENT0; colorFramebufferAttachment <
                                                            colorFramebufferAttachmentEnd; ++colorFramebufferAttachment, ++openGLES3Attachment, ++colorTexture) {
      // Sanity check
      RHI_ASSERT(nullptr != colorFramebufferAttachments->texture,
                 "Invalid OpenGL ES 3 color framebuffer attachment texture")

      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *colorTexture = colorFramebufferAttachment->texture;
      (*colorTexture)->AddReference();

      // Security check: Is the given resource owned by this RHI?
#ifdef DEBUG
      if (&openGLES3Rhi != &(*colorTexture)->getRhi())
            {
              // Output an error message and keep on going in order to keep a reasonable behaviour even in case on an error
              RE_LOG(Critical, "OpenGL ES 3 error: The given color texture at index %u is owned by another RHI instance", colorTexture - mColorTextures)

              // Continue, there's no point in trying to do any error handling in here
              continue;
            }
#endif

      // Evaluate the color texture type
      switch ((*colorTexture)->getResourceType()) {
        case RERHI::ResourceType::TEXTURE_2D: {
          const Texture2D *texture2D = static_cast<Texture2D *>(*colorTexture);

          // Sanity checks
          RHI_ASSERT(colorFramebufferAttachments->mipmapIndex <
                     Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()),
                     "Invalid OpenGL ES 3 color framebuffer attachment mipmap index")
          RHI_ASSERT(0 == colorFramebufferAttachment->layerIndex,
                     "Invalid OpenGL ES 3 color framebuffer attachment layer index")

          // Set the OpenGL ES 3 framebuffer color attachment
          glFramebufferTexture2D(GL_FRAMEBUFFER, openGLES3Attachment, GL_TEXTURE_2D, texture2D->getOpenGLES3Texture(),
                                 static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));

          // Update the framebuffer width and height if required
          ::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2D->getWidth(),
                                      texture2D->getHeight(), mWidth, mHeight);
          break;
        }

        case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
          // Set the OpenGL ES 3 framebuffer color attachment
          const Texture2DArray *texture2DArray = static_cast<Texture2DArray *>(*colorTexture);
          glFramebufferTextureLayer(GL_FRAMEBUFFER, openGLES3Attachment, texture2DArray->getOpenGLES3Texture(),
                                    static_cast<GLint>(colorFramebufferAttachment->mipmapIndex),
                                    static_cast<GLint>(colorFramebufferAttachment->layerIndex));

          // If this is the primary render target, get the framebuffer width and height
          ::detail::updateWidthHeight(colorFramebufferAttachments->mipmapIndex, texture2DArray->getWidth(),
                                      texture2DArray->getHeight(), mWidth, mHeight);
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
          RHI_ASSERT(false,
                     "The type of the given color texture at index %ld is not supported by the OpenGL ES 3 RHI implementation",
                     colorTexture - mColorTextures)
          break;
      }
    }
  }

  // Add a reference to the used depth stencil texture
  if (nullptr != depthStencilFramebufferAttachment) {
    mDepthStencilTexture = depthStencilFramebufferAttachment->texture;
    RHI_ASSERT(nullptr != mDepthStencilTexture, "Invalid OpenGL ES 3 depth stencil framebuffer attachment texture")
    mDepthStencilTexture->AddReference();

    // Evaluate the color texture type
    switch (mDepthStencilTexture->getResourceType()) {
      case RERHI::ResourceType::TEXTURE_2D: {
        const Texture2D *texture2D = static_cast<const Texture2D *>(mDepthStencilTexture);

        // Sanity checks
        RHI_ASSERT(depthStencilFramebufferAttachment->mipmapIndex <
                   Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()),
                   "Invalid OpenGL ES 3 depth stencil framebuffer attachment mipmap index")
        RHI_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex,
                   "Invalid OpenGL ES 3 depth stencil framebuffer attachment layer index")

        // Bind the depth stencil texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture2D->getOpenGLES3Texture(),
                               static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));

        // Update the framebuffer width and height if required
        ::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2D->getWidth(),
                                    texture2D->getHeight(), mWidth, mHeight);
        break;
      }

      case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
        // Bind the depth stencil texture to framebuffer
        const Texture2DArray *texture2DArray = static_cast<const Texture2DArray *>(mDepthStencilTexture);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture2DArray->getOpenGLES3Texture(),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));

        // Update the framebuffer width and height if required
        ::detail::updateWidthHeight(depthStencilFramebufferAttachment->mipmapIndex, texture2DArray->getWidth(),
                                    texture2DArray->getHeight(), mWidth, mHeight);
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
        RHI_ASSERT(false,
                   "The type of the given depth stencil texture is not supported by the OpenGL ES 3 RHI implementation")
        break;
    }
  }

  // Check the status of the OpenGL ES 3 framebuffer
  const GLenum openGLES3Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (openGLES3Status) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      RHI_ASSERT(false,
                 "OpenGL ES 3 error: Not all framebuffer attachment points are framebuffer attachment complete (\"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      RHI_ASSERT(false,
                 "OpenGL ES 3 error: No images are attached to the framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\")")
      break;

      // Not supported by OpenGL ES 3
      //	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      //		RHI_ASSERT(false, "OpenGL ES 3 error: Incomplete draw buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\")")
      //		break;

      // Not supported by OpenGL ES 3
      //	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      //		RHI_ASSERT(false, "OpenGL ES 3 error: Incomplete read buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\")")
      //		break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      RHI_ASSERT(false,
                 "OpenGL ES 3 error: Incomplete multisample framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\")")
      break;

    case GL_FRAMEBUFFER_UNDEFINED:
      RHI_ASSERT(false, "OpenGL ES 3 error: Undefined framebuffer (\"GL_FRAMEBUFFER_UNDEFINED\")")
      break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
      RHI_ASSERT(false,
                 "OpenGL ES 3 error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions (\"GL_FRAMEBUFFER_UNSUPPORTED\")")
      break;

      // Not supported by OpenGL ES 3
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      RHI_ASSERT(false,
                 "OpenGL ES 3 error: Not all attached images have the same width and height (\"GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\")")
      break;

      // Not supported by OpenGL ES 3
      // OpenGL: From "GL_EXT_framebuffer_object" (should no longer matter, should)
      //	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      //		RHI_ASSERT(false, "OpenGL ES 3 error: Incomplete formats framebuffer object (\"GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\")")
      //		break;

    default:
    case GL_FRAMEBUFFER_COMPLETE:
      // Nothing here
      break;
  }

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLES3FramebufferBackup));
#endif

  // Validate the framebuffer width and height
  if (0 == mWidth || UINT_MAX == mWidth) {
    RHI_ASSERT(false, "Invalid OpenGL ES 3 framebuffer width")
    mWidth = 1;
  }
  if (0 == mHeight || UINT_MAX == mHeight) {
    RHI_ASSERT(false, "Invalid OpenGL ES 3 framebuffer height")
    mHeight = 1;
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
        {
          RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
          glObjectLabelKHR(GL_FRAMEBUFFER, mOpenGLES3Framebuffer, -1, detailedDebugName);
        }
#endif
}

/**
*  @brief
*    Destructor
*/
Framebuffer::~Framebuffer() {
// Destroy the OpenGL ES 3 framebuffer
// -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteFramebuffers(1, &mOpenGLES3Framebuffer);
  glDeleteRenderbuffers(1, &mDepthRenderbuffer);

// Release the reference to the used color textures
  if (nullptr != mColorTextures) {
// Release references
    RERHI::RHITexture **colorTexturesEnd = mColorTextures + mNumberOfColorTextures;
    for (RERHI::RHITexture **colorTexture = mColorTextures; colorTexture < colorTexturesEnd; ++colorTexture) {
      (*colorTexture)->Release();
    }

// Cleanup
    RHI_FREE(getRhi().getContext(), mColorTextures);
  }

// Release the reference to the used depth stencil texture
  if (nullptr != mDepthStencilTexture) {
// Release reference
    mDepthStencilTexture->Release();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
