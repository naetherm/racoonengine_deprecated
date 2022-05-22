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
#include "RERHIOpenGL/State/RHIGraphicsPipelineState.h"
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
GraphicsPipelineState::GraphicsPipelineState(RHIDynamicRHI &openGLRhi,
                                             const RERHI::GraphicsPipelineState &graphicsPipelineState, RECore::uint16 id
                                             RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIGraphicsPipelineState(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLPrimitiveTopology(0xFFFF),  // Unknown default setting
  mNumberOfVerticesPerPatch(0),
  mGraphicsProgram(graphicsPipelineState.graphicsProgram),
  mRenderPass(graphicsPipelineState.renderPass),
  mRasterizerState(graphicsPipelineState.rasterizerState),
  mDepthStencilState(graphicsPipelineState.depthStencilState),
  mBlendState(graphicsPipelineState.blendState) {
// Tessellation support: Up to 32 vertices per patch are supported "RERHI::PrimitiveTopology::PATCH_LIST_1" ... "RERHI::PrimitiveTopology::PATCH_LIST_32"
  if (graphicsPipelineState.primitiveTopology >= RERHI::PrimitiveTopology::PATCH_LIST_1) {
// Use tessellation

// Get number of vertices that will be used to make up a single patch primitive
// -> There's no need to check for the "GL_ARB_tessellation_shader" extension, it's there if "RERHI::Capabilities::maximumNumberOfPatchVertices" is not 0
    const int numberOfVerticesPerPatch = static_cast<int>(graphicsPipelineState.primitiveTopology) -
                                         static_cast<int>(RERHI::PrimitiveTopology::PATCH_LIST_1) + 1;
    if (numberOfVerticesPerPatch <= static_cast<int>(openGLRhi.getCapabilities().maximumNumberOfPatchVertices)) {
// Set number of vertices that will be used to make up a single patch primitive
      mNumberOfVerticesPerPatch = numberOfVerticesPerPatch;

// Set OpenGL primitive topology
      mOpenGLPrimitiveTopology = GL_PATCHES;
    } else {
// Error!
      RHI_ASSERT(false, "Invalid number of OpenGL vertices per patch")
    }
  } else {
// Do not use tessellation

// Map and backup the set OpenGL primitive topology
    mOpenGLPrimitiveTopology = Mapping::getOpenGLType(graphicsPipelineState.primitiveTopology);
  }

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
  static_cast<RHIDynamicRHI &>(getRhi()).setOpenGLGraphicsProgram(mGraphicsProgram);

  // Set the OpenGL rasterizer state
  mRasterizerState.setOpenGLRasterizerStates();

  // Set OpenGL depth stencil state
  mDepthStencilState.setOpenGLDepthStencilStates();

  // Set OpenGL blend state
  mBlendState.setOpenGLBlendStates();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
