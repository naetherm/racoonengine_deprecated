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
#include "RERHIVulkan/Texture/RHITextureManager.h"
#include "RERHIVulkan/Texture/RHITexture1D.h"
#include "RERHIVulkan/Texture/RHITexture1DArray.h"
#include "RERHIVulkan/Texture/RHITexture2D.h"
#include "RERHIVulkan/Texture/RHITexture2DArray.h"
#include "RERHIVulkan/Texture/RHITexture3D.h"
#include "RERHIVulkan/Texture/RHITextureCube.h"
#include "RERHIVulkan/Texture/RHITextureCubeArray.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"
#include "RERHIVulkan/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {

TextureManager::TextureManager(RHIDynamicRHI &vulkanRhi) :
  RHITextureManager(vulkanRhi) {}

/**
*  @brief
*    Destructor
*/
TextureManager::~TextureManager() {}


RERHI::RHITexture1D *
TextureManager::createTexture1D(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void *data,
                                RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage
                                RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0, "Vulkan create texture 1D was called with invalid parameters")

// Create 1D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), Texture1D)(vulkanRhi, width, textureFormat, data, textureFlags
                                                    RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture1DArray *
TextureManager::createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat,
                                     const void *data, RECore::uint32 textureFlags,
                                     [[maybe_unused]] RERHI::TextureUsage textureUsage
                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && numberOfSlices > 0, "Vulkan create texture 1D array was called with invalid parameters")

// Create 1D texture array resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), Texture1DArray)(vulkanRhi, width, numberOfSlices, textureFormat, data,
                                                         textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2D *
TextureManager::createTexture2D(RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat,
                                const void *data, RECore::uint32 textureFlags,
                                [[maybe_unused]] RERHI::TextureUsage textureUsage, RECore::uint8 numberOfMultisamples,
                                [[maybe_unused]] const RERHI::OptimizedTextureClearValue *optimizedTextureClearValue
                                RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0, "Vulkan create texture 2D was called with invalid parameters")

// Create 2D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), Texture2D)(vulkanRhi, width, height, textureFormat, data, textureFlags,
                                                    numberOfMultisamples RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture2DArray *TextureManager::createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices,
                                                               RERHI::TextureFormat::Enum textureFormat,
                                                               const void *data, RECore::uint32 textureFlags,
                                                               [[maybe_unused]] RERHI::TextureUsage textureUsage
                                                               RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0 && numberOfSlices > 0,
             "Vulkan create texture 2D array was called with invalid parameters")

// Create 2D texture array resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), Texture2DArray)(vulkanRhi, width, height, numberOfSlices, textureFormat, data,
                                                         textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITexture3D *TextureManager::createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth,
                                                     RERHI::TextureFormat::Enum textureFormat, const void *data,
                                                     RECore::uint32 textureFlags,
                                                     [[maybe_unused]] RERHI::TextureUsage textureUsage
                                                     RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && height > 0 && depth > 0, "Vulkan create texture 3D was called with invalid parameters")

// Create 3D texture resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), Texture3D)(vulkanRhi, width, height, depth, textureFormat, data, textureFlags
                                                    RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCube *
TextureManager::createTextureCube(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void *data,
                                  RECore::uint32 textureFlags, [[maybe_unused]] RERHI::TextureUsage textureUsage
                                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0, "Vulkan create texture cube was called with invalid parameters")

// Create cube texture resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), TextureCube)(vulkanRhi, width, textureFormat, data, textureFlags
                                                      RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}

RERHI::RHITextureCubeArray *TextureManager::createTextureCubeArray(RECore::uint32 width, RECore::uint32 numberOfSlices,
                                                                   RERHI::TextureFormat::Enum textureFormat,
                                                                   const void *data, RECore::uint32 textureFlags,
                                                                   [[maybe_unused]] RERHI::TextureUsage textureUsage
                                                                   RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &vulkanRhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity check
  RHI_ASSERT(width > 0 && numberOfSlices > 0, "Vulkan create texture cube was called with invalid parameters")

// Create cube texture resource
// -> The indication of the texture usage is only relevant for Direct3D, Vulkan has no texture usage indication
  return RHI_NEW(vulkanRhi.getContext(), TextureCubeArray)(vulkanRhi, width, numberOfSlices, textureFormat, data,
                                                           textureFlags RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
