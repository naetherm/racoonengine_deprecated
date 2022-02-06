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
#include "RERHIOpenGLES3/State/RHIGraphicsPipelineState.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {

GraphicsPipelineState::GraphicsPipelineState(RHIDynamicRHI &openGLES3Rhi,
                                             const RERHI::GraphicsPipelineState &graphicsPipelineState, uint16_t id
                                             RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIGraphicsPipelineState(openGLES3Rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3PrimitiveTopology(Mapping::getOpenGLES3Type(graphicsPipelineState.primitiveTopology)),
  mGraphicsProgram(graphicsPipelineState.graphicsProgram),
  mRenderPass(graphicsPipelineState.renderPass),
  mRasterizerState(graphicsPipelineState.rasterizerState),
  mDepthStencilState(graphicsPipelineState.depthStencilState),
  mBlendState(graphicsPipelineState.blendState) {
// Ensure a correct reference counter behaviour
  graphicsPipelineState.rootSignature->AddReference();
  graphicsPipelineState.rootSignature->Release();

// Add a reference to the referenced RHI resources
  mGraphicsProgram->AddReference();
  mRenderPass->AddReference();
}

GraphicsPipelineState::~GraphicsPipelineState() {
// Release referenced RHI resources
  mGraphicsProgram->Release();
  mRenderPass->Release();

// Free the unique compact graphics pipeline state ID
  static_cast<RHIDynamicRHI &>(getRhi()).GraphicsPipelineStateMakeId.destroyID(getId());
}

void GraphicsPipelineState::bindGraphicsPipelineState() const {
  static_cast<RHIDynamicRHI &>(getRhi()).setGraphicsProgram(mGraphicsProgram);

  // Set the OpenGL ES 3 rasterizer state
  mRasterizerState.setOpenGLES3RasterizerStates();

  // Set OpenGL ES 3 depth stencil state
  mDepthStencilState.setOpenGLES3DepthStencilStates();

  // Set OpenGL ES 3 blend state
  mBlendState.setOpenGLES3BlendStates();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
