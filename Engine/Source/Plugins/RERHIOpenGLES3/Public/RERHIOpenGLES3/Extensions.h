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
*    Abstract OpenGL ES 3 extensions base interface
*
*  @note
*    - Extensions are only optional, so do always take into account that an extension may not be available
*/
class IExtensions
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~IExtensions()
  {}


  //[-------------------------------------------------------]
  //[ Public virtual RHIDynamicRHI::RHIExtensions methods      ]
  //[-------------------------------------------------------]
public:
  ///////////////////////////////////////////////////////////
  // Returns whether an extension is supported or not
  ///////////////////////////////////////////////////////////
  // EXT
  [[nodiscard]] virtual bool isGL_EXT_texture_compression_s3tc() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_texture_compression_dxt1() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_texture_compression_latc() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_texture_buffer() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_draw_elements_base_vertex() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_base_instance() const = 0;
  [[nodiscard]] virtual bool isGL_EXT_clip_control() const = 0;
  // AMD
  [[nodiscard]] virtual bool isGL_AMD_compressed_3DC_texture() const = 0;
  // NV
  [[nodiscard]] virtual bool isGL_NV_fbo_color_attachments() const = 0;
  // OES
  [[nodiscard]] virtual bool isGL_OES_element_index_uint() const = 0;
  [[nodiscard]] virtual bool isGL_OES_packed_depth_stencil() const = 0;
  [[nodiscard]] virtual bool isGL_OES_depth24() const = 0;
  [[nodiscard]] virtual bool isGL_OES_depth32() const = 0;
  // KHR
  [[nodiscard]] virtual bool isGL_KHR_debug() const = 0;


  //[-------------------------------------------------------]
  //[ Protected methods                                     ]
  //[-------------------------------------------------------]
protected:
  /**
  *  @brief
  *    Default constructor
  */
  inline IExtensions()
  {}

  explicit IExtensions(const IExtensions& source) = delete;
  IExtensions& operator =(const IExtensions& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
