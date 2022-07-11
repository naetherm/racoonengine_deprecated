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
#include "RERHIVulkan/RERHIVulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class RootSignature;


/**
*  @brief
*    Vulkan resource group class
*/
class ResourceGroup final : public RERHI::RHIResourceGroup
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rootSignature
  *    Root signature
  *  @param[in] rootParameterIndex
  *    Root parameter index
  *  @param[in] vkDescriptorSet
  *    Wrapped Vulkan descriptor set
  *  @param[in] numberOfResources
  *    Number of resources, having no resources is invalid
  *  @param[in] resources
  *    At least "numberOfResources" resource pointers, must be valid, the resource group will keep a reference to the resources
  *  @param[in] samplerStates
  *    If not a null pointer at least "numberOfResources" sampler state pointers, must be valid if there's at least one texture resource, the resource group will keep a reference to the sampler states
  */
  ResourceGroup(RootSignature& rootSignature, RECore::uint32 rootParameterIndex, VkDescriptorSet vkDescriptorSet, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~ResourceGroup() override;

  /**
  *  @brief
  *    Return the Vulkan descriptor set
  *
  *  @return
  *    The Vulkan descriptor set, can be a null handle
  */
  [[nodiscard]] inline VkDescriptorSet getVkDescriptorSet() const
  {
    return mVkDescriptorSet;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), ResourceGroup, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit ResourceGroup(const ResourceGroup& source) = delete;
  ResourceGroup& operator =(const ResourceGroup& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RootSignature&		 mRootSignature;		///< Root signature
  VkDescriptorSet		 mVkDescriptorSet;		///< "mVkDescriptorPool" of the root signature is the owner which manages the memory, can be a null handle (e.g. for a sampler resource group)
  RECore::uint32			 mNumberOfResources;	///< Number of resources this resource group groups together
  RERHI::RHIResource**	 mResources;			///< RHI resource, we keep a reference to it
  RERHI::RHISamplerState** mSamplerStates;		///< Sampler states, we keep a reference to it


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
