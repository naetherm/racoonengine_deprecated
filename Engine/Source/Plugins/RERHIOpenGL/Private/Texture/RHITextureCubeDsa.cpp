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
#include "RERHIOpenGL/Texture/RHITextureCubeDsa.h"
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
TextureCubeDsa::TextureCubeDsa(RHIDynamicRHI &openGLRhi, uint32_t width, RERHI::TextureFormat::Enum textureFormat,
                               const void *data, uint32_t textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  TextureCube(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
// Sanity checks
  RHI_ASSERT(0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data,
             "Invalid OpenGL texture parameters")
  RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data,
             "OpenGL render target textures can't be filled using provided data")

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently set alignment
          GLint openGLAlignmentBackup = 0;
          glGetIntegerv(GL_UNPACK_ALIGNMENT, &openGLAlignmentBackup);
#endif

// Set correct unpack alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, (RERHI::TextureFormat::getNumberOfBytesPerElement(textureFormat) & 3) ? 1 : 4);

// Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS));
  const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

// Create the OpenGL texture instance
// TODO(naetherm) "GL_ARB_direct_state_access" AMD graphics card driver bug ahead
// -> AMD graphics card: 13.02.2017 using Radeon software 17.1.1 on Microsoft Windows: Looks like "GL_ARB_direct_state_access" is broken when trying to use "glCompressedTextureSubImage3D()" for upload
// -> Describes the same problem: https://community.amd.com/thread/194748 - "Upload data to GL_TEXTURE_CUBE_MAP with glTextureSubImage3D (DSA) broken ?"
#ifdef _WIN32
  const bool isArbDsa = false;
#else
  const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
#endif
  if (isArbDsa) {
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mOpenGLTexture);
    glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat,
                       static_cast<GLsizei>(width), static_cast<GLsizei>(width));
  } else {
    glGenTextures(1, &mOpenGLTexture);
  }

// Upload the texture data
  if (RERHI::TextureFormat::isCompressed(textureFormat)) {
// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Face0, Face1, Face2, Face3, Face4, Face5
//   Mip1: Face0, Face1, Face2, Face3, Face4, Face5
//   etc.

// Upload all mipmaps of all faces
      const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
      for (uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, width));
        if (isArbDsa) {
// With ARB DSA cube maps are a special form of a cube map array so we can upload all 6 faces at once per mipmap
// See https://www.khronos.org/opengl/wiki/Direct_State_Access (Last paragraph in "Changes from EXT")
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0,
                                        static_cast<GLsizei>(width), static_cast<GLsizei>(width), 6, format,
                                        numberOfBytesPerSlice * 6, data);

// Move on to the next mipmap
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice * 6;
        } else {
          for (uint32_t face = 0; face < 6; ++face) {
// Upload the current face
            glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                          static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width),
                                          static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);

// Move on to the next face
            data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
          }
        }

// Move on to the next mipmap and ensure the size is always at least 1x1
        width = getHalfSize(width);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width),
                                        static_cast<GLsizei>(width), 6, Mapping::getOpenGLFormat(textureFormat),
                                        static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
                                          textureFormat, width, width)) * 6, data);
        }
      } else {
        const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
          textureFormat, width, width));
        for (uint32_t face = 0; face < 6; ++face) {
// Upload the current face
          glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, mOpenGLInternalFormat,
                                        static_cast<GLsizei>(width), static_cast<GLsizei>(width), 0,
                                        numberOfBytesPerSlice, data);

// Move on to the next face
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
        }
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
          if (isArbDsa) {
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
            glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLint>(face),
                                static_cast<GLsizei>(width), static_cast<GLsizei>(width), 1, format, type, data);
          } else {
            glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, static_cast<GLint>(mipmap),
                                static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width),
                                static_cast<GLsizei>(width), 0, format, type, data);
          }

// Move on to the next face
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
        }

// Move on to the next mipmap and ensure the size is always at least 1x1
        width = getHalfSize(width);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(width), 6,
                              Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
        }
      } else {
        const uint32_t numberOfBytesPerSlice = RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width,
                                                                                              width);
        const uint32_t openGLFormat = Mapping::getOpenGLFormat(textureFormat);
        const uint32_t openGLType = Mapping::getOpenGLType(textureFormat);
        for (uint32_t face = 0; face < 6; ++face) {
          glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,
                              static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width),
                              static_cast<GLsizei>(width), 0, openGLFormat, openGLType, data);
          data = static_cast<const uint8_t *>(data) + numberOfBytesPerSlice;
        }
      }
    }
  }

// Build mipmaps automatically on the GPU? (or GPU driver)
  if (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) {
    if (isArbDsa) {
      glGenerateTextureMipmap(mOpenGLTexture);
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP);
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
  } else {
    if (isArbDsa) {
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  if (isArbDsa) {
    glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
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
