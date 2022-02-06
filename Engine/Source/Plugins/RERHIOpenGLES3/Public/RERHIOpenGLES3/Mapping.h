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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    OpenGL ES 3 mapping
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
  *    "RERHI::FilterMode" to OpenGL ES 3 magnification filter mode
  *
  *  @param[in] context
  *    RHI context to use
  *  @param[in] filterMode
  *    "RERHI::FilterMode" to map
  *
  *  @return
  *    OpenGL ES 3 magnification filter mode
  */
  [[nodiscard]] static GLint getOpenGLES3MagFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode);

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
  [[nodiscard]] static GLint getOpenGLES3MinFilterMode([[maybe_unused]] const RERHI::RHIContext& context, RERHI::FilterMode filterMode, bool hasMipmaps);

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
  [[nodiscard]] static GLint getOpenGLES3CompareMode([[maybe_unused]] RERHI::FilterMode filterMode);

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
  [[nodiscard]] static GLint getOpenGLES3TextureAddressMode(RERHI::TextureAddressMode textureAddressMode);

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
  [[nodiscard]] static GLenum getOpenGLES3ComparisonFunc(RERHI::ComparisonFunc comparisonFunc);

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
  [[nodiscard]] static GLint getOpenGLES3Size(RERHI::VertexAttributeFormat vertexAttributeFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3Type(RERHI::VertexAttributeFormat vertexAttributeFormat);

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
  [[nodiscard]] static GLboolean isOpenGLES3VertexAttributeFormatNormalized(RERHI::VertexAttributeFormat vertexAttributeFormat);

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
  [[nodiscard]] static GLboolean isOpenGLES3VertexAttributeFormatInteger(RERHI::VertexAttributeFormat vertexAttributeFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3Type(RERHI::BufferUsage bufferUsage);

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
  [[nodiscard]] static GLenum getOpenGLES3Type(RERHI::IndexBufferFormat::Enum indexBufferFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3InternalFormat(RERHI::TextureFormat::Enum textureFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3Format(RERHI::TextureFormat::Enum textureFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3Type(RERHI::TextureFormat::Enum textureFormat);

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
  [[nodiscard]] static GLenum getOpenGLES3Type(RERHI::PrimitiveTopology primitiveTopology);

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
  [[nodiscard]] static GLbitfield getOpenGLES3MapRangeType(RERHI::MapType mapType);

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
  [[nodiscard]] static GLenum getOpenGLES3BlendType(RERHI::Blend blend);

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
