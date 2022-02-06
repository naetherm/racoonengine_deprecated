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
#include "RERHI/State/RHIState.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract pipeline state interface
*/
class RHIPipelineState : public RHIState {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIPipelineState() override
  {}

  /**
  *  @brief
  *    Return the unique compact graphics or compute pipeline state ID; one internal ID pool per concrete pipeline state type
  *
  *  @return
  *    The unique compact graphics or compute pipeline state ID
  */
  inline uint16_t getId() const {
    return mId;
  }

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] resourceType
  *    The resource type
  *  @param[in] rhi
  *    Owner RHI instance
  *  @param[in] id
  *    The unique compact graphics or compute pipeline state ID
  */
  inline RHIPipelineState(ResourceType resourceType, RHIDynamicRHI &rhi, uint16_t id
                          RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIState(resourceType, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
    mId(id) {}

  explicit RHIPipelineState(const RHIPipelineState &source) = delete;

  RHIPipelineState &operator=(const RHIPipelineState &source) = delete;

  // Private data
private:
  uint16_t mId;
};

typedef RECore::SmartRefCount<RHIPipelineState> RHIPipelineStatePtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
