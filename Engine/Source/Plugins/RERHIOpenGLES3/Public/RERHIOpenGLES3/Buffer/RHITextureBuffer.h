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
*    Abstract OpenGL ES 3 texture buffer object (TBO) interface
*/
class TextureBuffer : public RERHI::RHITextureBuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  virtual ~TextureBuffer() override
  {
    // Destroy the OpenGL ES 3 texture instance
    // -> Silently ignores 0's and names that do not correspond to existing textures
    glDeleteTextures(1, &mOpenGLES3Texture);

    // Destroy the OpenGL ES 3  texture buffer
    // -> Silently ignores 0's and names that do not correspond to existing buffer objects
    glDeleteBuffers(1, &mOpenGLES3TextureBuffer);
  }

  /**
  *  @brief
  *    Return the OpenGL ES 3 texture buffer instance
  *
  *  @return
  *    The OpenGL ES 3 texture buffer instance, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3TextureBuffer() const
  {
    return mOpenGLES3TextureBuffer;
  }

  /**
  *  @brief
  *    Return the OpenGL ES 3 texture instance
  *
  *  @return
  *    The OpenGL ES 3 texture instance, can be zero if no resource is allocated
  */
  [[nodiscard]] inline GLuint getOpenGLES3Texture() const
  {
    return mOpenGLES3Texture;
  }

  [[nodiscard]] inline uint32_t getBufferSize() const
  {
    return mBufferSize;
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  *  @param[in] numberOfBytes
  *    Number of bytes within the texture buffer, must be valid
  */
  TextureBuffer(RHIDynamicRHI& openGLES3Rhi, uint32_t numberOfBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), TextureBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint	 mOpenGLES3TextureBuffer;	///< OpenGL ES 3 texture buffer, can be zero if no resource is allocated
  GLuint	 mOpenGLES3Texture;			///< OpenGL ES 3 texture, can be zero if no resource is allocated
  uint32_t mBufferSize;				///< Holds the size of the buffer


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TextureBuffer(const TextureBuffer& source) = delete;
  TextureBuffer& operator =(const TextureBuffer& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
