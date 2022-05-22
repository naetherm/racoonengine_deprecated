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
#include "RERHIOpenGLES3/Texture/RHITexture1DArray.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


Texture1DArray::Texture1DArray(RHIDynamicRHI &openGLES3Rhi, RECore::uint32 width, RECore::uint32 numberOfSlices,
                               RERHI::TextureFormat::Enum textureFormat, const void *data, RECore::uint32 textureFlags
                               RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHITexture1DArray(openGLES3Rhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3Texture(0) {
// OpenGL ES 3 has no 1D texture arrays, just use a 2D texture array with a height of one

// TODO(naetherm) Check support formats

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently set alignment
          GLint openGLES3AlignmentBackup = 0;
          glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLES3AlignmentBackup);

          // Backup the currently bound OpenGL ES 3 texture
          GLint openGLES3TextureBackup = 0;
          glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &openGLES3TextureBackup);
#endif

// Set correct alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// Create the OpenGL ES 3 texture instance
  glGenTextures(1, &mOpenGLES3Texture);
  glBindTexture(GL_TEXTURE_2D_ARRAY, mOpenGLES3Texture);

// TODO(naetherm) Add support for user provided mipmaps
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   etc.

// Upload the base map of the texture (mipmaps are automatically created as soon as the base map is changed)
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, Mapping::getOpenGLES3InternalFormat(textureFormat), static_cast<GLsizei>(width),
               1, static_cast<GLsizei>(numberOfSlices), 0, Mapping::getOpenGLES3Format(textureFormat),
               Mapping::getOpenGLES3Type(textureFormat), data);

// Build mipmaps automatically on the GPU? (or GPU driver)
  if (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) {
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 texture
          glBindTexture(GL_TEXTURE_2D_ARRAY, static_cast<GLuint>(openGLES3TextureBackup));

          // Restore previous alignment
          glPixelStorei(GL_UNPACK_ALIGNMENT, openGLES3AlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture array", 19)	// 19 = "1D texture array: " including terminating zero
            glObjectLabelKHR(GL_TEXTURE, mOpenGLES3Texture, -1, detailedDebugName);
          }
#endif
}

/**
*  @brief
*    Destructor
*/
Texture1DArray::~Texture1DArray() {
// Destroy the OpenGL ES 3 texture instance
// -> Silently ignores 0's and names that do not correspond to existing textures
  glDeleteTextures(1, &mOpenGLES3Texture);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
