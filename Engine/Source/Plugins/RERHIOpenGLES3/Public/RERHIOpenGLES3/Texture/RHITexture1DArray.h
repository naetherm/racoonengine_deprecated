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
*    OpenGL ES 3 1D array texture class
*/
class Texture1DArray final : public RERHI::RHITexture1DArray
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
  *  @param[in] width
  *    Texture width, must be >0
  *  @param[in] numberOfSlices
  *    Number of slices, must be >0
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer
  *  @param[in] textureFlags
  *    Texture flags, see "RERHI::TextureFlag::Enum"
  */
  Texture1DArray(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 width, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture1DArray() override;

  /**
  *  @brief
  *    Return the OpenGL ES 3 texture
  *
  *  @return
  *    The OpenGL ES 3 texture, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLES3Texture() const
  {
    return mOpenGLES3Texture;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Texture1DArray, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture1DArray(const Texture1DArray& source) = delete;
  Texture1DArray& operator =(const Texture1DArray& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint mOpenGLES3Texture;	///< OpenGL ES 3 texture, can be zero if no resource is allocated


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
