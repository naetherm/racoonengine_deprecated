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
*    OpenGL ES 3 index buffer object (IBO, "element array buffer" in OpenGL terminology) class
*/
class IndexBuffer final : public RERHI::RHIIndexBuffer
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
  *    Number of bytes within the index buffer, must be valid
  *  @param[in] data
  *    Index buffer data, can be a null pointer (empty buffer)
  *  @param[in] bufferUsage
  *    Indication of the buffer usage
  *  @param[in] indexBufferFormat
  *    Index buffer data format
  */
  IndexBuffer(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~IndexBuffer() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 element array buffer
  *
  *  @return
  *    The OpenGL ES 3 element array buffer, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3ElementArrayBuffer() const
  {
    return mOpenGLES3ElementArrayBuffer;
  }

  /**
  *  @brief
  *    Return the OpenGL ES 3 element array buffer data type
  *
  *  @return
  *    The OpenGL ES 3 element array buffer data type
  */
  [[nodiscard]] inline GLenum getOpenGLES3Type() const
  {
    return mOpenGLES3Type;
  }

  /**
  *  @brief
  *    Return the number of bytes of an index
  *
  *  @return
  *    The number of bytes of an index
  */
  [[nodiscard]] inline RECore::uint32 getIndexSizeInBytes() const
  {
    return mIndexSizeInBytes;
  }

  [[nodiscard]] inline RECore::uint32 getBufferSize() const
  {
    return mBufferSize;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), IndexBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit IndexBuffer(const IndexBuffer& source) = delete;
  IndexBuffer& operator =(const IndexBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint	 mOpenGLES3ElementArrayBuffer;	///< OpenGL ES 3 element array buffer, can be zero if no resource is allocated
  GLenum   mOpenGLES3Type;				///< OpenGL ES 3 element array buffer data type
  RECore::uint32 mIndexSizeInBytes;				///< Number of bytes of an index
  RECore::uint32 mBufferSize;					///< Holds the size of the buffer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
