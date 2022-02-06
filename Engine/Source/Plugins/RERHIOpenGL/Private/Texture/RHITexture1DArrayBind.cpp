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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGL/Texture/RHITexture1DArrayBind.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Mapping.h"


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
Texture1DArrayBind::Texture1DArrayBind(RHIDynamicRHI &openGLRhi, uint32_t width, uint32_t numberOfSlices,
                                       RERHI::TextureFormat::Enum textureFormat, const void *data, uint32_t textureFlags
                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  Texture1DArray(openGLRhi, width, numberOfSlices, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
          GLint openGLAlignmentBackup = 0;
          glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

          // Backup the currently bound OpenGL texture
          GLint openGLTextureBackup = 0;
          glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY_EXT, &openGLTextureBackup);
#endif

// Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, (RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

// Create the OpenGL texture instance
  glGenTextures(1, &mOpenGLTexture);

// Make this OpenGL texture instance to the currently used one
  glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, mOpenGLTexture);

// TODO(naetherm) Add support for user provided mipmaps
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   etc.

// Upload the base map of the texture (mipmaps are automatically created as soon as the base map is changed)
  glTexImage2D(GL_TEXTURE_1D_ARRAY_EXT, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width),
               static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLFormat(textureFormat),
               Mapping::getOpenGLType(textureFormat), data);

// Build mipmaps automatically on the GPU? (or GPU driver)
  if ((textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) &&
      openGLRhi.getExtensions().isGL_ARB_framebuffer_object()) {
    glGenerateMipmap(GL_TEXTURE_1D_ARRAY_EXT);
    glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_1D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL texture
          glBindTexture(GL_TEXTURE_1D_ARRAY_EXT, static_cast<GLuint>(openGLTextureBackup));

          // Restore previous alignment
          glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
            glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
