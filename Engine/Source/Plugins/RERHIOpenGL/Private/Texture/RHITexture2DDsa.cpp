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
#include "RERHIOpenGL/Texture/RHITexture2DDsa.h"
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
Texture2DDsa::Texture2DDsa(RHIDynamicRHI& openGLRhi, uint32_t width, uint32_t height, RERHI::TextureFormat::Enum textureFormat, const void* data, uint32_t textureFlags, uint8_t numberOfMultisamples RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
Texture2D(openGLRhi, width, height, textureFormat, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER)
{
// Sanity checks
RHI_ASSERT(numberOfMultisamples == 1 || numberOfMultisamples == 2 || numberOfMultisamples == 4 || numberOfMultisamples == 8, "Invalid OpenGL texture parameters")
RHI_ASSERT(numberOfMultisamples == 1 || nullptr == data, "Invalid OpenGL texture parameters")
RHI_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS), "Invalid OpenGL texture parameters")
RHI_ASSERT(numberOfMultisamples == 1 || 0 == (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS), "Invalid OpenGL texture parameters")
RHI_ASSERT(numberOfMultisamples == 1 || 0 != (textureFlags & RERHI::TextureFlag::RENDER_TARGET), "Invalid OpenGL texture parameters")
RHI_ASSERT(0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

// Multisample texture?
const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();
if (numberOfMultisamples > 1)
{
if (isArbDsa)
{
// Create the OpenGL texture instance
glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &mOpenGLTexture);
glTextureStorage2DMultisample(mOpenGLTexture, numberOfMultisamples, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);
}
else
{
// Create the OpenGL texture instance
glGenTextures(1, &mOpenGLTexture);

#ifdef RHI_OPENGL_STATE_CLEANUP
// Backup the currently bound OpenGL texture
						GLint openGLTextureBackup = 0;
						glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, &openGLTextureBackup);
#endif

// Make this OpenGL texture instance to the currently used one
glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mOpenGLTexture);

// Define the texture
// -> Sadly, there's no direct state access (DSA) function defined for this in "GL_EXT_direct_state_access"
glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, numberOfMultisamples, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_TRUE);

#ifdef RHI_OPENGL_STATE_CLEANUP
// Be polite and restore the previous bound OpenGL texture
						glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, static_cast<GLuint>(openGLTextureBackup));
#endif
}
}
else
{
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
const uint32_t numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width, height) : 1;

// Create the OpenGL texture instance
if (isArbDsa)
{
glCreateTextures(GL_TEXTURE_2D, 1, &mOpenGLTexture);
glTextureStorage2D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}
else
{
glGenTextures(1, &mOpenGLTexture);
}

// Upload the texture data
if (RERHI::TextureFormat::isCompressed(textureFormat))
{
// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
if (dataContainsMipmaps)
{
// Upload all mipmaps
const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
for (uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap)
{
// Upload the current mipmap
const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
if (isArbDsa)
{
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
glCompressedTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), format, numberOfBytesPerSlice, data);
}
else
{
glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, numberOfBytesPerSlice, data);
}

// Move on to the next mipmap and ensure the size is always at least 1x1
data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
width = getHalfSize(width);
height = getHalfSize(height);
}
}
else
{
// The user only provided us with the base texture, no mipmaps
if (isArbDsa)
{
if (nullptr != data)
{
glCompressedTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
}
}
else
{
glCompressedTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height)), data);
}
}
}
else
{
// Texture format is not compressed

// Did the user provided data containing mipmaps from 0-n down to 1x1 linearly in memory?
if (dataContainsMipmaps)
{
// Upload all mipmaps
const uint32_t format = Mapping::getOpenGLFormat(textureFormat);
const uint32_t type = Mapping::getOpenGLType(textureFormat);
for (uint32_t mipmap = 0; mipmap < numberOfMipmaps; ++mipmap)
{
// Upload the current mipmap
const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, height));
if (isArbDsa)
{
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
glTextureSubImage2D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), format, type, data);
}
else
{
glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format, type, data);
}

// Move on to the next mipmap and ensure the size is always at least 1x1
data = static_cast<const uint8_t*>(data) + numberOfBytesPerSlice;
width = getHalfSize(width);
height = getHalfSize(height);
}
}
else
{
// The user only provided us with the base texture, no mipmaps
if (isArbDsa)
{
if (nullptr != data)
{
glTextureSubImage2D(mOpenGLTexture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
}
}
else
{
glTextureImage2DEXT(mOpenGLTexture, GL_TEXTURE_2D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
}
}
}

// Build mipmaps automatically on the GPU? (or GPU driver)
if (textureFlags & RERHI::TextureFlag::GENERATE_MIPMAPS)
{
if (isArbDsa)
{
glGenerateTextureMipmap(mOpenGLTexture);
glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}
else
{
glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_2D);
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}
}
else
{
if (isArbDsa)
{
glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
else
{
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
}

if (isArbDsa)
{
glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
else
{
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#ifdef RHI_OPENGL_STATE_CLEANUP
// Restore previous alignment
					glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif
}

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
if (openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "2D texture", 13)	// 13 = "2D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
				}
#endif
}

void Texture2DDsa::setMinimumMaximumMipmapIndex(uint32_t minimumMipmapIndex, uint32_t maximumMipmapIndex)
{
  // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sampler_objects.txt
  // "
  //  2) What is the set of state associated with a sampler object?
  //     Specifically, should TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL be
  //     part of the sampler or the texture?
  //  DISCUSSION: TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL are presently
  //  part of the image state (texture) and are thus not included in the
  //  sampler object.
  // "
  glTextureParameteri(mOpenGLTexture, GL_TEXTURE_BASE_LEVEL, static_cast<GLint>(minimumMipmapIndex));
  glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(maximumMipmapIndex));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
