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
#include <RERHI/RHIRootSignature.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    Vulkan root signature ("pipeline layout" in Vulkan terminology) class
*/
class RootSignature final : public RERHI::RHIRootSignature
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] rootSignature
  *    Root signature to use
  */
  RootSignature(RHIDynamicRHI& vulkanRhi, const RERHI::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~RootSignature() override;

  /**
  *  @brief
  *    Return the root signature data
  *
  *  @return
  *    The root signature data
  */
  [[nodiscard]] inline const RERHI::RootSignature& getRootSignature() const
  {
    return mRootSignature;
  }

  /**
  *  @brief
  *    Return the Vulkan pipeline layout
  *
  *  @return
  *    The Vulkan pipeline layout
  */
  [[nodiscard]] inline VkPipelineLayout getVkPipelineLayout() const
  {
    return mVkPipelineLayout;
  }

  /**
  *  @brief
  *    Return the Vulkan descriptor pool
  *
  *  @return
  *    The Vulkan descriptor pool
  */
  [[nodiscard]] inline VkDescriptorPool getVkDescriptorPool() const
  {
    return mVkDescriptorPool;
  }


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRootSignature methods            ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RERHI::RHIResourceGroup* createResourceGroup(RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), RootSignature, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RootSignature(const RootSignature& source) = delete;
  RootSignature& operator =(const RootSignature& source) = delete;


  //[-------------------------------------------------------]
  //[ Private definitions                                   ]
  //[-------------------------------------------------------]
private:
  typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayouts;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RERHI::RootSignature	   mRootSignature;
  VkDescriptorSetLayouts mVkDescriptorSetLayouts;
  VkPipelineLayout	   mVkPipelineLayout;
  VkDescriptorPool	   mVkDescriptorPool;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
