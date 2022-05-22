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
#include "RERHIOpenGLES3/Mapping.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


GLint Mapping::getOpenGLES3MagFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode)
{
  switch (filterMode)
  {
    case RERHI::FilterMode::MIN_MAG_MIP_POINT:
      return GL_NEAREST;

    case RERHI::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return GL_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return GL_LINEAR;

    case RERHI::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return GL_LINEAR;

    case RERHI::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return GL_NEAREST;

    case RERHI::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return GL_NEAREST;

    case RERHI::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return GL_LINEAR;

    case RERHI::FilterMode::MIN_MAG_MIP_LINEAR:
      return GL_LINEAR;

    case RERHI::FilterMode::ANISOTROPIC:
      return GL_LINEAR;	// There's no special setting in OpenGL ES 3

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_ANISOTROPIC:
      return GL_LINEAR;	// There's no special setting in OpenGL ES 3

    case RERHI::FilterMode::UNKNOWN:
      RHI_ASSERT(false, "OpenGL ES 3 filter mode must not be unknown")
      return GL_NEAREST;

    default:
      return GL_NEAREST;	// We should never be in here
  }
}

/**
*  @brief
*    "RERHI::FilterMode" to OpenGL ES 3 minification filter mode
*
*  @param[in] context
*    RHI context to use
*  @param[in] filterMode
*    "RERHI::FilterMode" to map
*  @param[in] hasMipmaps
*    Are mipmaps available?
*
*  @return
*    OpenGL ES 3 minification filter mode
*/
GLint Mapping::getOpenGLES3MinFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode, bool hasMipmaps)
{
  switch (filterMode)
  {
    case RERHI::FilterMode::MIN_MAG_MIP_POINT:
      return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

    case RERHI::FilterMode::MIN_MAG_POINT_MIP_LINEAR:
      return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:
      return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

    case RERHI::FilterMode::MIN_POINT_MAG_MIP_LINEAR:
      return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

    case RERHI::FilterMode::MIN_LINEAR_MAG_MIP_POINT:
      return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

    case RERHI::FilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    case RERHI::FilterMode::MIN_MAG_LINEAR_MIP_POINT:
      return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

    case RERHI::FilterMode::MIN_MAG_MIP_LINEAR:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    case RERHI::FilterMode::ANISOTROPIC:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;	// There's no special setting in OpenGL ES 3

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_POINT:
      return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
      return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
      return hasMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
      return hasMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
      return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
      return hasMipmaps ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    case RERHI::FilterMode::COMPARISON_ANISOTROPIC:
      return hasMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;	// There's no special setting in OpenGL ES 3

    case RERHI::FilterMode::UNKNOWN:
      RHI_ASSERT(false, "OpenGL ES 3 filter mode must not be unknown")
      return GL_NEAREST;

    default:
      return GL_NEAREST;	// We should never be in here
  }
}

/**
*  @brief
*    "RERHI::FilterMode" to OpenGL ES 3 compare mode
*
*  @param[in] filterMode
*    "RERHI::FilterMode" to map
*
*  @return
*    OpenGL ES 3 compare mode
*/
GLint Mapping::getOpenGLES3CompareMode([[maybe_unused]] RERHI::FilterMode filterMode)
{
  // "GL_COMPARE_REF_TO_TEXTURE" is not supported by OpenGL ES 3
  return GL_NONE;
}

//[-------------------------------------------------------]
//[ RERHI::TextureAddressMode                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::TextureAddressMode" to OpenGL ES 3 texture address mode
*
*  @param[in] textureAddressMode
*    "RERHI::TextureAddressMode" to map
*
*  @return
*    OpenGL ES 3 texture address mode
*/
GLint Mapping::getOpenGLES3TextureAddressMode(RERHI::TextureAddressMode textureAddressMode)
{
  static constexpr GLint MAPPING[] =
    {
      GL_REPEAT,			// RERHI::TextureAddressMode::WRAP
      GL_MIRRORED_REPEAT,	// RERHI::TextureAddressMode::MIRROR
      GL_CLAMP_TO_EDGE,	// RERHI::TextureAddressMode::CLAMP
      GL_CLAMP_TO_EDGE,	// RERHI::TextureAddressMode::BORDER - Not supported by OpenGL ES 3
      GL_MIRRORED_REPEAT	// RERHI::TextureAddressMode::MIRROR_ONCE	// TODO(naetherm) OpenGL ES 3 equivalent?
    };
  return MAPPING[static_cast<int>(textureAddressMode) - 1];	// Lookout! The "RERHI::TextureAddressMode"-values start with 1, not 0
}

//[-------------------------------------------------------]
//[ RERHI::ComparisonFunc                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::ComparisonFunc" to OpenGL ES 3 comparison function
*
*  @param[in] comparisonFunc
*    "RERHI::ComparisonFunc" to map
*
*  @return
*    OpenGL ES 3 comparison function
*/
GLenum Mapping::getOpenGLES3ComparisonFunc(RERHI::ComparisonFunc comparisonFunc)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_NEVER,		// RERHI::ComparisonFunc::NEVER
      GL_LESS,		// RERHI::ComparisonFunc::LESS
      GL_EQUAL,		// RERHI::ComparisonFunc::EQUAL
      GL_LEQUAL,		// RERHI::ComparisonFunc::LESS_EQUAL
      GL_GREATER,		// RERHI::ComparisonFunc::GREATER
      GL_NOTEQUAL,	// RERHI::ComparisonFunc::NOT_EQUAL
      GL_GEQUAL,		// RERHI::ComparisonFunc::GREATER_EQUAL
      GL_ALWAYS		// RERHI::ComparisonFunc::ALWAYS
    };
  return MAPPING[static_cast<int>(comparisonFunc) - 1];	// Lookout! The "RERHI::ComparisonFunc"-values start with 1, not 0
}

//[-------------------------------------------------------]
//[ RERHI::VertexAttributeFormat                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::VertexAttributeFormat" to OpenGL ES 3 size (number of elements)
*
*  @param[in] vertexAttributeFormat
*    "RERHI::VertexAttributeFormat" to map
*
*  @return
*    OpenGL ES 3 size (number of elements)
*/
GLint Mapping::getOpenGLES3Size(RERHI::VertexAttributeFormat vertexAttributeFormat)
{
  static constexpr GLint MAPPING[] =
    {
      1,	// RERHI::VertexAttributeFormat::FLOAT_1
      2,	// RERHI::VertexAttributeFormat::FLOAT_2
      3,	// RERHI::VertexAttributeFormat::FLOAT_3
      4,	// RERHI::VertexAttributeFormat::FLOAT_4
      4,	// RERHI::VertexAttributeFormat::R8G8B8A8_UNORM
      4,	// RERHI::VertexAttributeFormat::R8G8B8A8_UINT
      2,	// RERHI::VertexAttributeFormat::SHORT_2
      4,	// RERHI::VertexAttributeFormat::SHORT_4
      1	// RERHI::VertexAttributeFormat::UINT_1
    };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

/**
*  @brief
*    "RERHI::VertexAttributeFormat" to OpenGL ES 3 type
*
*  @param[in] vertexAttributeFormat
*    "RERHI::VertexAttributeFormat" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLenum Mapping::getOpenGLES3Type(RERHI::VertexAttributeFormat vertexAttributeFormat)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_FLOAT,			// RERHI::VertexAttributeFormat::FLOAT_1
      GL_FLOAT,			// RERHI::VertexAttributeFormat::FLOAT_2
      GL_FLOAT,			// RERHI::VertexAttributeFormat::FLOAT_3
      GL_FLOAT,			// RERHI::VertexAttributeFormat::FLOAT_4
      GL_UNSIGNED_BYTE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UNORM
      GL_UNSIGNED_BYTE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UINT
      GL_SHORT,			// RERHI::VertexAttributeFormat::SHORT_2
      GL_SHORT,			// RERHI::VertexAttributeFormat::SHORT_4
      GL_UNSIGNED_INT		// RERHI::VertexAttributeFormat::UINT_1
    };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

/**
*  @brief
*    Return whether or not "RERHI::VertexAttributeFormat" is a normalized format
*
*  @param[in] vertexAttributeFormat
*    "RERHI::VertexAttributeFormat" to check
*
*  @return
*    "GL_TRUE" if the format is normalized, else "GL_FALSE"
*/
GLboolean Mapping::isOpenGLES3VertexAttributeFormatNormalized(RERHI::VertexAttributeFormat vertexAttributeFormat)
{
  static constexpr GLboolean MAPPING[] =
    {
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_1
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_2
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_3
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_4
      GL_TRUE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UNORM
      GL_FALSE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UINT
      GL_FALSE,	// RERHI::VertexAttributeFormat::SHORT_2
      GL_FALSE,	// RERHI::VertexAttributeFormat::SHORT_4
      GL_FALSE	// RERHI::VertexAttributeFormat::UINT_1
    };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

/**
*  @brief
*    Return whether or not "RERHI::VertexAttributeFormat" is an integer format
*
*  @param[in] vertexAttributeFormat
*    "RERHI::VertexAttributeFormat" to check
*
*  @return
*    "GL_TRUE" if the format is integer, else "GL_FALSE"
*/
GLboolean Mapping::isOpenGLES3VertexAttributeFormatInteger(RERHI::VertexAttributeFormat vertexAttributeFormat)
{
  static constexpr GLboolean MAPPING[] =
    {
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_1
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_2
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_3
      GL_FALSE,	// RERHI::VertexAttributeFormat::FLOAT_4
      GL_FALSE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UNORM
      GL_TRUE,	// RERHI::VertexAttributeFormat::R8G8B8A8_UINT
      GL_TRUE,	// RERHI::VertexAttributeFormat::SHORT_2
      GL_TRUE,	// RERHI::VertexAttributeFormat::SHORT_4
      GL_TRUE		// RERHI::VertexAttributeFormat::UINT_1
    };
  return MAPPING[static_cast<int>(vertexAttributeFormat)];
}

//[-------------------------------------------------------]
//[ RERHI::BufferUsage                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::BufferUsage" to OpenGL ES 3 usage
*
*  @param[in] bufferUsage
*    "RERHI::BufferUsage" to map
*
*  @return
*    OpenGL ES 3 usage
*/
GLenum Mapping::getOpenGLES3Type(RERHI::BufferUsage bufferUsage)
{
  // OpenGL ES 3 only supports: "STREAM_DRAW", "STATIC_DRAW" and "DYNAMIC_DRAW"

  // These constants directly map to "GL_ARB_vertex_buffer_object" and OpenGL ES 3 constants, do not change them
  // -> This also means that we have to use a switch statement for the mapping
  switch (bufferUsage)
  {
    case RERHI::BufferUsage::STREAM_DRAW:
    case RERHI::BufferUsage::STREAM_READ:
    case RERHI::BufferUsage::STREAM_COPY:
      return GL_STREAM_DRAW;

    case RERHI::BufferUsage::STATIC_DRAW:
    case RERHI::BufferUsage::STATIC_READ:
    case RERHI::BufferUsage::STATIC_COPY:
      return GL_STATIC_DRAW;

    case RERHI::BufferUsage::DYNAMIC_DRAW:
    case RERHI::BufferUsage::DYNAMIC_READ:
    case RERHI::BufferUsage::DYNAMIC_COPY:
    default:
      return GL_DYNAMIC_DRAW;
  }
}

//[-------------------------------------------------------]
//[ RERHI::IndexBufferFormat                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::IndexBufferFormat" to OpenGL ES 3 type
*
*  @param[in] indexBufferFormat
*    "RERHI::IndexBufferFormat" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLenum Mapping::getOpenGLES3Type(RERHI::IndexBufferFormat::Enum indexBufferFormat)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_UNSIGNED_BYTE,	// RERHI::IndexBufferFormat::UNSIGNED_CHAR  - One byte per element, RECore::uint8 (may not be supported by each API)
      GL_UNSIGNED_SHORT,	// RERHI::IndexBufferFormat::UNSIGNED_SHORT - Two bytes per element, RECore::uint16
      GL_UNSIGNED_INT		// RERHI::IndexBufferFormat::UNSIGNED_INT   - Four bytes per element, RECore::uint32 (may not be supported by each API)
    };
  return MAPPING[indexBufferFormat];
}

//[-------------------------------------------------------]
//[ RERHI::TextureFormat                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::TextureFormat" to OpenGL ES 3 internal format
*
*  @param[in] textureFormat
*    "RERHI::TextureFormat" to map
*
*  @return
*    OpenGL ES 3 internal format
*/
GLenum Mapping::getOpenGLES3InternalFormat(RERHI::TextureFormat::Enum textureFormat)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_R8,								// RERHI::TextureFormat::R8            - 8-bit pixel format, all bits red
      GL_RGB,								// RERHI::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
      GL_RGBA,							// RERHI::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      GL_RGBA,							// RERHI::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear	- TODO(naetherm) OpenGL ES 3 sRGB format
      GL_RGBA,							// RERHI::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha	TODO(naetherm) Texture format isn't supported
      GL_R11F_G11F_B10F,					// RERHI::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - available in OpenGL ES 3
      GL_RGBA16F,							// RERHI::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
      GL_RGBA32F,							// RERHI::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
      GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,	// RERHI::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - "GL_EXT_texture_compression_dxt1" OpenGL ES extension
      GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,	// RERHI::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - "GL_EXT_texture_compression_dxt1" OpenGL ES extension; sRGB = RGB hardware gamma correction, the alpha channel always remains linear	- TODO(naetherm) OpenGL ES 3 sRGB format
      GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,	// RERHI::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - "GL_EXT_texture_compression_s3tc" OpenGL ES extension
      GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,	// RERHI::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - "GL_EXT_texture_compression_s3tc" OpenGL ES extension; sRGB = RGB hardware gamma correction, the alpha channel always remains linear	- TODO(naetherm) OpenGL ES 3 sRGB format
      GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,	// RERHI::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - "GL_EXT_texture_compression_s3tc" OpenGL ES extension
      GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,	// RERHI::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - "GL_EXT_texture_compression_s3tc" OpenGL ES extension; sRGB = RGB hardware gamma correction, the alpha channel always remains linear	- TODO(naetherm) OpenGL ES 3 sRGB format
      GL_3DC_X_AMD,						// RERHI::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - "GL_AMD_compressed_3DC_texture" OpenGL ES extension
      GL_3DC_XY_AMD,						// RERHI::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - "GL_AMD_compressed_3DC_texture" OpenGL ES extension
      GL_ETC1_RGB8_OES,					// RERHI::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices
      GL_R16_EXT,							// RERHI::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      GL_R32UI,							// RERHI::TextureFormat::R32_UINT      - 32-bit unsigned integer format
      GL_R32F,							// RERHI::TextureFormat::R32_FLOAT     - 32-bit float format
      GL_DEPTH_COMPONENT32F,				// RERHI::TextureFormat::D32_FLOAT     - 32-bit float depth format
      0,									// RERHI::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel	TODO(naetherm) "GL_RG16_SNORM" OpenGL ES 3 needs https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_texture_norm16.txt
      GL_RG16F,							// RERHI::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      0									// RERHI::TextureFormat::UNKNOWN       - Unknown
    };
  return MAPPING[textureFormat];
}

/**
*  @brief
*    "RERHI::TextureFormat" to OpenGL ES 3 format
*
*  @param[in] textureFormat
*    "RERHI::TextureFormat" to map
*
*  @return
*    OpenGL ES 3 format
*/
GLenum Mapping::getOpenGLES3Format(RERHI::TextureFormat::Enum textureFormat)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_RED,				// RERHI::TextureFormat::R8            - 8-bit pixel format, all bits red
      GL_RGB,				// RERHI::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
      GL_RGBA,			// RERHI::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      GL_RGBA,			// RERHI::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      GL_RGBA,			// RERHI::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha	TODO(naetherm) Texture format isn't supported
      GL_RGB,				// RERHI::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - available in OpenGL ES 3
      GL_RGBA,			// RERHI::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha) - Not supported by OpenGL ES 3
      GL_RGBA,			// RERHI::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha) - Not supported by OpenGL ES 3
      0,					// RERHI::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - Compressed format, so not supported in here
      0,					// RERHI::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - Compressed format, so not supported in here
      GL_RED,				// RERHI::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      GL_RED_INTEGER,		// RERHI::TextureFormat::R32_UINT      - 32-bit unsigned integer format
      GL_RED,				// RERHI::TextureFormat::R32_FLOAT     - 32-bit float format
      GL_DEPTH_COMPONENT,	// RERHI::TextureFormat::D32_FLOAT     - 32-bit float depth format
      GL_RG,				// RERHI::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
      GL_RG,				// RERHI::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      0					// RERHI::TextureFormat::UNKNOWN       - Unknown
    };
  return MAPPING[textureFormat];
}

/**
*  @brief
*    "RERHI::TextureFormat" to OpenGL ES 3 type
*
*  @param[in] textureFormat
*    "RERHI::TextureFormat" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLenum Mapping::getOpenGLES3Type(RERHI::TextureFormat::Enum textureFormat)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_UNSIGNED_BYTE,					// RERHI::TextureFormat::R8            - 8-bit pixel format, all bits red
      GL_UNSIGNED_BYTE,					// RERHI::TextureFormat::R8G8B8        - 24-bit pixel format, 8 bits for red, green and blue
      GL_UNSIGNED_BYTE,					// RERHI::TextureFormat::R8G8B8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      GL_UNSIGNED_BYTE,					// RERHI::TextureFormat::R8G8B8A8_SRGB - 32-bit pixel format, 8 bits for red, green, blue and alpha; sRGB = RGB hardware gamma correction, the alpha channel always remains linear
      GL_UNSIGNED_BYTE,					// RERHI::TextureFormat::B8G8R8A8      - 32-bit pixel format, 8 bits for red, green, blue and alpha
      GL_UNSIGNED_INT_10F_11F_11F_REV,	// RERHI::TextureFormat::R11G11B10F    - 32-bit float format using 11 bits the red and green channel, 10 bits the blue channel; red and green channels have a 6 bits mantissa and a 5 bits exponent and blue has a 5 bits mantissa and 5 bits exponent - available in OpenGL ES 3
      GL_FLOAT,							// RERHI::TextureFormat::R16G16B16A16F - 64-bit float format using 16 bits for the each channel (red, green, blue, alpha) - Not supported by OpenGL ES 3
      GL_FLOAT,							// RERHI::TextureFormat::R32G32B32A32F - 128-bit float format using 32 bits for the each channel (red, green, blue, alpha) - Not supported by OpenGL ES 3
      0,									// RERHI::TextureFormat::BC1           - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC1_SRGB      - DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC2           - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC2_SRGB      - DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = hardware gamma correction - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC3           - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC3_SRGB      - DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block); sRGB = RGB hardware gamma correction, the alpha channel always remains linear - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC4           - 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::BC5           - 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) - Compressed format, so not supported in here
      0,									// RERHI::TextureFormat::ETC1          - 3 component texture compression meant for mobile devices - Compressed format, so not supported in here
      GL_UNSIGNED_SHORT,					// RERHI::TextureFormat::R16_UNORM     - 16-bit unsigned-normalized-integer format that supports 16 bits for the red channel
      GL_UNSIGNED_INT,					// RERHI::TextureFormat::R32_UINT      - 32-bit unsigned integer format
      GL_FLOAT,							// RERHI::TextureFormat::R32_FLOAT     - 32-bit float format
      GL_FLOAT,							// RERHI::TextureFormat::D32_FLOAT     - 32-bit float depth format
      GL_BYTE,							// RERHI::TextureFormat::R16G16_SNORM  - A two-component, 32-bit signed-normalized-integer format that supports 16 bits for the red channel and 16 bits for the green channel
      GL_FLOAT,							// RERHI::TextureFormat::R16G16_FLOAT  - A two-component, 32-bit floating-point format that supports 16 bits for the red channel and 16 bits for the green channel
      0									// RERHI::TextureFormat::UNKNOWN       - Unknown
    };
  return MAPPING[textureFormat];
}

//[-------------------------------------------------------]
//[ RERHI::PrimitiveTopology                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::PrimitiveTopology" to OpenGL ES 3 type
*
*  @param[in] primitiveTopology
*    "RERHI::PrimitiveTopology" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLenum Mapping::getOpenGLES3Type(RERHI::PrimitiveTopology primitiveTopology)
{
  static constexpr GLenum MAPPING[] =
    {
      GL_POINTS,						// RERHI::PrimitiveTopology::POINT_LIST
      GL_LINES,						// RERHI::PrimitiveTopology::LINE_LIST
      GL_LINE_STRIP,					// RERHI::PrimitiveTopology::LINE_STRIP
      GL_TRIANGLES,					// RERHI::PrimitiveTopology::TRIANGLE_LIST
      GL_TRIANGLE_STRIP,				// RERHI::PrimitiveTopology::TRIANGLE_STRIP
      0,								// Unused value 6
      0,								// Unused value 7
      0,								// Unused value 8
      0,								// Unused value 9
      GL_LINES_ADJACENCY_EXT,			// RERHI::PrimitiveTopology::LINE_LIST_ADJ - "GL_EXT_geometry_shader"-extension
      GL_LINE_STRIP_ADJACENCY_EXT,	// RERHI::PrimitiveTopology::LINE_STRIP_ADJ - "GL_EXT_geometry_shader"-extension
      GL_TRIANGLES_ADJACENCY_EXT,		// RERHI::PrimitiveTopology::TRIANGLE_LIST_ADJ - "GL_EXT_geometry_shader"-extension
      GL_TRIANGLE_STRIP_ADJACENCY_EXT	// RERHI::PrimitiveTopology::TRIANGLE_STRIP_ADJ - "GL_EXT_geometry_shader"-extension
    };
  return MAPPING[static_cast<int>(primitiveTopology) - 1];	// Lookout! The "RERHI::PrimitiveTopology"-values start with 1, not 0
}

//[-------------------------------------------------------]
//[ RERHI::MapType                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::MapType" to OpenGL ES 3 access bit field for "glMapBufferRange()"
*
*  @param[in] mapType
*    "RERHI::MapType" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLbitfield Mapping::getOpenGLES3MapRangeType(RERHI::MapType mapType)
{
  // OpenGL ES 3 defines access bits for "glMapBufferRange()"
  static constexpr GLbitfield MAPPING[] =
    {
      GL_MAP_READ_BIT,					// RERHI::MapType::READ
      GL_MAP_WRITE_BIT,					// RERHI::MapType::WRITE
      GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,	// RERHI::MapType::READ_WRITE
      GL_MAP_WRITE_BIT,					// RERHI::MapType::WRITE_DISCARD
      GL_MAP_WRITE_BIT					// RERHI::MapType::WRITE_NO_OVERWRITE
    };
  return MAPPING[static_cast<int>(mapType) - 1];	// Lookout! The "RERHI::MapType"-values start with 1, not 0
}

//[-------------------------------------------------------]
//[ RERHI::MapType                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    "RERHI::Blend" to OpenGL ES 3 type
*
*  @param[in] blend
*    "RERHI::Blend" to map
*
*  @return
*    OpenGL ES 3 type
*/
GLenum Mapping::getOpenGLES3BlendType(RERHI::Blend blend)
{
  if (blend <= RERHI::Blend::SRC_ALPHA_SAT)
  {
    static constexpr GLenum MAPPING[] =
      {
        GL_ZERO,				// RERHI::Blend::ZERO
        GL_ONE,					// RERHI::Blend::ONE
        GL_SRC_COLOR,			// RERHI::Blend::SRC_COLOR
        GL_ONE_MINUS_SRC_COLOR,	// RERHI::Blend::INV_SRC_COLOR
        GL_SRC_ALPHA,			// RERHI::Blend::SRC_ALPHA
        GL_ONE_MINUS_SRC_ALPHA,	// RERHI::Blend::INV_SRC_ALPHA
        GL_DST_ALPHA,			// RERHI::Blend::DEST_ALPHA
        GL_ONE_MINUS_DST_ALPHA,	// RERHI::Blend::INV_DEST_ALPHA
        GL_DST_COLOR,			// RERHI::Blend::DEST_COLOR
        GL_ONE_MINUS_DST_COLOR,	// RERHI::Blend::INV_DEST_COLOR
        GL_SRC_ALPHA_SATURATE	// RERHI::Blend::SRC_ALPHA_SAT
      };
    return MAPPING[static_cast<int>(blend) - static_cast<int>(RERHI::Blend::ZERO)];
  }
  else
  {
    static constexpr GLenum MAPPING[] =
      {
        GL_SRC_COLOR,			// RERHI::Blend::BLEND_FACTOR			TODO(naetherm) Mapping "RERHI::Blend::BLEND_FACTOR" to OpenGL ES 3 possible?
        GL_ONE_MINUS_SRC_COLOR,	// RERHI::Blend::INV_BLEND_FACTOR		TODO(naetherm) Mapping "RERHI::Blend::INV_BLEND_FACTOR" to OpenGL ES 3 possible?
        GL_SRC_COLOR,			// RERHI::Blend::SRC_1_COLOR			TODO(naetherm) Mapping "RERHI::Blend::SRC_1_COLOR" to OpenGL ES 3 possible?
        GL_ONE_MINUS_SRC_COLOR,	// RERHI::Blend::INV_SRC_1_COLOR		TODO(naetherm) Mapping "RERHI::Blend::INV_SRC_1_COLOR" to OpenGL ES 3 possible?
        GL_SRC_COLOR,			// RERHI::Blend::SRC_1_ALPHA			TODO(naetherm) Mapping "RERHI::Blend::SRC_1_ALPHA" to OpenGL ES 3 possible?
        GL_ONE_MINUS_SRC_COLOR,	// RERHI::Blend::INV_SRC_1_ALPHA		TODO(naetherm) Mapping "RERHI::Blend::INV_SRC_1_ALPHA" to OpenGL ES 3 possible?
      };
    return MAPPING[static_cast<int>(blend) - static_cast<int>(RERHI::Blend::BLEND_FACTOR)];
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
