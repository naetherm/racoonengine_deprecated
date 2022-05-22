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
#include "RERHIOpenGL/RERHIOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OpenGL 2D array texture interface
*/
class Texture2DArray : public RERHI::RHITexture2DArray
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture2DArray() override;

  /**
  *  @brief
  *    Return the number of multisamples
  *
  *  @return
  *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  */
  [[nodiscard]] inline RECore::uint8 getNumberOfMultisamples() const
  {
    return mNumberOfMultisamples;
  }

  /**
  *  @brief
  *    Return the OpenGL texture
  *
  *  @return
  *    The OpenGL texture, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLTexture() const
  {
    return mOpenGLTexture;
  }

  /**
  *  @brief
  *    Return the OpenGL internal format
  *
  *  @return
  *    The OpenGL internal format
  */
  [[nodiscard]] inline GLuint getOpenGLInternalFormat() const
  {
    return mOpenGLInternalFormat;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Texture2DArray, this);
  }


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] width
  *    The width of the texture
  *  @param[in] height
  *    The height of the texture
  *  @param[in] numberOfSlices
  *    The number of slices
  *  @param[in] textureFormat
  *    Texture format
  */
  Texture2DArray(RHIDynamicRHI& openGLRhi, RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  RECore::uint8 mNumberOfMultisamples;	///< The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  GLuint  mOpenGLTexture;			///< OpenGL texture, can be zero if no resource is allocated
  GLuint  mOpenGLInternalFormat;	///< OpenGL internal format


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture2DArray(const Texture2DArray& source) = delete;
  Texture2DArray& operator =(const Texture2DArray& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
