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
#include "RERHIOpenGL/Texture/RHITextureManager.h"
#include "RERHIOpenGL/Texture/RHITexture1DBind.h"
#include "RERHIOpenGL/Texture/RHITexture1DDsa.h"
#include "RERHIOpenGL/Texture/RHITexture1DArrayBind.h"
#include "RERHIOpenGL/Texture/RHITexture1DArrayDsa.h"
#include "RERHIOpenGL/Texture/RHITexture2DBind.h"
#include "RERHIOpenGL/Texture/RHITexture2DDsa.h"
#include "RERHIOpenGL/Texture/RHITexture2DArrayBind.h"
#include "RERHIOpenGL/Texture/RHITexture2DArrayDsa.h"
#include "RERHIOpenGL/Texture/RHITexture3DBind.h"
#include "RERHIOpenGL/Texture/RHITexture3DDsa.h"
#include "RERHIOpenGL/Texture/RHITextureCubeBind.h"
#include "RERHIOpenGL/Texture/RHITextureCubeDsa.h"
#include "RERHIOpenGL/Texture/RHITextureCubeArrayBind.h"
#include "RERHIOpenGL/Texture/RHITextureCubeArrayDsa.h"
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
TextureManager::TextureManager(RHIDynamicRHI& openGLRhi) :
RHITextureManager(openGLRhi),
mExtensions(&openGLRhi.getExtensions())
{}

RERHI::RHITexture1D* TextureManager::createTexture1D(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0, "OpenGL create texture 1D was called with invalid parameters")

  // Create 1D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), Texture1DDsa)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), Texture1DBind)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

RERHI::RHITexture1DArray* TextureManager::createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && numberOfSlices > 0, "OpenGL create texture 1D array was called with invalid parameters")

  // Create 1D texture array resource, "GL_EXT_texture_array"-extension required
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_texture_array())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), Texture1DArrayDsa)(openGLRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), Texture1DArrayBind)(openGLRhi, width, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    return nullptr;
  }
}

RERHI::RHITexture2D* TextureManager::createTexture2D(RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage, RECore::uint8 numberOfMultisamples, [[maybe_unused]] const RERHI::OptimizedTextureClearValue* optimizedTextureClearValue RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0, "OpenGL create texture 2D was called with invalid parameters")

  // Create 2D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), Texture2DDsa)(openGLRhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), Texture2DBind)(openGLRhi, width, height, textureFormat, data, textureFlags, numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

RERHI::RHITexture2DArray* TextureManager::createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0 && numberOfSlices > 0, "OpenGL create texture 2D array was called with invalid parameters")

  // Create 2D texture array resource, "GL_EXT_texture_array"-extension required
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_texture_array())
  {
    // Is "GL_EXT_direct_state_access" there?
    if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
    {
      // Effective direct state access (DSA)
      return RHI_NEW(openGLRhi.getContext(), Texture2DArrayDsa)(openGLRhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
    else
    {
      // Traditional bind version
      return RHI_NEW(openGLRhi.getContext(), Texture2DArrayBind)(openGLRhi, width, height, numberOfSlices, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
    }
  }
  else
  {
    return nullptr;
  }
}

RERHI::RHITexture3D* TextureManager::createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0 && depth > 0, "OpenGL create texture 3D was called with invalid parameters")

  // Create 3D texture resource: Is "GL_EXT_direct_state_access" there?
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), Texture3DDsa)(openGLRhi, width, height, depth, textureFormat, data, textureFlags, textureUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), Texture3DBind)(openGLRhi, width, height, depth, textureFormat, data, textureFlags, textureUsage RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

RERHI::RHITextureCube* TextureManager::createTextureCube(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& openGLRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0, "OpenGL create texture cube was called with invalid parameters")

  // Create cube texture resource
  // -> The indication of the texture usage is only relevant for Direct3D, OpenGL has no texture usage indication
  // Is "GL_EXT_direct_state_access" or "GL_ARB_direct_state_access" there?
  if (mExtensions->isGL_EXT_direct_state_access() || mExtensions->isGL_ARB_direct_state_access())
  {
    // Effective direct state access (DSA)
    return RHI_NEW(openGLRhi.getContext(), TextureCubeDsa)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
  else
  {
    // Traditional bind version
    return RHI_NEW(openGLRhi.getContext(), TextureCubeBind)(openGLRhi, width, textureFormat, data, textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
  }
}

RERHI::RHITextureCubeArray* TextureManager::createTextureCubeArray([[maybe_unused]] RECore::uint32 width, [[maybe_unused]] RECore::uint32 numberOfSlices, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  // TODO(naetherm) Implement me
#ifdef DEBUG
  debugName = debugName;
#endif
  return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
