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
#include "RERHIOpenGLES3/Texture/RHITexture2D.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


Texture2D::Texture2D(RHIDynamicRHI &openGLES3Rhi, RECore::uint32 width, RECore::uint32 height,
                     RERHI::TextureFormat::Enum textureFormat, const void *data, RECore::uint32 textureFlags
                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHITexture2D(openGLES3Rhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3Texture(0) {
// Sanity checks
  RHI_ASSERT(0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data,
             "Invalid OpenGL ES 3 texture parameters")
  RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data,
             "OpenGL ES 3 render target textures can't be filled using provided data")

// TODO(naetherm) Check support formats

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently set alignment
          GLint openGLES3AlignmentBackup = 0;
          glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLES3AlignmentBackup);

          // Backup the currently bound OpenGL ES 3 texture
          GLint openGLES3TextureBackup = 0;
          glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLES3TextureBackup);
#endif

// Set correct alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS));
  const RECore::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height) : 1;

// Create the OpenGL ES 3 texture instance
  glGenTextures(1, &mOpenGLES3Texture);
  glBindTexture(GL_TEXTURE_2D, mOpenGLES3Texture);

// Upload the texture data
  if (RERHI::TextureFormat::isCompressed(textureFormat)) {
// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Upload all mipmaps
      const GLenum internalFormat = Mapping::getOpenGLES3InternalFormat(textureFormat);
      for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
// Upload the current mipmap
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, height));
        glCompressedTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(mipmap), internalFormat, static_cast<GLsizei>(width),
                               static_cast<GLsizei>(height), 0, numberOfBytesPerSlice, data);

// Move on to the next mipmap and ensure the size is always at least 1x1
        data = static_cast<const RECore::uint8 *>(data) + numberOfBytesPerSlice;
        width = getHalfSize(width);
        height = getHalfSize(height);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      glCompressedTexImage2D(GL_TEXTURE_2D, 0, Mapping::getOpenGLES3InternalFormat(textureFormat),
                             static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0,
                             static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width,
                                                                                                 height)), data);
    }
  } else {
// Texture format is not compressed

// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Upload all mipmaps
      const GLenum internalFormat = Mapping::getOpenGLES3InternalFormat(textureFormat);
      const GLenum format = Mapping::getOpenGLES3Format(textureFormat);
      const GLenum type = Mapping::getOpenGLES3Type(textureFormat);
      for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
// Upload the current mipmap
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, height));
        glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(mipmap), internalFormat, static_cast<GLsizei>(width),
                     static_cast<GLsizei>(height), 0, format, type, data);

// Move on to the next mipmap and ensure the size is always at least 1x1
        data = static_cast<const RECore::uint8 *>(data) + numberOfBytesPerSlice;
        width = getHalfSize(width);
        height = getHalfSize(height);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      glTexImage2D(GL_TEXTURE_2D, 0, Mapping::getOpenGLES3InternalFormat(textureFormat), static_cast<GLsizei>(width),
                   static_cast<GLsizei>(height), 0, Mapping::getOpenGLES3Format(textureFormat),
                   Mapping::getOpenGLES3Type(textureFormat), data);
    }
  }

// Build mipmaps automatically on the GPU? (or GPU driver)
  if (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) {
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 texture
          glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLES3TextureBackup));

          // Restore previous alignment
          glPixelStorei(GL_UNPACK_ALIGNMENT, openGLES3AlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
            glObjectLabelKHR(GL_TEXTURE, mOpenGLES3Texture, -1, detailedDebugName);
          }
#endif
}

/**
*  @brief
*    Destructor
*/
Texture2D::~Texture2D() {
// Destroy the OpenGL ES 3 texture instance
// -> Silently ignores 0's and names that do not correspond to existing textures
  glDeleteTextures(1, &mOpenGLES3Texture);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
