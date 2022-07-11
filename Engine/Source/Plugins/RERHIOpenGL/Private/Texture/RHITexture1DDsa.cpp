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
#include "RERHIOpenGL/Texture/RHITexture1DDsa.h"
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
Texture1DDsa::Texture1DDsa(RHIDynamicRHI& openGLRhi, RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
Texture1D(openGLRhi, width, textureFormat RHI_RESOURCE_DEBUG_PASS_PARAMETER)
{
// Sanity checks
RHI_ASSERT(0 == (textureFlags & RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS) || nullptr != data, "Invalid OpenGL texture parameters")
RHI_ASSERT((textureFlags & RERHI::TextureFlag::RENDER_TARGET) == 0 || nullptr == data, "OpenGL render target textures can't be filled using provided data")

// Multisample texture?
const bool isArbDsa = openGLRhi.getExtensions().isGL_ARB_direct_state_access();

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
const RECore::uint32 numberOfMipmaps = (dataContainsMipmaps || generateMipmaps) ? getNumberOfMipmaps(width) : 1;

// Create the OpenGL texture instance
if (isArbDsa)
{
glCreateTextures(GL_TEXTURE_1D, 1, &mOpenGLTexture);
glTextureStorage1D(mOpenGLTexture, static_cast<GLsizei>(numberOfMipmaps), mOpenGLInternalFormat, static_cast<GLsizei>(width));
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
const RECore::uint32 format = Mapping::getOpenGLFormat(textureFormat);
for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap)
{
// Upload the current mipmap
const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
if (isArbDsa)
{
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
glCompressedTextureSubImage1D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, static_cast<GLsizei>(width), format, numberOfBytesPerSlice, data);
}
else
{
glCompressedTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, static_cast<GLint>(mipmap), format, static_cast<GLsizei>(width), 0, numberOfBytesPerSlice, data);
}

// Move on to the next mipmap and ensure the size is always at least 1
data = static_cast<const RECore::uint8*>(data) + numberOfBytesPerSlice;
width = getHalfSize(width);
}
}
else
{
// The user only provided us with the base texture, no mipmaps
if (isArbDsa)
{
if (nullptr != data)
{
glCompressedTextureSubImage1D(mOpenGLTexture, 0, 0, static_cast<GLsizei>(width), Mapping::getOpenGLFormat(textureFormat), static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1)), data);
}
}
else
{
glCompressedTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, 0, mOpenGLInternalFormat, static_cast<GLsizei>(width), 0, static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1)), data);
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
const RECore::uint32 format = Mapping::getOpenGLFormat(textureFormat);
const RECore::uint32 type = Mapping::getOpenGLType(textureFormat);
for (RECore::uint32 mipmap = 0; mipmap < numberOfMipmaps; ++mipmap)
{
// Upload the current mipmap
const GLsizei numberOfBytesPerSlice = static_cast<GLsizei>(RERHI::TextureFormat::getNumberOfBytesPerSlice(textureFormat, width, 1));
if (isArbDsa)
{
// We know that "data" must be valid when we're in here due to the "RERHI::TextureFlag::DATA_CONTAINS_MIPMAPS"-flag
glTextureSubImage1D(mOpenGLTexture, static_cast<GLint>(mipmap), 0, static_cast<GLsizei>(width), format, type, data);
}
else
{
glTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, static_cast<GLint>(mipmap), static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, format, type, data);
}

// Move on to the next mipmap and ensure the size is always at least 1
data = static_cast<const RECore::uint8*>(data) + numberOfBytesPerSlice;
width = getHalfSize(width);
}
}
else
{
// The user only provided us with the base texture, no mipmaps
if (isArbDsa)
{
if (nullptr != data)
{
glTextureSubImage1D(mOpenGLTexture, 0, 0, static_cast<GLsizei>(width), Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
}
}
else
{
glTextureImage1DEXT(mOpenGLTexture, GL_TEXTURE_1D, 0, static_cast<GLint>(mOpenGLInternalFormat), static_cast<GLsizei>(width), 0, Mapping::getOpenGLFormat(textureFormat), Mapping::getOpenGLType(textureFormat), data);
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
glGenerateTextureMipmapEXT(mOpenGLTexture, GL_TEXTURE_1D);
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
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
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
}

if (isArbDsa)
{
glTextureParameteri(mOpenGLTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
else
{
glTextureParameteriEXT(mOpenGLTexture, GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

#ifdef RHI_OPENGL_STATE_CLEANUP
// Restore previous alignment
				glPixelStorei(GL_UNPACK_ALIGNMENT, openGLAlignmentBackup);
#endif

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
if (openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "1D texture", 13)	// 13 = "1D texture: " including terminating zero
					glObjectLabel(GL_TEXTURE, mOpenGLTexture, -1, detailedDebugName);
				}
#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
