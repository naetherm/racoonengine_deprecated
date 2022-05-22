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
#include "RERHIOpenGL/State/RHISamplerStateSo.h"
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
SamplerStateSo::SamplerStateSo(RHIDynamicRHI& openGLRhi, const RERHI::SamplerState& samplerState RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
SamplerState(openGLRhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mOpenGLSampler(0)
{
  // Sanity check
  RHI_ASSERT(samplerState.maxAnisotropy <= openGLRhi.getCapabilities().maximumAnisotropy, "Maximum OpenGL anisotropy value violated")

  // Create the OpenGL sampler
  glGenSamplers(1, &mOpenGLSampler);

  // RERHI::SamplerState::filter
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MAG_FILTER, Mapping::getOpenGLMagFilterMode(openGLRhi.getContext(), samplerState.filter));
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_MIN_FILTER, Mapping::getOpenGLMinFilterMode(openGLRhi.getContext(), samplerState.filter, samplerState.maxLod > 0.0f));

  // RERHI::SamplerState::addressU
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_S, Mapping::getOpenGLTextureAddressMode(samplerState.addressU));

  // RERHI::SamplerState::addressV
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_T, Mapping::getOpenGLTextureAddressMode(samplerState.addressV));

  // RERHI::SamplerState::addressW
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_WRAP_R, Mapping::getOpenGLTextureAddressMode(samplerState.addressW));

  // RERHI::SamplerState::mipLodBias
  // -> "GL_EXT_texture_lod_bias"-extension
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_LOD_BIAS, samplerState.mipLodBias);

  // RERHI::SamplerState::maxAnisotropy
  // -> Maximum anisotropy is "RECore::uint32" in Direct3D 10 & 11
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(samplerState.maxAnisotropy));

  // RERHI::SamplerState::comparisonFunc
  // -> "GL_EXT_shadow_funcs"/"GL_EXT_shadow_samplers"-extension
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_MODE, Mapping::getOpenGLCompareMode(openGLRhi.getContext(), samplerState.filter));
  glSamplerParameteri(mOpenGLSampler, GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(Mapping::getOpenGLComparisonFunc(samplerState.comparisonFunc)));

  // RERHI::SamplerState::borderColor[4]
  glSamplerParameterfv(mOpenGLSampler, GL_TEXTURE_BORDER_COLOR, samplerState.borderColor);

  // RERHI::SamplerState::minLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MIN_LOD, samplerState.minLod);

  // RERHI::SamplerState::maxLod
  glSamplerParameterf(mOpenGLSampler, GL_TEXTURE_MAX_LOD, samplerState.maxLod);

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Sampler state", 16)	// 16 = "Sampler state: " including terminating zero
					glObjectLabel(GL_SAMPLER, mOpenGLSampler, -1, detailedDebugName);
				}
#endif
}

SamplerStateSo::~SamplerStateSo()
{
  // Destroy the OpenGL sampler
  // -> Silently ignores 0's and names that do not correspond to existing samplers
  glDeleteSamplers(1, &mOpenGLSampler);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
