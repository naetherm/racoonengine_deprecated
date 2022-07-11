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
#include "RERHIOpenGL/RenderTarget/RHIFramebufferBind.h"
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
FramebufferBind::FramebufferBind(RERHI::RHIRenderPass &renderPass,
                                 const RERHI::FramebufferAttachment *colorFramebufferAttachments,
                                 const RERHI::FramebufferAttachment *depthStencilFramebufferAttachment
                                 RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  Framebuffer(renderPass, colorFramebufferAttachments, depthStencilFramebufferAttachment
              RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
// Texture reference handling is done within the base class "Framebuffer"

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL framebuffer
          GLint openGLFramebufferBackup = 0;
          glGetIntegerv(GL_FRAMEBUFFER_BINDING, &openGLFramebufferBackup);
#endif

// Create the OpenGL framebuffer
  glGenFramebuffers(1, &mOpenGLFramebuffer);

// Bind this OpenGL framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, mOpenGLFramebuffer);

// Loop through all framebuffer color attachments
#ifdef DEBUG
  OpenGLRhi& openGLRhi = static_cast<OpenGLRhi&>(renderPass.getRhi());
#endif
  const RERHI::FramebufferAttachment *colorFramebufferAttachment = colorFramebufferAttachments;
  const RERHI::FramebufferAttachment *colorFramebufferAttachmentEnd =
    colorFramebufferAttachments + mNumberOfColorTextures;
  for (GLenum openGLAttachment = GL_COLOR_ATTACHMENT0;
       colorFramebufferAttachment < colorFramebufferAttachmentEnd; ++colorFramebufferAttachment, ++openGLAttachment) {
    RERHI::RHITexture *texture = colorFramebufferAttachment->texture;

// Sanity check: Is the given resource owned by this RHI?
    RHI_ASSERT(&openGLRhi == &texture->getRhi(),
               "OpenGL error: The given color texture at index %u is owned by another RHI instance",
               colorFramebufferAttachment - colorFramebufferAttachments)

// Evaluate the color texture type
    switch (texture->getResourceType()) {
      case RERHI::ResourceType::TEXTURE_2D: {
// Set the OpenGL framebuffer color attachment
        const Texture2D *texture2D = static_cast<const Texture2D *>(texture);
        glFramebufferTexture2D(GL_FRAMEBUFFER, openGLAttachment,
                               static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1)
                                                   ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
                               texture2D->getOpenGLTexture(),
                               static_cast<GLint>(colorFramebufferAttachment->mipmapIndex));
        if (!mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
        break;
      }

      case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
// Set the OpenGL framebuffer color attachment
        const Texture2DArray *texture2DArray = static_cast<const Texture2DArray *>(texture);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, openGLAttachment, texture2DArray->getOpenGLTexture(),
                                  static_cast<GLint>(colorFramebufferAttachment->mipmapIndex),
                                  static_cast<GLint>(colorFramebufferAttachment->layerIndex));
        if (!mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
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
                   "The type of the given color texture at index %ld is not supported by the OpenGL RHI implementation",
                   colorFramebufferAttachment - colorFramebufferAttachments)
        break;
    }
  }

// Depth stencil texture
  if (nullptr != mDepthStencilTexture) {
// Sanity check: Is the given resource owned by this RHI?
    RHI_ASSERT(&openGLRhi == &mDepthStencilTexture->getRhi(),
               "OpenGL error: The given depth stencil texture is owned by another RHI instance")

// Evaluate the depth stencil texture type
    switch (mDepthStencilTexture->getResourceType()) {
      case RERHI::ResourceType::TEXTURE_2D: {
        const Texture2D *texture2D = static_cast<const Texture2D *>(mDepthStencilTexture);

// Sanity checks
        RHI_ASSERT(depthStencilFramebufferAttachment->mipmapIndex <
                   Texture2D::getNumberOfMipmaps(texture2D->getWidth(), texture2D->getHeight()),
                   "Invalid OpenGL depth stencil framebuffer attachment mipmap index")
        RHI_ASSERT(0 == depthStencilFramebufferAttachment->layerIndex,
                   "Invalid OpenGL depth stencil framebuffer attachment layer index")

// Bind the depth stencil texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               static_cast<GLenum>((texture2D->getNumberOfMultisamples() > 1)
                                                   ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D),
                               texture2D->getOpenGLTexture(),
                               static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex));
        if (!mMultisampleRenderTarget && texture2D->getNumberOfMultisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
        break;
      }

      case RERHI::ResourceType::TEXTURE_2D_ARRAY: {
// Bind the depth stencil texture to framebuffer
        const Texture2DArray *texture2DArray = static_cast<const Texture2DArray *>(mDepthStencilTexture);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture2DArray->getOpenGLTexture(),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->mipmapIndex),
                                  static_cast<GLint>(depthStencilFramebufferAttachment->layerIndex));
        if (!mMultisampleRenderTarget && texture2DArray->getNumberOfMultisamples() > 1) {
          mMultisampleRenderTarget = true;
        }
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
                   "OpenGL error: The type of the given depth stencil texture is not supported by the OpenGL RHI implementation")
        break;
    }
  }

// Check the status of the OpenGL framebuffer
  const GLenum openGLStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch (openGLStatus) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      RHI_ASSERT(false,
                 "OpenGL error: Not all framebuffer attachment points are framebuffer attachment complete (\"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      RHI_ASSERT(false,
                 "OpenGL error: No images are attached to the framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      RHI_ASSERT(false, "OpenGL error: Incomplete draw buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      RHI_ASSERT(false, "OpenGL error: Incomplete read buffer framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\")")
      break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      RHI_ASSERT(false, "OpenGL error: Incomplete multisample framebuffer (\"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\")")
      break;

    case GL_FRAMEBUFFER_UNDEFINED:
      RHI_ASSERT(false, "OpenGL error: Undefined framebuffer (\"GL_FRAMEBUFFER_UNDEFINED\")")
      break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
      RHI_ASSERT(false,
                 "OpenGL error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions (\"GL_FRAMEBUFFER_UNSUPPORTED\")")
      break;

// From "GL_EXT_framebuffer_object" (should no longer matter, should)
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      RHI_ASSERT(false,
                 "OpenGL error: Not all attached images have the same width and height (\"GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\")")
      break;

// From "GL_EXT_framebuffer_object" (should no longer matter, should)
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      RHI_ASSERT(false,
                 "OpenGL error: Incomplete formats framebuffer object (\"GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\")")
      break;

    default:
    case GL_FRAMEBUFFER_COMPLETE:
// Nothing here
      break;
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL framebuffer
          glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(openGLFramebufferBackup));
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "FBO", 6)	// 6 = "FBO: " including terminating zero
            glObjectLabel(GL_FRAMEBUFFER, mOpenGLFramebuffer, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
