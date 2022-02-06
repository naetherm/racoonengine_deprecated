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
#include "RERHIOpenGLES3/RHIResourceGroup.h"
#include "RERHIOpenGLES3/OpenGLES3Context.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Extensions.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


ResourceGroup::ResourceGroup(RHIDynamicRHI &openGLES3Rhi, const RERHI::RootSignature &rootSignature,
                             uint32_t rootParameterIndex, uint32_t numberOfResources, RERHI::RHIResource **resources,
                             RERHI::RHISamplerState **samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIResourceGroup(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootParameterIndex(rootParameterIndex),
  mNumberOfResources(numberOfResources),
  mResources(RHI_MALLOC_TYPED(openGLES3Rhi.getContext(), RERHI::RHIResource*, mNumberOfResources)),
  mSamplerStates(nullptr),
  mResourceIndexToUniformBlockBindingIndex(nullptr) {
  // Get the uniform block binding start index
  const RERHI::RHIContext &context = openGLES3Rhi.getContext();
  const bool isGL_EXT_texture_buffer = openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_EXT_texture_buffer();
  uint32_t uniformBlockBindingIndex = 0;
  for (uint32_t currentRootParameterIndex = 0;
       currentRootParameterIndex < rootParameterIndex; ++currentRootParameterIndex) {
    const RERHI::RootParameter &rootParameter = rootSignature.parameters[currentRootParameterIndex];
    if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
      RHI_ASSERT(
        nullptr != reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges),
        "Invalid OpenGL ES 3 descriptor ranges")
      const uint32_t numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
      for (uint32_t descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex) {
        const RERHI::DescriptorRange &descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex];
        if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType) {
          ++uniformBlockBindingIndex;
        } else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType && !isGL_EXT_texture_buffer &&
                   nullptr != strstr(descriptorRange.baseShaderRegisterName, "TextureBuffer")) {
          // Texture buffer emulation using uniform buffer
          ++uniformBlockBindingIndex;
        }
      }
    }
  }

  // Process all resources and add our reference to the RHI resource
  const RERHI::RootParameter &rootParameter = rootSignature.parameters[rootParameterIndex];
  for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources) {
    RERHI::RHIResource *resource = *resources;
    RHI_ASSERT(nullptr != resource, "Invalid OpenGL ES 3 resource")
    mResources[resourceIndex] = resource;
    resource->AddReference();

    // Uniform block binding index handling
    bool isUniformBuffer = false;
    const RERHI::DescriptorRange &descriptorRange = reinterpret_cast<const RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];
    if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType) {
      isUniformBuffer = true;
    } else if (RERHI::DescriptorRangeType::SAMPLER != descriptorRange.rangeType && !isGL_EXT_texture_buffer &&
               nullptr != strstr(descriptorRange.baseShaderRegisterName, "TextureBuffer")) {
      // Texture buffer emulation using uniform buffer
      isUniformBuffer = true;
    }
    if (isUniformBuffer) {
      if (nullptr == mResourceIndexToUniformBlockBindingIndex) {
        mResourceIndexToUniformBlockBindingIndex = RHI_MALLOC_TYPED(context, uint32_t, mNumberOfResources);
        memset(mResourceIndexToUniformBlockBindingIndex, 0, sizeof(uint32_t) * mNumberOfResources);
      }
      mResourceIndexToUniformBlockBindingIndex[resourceIndex] = uniformBlockBindingIndex;
      ++uniformBlockBindingIndex;
    }
  }
  if (nullptr != samplerStates) {
    mSamplerStates = RHI_MALLOC_TYPED(context, RERHI::RHISamplerState*, mNumberOfResources);
    for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      RERHI::RHISamplerState *samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->AddReference();
      }
    }
  }
}

ResourceGroup::~ResourceGroup() {
// Remove our reference from the RHI resources
  const RERHI::RHIContext &context = getRhi().getContext();
  if (nullptr != mSamplerStates) {
    for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
      RERHI::RHISamplerState *samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState) {
        samplerState->Release();
      }
    }
    RHI_FREE(context, mSamplerStates);
  }
  for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex) {
    mResources[resourceIndex]->Release();
  }
  RHI_FREE(context, mResources);
  RHI_FREE(context, mResourceIndexToUniformBlockBindingIndex);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
