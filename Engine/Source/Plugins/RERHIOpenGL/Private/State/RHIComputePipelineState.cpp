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
#include "RERHIOpenGL/State/RHIComputePipelineState.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGL {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
ComputePipelineState::ComputePipelineState(RHIDynamicRHI& openGLRhi, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHIComputePipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
{}

ComputePipelineState::~ComputePipelineState()
{
  // Free the unique compact compute pipeline state ID
  static_cast<RHIDynamicRHI&>(getRhi()).ComputePipelineStateMakeId.destroyID(getId());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
