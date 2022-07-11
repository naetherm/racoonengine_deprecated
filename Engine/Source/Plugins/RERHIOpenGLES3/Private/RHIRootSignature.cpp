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
#include "RERHIOpenGLES3/RHIRootSignature.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/RHIResourceGroup.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

RootSignature::RootSignature(RHIDynamicRHI &openGLES3Rhi, const RERHI::RootSignature &rootSignature
                             RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIRootSignature(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mRootSignature(rootSignature) {
  const RERHI::RHIContext &context = openGLES3Rhi.getContext();

  { // Copy the parameter data
    const RECore::uint32 numberOfParameters = mRootSignature.numberOfParameters;
    if (numberOfParameters > 0) {
      mRootSignature.parameters = RHI_MALLOC_TYPED(context, RERHI::RootParameter, numberOfParameters);
      RERHI::RootParameter *destinationRootParameters = const_cast<RERHI::RootParameter *>(mRootSignature.parameters);
      memcpy(destinationRootParameters, rootSignature.parameters, sizeof(RERHI::RootParameter) * numberOfParameters);

      // Copy the descriptor table data
      for (RECore::uint32 i = 0; i < numberOfParameters; ++i) {
        RERHI::RootParameter &destinationRootParameter = destinationRootParameters[i];
        const RERHI::RootParameter &sourceRootParameter = rootSignature.parameters[i];
        if (RERHI::RootParameterType::DESCRIPTOR_TABLE == destinationRootParameter.parameterType) {
          const RECore::uint32 numberOfDescriptorRanges = destinationRootParameter.descriptorTable.numberOfDescriptorRanges;
          destinationRootParameter.descriptorTable.descriptorRanges = reinterpret_cast<uintptr_t>(RHI_MALLOC_TYPED(
            context, RERHI::DescriptorRange, numberOfDescriptorRanges));
          memcpy(reinterpret_cast<RERHI::DescriptorRange *>(destinationRootParameter.descriptorTable.descriptorRanges),
                 reinterpret_cast<const RERHI::DescriptorRange *>(sourceRootParameter.descriptorTable.descriptorRanges),
                 sizeof(RERHI::DescriptorRange) * numberOfDescriptorRanges);
        }
      }
    }
  }

  { // Copy the static sampler data
    const RECore::uint32 numberOfStaticSamplers = mRootSignature.numberOfStaticSamplers;
    if (numberOfStaticSamplers > 0) {
      mRootSignature.staticSamplers = RHI_MALLOC_TYPED(context, RERHI::StaticSampler, numberOfStaticSamplers);
      memcpy(const_cast<RERHI::StaticSampler *>(mRootSignature.staticSamplers), rootSignature.staticSamplers,
             sizeof(RERHI::StaticSampler) * numberOfStaticSamplers);
    }
  }
}

RootSignature::~RootSignature() {
// Destroy the root signature data
  const RERHI::RHIContext &context = getRhi().getContext();
  if (nullptr != mRootSignature.parameters) {
    for (RECore::uint32 i = 0; i < mRootSignature.numberOfParameters; ++i) {
      const RERHI::RootParameter &rootParameter = mRootSignature.parameters[i];
      if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType) {
        RHI_FREE(context, reinterpret_cast<RERHI::DescriptorRange *>(rootParameter.descriptorTable.descriptorRanges));
      }
    }
    RHI_FREE(context, const_cast<RERHI::RootParameter *>(mRootSignature.parameters));
  }
  RHI_FREE(context, const_cast<RERHI::StaticSampler *>(mRootSignature.staticSamplers));
}

RERHI::RHIResourceGroup *RootSignature::createResourceGroup(RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources,
                                                            RERHI::RHIResource **resources,
                                                            RERHI::RHISamplerState **samplerStates
                                                            RHI_RESOURCE_DEBUG_NAME_PARAMETER) {
  RHIDynamicRHI &openGLES3Rhi = static_cast<RHIDynamicRHI &>(getRhi());

// Sanity checks
  RHI_ASSERT(rootParameterIndex < mRootSignature.numberOfParameters,
             "The OpenGL ES 3 root parameter index is out-of-bounds")
  RHI_ASSERT(numberOfResources > 0, "The number of OpenGL ES 3 resources must not be zero")
  RHI_ASSERT(nullptr != resources, "The OpenGL ES 3 resource pointers must be valid")

// Create resource group
  return RHI_NEW(openGLES3Rhi.getContext(),
                 ResourceGroup)(openGLES3Rhi, getRootSignature(), rootParameterIndex, numberOfResources, resources,
                                samplerStates
                                RHI_RESOURCE_DEBUG_PASS_PARAMETER);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
