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
*    OpenGL ES 3 2D texture class
*/
class Texture2D final : public RERHI::RHITexture2D
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
  *  @param[in] height
  *    Texture height, must be >0
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer
  *  @param[in] textureFlags
  *    Texture flags, see "RERHI::TextureFlag::Enum"
  */
  Texture2D(RHIDynamicRHI& openGLES3Rhi, RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat, const void* data, RECore::uint32 textureFlags RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~Texture2D() override;

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

  /**
  *  @brief
  *    Set minimum maximum mipmap index
  *
  *  @param[in] minimumMipmapIndex
  *    Minimum mipmap index, the most detailed mipmap, also known as base mipmap, 0 by default
  *  @param[in] maximumMipmapIndex
  *    Maximum mipmap index, the least detailed mipmap, <number of mipmaps> by default
  */
  void setMinimumMaximumMipmapIndex(RECore::uint32 minimumMipmapIndex, RECore::uint32 maximumMipmapIndex)
  {
#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently bound OpenGL ES 3 texture
				GLint openGLES3TextureBackup = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &openGLES3TextureBackup);
#endif

    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sampler_objects.txt
    // "
    //  2) What is the set of state associated with a sampler object?
    //     Specifically, should TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL be
    //     part of the sampler or the texture?
    //  DISCUSSION: TEXTURE_BASE_LEVEL and TEXTURE_MAX_LEVEL are presently
    //  part of the image state (texture) and are thus not included in the
    //  sampler object.
    // "
    glBindTexture(GL_TEXTURE_2D, mOpenGLES3Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, static_cast<GLint>(minimumMipmapIndex));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(maximumMipmapIndex));

#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL ES 3 texture
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(openGLES3TextureBackup));
#endif
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIResource methods                 ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] inline virtual void* getInternalResourceHandle() const override
  {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(mOpenGLES3Texture));
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), Texture2D, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit Texture2D(const Texture2D& source) = delete;
  Texture2D& operator =(const Texture2D& source) = delete;


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
