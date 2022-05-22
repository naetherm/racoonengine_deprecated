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
#include "RERHI/RERHI.h"
#include "RERHI/RHIResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


class RHIResourceGroup;

class RHISamplerState;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract root signature ("pipeline layout" in Vulkan terminology) interface
*
*  @note
*    - Overview of the binding models of explicit APIs: "Choosing a binding model" - https://github.com/gpuweb/gpuweb/issues/19
*/
class RHIRootSignature : public RHIResource {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIRootSignature() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfRootSignatures;
#endif
  }

  // Public virtual Rhi::RHIRootSignature methods
public:
  /**
  *  @brief
  *    Create a resource group instance
  *
  *  @param[in] rootParameterIndex
  *    The root parameter index number for binding
  *  @param[in] numberOfResources
  *    Number of resources, having no resources is invalid
  *  @param[in] resources
  *    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
  *  @param[in] samplerStates
  *    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
  *
  *  @return
  *    The created resource group instance, a null pointer on error. Release the returned instance if you no longer need it.
  */
  [[nodiscard]] virtual RHIResourceGroup *
  createResourceGroup(RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RHIResource **resources,
                      RHISamplerState **samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline explicit RHIRootSignature(RHIDynamicRHI &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIResource(ResourceType::ROOT_SIGNATURE, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedRootSignatures;
        ++rhi.getStatistics().currentNumberOfRootSignatures;
#endif
  }

  explicit RHIRootSignature(const RHIRootSignature &source) = delete;

  RHIRootSignature &operator=(const RHIRootSignature &source) = delete;

};

typedef RECore::SmartRefCount<RHIRootSignature> RHIRootSignaturePtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
