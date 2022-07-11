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
*    Asynchronous query type
*/
enum class QueryType
{
  OCCLUSION			= 0,	///< Occlusion query, result is a "RECore::uint64" containing the number of passed samples
  PIPELINE_STATISTICS = 1,	///< Pipeline statistics query, result is a "Rhi::PipelineStatisticsQueryResult"
  TIMESTAMP			= 2		///< Timestamp query, result is a "RECore::uint64" containing a timestamp in nanosecond
};

/**
*  @brief
*    Asynchronous query control flags
*/
struct QueryControlFlags final
{
  enum Enum
  {
    PRECISE = 1 << 0	///< Query control precise
  };
};

/**
*  @brief
*    Asynchronous query result flags
*/
struct QueryResultFlags final
{
  enum Enum
  {
    WAIT = 1 << 0	///< Wait for the result
  };
};

/**
*  @brief
*    Asynchronous pipeline statistics query result
*
*  @note
*    - This pipeline statistics structure maps directly to Direct3D 11 and Direct3D 12, do not change it
*
*  @see
*    - "D3D11_QUERY_DATA_PIPELINE_STATISTICS"-documentation for details
*/
struct PipelineStatisticsQueryResult final
{
  RECore::uint64 numberOfInputAssemblerVertices;					///< Number of vertices read by input assembler
  RECore::uint64 numberOfInputAssemblerPrimitives;					///< Number of primitives read by the input assembler
  RECore::uint64 numberOfVertexShaderInvocations;					///< Number of times a vertex shader was invoked
  RECore::uint64 numberOfGeometryShaderInvocations;					///< Number of times a geometry shader was invoked
  RECore::uint64 numberOfGeometryShaderOutputPrimitives;			///< Number of primitives output by a geometry shader
  RECore::uint64 numberOfClippingInputPrimitives;					///< Number of primitives that were sent to the rasterizer
  RECore::uint64 numberOfClippingOutputPrimitives;					///< Number of primitives that were rendered
  RECore::uint64 numberOfFragmentShaderInvocations;					///< Number of times a fragment shader (FS, "pixel shader" in Direct3D terminology) was invoked
  RECore::uint64 numberOfTessellationControlShaderInvocations;		///< Number of times a hull shader (TCS, "hull shader" in Direct3D terminology) was invoked
  RECore::uint64 numberOfTessellationEvaluationShaderInvocations;	///< Number of times a domain shader (TES, "domain shader" in Direct3D terminology) was invoked
  RECore::uint64 numberOfComputeShaderInvocations;					///< Number of times a compute shader was invoked
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
