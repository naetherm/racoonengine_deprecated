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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resource type
*/
enum class ResourceType
{
  ROOT_SIGNATURE				   = 0,		///< Root signature
  RESOURCE_GROUP				   = 1,		///< Resource group
  GRAPHICS_PROGRAM			   = 2,		///< Graphics program, "Rhi::IShader"-related
  VERTEX_ARRAY				   = 3,		///< Vertex array object (VAO, input-assembler (IA) stage), "Rhi::IBuffer"-related
  RENDER_PASS					   = 4,		///< Render pass
  QUERY_POOL					   = 5,		///< Asynchronous query pool
  // IRenderTarget
  SWAP_CHAIN					   = 6,		///< Swap chain
  FRAMEBUFFER					   = 7,		///< Framebuffer object (FBO)
  // IBuffer
  VERTEX_BUFFER				   = 8,		///< Vertex buffer object (VBO, input-assembler (IA) stage)
  INDEX_BUFFER				   = 9,		///< Index buffer object (IBO, input-assembler (IA) stage)
  TEXTURE_BUFFER				   = 10,	///< Texture buffer object (TBO)
  STRUCTURED_BUFFER			   = 11,	///< Structured buffer object (SBO)
  INDIRECT_BUFFER				   = 12,	///< Indirect buffer object
  UNIFORM_BUFFER				   = 13,	///< Uniform buffer object (UBO, "constant buffer" in Direct3D terminology)
  // ITexture
  TEXTURE_1D					   = 14,	///< Texture 1D
  TEXTURE_1D_ARRAY			   = 15,	///< Texture 1D array
  TEXTURE_2D					   = 16,	///< Texture 2D
  TEXTURE_2D_ARRAY			   = 17,	///< Texture 2D array
  TEXTURE_3D					   = 18,	///< Texture 3D
  TEXTURE_CUBE				   = 19,	///< Texture cube
  TEXTURE_CUBE_ARRAY			   = 20,	///< Texture cube array
  // IState
  // IPipelineState
  GRAPHICS_PIPELINE_STATE	   = 21,	///< Graphics pipeline state (PSO)
  COMPUTE_PIPELINE_STATE	   = 22,	///< Compute pipeline state (PSO)
  SAMPLER_STATE				   = 23,	///< Sampler state
  // IShader
  VERTEX_SHADER				   = 24,	///< Vertex shader (VS)
  TESSELLATION_CONTROL_SHADER	   = 25,	///< Tessellation control shader (TCS, "hull shader" in Direct3D terminology)
  TESSELLATION_EVALUATION_SHADER = 26,	///< Tessellation evaluation shader (TES, "domain shader" in Direct3D terminology)
  GEOMETRY_SHADER				   = 27,	///< Geometry shader (GS)
  FRAGMENT_SHADER				   = 28,	///< Fragment shader (FS, "pixel shader" in Direct3D terminology)
  TASK_SHADER					   = 29,	///< Task shader (TS, "amplification shader" in Direct3D terminology)
  MESH_SHADER					   = 30,	///< Mesh shader (MS)
  COMPUTE_SHADER				   = 31		///< Compute shader (CS)
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
