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
#include "RERHIVulkan/Mapping.h"
#include "RERHIVulkan/RHIDynamicRHI.h"
#include "RERHIVulkan/Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


VkFilter Mapping::getVulkanMagFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode)
{
  switch (filterMode)
  {
    case RERHI::FilterMode::MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::UNKNOWN:
      RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_FILTER_NEAREST;

    default:
      return VK_FILTER_NEAREST;	// We should never be in here
  }
}

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
VkFilter Mapping::getVulkanMinFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode)
{
  switch (filterMode)
  {
    case RERHI::FilterMode::MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_FILTER_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_FILTER_LINEAR;

    case RERHI::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_FILTER_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::UNKNOWN:
      RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_FILTER_NEAREST;

    default:
      return VK_FILTER_NEAREST;	// We should never be in here
  }
}

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
VkSamplerMipmapMode Mapping::getVulkanMipmapMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode)
{
  switch (filterMode)
  {
    case RERHI::FilterMode::MIN_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::MIN_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::ANISOTROPIC:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;

    case RERHI::FilterMode::COMPARISON_ANISOTROPIC:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;	// There's no special setting in Vulkan

    case RERHI::FilterMode::UNKNOWN:
      RHI_ASSERT(false, "Vulkan filter mode must not be unknown")
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;

    default:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;	// We should never be in here
  }
}

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
VkSamplerAddressMode Mapping::getVulkanTextureAddressMode(RERHI::TextureAddressMode textureAddressMode)
{
  static constexpr VkSamplerAddressMode MAPPING[] =
    {
      VK_SAMPLER_ADDRESS_MODE_REPEAT,					// RERHI::TextureAddressMode::WRAP
      VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,		// RERHI::TextureAddressMode::MIRROR
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,			// RERHI::TextureAddressMode::CLAMP
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,		// RERHI::TextureAddressMode::BORDER
      VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE	// RERHI::TextureAddressMode::MIRROR_ONCE
    };
  return MAPPING[static_cast<int>(textureAddressMode) - 1];	// Lookout! The "RERHI::TextureAddressMode"-values start with 1, not 0
}

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
VkBlendFactor Mapping::getVulkanBlendFactor(RERHI::Blend blend)
{
  static constexpr VkBlendFactor MAPPING[] =
    {
      VK_BLEND_FACTOR_ZERO,						// RERHI::Blend::ZERO				 = 1
      VK_BLEND_FACTOR_ONE,						// RERHI::Blend::ONE				 = 2
      VK_BLEND_FACTOR_SRC_COLOR,					// RERHI::Blend::SRC_COLOR		 = 3
      VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,		// RERHI::Blend::INV_SRC_COLOR	 = 4
      VK_BLEND_FACTOR_SRC_ALPHA,					// RERHI::Blend::SRC_ALPHA		 = 5
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		// RERHI::Blend::INV_SRC_ALPHA	 = 6
      VK_BLEND_FACTOR_DST_ALPHA,					// RERHI::Blend::DEST_ALPHA		 = 7
      VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		// RERHI::Blend::INV_DEST_ALPHA	 = 8
      VK_BLEND_FACTOR_DST_COLOR,					// RERHI::Blend::DEST_COLOR		 = 9
      VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,		// RERHI::Blend::INV_DEST_COLOR	 = 10
      VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,			// RERHI::Blend::SRC_ALPHA_SAT	 = 11
      VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 12 !
      VK_BLEND_FACTOR_MAX_ENUM,					// <undefined>					 = 13 !
      VK_BLEND_FACTOR_CONSTANT_COLOR,				// RERHI::Blend::BLEND_FACTOR		 = 14
      VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,	// RERHI::Blend::INV_BLEND_FACTOR  = 15
      VK_BLEND_FACTOR_SRC1_COLOR,					// RERHI::Blend::SRC_1_COLOR		 = 16
      VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,		// RERHI::Blend::INV_SRC_1_COLOR	 = 17
      VK_BLEND_FACTOR_SRC1_ALPHA,					// RERHI::Blend::SRC_1_ALPHA		 = 18
      VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA		// RERHI::Blend::INV_SRC_1_ALPHA	 = 19
    };
  return MAPPING[static_cast<int>(blend) - 1];	// Lookout! The "RERHI::Blend"-values start with 1, not 0, there are also holes
}

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
VkBlendOp Mapping::getVulkanBlendOp(RERHI::BlendOp blendOp)
{
  static constexpr VkBlendOp MAPPING[] =
    {
      VK_BLEND_OP_ADD,				// RERHI::BlendOp::ADD
      VK_BLEND_OP_SUBTRACT,			// RERHI::BlendOp::SUBTRACT
      VK_BLEND_OP_REVERSE_SUBTRACT,	// RERHI::BlendOp::REV_SUBTRACT
      VK_BLEND_OP_MIN,				// RERHI::BlendOp::MIN
      VK_BLEND_OP_MAX					// RERHI::BlendOp::MAX
    };
  return MAPPING[static_cast<int>(blendOp) - 1];	// Lookout! The "RERHI::Blend"-values start with 1, not 0
}

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
VkCompareOp Mapping::getVulkanComparisonFunc(RERHI::ComparisonFunc comparisonFunc)
{
  static constexpr VkCompareOp MAPPING[] =
    {
      VK_COMPARE_OP_NEVER,			// RERHI::ComparisonFunc::NEVER
      VK_COMPARE_OP_LESS,				// RERHI::ComparisonFunc::LESS
      VK_COMPARE_OP_EQUAL,			// RERHI::ComparisonFunc::EQUAL
      VK_COMPARE_OP_LESS_OR_EQUAL,	// RERHI::ComparisonFunc::LESS_EQUAL
      VK_COMPARE_OP_GREATER,			// RERHI::ComparisonFunc::GREATER
      VK_COMPARE_OP_NOT_EQUAL,		// RERHI::ComparisonFunc::NOT_EQUAL
      VK_COMPARE_OP_GREATER_OR_EQUAL,	// RERHI::ComparisonFunc::GREATER_EQUAL
      VK_COMPARE_OP_ALWAYS			// RERHI::ComparisonFunc::ALWAYS
    };
  return MAPPING[static_cast<int>(comparisonFunc) - 1];	// Lookout! The "RERHI::ComparisonFunc"-values start with 1, not 0
}

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
VkFormat Mapping::getVulkanFormat(RERHI::VertexAttributeFormat vertexAttributeFormat)
{
  static constexpr VkFormat MAPPING[] =
    {
      VK_FORMAT_R32_SFLOAT,			// RERHI::VertexAttributeFormat::FLOAT_1
      VK_FORMAT_R32G32_SFLOAT,		// RERHI::VertexAttributeFormat::FLOAT_2
      VK_FORMAT_R32G32B32_SFLOAT,		// RERHI::VertexAttributeFormat::FLOAT_3
      VK_FORMAT_R32G32B32A32_SFLOAT,	// RERHI::VertexAttributeFormat::FLOAT_4
      VK_FORMAT_R8G8B8A8_UNORM,		// RERHI::VertexAttributeFormat::R8G8B8A8_UNORM
      VK_FORMAT_R8G8B8A8_UINT,		// RERHI::VertexAttributeFormat::R8G8B8A8_UINT
      VK_FORMAT_R16G16_SINT,			// RERHI::VertexAttributeFormat::SHORT_2
      VK_FORMAT_R16G16B16A16_SINT,	// RERHI::VertexAttributeFormat::SHORT_4
      VK_FORMAT_R32_UINT				// RERHI::VertexAttributeFormat::UINT_1
    };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

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
VkIndexType Mapping::getVulkanType([[maybe_unused]] const RERHI::RHIContext& context, RERHI::IndexBufferFormat::Enum indexBufferFormat)
{
  RHI_ASSERT(RERHI::IndexBufferFormat::UNSIGNED_CHAR != indexBufferFormat, "One byte per element index buffer format isn't supported by Vulkan")
  static constexpr VkIndexType MAPPING[] =
    {
      VK_INDEX_TYPE_MAX_ENUM,	// RERHI::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, RECore::uint8 (may not be supported by each API) - Not supported by Vulkan
      VK_INDEX_TYPE_UINT16,	// RERHI::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, RECore::uint16
      VK_INDEX_TYPE_UINT32	// RERHI::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, RECore::uint32 (may not be supported by each API)
    };
  return MAPPING[indexBufferFormat];
}

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
VkPrimitiveTopology Mapping::getVulkanType(RERHI::PrimitiveTopology primitiveTopology)
{
  // Tessellation support: Up to 32 vertices per patch are supported "RERHI::PrimitiveTopology::PATCH_LIST_1" ... "RERHI::PrimitiveTopology::PATCH_LIST_32"
  if (primitiveTopology >= RERHI::PrimitiveTopology::PATCH_LIST_1)
  {
    // Use tessellation
    return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
  }
  else
  {
    static constexpr VkPrimitiveTopology MAPPING[] =
      {
        VK_PRIMITIVE_TOPOLOGY_POINT_LIST,					// RERHI::PrimitiveTopology::POINT_LIST
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST,					// RERHI::PrimitiveTopology::LINE_LIST
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,					// RERHI::PrimitiveTopology::LINE_STRIP
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,				// RERHI::PrimitiveTopology::TRIANGLE_LIST
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,				// RERHI::PrimitiveTopology::TRIANGLE_STRIP
        VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 6
        VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 7
        VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 8
        VK_PRIMITIVE_TOPOLOGY_MAX_ENUM,						// Unused value 9
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,		// RERHI::PrimitiveTopology::LINE_LIST_ADJ
        VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,	// RERHI::PrimitiveTopology::LINE_STRIP_ADJ
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,	// RERHI::PrimitiveTopology::TRIANGLE_LIST_ADJ
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY	// RERHI::PrimitiveTopology::TRIANGLE_STRIP_ADJ
      };
    return MAPPING[static_cast<int>(primitiveTopology) - 1];	// Lookout! The "RERHI::PrimitiveTopology"-values start with 1, not 0
  }
}

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
VkFormat Mapping::getVulkanFormat(RERHI::TextureFormat::Enum textureFormat)
{
  static constexpr VkFormat MAPPING[] =
    {
      VK_FORMAT_R8_UNORM,					// RERHI::TextureFormat::R8            - 8-bit pixel format, all bits red
      VK_FORMAT_R8G8B8_UNORM,				// RERHI::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
      VK_FORMAT_R8G8B8A8_UNORM,			// RERHI::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      VK_FORMAT_R8G8B8A8_SRGB,			// RERHI::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      VK_FORMAT_B8G8R8A8_UNORM,			// RERHI::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      VK_FORMAT_B10G11R11_UFLOAT_PACK32,	// RERHI::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent
      VK_FORMAT_R16G16B16A16_SFLOAT,		// RERHI::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
      VK_FORMAT_R32G32B32A32_SFLOAT,		// RERHI::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
      VK_FORMAT_BC1_RGB_UNORM_BLOCK,		// RERHI::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
      VK_FORMAT_BC1_RGB_SRGB_BLOCK,		// RERHI::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      VK_FORMAT_BC2_UNORM_BLOCK,			// RERHI::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      VK_FORMAT_BC2_SRGB_BLOCK,			// RERHI::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      VK_FORMAT_BC3_UNORM_BLOCK,			// RERHI::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
      VK_FORMAT_BC3_SRGB_BLOCK,			// RERHI::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      VK_FORMAT_BC4_UNORM_BLOCK,			// RERHI::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
      VK_FORMAT_BC5_UNORM_BLOCK,			// RERHI::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
      VK_FORMAT_UNDEFINED,				// RERHI::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - not supported in Direct3D 11 - TODO(naetherm) Check for Vulkan format
      VK_FORMAT_R16_UNORM,				// RERHI::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      VK_FORMAT_R32_UINT,					// RERHI::TextureFormat::R32_UINT      - 32-bit unsigned integer format
      VK_FORMAT_R32_SFLOAT,				// RERHI::TextureFormat::R32_FLOAT     - 32-bit float format
      VK_FORMAT_D32_SFLOAT,				// RERHI::TextureFormat::D32_FLOAT     - 32-bit float depth format
      VK_FORMAT_R16G16_UNORM,				// RERHI::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
      VK_FORMAT_R16G16_SFLOAT,			// RERHI::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      VK_FORMAT_UNDEFINED					// RERHI::TextureFormat::UNKNOWN       - Unknown
    };
  return MAPPING[textureFormat];
}

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
VkSampleCountFlagBits Mapping::getVulkanSampleCountFlagBits([[maybe_unused]] const RERHI::RHIContext& context, RECore::uint8 numberOfMultisamples)
{
  RHI_ASSERT(numberOfMultisamples <= 8, "Invalid number of Vulkan multisamples")
  static constexpr VkSampleCountFlagBits MAPPING[] =
    {
      VK_SAMPLE_COUNT_1_BIT,
      VK_SAMPLE_COUNT_2_BIT,
      VK_SAMPLE_COUNT_4_BIT,
      VK_SAMPLE_COUNT_8_BIT
    };
  return MAPPING[numberOfMultisamples - 1];	// Lookout! The "numberOfMultisamples"-values start with 1, not 0
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
