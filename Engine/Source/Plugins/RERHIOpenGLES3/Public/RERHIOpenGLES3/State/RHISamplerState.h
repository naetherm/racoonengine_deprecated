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
*    OpenGL ES 3 sampler state class
*/
class SamplerState final : public RERHI::RHISamplerState
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
  *  @param[in] samplerState
  *    Sampler state to use
  */
  SamplerState(RHIDynamicRHI& openGLES3Rhi, const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~SamplerState() override
  {}

  /**
  *  @brief
  *    Set the OpenGL ES 3 sampler states
  */
  void setOpenGLES3SamplerStates() const;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), SamplerState, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit SamplerState(const SamplerState& source) = delete;
  SamplerState& operator =(const SamplerState& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  // "RERHI::SamplerState" translated into OpenGL ES 3
  GLint  mOpenGLMagFilterMode;		///< RERHI::SamplerState::filter
  GLint  mOpenGLMinFilterMode;		///< RERHI::SamplerState::filter
  GLint  mOpenGLTextureAddressModeS;	///< RERHI::SamplerState::addressU
  GLint  mOpenGLTextureAddressModeT;	///< RERHI::SamplerState::addressV
  GLint  mOpenGLTextureAddressModeR;	///< RERHI::SamplerState::addressW
  float  mMipLodBias;					///< RERHI::SamplerState::mipLodBias
  float  mMaxAnisotropy;				///< RERHI::SamplerState::maxAnisotropy
  GLint  mOpenGLCompareMode;			///< RERHI::SamplerState::comparisonFunc
  GLenum mOpenGLComparisonFunc;		///< RERHI::SamplerState::comparisonFunc
  float  mMinLod;						///< RERHI::SamplerState::minLod
  float  mMaxLod;						///< RERHI::SamplerState::maxLod


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
