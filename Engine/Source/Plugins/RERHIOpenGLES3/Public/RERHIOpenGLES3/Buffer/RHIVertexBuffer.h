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
*    OpenGL ES 3 vertex buffer object (VBO, "array buffer" in OpenGL terminology) class
*/
class VertexBuffer final : public RERHI::RHIVertexBuffer
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
  *    Number of bytes within the vertex buffer, must be valid
  *  @param[in] data
  *    Vertex buffer data, can be a null pointer (empty buffer)
  *  @param[in] bufferUsage
  *    Indication of the buffer usage
  */
  VertexBuffer(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 numberOfBytes, const void* data, RERHI::BufferUsage bufferUsage RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~VertexBuffer() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 array buffer
  *
  *  @return
  *    The OpenGL ES 3 array buffer, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3ArrayBuffer() const
  {
    return mOpenGLES3ArrayBuffer;
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
    RHI_DELETE(getRhi().getContext(), VertexBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit VertexBuffer(const VertexBuffer& source) = delete;
  VertexBuffer& operator =(const VertexBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint	 mOpenGLES3ArrayBuffer;	///< OpenGL ES 3 array buffer, can be zero if no resource is allocated
  RECore::uint32 mBufferSize;			///< Holds the size of the buffer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
