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
#include "RERHIOpenGLES3/Texture/RHITextureManager.h"
#include "RERHIOpenGLES3/Texture/RHITexture1D.h"
#include "RERHIOpenGLES3/Texture/RHITexture1DArray.h"
#include "RERHIOpenGLES3/Texture/RHITexture2D.h"
#include "RERHIOpenGLES3/Texture/RHITexture2DArray.h"
#include "RERHIOpenGLES3/Texture/RHITexture3D.h"
#include "RERHIOpenGLES3/Texture/RHITextureCube.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/OpenGLES3Context.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


TextureManager::TextureManager(RHIDynamicRHI &openGLES3Rhi) :
  RHITextureManager(openGLES3Rhi),
  mExtensions(&openGLES3Rhi.getOpenGLES3Context().getExtensions()) {}


RERHI::RHITexture1D *
TextureManager::createTexture1D(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void *data,
                                RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage
                                RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0, "OpenGL ES 3 create texture 1D was called with invalid parameters")

// Create 1D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), Texture1D)(openGLES3Rhi, width, textureFormat, data, textureFlags
                                                       RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture1DArray *
TextureManager::createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat,
                                     const void *data, RECore::uint32 textureFlags,
                                     [[maybe_unused]] RERHI::TextureUsage textureUsage
                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && numberOfSlices > 0, "OpenGL ES 3 create texture 1D array was called with invalid parameters")

// Create 1D texture array resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), Texture1DArray)(openGLES3Rhi, width, numberOfSlices, textureFormat, data,
                                                            textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2D *
TextureManager::createTexture2D(RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat,
                                const void *data, RECore::uint32 textureFlags,
                                [[maybe_unused]] RERHI::TextureUsage textureUsage,
                                [[maybe_unused]] RECore::uint8 numberOfMultisamples,
                                [[maybe_unused]] const RERHI::OptimizedTextureClearValue *optimizedTextureClearValue
                                RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0, "OpenGL ES 3 create texture 2D was called with invalid parameters")

// Create 2D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), Texture2D)(openGLES3Rhi, width, height, textureFormat, data, textureFlags
                                                       RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2DArray *TextureManager::createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices,
                                                               RERHI::TextureFormat::Enum textureFormat,
                                                               const void *data, RECore::uint32 textureFlags,
                                                               [[maybe_unused]] RERHI::TextureUsage textureUsage
                                                               RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0 && numberOfSlices > 0,
             "OpenGL ES 3 create texture 2D array was called with invalid parameters")

// Create 2D texture array resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), Texture2DArray)(openGLES3Rhi, width, height, numberOfSlices, textureFormat,
                                                            data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture3D *TextureManager::createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth,
                                                     RERHI::TextureFormat::Enum textureFormat, const void *data,
                                                     RECore::uint32 textureFlags,
                                                     [[maybe_unused]] RERHI::TextureUsage textureUsage
                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0 && depth > 0, "OpenGL ES 3 create texture 3D was called with invalid parameters")

// Create 3D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), Texture3D)(openGLES3Rhi, width, height, depth, textureFormat, data,
                                                       textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCube *
TextureManager::createTextureCube(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void *data,
                                  RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage
                                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0, "OpenGL ES 3 create texture cube was called with invalid parameters")

// Create cube texture resource
// -> The indication of the texture usage is only relevant for Direct3D, OpenGL ES 3 has no texture usage indication
  return RHI_NEW(openGLES3Rhi.getContext(), TextureCube)(openGLES3Rhi, width, textureFormat, data, textureFlags
                                                         RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCubeArray *
TextureManager::createTextureCubeArray([[maybe_unused]] RECore::uint32 width, [[maybe_unused]] RECore::uint32 numberOfSlices,
                                       [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat,
                                       [[maybe_unused]] const void *data, [[maybe_unused]] RECore::uint32 textureFlags,
                                       [[maybe_unused]] RERHI::TextureUsage textureUsage
                                       RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
// TODO(naetherm) Implement me, OpenGL ES 3.1 "GL_EXT_texture_cube_map_array"-extension
#ifdef DEBUG
  debugName = debugName;
#endif
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
