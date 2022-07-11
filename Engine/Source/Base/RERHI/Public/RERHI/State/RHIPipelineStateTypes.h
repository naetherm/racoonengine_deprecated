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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHI/RERHI.h"
#include "RERHI/Buffer/RHIVertexArrayTypes.h"
#include "RERHI/State/RHISamplerStateTypes.h"
#include "RERHI/State/RHIRasterizerStateTypes.h"
#include "RERHI/State/RHIDepthStencilStateTypes.h"
#include "RERHI/State/RHIBlendStateTypes.h"
#include "RERHI/Texture/RHITextureTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI
{


class RHIRootSignature;
class RHIGraphicsProgram;
class RHIRenderPass;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input-assembler (IA) stage: Primitive topology types
*
*  @note
*    - These constants directly map to Direct3D 9 & 10 & 11 constants, do not change them
*    - For a visual overview see "Primitive Topologies" -> "The following illustration shows the vertex ordering for all of the primitive types that the input assembler can produce." at https://docs.microsoft.com/en-gb/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies?redirectedfrom=MSDN
*/
enum class PrimitiveTopology
{
  UNKNOWN            = 0,		///< Unknown primitive type
  POINT_LIST         = 1,		///< Point list, use "PATCH_LIST_1" for tessellation
  LINE_LIST          = 2,		///< Line list, use "PATCH_LIST_2" for tessellation
  LINE_STRIP         = 3,		///< Line strip
  TRIANGLE_LIST      = 4,		///< Triangle list, use "PATCH_LIST_3" for tessellation
  TRIANGLE_STRIP     = 5,		///< Triangle strip
  LINE_LIST_ADJ      = 10,	///< Line list with adjacency data, use "PATCH_LIST_2" for tessellation
  LINE_STRIP_ADJ     = 11,	///< Line strip with adjacency data
  TRIANGLE_LIST_ADJ  = 12,	///< Triangle list with adjacency data, use "PATCH_LIST_3" for tessellation
  TRIANGLE_STRIP_ADJ = 13,	///< Triangle strip with adjacency data
  PATCH_LIST_1       = 33,	///< Patch list with 1 vertex per patch (tessellation relevant topology type) - "POINT_LIST" used for tessellation
  PATCH_LIST_2       = 34,	///< Patch list with 2 vertices per patch (tessellation relevant topology type) - "LINE_LIST" used for tessellation
  PATCH_LIST_3       = 35,	///< Patch list with 3 vertices per patch (tessellation relevant topology type) - "TRIANGLE_LIST" used for tessellation
  PATCH_LIST_4       = 36,	///< Patch list with 4 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_5       = 37,	///< Patch list with 5 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_6       = 38,	///< Patch list with 6 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_7       = 39,	///< Patch list with 7 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_8       = 40,	///< Patch list with 8 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_9       = 41,	///< Patch list with 9 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_10      = 42,	///< Patch list with 10 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_11      = 43,	///< Patch list with 11 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_12      = 44,	///< Patch list with 12 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_13      = 45,	///< Patch list with 13 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_14      = 46,	///< Patch list with 14 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_15      = 47,	///< Patch list with 15 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_16      = 48,	///< Patch list with 16 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_17      = 49,	///< Patch list with 17 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_18      = 50,	///< Patch list with 18 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_19      = 51,	///< Patch list with 19 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_20      = 52,	///< Patch list with 20 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_21      = 53,	///< Patch list with 21 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_22      = 54,	///< Patch list with 22 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_23      = 55,	///< Patch list with 23 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_24      = 56,	///< Patch list with 24 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_25      = 57,	///< Patch list with 25 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_26      = 58,	///< Patch list with 26 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_27      = 59,	///< Patch list with 27 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_28      = 60,	///< Patch list with 28 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_29      = 61,	///< Patch list with 29 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_30      = 62,	///< Patch list with 30 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_31      = 63,	///< Patch list with 31 vertices per patch (tessellation relevant topology type)
  PATCH_LIST_32      = 64		///< Patch list with 32 vertices per patch (tessellation relevant topology type)
};

/**
*  @brief
*    Primitive topology type specifying how the graphics pipeline interprets geometry or hull shader input primitives
*
*  @note
*    - These constants directly map to Direct3D 12 constants, do not change them
*
*  @see
*    - "D3D12_PRIMITIVE_TOPOLOGY_TYPE"-documentation for details
*/
enum class PrimitiveTopologyType
{
  UNDEFINED	= 0,	///< The shader has not been initialized with an input primitive type
  POINT		= 1,	///< Interpret the input primitive as a point
  LINE		= 2,	///< Interpret the input primitive as a line
  TRIANGLE	= 3,	///< Interpret the input primitive as a triangle
  PATCH		= 4		///< Interpret the input primitive as a control point patch
};

/**
*  @brief
*    Graphics pipeline state
*/
struct SerializedGraphicsPipelineState
{
  PrimitiveTopology	  primitiveTopology;			///< Input-assembler (IA) stage: Primitive topology used for draw calls
  PrimitiveTopologyType primitiveTopologyType;		///< The primitive topology type specifies how the graphics pipeline interprets geometry or hull shader input primitives
  RasterizerState		  rasterizerState;				///< Rasterizer state
  DepthStencilState	  depthStencilState;			///< Depth stencil state
  BlendState			  blendState;					///< Blend state
  RECore::uint32			  numberOfRenderTargets;		///< Number of render targets
  TextureFormat::Enum	  renderTargetViewFormats[8];	///< Render target view formats
  TextureFormat::Enum	  depthStencilViewFormat;		///< Depth stencil view formats
};
struct GraphicsPipelineState : public SerializedGraphicsPipelineState
{
  RHIRootSignature*   rootSignature;	///< Root signature (graphics pipeline state instances keep a reference to the root signature), must be valid
  RHIGraphicsProgram* graphicsProgram;	///< Graphics program used by the graphics pipeline state (graphics pipeline state instances keep a reference to the graphics program), must be valid
  VertexAttributes  vertexAttributes;	///< Vertex attributes, can be empty e.g. in case a task and mesh shader is used
  RHIRenderPass*	  renderPass;		///< Render pass, the graphics pipeline state keeps a reference, must be valid
};
struct GraphicsPipelineStateBuilder final : public GraphicsPipelineState
{
  inline GraphicsPipelineStateBuilder()
  {
    // "GraphicsPipelineState"-part
    rootSignature						= nullptr;
    graphicsProgram						= nullptr;
    vertexAttributes.numberOfAttributes	= 0;
    vertexAttributes.attributes			= nullptr;
    renderPass							= nullptr;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology		   = PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType	   = PrimitiveTopologyType::TRIANGLE;
    rasterizerState			   = RasterizerStateBuilder::getDefaultRasterizerState();
    depthStencilState		   = DepthStencilStateBuilder::getDefaultDepthStencilState();
    blendState				   = BlendStateBuilder::getDefaultBlendState();
    numberOfRenderTargets	   = 1;
    renderTargetViewFormats[0] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6] = TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7] = TextureFormat::R8G8B8A8;
    depthStencilViewFormat	   = TextureFormat::D32_FLOAT;
  }

  inline GraphicsPipelineStateBuilder(RHIRootSignature* _rootSignature, RHIGraphicsProgram* _graphicsProgram, RHIRenderPass& _renderPass)
  {
    // "GraphicsPipelineState"-part which has no vertex attributes, e.g. because the graphics program is using a task and mesh shader
    rootSignature	= _rootSignature;
    graphicsProgram = _graphicsProgram;
    renderPass		= &_renderPass;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
    rasterizerState				= RasterizerStateBuilder::getDefaultRasterizerState();
    depthStencilState			= DepthStencilStateBuilder::getDefaultDepthStencilState();
    blendState					= BlendStateBuilder::getDefaultBlendState();
    numberOfRenderTargets		= 1;
    renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
    depthStencilViewFormat		= TextureFormat::D32_FLOAT;
  }

  inline GraphicsPipelineStateBuilder(RHIRootSignature* _rootSignature, RHIGraphicsProgram* _graphicsProgram, const VertexAttributes& _vertexAttributes, RHIRenderPass& _renderPass)
  {
    // "GraphicsPipelineState"-part
    rootSignature	 = _rootSignature;
    graphicsProgram	 = _graphicsProgram;
    vertexAttributes = _vertexAttributes;
    renderPass		 = &_renderPass;

    // "SerializedGraphicsPipelineState"-part
    primitiveTopology			= PrimitiveTopology::TRIANGLE_LIST;
    primitiveTopologyType		= PrimitiveTopologyType::TRIANGLE;
    rasterizerState				= RasterizerStateBuilder::getDefaultRasterizerState();
    depthStencilState			= DepthStencilStateBuilder::getDefaultDepthStencilState();
    blendState					= BlendStateBuilder::getDefaultBlendState();
    numberOfRenderTargets		= 1;
    renderTargetViewFormats[0]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[1]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[2]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[3]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[4]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[5]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[6]	= TextureFormat::R8G8B8A8;
    renderTargetViewFormats[7]	= TextureFormat::R8G8B8A8;
    depthStencilViewFormat		= TextureFormat::D32_FLOAT;
  }
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
