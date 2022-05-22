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
#include <RERHI/RHIRootSignature.h>
#include "RERHINull/RHIDynamicRHI.h"
#include "RERHINull/RHIResourceGroup.h"


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
*    Null root signature ("pipeline layout" in Vulkan terminology) class
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
  *  @param[in] nullRhi
  *    Owner null RHI instance
  *  @param[in] rootSignature
  *    Root signature to use
  */
  RootSignature(RHIDynamicRHI& nullRhi, const RERHI::RootSignature& rootSignature RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

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
  [[nodiscard]] const RERHI::RootSignature& getRootSignature() const;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIRootSignature methods            ]
  //[-------------------------------------------------------]
public:
  virtual RERHI::RHIResourceGroup* createResourceGroup(RECore::uint32 rootParameterIndex, RECore::uint32 numberOfResources, RERHI::RHIResource** resources, RERHI::RHISamplerState** samplerStates = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit RootSignature(const RootSignature& source) = delete;
  RootSignature& operator =(const RootSignature& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  RERHI::RootSignature mRootSignature;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
