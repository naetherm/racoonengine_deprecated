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
#include "RERHIOpenGLES3/State/RHIBlendState.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

BlendState::BlendState(const RERHI::BlendState& blendState) :
  mBlendState(blendState),
  mOpenGLES3SrcBlend(Mapping::getOpenGLES3BlendType(mBlendState.renderTarget[0].srcBlend)),
  mOpenGLES3DstBlend(Mapping::getOpenGLES3BlendType(mBlendState.renderTarget[0].destBlend))
{}

void BlendState::setOpenGLES3BlendStates() const
{
  if (mBlendState.alphaToCoverageEnable)
  {
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
  }
  else
  {
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
  }

  if (mBlendState.renderTarget[0].blendEnable)
  {
    glEnable(GL_BLEND);
    glBlendFunc(mOpenGLES3SrcBlend, mOpenGLES3DstBlend);
  }
  else
  {
    glDisable(GL_BLEND);
  }

  // TODO(naetherm) Map the rest of the blend states
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
