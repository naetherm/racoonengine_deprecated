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
#include "RERHIOpenGL/RERHIOpenGL.h"
#include "RERHIOpenGL/State/RHISamplerState.h"


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
*    OpenGL sampler state class, traditional bind version to emulate a sampler object
*/
class SamplerStateBind final : public SamplerState
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] samplerState
  *    Sampler state to use
  */
  SamplerStateBind(RHIDynamicRHI& openGLRhi, const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~SamplerStateBind() override = default;

  /**
  *  @brief
  *    Set the OpenGL sampler states
  */
  void setOpenGLSamplerStates() const;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit SamplerStateBind(const SamplerStateBind& source) = delete;
  SamplerStateBind& operator =(const SamplerStateBind& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // "RERHI::SamplerState" translated into OpenGL
  GLint  mOpenGLMagFilterMode;		///< RERHI::SamplerState::filter
  GLint  mOpenGLMinFilterMode;		///< RERHI::SamplerState::filter
  GLint  mOpenGLTextureAddressModeS;	///< RERHI::SamplerState::addressU
  GLint  mOpenGLTextureAddressModeT;	///< RERHI::SamplerState::addressV
  GLint  mOpenGLTextureAddressModeR;	///< RERHI::SamplerState::addressW
  float  mMipLodBias;					///< RERHI::SamplerState::mipLodBias
  float  mMaxAnisotropy;				///< RERHI::SamplerState::maxAnisotropy
  GLint  mOpenGLCompareMode;			///< RERHI::SamplerState::comparisonFunc
  GLenum mOpenGLComparisonFunc;		///< RERHI::SamplerState::comparisonFunc
  float  mBorderColor[4];				///< RERHI::SamplerState::borderColor[4]
  float  mMinLod;						///< RERHI::SamplerState::minLod
  float  mMaxLod;						///< RERHI::SamplerState::maxLod


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
