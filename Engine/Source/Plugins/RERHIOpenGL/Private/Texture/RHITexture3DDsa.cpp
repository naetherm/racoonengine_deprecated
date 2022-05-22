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
#include "RERHIOpenGL/Texture/RHITexture3DDsa.h"
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
Texture3DDsa::Texture3DDsa(RHIDynamicRHI &openGLRhi, RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth,
                           RERHI::TextureFormat::Enum textureFormat, const void *data, RECore::uint32 textureFlags,
                           RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  Texture3D(openGLRhi, width, height, depth, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
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

// Create OpenGL pixel unpack buffer for dynamic textures, if necessary
  if (RERHI::TextureUsage::IMMUTABLE != textureUsage) {
// Create the OpenGL pixel unpack buffer
    glCreateBuffers(1, &mOpenGLPixelUnpackBuffer);

// The OpenGL pixel unpack buffer must be able to hold the top-level mipmap
// TODO(naetherm) Or must the OpenGL pixel unpack buffer able to hold the entire texture including all mipmaps? Depends on the later usage I assume.
    const RECore::uint32 numberOfBytes = RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth;
    glNamedBufferData(mOpenGLPixelUnpackBuffer, static_cast<GLsizeiptr>(numberOfBytes), nullptr, GL_STREAM_DRAW);
  }

// Calculate the number of mipmaps
  const bool dataContainsMipmaps = (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS);
  const bool generateMipmaps = (!dataContainsMipmaps && (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS));
  RHI_ASSERT(RERHI::TextureUsage::IMMUTABLE != textureUsage || !generateMipmaps,
             "OpenGL immutable texture usage can't be combined with automatic mipmap generation")
  const RECore::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height, depth)
                                                                            : 1;

// Create the OpenGL texture instance
  const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
  if (isArbDsa) {
    glCreateTextures(GL_TEXTURE_3D, 1, &mOpenGLTexture);
    glTextureStorage3D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat,
                       static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth));
  } else {
    glGenTextures(1, &mOpenGLTexture);
  }

// Upload the texture data
  if (RERHI::TextureFormat::isCompressed(textureFormat)) {
// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   etc.

// Upload all mipmaps
      const RECore::uint32 format = Mapping::getOpenGLFormat(textureFormat);
      for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
// Upload the current mipmap
        const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(
          RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
        if (isArbDsa) {
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glCompressedTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0,
                                        static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                                        static_cast<GLsizei>(depth), format, numberOfBytesPerMipmap, data);
        } else {
          glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap), format,
                                        static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                                        static_cast<GLsizei>(depth), 0, numberOfBytesPerMipmap, data);
        }

// Move on to the next mipmap and ensure the size is always at least 1x1x1
        data = static_cast<const RECore::uint8 *>(data) + numberOfBytesPerMipmap;
        width = getHalfSize(width);
        height = getHalfSize(height);
        depth = getHalfSize(depth);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glCompressedTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width),
                                        static_cast<GLsizei>(height), static_cast<GLsizei>(depth),
                                        Mapping::getOpenGLFormat(textureFormat),
                                        static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(
                                          textureFormat, width, height)), data);
        }
      } else {
        glCompressedTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, mOpenGLInternalFormat,
                                      static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                                      static_cast<GLsizei>(depth), 0,
                                      static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat,
                                                                                                          width,
                                                                                                          height)),
                                      data);
      }
    }
  } else {
// Texture format is not compressed

// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
    if (dataContainsMipmaps) {
// Data layout: The RHI provides: CRN and KTX files are organized in mip-major order, like this:
//   Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
//   etc.

// Upload all mipmaps
      const RECore::uint32 format = Mapping::getOpenGLFormat(textureFormat);
      const RECore::uint32 type = Mapping::getOpenGLType(textureFormat);
      for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap) {
// Upload the current mipmap
        const GLsizei numberOfBytesPerMipmap = static_cast<GLsizei>(
          RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height) * depth);
        if (isArbDsa) {
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
          glTextureSubImage3D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, 0, static_cast<GLsizei>(width),
                              static_cast<GLsizei>(height), static_cast<GLsizei>(depth), format, type, data);
        } else {
          glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, static_cast<GLint>(mipmap),
                              static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width),
                              static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0, format, type, data);
        }

// Move on to the next mipmap and ensure the size is always at least 1x1x1
        data = static_cast<const RECore::uint8 *>(data) + numberOfBytesPerMipmap;
        width = getHalfSize(width);
        height = getHalfSize(height);
        depth = getHalfSize(depth);
      }
    } else {
// The user only provided us with the base texture, no mipmaps
      if (isArbDsa) {
        if (nullptr != data) {
          glTextureSubImage3D(mOpenGLTexture, 0, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height),
                              static_cast<GLsizei>(depth), Mapping::getOpenGLFormat(textureFormat),
                              Mapping::getOpenGLType(textureFormat), data);
        }
      } else {
        glTextureImage3DEXT(mOpenGLTexture, GL_TEXTURE_3D, 0, static_cast<GLint>(mOpenGLInternalFormat),
                            static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(depth), 0,
                            Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
      }
    }
  }

// Build mipmaps automatically on the GPU? (or GPU driver)
  if (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS) {
    if (isArbDsa) {
      glGenerateTextureMipmap(mOpenGLTexture);
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    } else {
      glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_3D);
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
  } else {
    if (isArbDsa) {
      glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  if (isArbDsa) {
    glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Restore previous alignment
          glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "3D texture", 13)	// 13 = "3D texture: " including terminating zero
            glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
          }
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
