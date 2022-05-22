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
#include "RERHINull/RERHINull.h"
#include <RERHI/RHIResourceGroup.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null resource group class
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
  ResourceGroup(RERHI::RHIDynamicRHI& rhi, RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~ResourceGroup() override;

  /**
  *  @brief
  *    Return the number of resources this resource group groups together
  *
  *  @return
  *    The number of resources this resource group groups together
  */
  [[nodiscard]] RECore::uint32 getNumberOfResources() const;

  /**
  *  @brief
  *    Return the RHI resources
  *
  *  @return
  *    The RHI resources, don't release or destroy the returned pointer
  */
  [[nodiscard]] RERHI::RHIResource** getResources() const;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


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
  RECore::uint32			 mRootParameterIndex;	///< The root parameter index number for binding
  RECore::uint32			 mNumberOfResources;	///< Number of resources this resource group groups together
  RERHI::RHIResource**	 mResources;			///< RHI resources, we keep a reference to it
  RERHI::RHISamplerState** mSamplerStates;		///< Sampler states, we keep a reference to it


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
