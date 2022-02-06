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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/State/RHISamplerState.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

SamplerState::SamplerState(RHIDynamicRHI& openGLES3Rhi, const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHISamplerState(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLMagFilterMode(Mapping::getOpenGLES3MagFilterMode(openGLES3Rhi.getContext(), samplerState.filter)),
  mOpenGLMinFilterMode(Mapping::getOpenGLES3MinFilterMode(openGLES3Rhi.getContext(), samplerState.filter, samplerState.maxLod > 0.0f)),
  mOpenGLTextureAddressModeS(Mapping::getOpenGLES3TextureAddressMode(samplerState.addressU)),
  mOpenGLTextureAddressModeT(Mapping::getOpenGLES3TextureAddressMode(samplerState.addressV)),
  mOpenGLTextureAddressModeR(Mapping::getOpenGLES3TextureAddressMode(samplerState.addressW)),
  mMipLodBias(samplerState.mipLodBias),
  mMaxAnisotropy(static_cast<float>(samplerState.maxAnisotropy)),	// Maximum anisotropy is "uint32_t" in Direct3D 10 & 11
  mOpenGLCompareMode(Mapping::getOpenGLES3CompareMode(samplerState.filter)),
  mOpenGLComparisonFunc(Mapping::getOpenGLES3ComparisonFunc(samplerState.comparisonFunc)),
  mMinLod(samplerState.minLod),
  mMaxLod(samplerState.maxLod)
{
  // Sanity check
  RHI_ASSERT(samplerState.maxAnisotropy <= openGLES3Rhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL ES 3 anisotropy value violated")

  // Ignore "RERHI::SamplerState.borderColor", border color is not supported by OpenGL ES 3

  // TODO(naetherm)  "GL_COMPARE_REF_TO_TEXTURE" is not supported by OpenGL ES 3, check/inform the user?
  // TODO(naetherm)  "GL_CLAMP_TO_BORDER" is not supported by OpenGL ES 3, check/inform the user?
}

void SamplerState::setOpenGLES3SamplerStates() const
{
  // TODO(naetherm) Support other targets, not just "GL_TEXTURE_2D"

  // RERHI::SamplerState::filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mOpenGLMagFilterMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mOpenGLMinFilterMode);

  // RERHI::SamplerState::addressU
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mOpenGLTextureAddressModeS);

  // RERHI::SamplerState::addressV
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mOpenGLTextureAddressModeT);
  /*
  // TODO(naetherm) Support for 3D textures
  // RERHI::SamplerState::addressW
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, mOpenGLTextureAddressModeR);

  // TODO(naetherm) Complete me
  // RERHI::SamplerState::mipLodBias
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

  // RERHI::SamplerState::maxAnisotropy
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, samplerState.maxAnisotropy);

  // RERHI::SamplerState::comparisonFunc
  glTexParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_MODE, Mapping::getOpenGLCompareMode(samplerState.filter));
  glTexParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_FUNC, Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc));

  // RERHI::SamplerState::borderColor[4]
  glSamplerParameterfv(mOpenGLSampler, GL_TEXTURE_BORDER_COLOR, samplerState.borderColor);

  // RERHI::SamplerState::minLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MIN_LOD, samplerState.minLod);

  // RERHI::SamplerState::maxLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_LOD, samplerState.maxLod);
  */
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
