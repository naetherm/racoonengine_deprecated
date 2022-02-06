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
#include "RERHINull/RHIResourceGroup.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] rhi
*    Owner RHI instance
*  @param[in] rootParameterIndex
*    The root parameter index number for binding
*  @param[in] numberOfResources
*    Number of resources, having no resources is invalid
*  @param[in] resources
*    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
*  @param[in] samplerStates
*    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
*/
ResourceGroup::ResourceGroup(RERHI::RHIDynamicRHI& rhi, uint32_t rootParameterIndex, uint32_t numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIResourceGroup(rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mRootParameterIndex(rootParameterIndex),
mNumberOfResources(numberOfResources),
mResources(RHI_MALLOC_TYPED(rhi.getContext(), RERHI::RHIResource*, mNumberOfResources)),
mSamplerStates(nullptr)
{
  // Process all resources and add our reference to the RHI resource
  for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex, ++resources)
  {
    RERHI::RHIResource* resource = *resources;
    RHI_ASSERT(nullptr != resource, "Invalid null resource")
    mResources[resourceIndex] = resource;
    resource->AddReference();
  }
  if (nullptr != samplerStates)
  {
    mSamplerStates = RHI_MALLOC_TYPED(rhi.getContext(), RERHI::RHISamplerState*, mNumberOfResources);
    for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
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
    for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
    {
      RERHI::RHISamplerState* samplerState = mSamplerStates[resourceIndex];
      if (nullptr != samplerState)
      {
        samplerState->Release();
      }
    }
    RHI_FREE(context, mSamplerStates);
  }
  for (uint32_t resourceIndex = 0; resourceIndex < mNumberOfResources; ++resourceIndex)
  {
    mResources[resourceIndex]->Release();
  }
  RHI_FREE(context, mResources);
}

/**
*  @brief
*    Return the number of resources this resource group groups together
*
*  @return
*    The number of resources this resource group groups together
*/
uint32_t ResourceGroup::getNumberOfResources() const
{
  return mNumberOfResources;
}

/**
*  @brief
*    Return the RHI resources
*
*  @return
*    The RHI resources, don't release or destroy the returned pointer
*/
RERHI::RHIResource** ResourceGroup::getResources() const
{
  return mResources;
}


void ResourceGroup::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), ResourceGroup, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
