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
#include "RERHINull/Texture/RHITextureManager.h"
#include "RERHINull/Texture/RHITexture1D.h"
#include "RERHINull/Texture/RHITexture1DArray.h"
#include "RERHINull/Texture/RHITexture2D.h"
#include "RERHINull/Texture/RHITexture2DArray.h"
#include "RERHINull/Texture/RHITexture3D.h"
#include "RERHINull/Texture/RHITextureCube.h"
#include "RERHINull/Texture/RHITextureCubeArray.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] nullRhi
*    Owner null RHI instance
*/
TextureManager::TextureManager(RHIDynamicRHI& nullRhi) :
RHITextureManager(nullRhi)
  {}

/**
*  @brief
*    Destructor
*/
TextureManager::~TextureManager() 
{}


RERHI::RHITexture1D* TextureManager::createTexture1D(RECore::uint32 width, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0, "Null create texture 1D was called with invalid parameters")

  // Create 1D texture resource
  return RHI_NEW(nullRhi.getContext(), Texture1D)(nullRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture1DArray* TextureManager::createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && numberOfSlices > 0, "Null create texture 1D array was called with invalid parameters")

  // Create 1D texture array resource
  return RHI_NEW(nullRhi.getContext(), Texture1DArray)(nullRhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2D* TextureManager::createTexture2D(RECore::uint32 width, RECore::uint32 height, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage, [[maybe_unused]] RECore::uint8 numberOfMultisamples, [[maybe_unused]] const RERHI::OptimizedTextureClearValue* optimizedTextureClearValue RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0, "Null create texture 2D was called with invalid parameters")

  // Create 2D texture resource
  return RHI_NEW(nullRhi.getContext(), Texture2D)(nullRhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2DArray* TextureManager::createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0 && numberOfSlices > 0, "Null create texture 2D array was called with invalid parameters")

  // Create 2D texture array resource
  return RHI_NEW(nullRhi.getContext(), Texture2DArray)(nullRhi, width, height, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture3D* TextureManager::createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0 && height > 0 && depth > 0, "Null create texture 3D was called with invalid parameters")

  // Create 3D texture resource
  return RHI_NEW(nullRhi.getContext(), Texture3D)(nullRhi, width, height, depth RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCube* TextureManager::createTextureCube(RECore::uint32 width, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0, "Null create texture cube was called with invalid parameters")

  // Create cube texture resource
  return RHI_NEW(nullRhi.getContext(), TextureCube)(nullRhi, width RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCubeArray* TextureManager::createTextureCubeArray(RECore::uint32 width, RECore::uint32 numberOfSlices, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER)
{
  RHIDynamicRHI& nullRhi = static_cast<RHIDynamicRHI&>(getRhi());

  // Sanity check
  RHI_ASSERT(width > 0, "Null create texture cube array was called with invalid parameters")

  // Create cube texture array resource
  return RHI_NEW(nullRhi.getContext(), TextureCubeArray)(nullRhi, width, numberOfSlices RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

void TextureManager::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), TextureManager, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
