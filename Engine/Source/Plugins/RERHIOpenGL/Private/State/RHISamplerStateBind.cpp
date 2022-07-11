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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGL/State/RHISamplerStateBind.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Mapping.h"


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
SamplerStateBind::SamplerStateBind(RHIDynamicRHI& openGLRhi, const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
SamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLMagFilterMode(Mapping::getOpenGLMagFilterMode(openGLRhi.getContext(), samplerState.filter)),
mOpenGLMinFilterMode(Mapping::getOpenGLMinFilterMode(openGLRhi.getContext(), samplerState.filter, samplerState.maxLod > 0.0f)),
mOpenGLTextureAddressModeS(Mapping::getOpenGLTextureAddressMode(samplerState.addressU)),
mOpenGLTextureAddressModeT(Mapping::getOpenGLTextureAddressMode(samplerState.addressV)),
mOpenGLTextureAddressModeR(Mapping::getOpenGLTextureAddressMode(samplerState.addressW)),
mMipLodBias(samplerState.mipLodBias),
mMaxAnisotropy(static_cast<float>(samplerState.maxAnisotropy)),	// Maximum anisotropy is "RECore::uint32" in Direct3D 10 & 11
mOpenGLCompareMode(Mapping::getOpenGLCompareMode(openGLRhi.getContext(), samplerState.filter)),
mOpenGLComparisonFunc(Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc)),
mMinLod(samplerState.minLod),
mMaxLod(samplerState.maxLod)
{
  // Sanity check
  RHI_ASSERT(samplerState.maxAnisotropy <= openGLRhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

  // RERHI::SamplerState::borderColor[4]
  mBorderColor[0] = samplerState.borderColor[0];
  mBorderColor[1] = samplerState.borderColor[1];
  mBorderColor[2] = samplerState.borderColor[2];
  mBorderColor[3] = samplerState.borderColor[3];
}

void SamplerStateBind::setOpenGLSamplerStates() const
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
  // -> "GL_EXT_texture_lod_bias"-extension
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

  // RERHI::SamplerState::maxAnisotropy
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, samplerState.maxAnisotropy);

  // RERHI::SamplerState::comparisonFunc
  // -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
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
} // RERHIOpenGL
