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
#include "RERHIOpenGL/RHIResourceGroup.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ResourceGroup::ResourceGroup(RERHI::RHIDynamicRHI& rhi, const RERHI::RootSignature& rootSignature, RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIResourceGroup(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mRootParameterIndex(rootParameterIndex),
mNumberOfResources(numberOfResources),
mResources(RHI_MALLOC_TYPED(rhi.getContext(), RERHI::RHIResource*, mNumberOfResources)),
mSamplerStates(nullptr),
mResourceIndexToUniformBlockBindingIndex(nullptr)
{
  // Get the uniform block binding start index
  const RERHI::RHIContext& context = rhi.getContext();
  RECore::uint32 uniformBlockBindingIndex = 0;
  for (RECore::uint32 currentRootParameterIndex = 0; currentRootParameterIndex < rootParameterIndex; ++currentRootParameterIndex)
  {
    const RERHI::RootParameter& rootParameter = rootSignature.parameters[currentRootParameterIndex];
    if (RERHI::RootParameterType::DESCRIPTOR_TABLE == rootParameter.parameterType)
    {
      RHI_ASSERT(nullptr != reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges), "Invalid OpenGL descriptor ranges")
      const RECore::uint32 numberOfDescriptorRanges = rootParameter.descriptorTable.numberOfDescriptorRanges;
      for (RECore::uint32 descriptorRangeIndex = 0; descriptorRangeIndex < numberOfDescriptorRanges; ++descriptorRangeIndex)
      {
        if (RERHI::DescriptorRangeType::UBV == reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[descriptorRangeIndex].rangeType)
        {
          ++uniformBlockBindingIndex;
        }
      }
    }
  }

  // Process all resources and add our reference to the RHI resource
  const RERHI::RootParameter& rootParameter = rootSignature.parameters[rootParameterIndex];
  for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources)
  {
    RERHI::RHIResource* resource = *resources;
    RHI_ASSERT(nullptr != resource, "Invalid OpenGL resource")
    mResources[resourceIndex] = resource;
    resource->AddReference();

    // Uniform block binding index handling
    const RERHI::DescriptorRange& descriptorRange = reinterpret_cast<const RERHI::DescriptorRange*>(rootParameter.descriptorTable.descriptorRanges)[resourceIndex];
    if (RERHI::DescriptorRangeType::UBV == descriptorRange.rangeType)
    {
      if (nullptr == mResourceIndexToUniformBlockBindingIndex)
      {
        mResourceIndexToUniformBlockBindingIndex = RHI_MALLOC_TYPED(context, RECore::uint32, mNumberOfResources);
        memset(mResourceIndexToUniformBlockBindingIndex, 0, sizeof(RECore::uint32) * mNumberOfResources);
      }
      mResourceIndexToUniformBlockBindingIndex[resourceIndex] = uniformBlockBindingIndex;
      ++uniformBlockBindingIndex;
    }
  }
  if (nullptr != samplerStates)
  {
    mSamplerStates = RHI_MALLOC_TYPED(context, RERHI::RHISamplerState*, mNumberOfResources);
    for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
    {
      RERHI::RHISamplerState* samplerState = mSamplerStates[resourceIndex] = samplerStates[resourceIndex];
      if (nullptr != samplerState)
      {
        samplerState->AddReference();
      }
    }
  }
}

/**
*  @brief
*    Destructor
*/
ResourceGroup::~ResourceGroup()
{
  // Remove our reference from the RHI resources
  const RERHI::RHIContext& context = getRhi().getContext();
  if (nullptr != mSamplerStates)
  {
    for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
    {
      RERHI::RHISamplerState* samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState)
      {
        samplerState->Release();
      }
    }
    RHI_FREE(context, mSamplerStates);
  }
  for (RECore::uint32 resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
  {
    mResources[resourceIndex]->Release();
  }
  RHI_FREE(context, mResources);
  RHI_FREE(context, mResourceIndexToUniformBlockBindingIndex);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
