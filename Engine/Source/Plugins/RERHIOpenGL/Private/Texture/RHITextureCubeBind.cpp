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
#include "RERHIOpenGL/Texture/RHITextureCubeBind.h"
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
TextureCubeBind::TextureCubeBind(RHIDynamicRHI &openGLRhi, uint32_t width, RERHI::TextureFormat::Enum textureFormat,
                                 const void *data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  TextureCube(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
// Sanity checks
  RHI_ASSERT(0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data,
             "Invalid OpenGL texture parameters")
  RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data,
             "OpenGL render target textures can't be filled using provided data")

// Create the OpenGL texture instance
  glGenTextures(1, &mOpenGLTexture);

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
          GLint openGLAlignmentBackup = 0;
          glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);

          // Backup the currently bound OpenGL texture
          GLint openGLTextureBackup = 0;
          glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &openGLTextureBackup);
#endif

// Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, (RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

// Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS));
  const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

// Make this OpenGL texture instance to the currently used one
  glBindTexture(GL_TEXTURE_CUBE_MAP, mOpenGLTexture);

// Upload the texture data
  if (RERHI::TextureFormat::isCompressed(textureFormat)) {
// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
//   etc.

// Upload all mipmaps of all faces
      for (uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, width));
        for (uint32_t face = 0; face < 6; ++face) {
// Upload the current face
          glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap),
                                    mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0,
                                    numberOfBytesPerSlice, data);

// Move on to the next face
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
        }

// Move on to the next mipmap and ensure the size is always at least 1x1
        width = getHalfSize(width);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      const uint32_t numberOfBytesPerSlice = RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width,
                                                                                            width);
      for (uint32_t face = 0; face < 6; ++face) {
        glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, mOpenGLInternalFormat,
                                  static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0,
                                  static_cast<GLsizei>(numberOfBytesPerSlice), data);
        data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
      }
    }
  } else {
// Texture format is not compressed

// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
//   etc.

// Upload all mipmaps of all faces
      const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
      const uint32_t type = Mapping::getOpenGLType(textureFormat);
      for (uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, width));
        for (uint32_t face = 0; face < 6; ++face) {
// Upload the current face
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap),
                       static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width),
                       static_cast<GLsizei>(width), 0, format, type, data);

// Move on to the next face
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
        }

// Move on to the next mipmap and ensure the size is always at least 1x1
        width = getHalfSize(width);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      const uint32_t numberOfBytesPerSlice = RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width,
                                                                                            width);
      const uint32_t openGLFormat = Mapping::getOpenGLFormat(textureFormat);
      const uint32_t openGLType = Mapping::getOpenGLType(textureFormat);
      for (uint32_t face = 0; face < 6; ++face) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, static_cast<GLint>(mOpenGLInternalFormat),
                     static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0, openGLFormat, openGLType, data);
        data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
      }
    }
  }

// Build mipmaps automatically on the GPU? (or GPU driver)
  if ((textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) &&
      openGLRhi.getExtensions().isGL_ARB_framebuffer_object()) {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL texture
          glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GLuint>(openGLTextureBackup));

          // Restore previous alignment
          glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Cube texture", 15)	// 15 = "Cube texture: " including terminating zero
            glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
