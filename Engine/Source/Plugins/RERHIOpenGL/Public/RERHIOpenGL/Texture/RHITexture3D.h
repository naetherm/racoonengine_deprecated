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
*    Abstract OpenGL 3D texture interface
*/
class Texture3D : public RERHI::RHITexture3D
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture3D() override;

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
  *    Return the texture format
  *
  *  @return
  *    The texture format
  */
  [[nodiscard]] inline RERHI::TextureFormat::Enum getTextureFormat() const
  {
    return mTextureFormat;
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

  /**
  *  @brief
  *    Return the OpenGL pixel unpack buffer for dynamic textures
  *
  *  @return
  *    The OpenGL pixel unpack buffer for dynamic textures, can be zero if no resource is allocated, do not destroy the returned resource
  */
  [[nodiscard]] inline GLuint getOpenGLPixelUnpackBuffer() const
  {
    return mOpenGLPixelUnpackBuffer;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void* getInternalResourceHandle() const override
  {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLTexture));
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Texture3D, this);
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
  *  @param[in] depth
  *    The depth of the texture
  *  @param[in] textureFormat
  *    Texture format
  */
  Texture3D(RHIDynamicRHI& openGLRhi, RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth, RERHI::TextureFormat::Enum textureFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);


  //[-------------------------------------------------------]
  //[ Protected data                                        ]
  //[-------------------------------------------------------]
protected:
  GLuint					 mOpenGLTexture;			///< OpenGL texture, can be zero if no resource is allocated
  RERHI::TextureFormat::Enum mTextureFormat;
  GLuint					 mOpenGLInternalFormat;		///< OpenGL internal format
  GLuint					 mOpenGLPixelUnpackBuffer;	///< OpenGL pixel unpack buffer for dynamic textures, can be zero if no resource is allocated


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture3D(const Texture3D& source) = delete;
  Texture3D& operator =(const Texture3D& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
