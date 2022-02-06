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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIVulkan/RERHIVulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    Vulkan mapping
*/
class Mapping final
{


  //[-------------------------------------------------------]
  //[ Public static methods                                 ]
  //[-------------------------------------------------------]
public:
  //[-------------------------------------------------------]
  //[ RERHI::FilterMode                                       ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::FilterMode" to Vulkan magnification filter mode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] filterMode
  *    "RERHI::FilterMode" to map
  *
  *  @return
  *    Vulkan magnification filter mode
  */
  [[nodiscard]] static VkFilter getVulkanMagFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode);

  /**
  *  @brief
  *    "RERHI::FilterMode" to Vulkan minification filter mode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] filterMode
  *    "RERHI::FilterMode" to map
  *
  *  @return
  *    Vulkan minification filter mode
  */
  [[nodiscard]] static VkFilter getVulkanMinFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode);

  /**
  *  @brief
  *    "RERHI::FilterMode" to Vulkan sampler mipmap mode
  *
  *  @param[in] context
  *    Rhi context to use
  *  @param[in] filterMode
  *    "RERHI::FilterMode" to map
  *
  *  @return
  *    Vulkan sampler mipmap mode
  */
  [[nodiscard]] static VkSamplerMipmapMode getVulkanMipmapMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode);

  //[-------------------------------------------------------]
  //[ RERHI::TextureAddressMode                               ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::TextureAddressMode" to Vulkan texture address mode
  *
  *  @param[in] textureAddressMode
  *    "RERHI::TextureAddressMode" to map
  *
  *  @return
  *    Vulkan texture address mode
  */
  [[nodiscard]] static VkSamplerAddressMode getVulkanTextureAddressMode(RERHI::TextureAddressMode textureAddressMode);

  //[-------------------------------------------------------]
  //[ RERHI::Blend                                            ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::Blend" to Vulkan blend factor
  *
  *  @param[in] blend
  *    "RERHI::Blend" to map
  *
  *  @return
  *    Vulkan blend factor
  */
  [[nodiscard]] static VkBlendFactor getVulkanBlendFactor(RERHI::Blend blend);

  /**
  *  @brief
  *    "RERHI::BlendOp" to Vulkan blend operation
  *
  *  @param[in] blendOp
  *    "RERHI::BlendOp" to map
  *
  *  @return
  *    Vulkan blend operation
  */
  [[nodiscard]] static VkBlendOp getVulkanBlendOp(RERHI::BlendOp blendOp);

  //[-------------------------------------------------------]
  //[ RERHI::ComparisonFunc                                   ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::ComparisonFunc" to Vulkan comparison function
  *
  *  @param[in] comparisonFunc
  *    "RERHI::ComparisonFunc" to map
  *
  *  @return
  *    Vulkan comparison function
  */
  [[nodiscard]] static VkCompareOp getVulkanComparisonFunc(RERHI::ComparisonFunc comparisonFunc);

  //[-------------------------------------------------------]
  //[ RERHI::VertexAttributeFormat and semantic               ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::VertexAttributeFormat" to Vulkan format
  *
  *  @param[in] vertexAttributeFormat
  *    "RERHI::VertexAttributeFormat" to map
  *
  *  @return
  *    Vulkan format
  */
  [[nodiscard]] static VkFormat getVulkanFormat(RERHI::VertexAttributeFormat vertexAttributeFormat);

  //[-------------------------------------------------------]
  //[ RERHI::IndexBufferFormat                                ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::IndexBufferFormat" to Vulkan type
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] indexBufferFormat
  *    "RERHI::IndexBufferFormat" to map
  *
  *  @return
  *    Vulkan index type
  */
  [[nodiscard]] static VkIndexType getVulkanType([[maybe_unused]] const RERHI::RHIContext& context, RERHI::IndexBufferFormat::Enum indexBufferFormat);

  //[-------------------------------------------------------]
  //[ RERHI::PrimitiveTopology                                ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::PrimitiveTopology" to Vulkan type
  *
  *  @param[in] primitiveTopology
  *    "RERHI::PrimitiveTopology" to map
  *
  *  @return
  *    Vulkan type
  */
  [[nodiscard]] static VkPrimitiveTopology getVulkanType(RERHI::PrimitiveTopology primitiveTopology);

  //[-------------------------------------------------------]
  //[ RERHI::TextureFormat                                    ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    "RERHI::TextureFormat" to Vulkan format
  *
  *  @param[in] textureFormat
  *    "RERHI::TextureFormat" to map
  *
  *  @return
  *    Vulkan format
  */
  [[nodiscard]] static VkFormat getVulkanFormat(RERHI::TextureFormat::Enum textureFormat);

  /**
  *  @brief
  *    Number of multisamples to Vulkan sample count flag bits
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] numberOfMultisamples
  *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  *
  *  @return
  *    Vulkan sample count flag bits
  */
  [[nodiscard]] static VkSampleCountFlagBits getVulkanSampleCountFlagBits([[maybe_unused]] const RERHI::RHIContext& context, uint8_t numberOfMultisamples);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
