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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"
#include "RERHIOpenGLES3/Buffer/RHITextureBuffer.h"


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
*    OpenGL ES 3 texture buffer object (TBO) class, traditional bind version
*/
class TextureBufferBind final : public TextureBuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] numberOfBytes
  *    Number of bytes within the texture buffer, must be valid
  *  @param[in] data
  *    Texture buffer data, can be a null pointer (empty buffer)
  *  @param[in] bufferUsage
  *    Indication of the buffer usage
  *  @param[in] textureFormat
  *    Texture buffer data format
  */
  TextureBufferBind(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~TextureBufferBind() override
  {}


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TextureBufferBind(const TextureBufferBind& source) = delete;
  TextureBufferBind& operator =(const TextureBufferBind& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
