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
	*    Draw instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawInstanced()"
	*    - Vulkan: "VkDrawIndirectCommand"
	*    - Metal: "MTLDrawPrimitivesIndirectArguments"
	*    - OpenGL:"DrawArraysIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_ARGUMENTS"-documentation for details
	*/
	struct DrawArguments final
	{
		RECore::uint32 vertexCountPerInstance;
		RECore::uint32 instanceCount;
		RECore::uint32 startVertexLocation;
		RECore::uint32 startInstanceLocation;
		inline DrawArguments(RECore::uint32 _vertexCountPerInstance, RECore::uint32 _instanceCount = 1, RECore::uint32 _startVertexLocation = 0, RECore::uint32 _startInstanceLocation = 0) :
			vertexCountPerInstance(_vertexCountPerInstance),
			instanceCount(_instanceCount),
			startVertexLocation(_startVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{}
	};

	/**
	*  @brief
	*    Draw indexed instanced arguments
	*
	*  @note
	*    - This structure directly maps to Direct3D 12, DirectX 11, Vulkan, Metal and OpenGL, do not change it
	*    - Direct3D 12: "D3D12_DRAW_INDEXED_ARGUMENTS"
	*    - Direct3D 11: No structure documentation found, only indications that same arguments and order as "ID3D11DeviceContext::DrawIndexedInstanced()"
	*    - Vulkan: "VkDrawIndexedIndirectCommand"
	*    - Metal: "MTLDrawIndexedPrimitivesIndirectArguments"
	*    - OpenGL:"DrawElementsIndirectCommand"
	*
	*  @see
	*    - "D3D12_DRAW_INDEXED_ARGUMENTS"-documentation for details
	*/
	struct DrawIndexedArguments final
	{
		RECore::uint32 indexCountPerInstance;
		RECore::uint32 instanceCount;
		RECore::uint32 startIndexLocation;
		RECore::int32  baseVertexLocation;
		RECore::uint32 startInstanceLocation;
		inline DrawIndexedArguments(RECore::uint32 _indexCountPerInstance, RECore::uint32 _instanceCount = 1, RECore::uint32 _startIndexLocation = 0, RECore::int32 _baseVertexLocation = 0, RECore::uint32 _startInstanceLocation = 0) :
			indexCountPerInstance(_indexCountPerInstance),
			instanceCount(_instanceCount),
			startIndexLocation(_startIndexLocation),
			baseVertexLocation(_baseVertexLocation),
			startInstanceLocation(_startInstanceLocation)
		{}
	};

	/**
	*  @brief
	*    Draw mesh tasks arguments
	*
	*  @note
	*    - This structure directly maps to Vulkan and OpenGL, do not change it
	*    - Vulkan: "VkDrawMeshTasksIndirectCommandNV"
	*    - OpenGL:"DrawMeshTasksIndirectCommandNV"
	*    - Direct3D 12 isn't compatible since it's using "D3D12_DISPATCH_ARGUMENTS" ("ThreadGroupCountX", "ThreadGroupCountY", "ThreadGroupCountZ")
	*
	*  @see
	*    - "VkDrawMeshTasksIndirectCommandNV"-documentation for details
	*/
	struct DrawMeshTasksArguments final
	{
		RECore::uint32 numberOfTasks;
		RECore::uint32 firstTask;
		inline DrawMeshTasksArguments(RECore::uint32 _numberOfTasks, RECore::uint32 _firstTask = 0) :
			numberOfTasks(_numberOfTasks),
			firstTask(_firstTask)
		{}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
