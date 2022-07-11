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
#include "RERHIOpenGL/State/RHIRasterizerState.h"
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
RasterizerState::RasterizerState(const RERHI::RasterizerState& rasterizerState) :
  mRasterizerState(rasterizerState),
  mOpenGLFrontFaceMode(static_cast<GLenum>(mRasterizerState.frontCounterClockwise ? GL_CCW : GL_CW))
{}

void RasterizerState::setOpenGLRasterizerStates() const
{
  // RERHI::RasterizerState::fillMode
  switch (mRasterizerState.fillMode)
  {
    // Wireframe
    case RERHI::FillMode::WIREFRAME:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;

      // Solid
    default:
    case RERHI::FillMode::SOLID:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
  }

  // RERHI::RasterizerState::cullMode
  switch (mRasterizerState.cullMode)
  {
    // No culling
    default:
    case RERHI::CullMode::NONE:
      glDisable(GL_CULL_FACE);
      break;

      // Selects clockwise polygons as front-facing
    case RERHI::CullMode::FRONT:
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      break;

      // Selects counterclockwise polygons as front-facing
    case RERHI::CullMode::BACK:
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      break;
  }

  // RERHI::RasterizerState::frontCounterClockwise
  glFrontFace(mOpenGLFrontFaceMode);

  // TODO(naetherm) Map the rest of the rasterizer states

  // RasterizerState::depthBias

  // RasterizerState::depthBiasClamp

  // RasterizerState::slopeScaledDepthBias

  // RasterizerState::depthClipEnable
  if (mRasterizerState.depthClipEnable)
  {
    glDisable(GL_DEPTH_CLAMP);
  }
  else
  {
    glEnable(GL_DEPTH_CLAMP);
  }

  // RasterizerState::scissorEnable
  if (mRasterizerState.scissorEnable)
  {
    glEnable(GL_SCISSOR_TEST);
  }
  else
  {
    glDisable(GL_SCISSOR_TEST);
  }

  // RasterizerState::multisampleEnable

  // RasterizerState::antialiasedLineEnable
  if (mRasterizerState.antialiasedLineEnable)
  {
    glEnable(GL_LINE_SMOOTH);
  }
  else
  {
    glDisable(GL_LINE_SMOOTH);
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
