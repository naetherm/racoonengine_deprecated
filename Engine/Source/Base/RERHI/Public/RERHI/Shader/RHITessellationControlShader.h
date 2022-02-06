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
#include "RERHI/Shader/RHIShader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract tessellation control shader (TCS, "hull shader" in Direct3D terminology) interface
*/
class RHITessellationControlShader : public RHIShader {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHITessellationControlShader() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfTessellationControlShaders;
#endif
  }

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline explicit RHITessellationControlShader(RHIDynamicRHI &rhi RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIShader(ResourceType::TESSELLATION_CONTROL_SHADER, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedTessellationControlShaders;
        ++rhi.getStatistics().currentNumberOfTessellationControlShaders;
#endif
  }

  explicit RHITessellationControlShader(const RHITessellationControlShader &source) = delete;

  RHITessellationControlShader &operator=(const RHITessellationControlShader &source) = delete;

};

typedef RECore::SmartRefCount<RHITessellationControlShader> RHITessellationControlShaderPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
