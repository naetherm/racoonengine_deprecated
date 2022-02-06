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
#include "RERHIOpenGL/RERHIOpenGL.h"
#include "RERHIOpenGL/Query/RHIQueryPool.h"


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
/**
*  @brief
*    OpenGL asynchronous pipeline statistics query pool interface
*/
class PipelineStatisticsQueryPool final : public QueryPool
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLRhi
  *    Owner OpenGL RHI instance
  *  @param[in] queryType
  *    Query type
  *  @param[in] numberOfQueries
  *    Number of queries
  */
  PipelineStatisticsQueryPool(RHIDynamicRHI& openGLRhi, RERHI::QueryType queryType, uint32_t numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~PipelineStatisticsQueryPool() override;

  void beginQuery(uint32_t queryIndex) const;

  void endQuery() const;

  bool getQueryPoolResults(uint8_t* data, uint32_t firstQueryIndex, uint32_t numberOfQueries, uint32_t strideInBytes, bool waitForResult) const;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), PipelineStatisticsQueryPool, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit PipelineStatisticsQueryPool(const PipelineStatisticsQueryPool& source) = delete;
  PipelineStatisticsQueryPool& operator =(const PipelineStatisticsQueryPool& source) = delete;

  bool getQueryPoolResult(GLuint openGLQuery, bool waitForResult, uint64_t& queryResult) const;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  GLuint* mVerticesSubmittedOpenGLQueries;				///< "GL_VERTICES_SUBMITTED_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfInputAssemblerVertices"
  GLuint* mPrimitivesSubmittedOpenGLQueries;				///< "GL_PRIMITIVES_SUBMITTED_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfInputAssemblerPrimitives"
  GLuint* mVertexShaderInvocationsOpenGLQueries;			///< "GL_VERTEX_SHADER_INVOCATIONS_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfVertexShaderInvocations"
  GLuint* mGeometryShaderInvocationsOpenGLQueries;		///< "GL_GEOMETRY_SHADER_INVOCATIONS", "RERHI::PipelineStatisticsQueryResult::numberOfGeometryShaderInvocations"
  GLuint* mGeometryShaderPrimitivesEmittedOpenGLQueries;	///< "GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfGeometryShaderOutputPrimitives"
  GLuint* mClippingInputPrimitivesOpenGLQueries;			///< "GL_CLIPPING_INPUT_PRIMITIVES_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfClippingInputPrimitives"
  GLuint* mClippingOutputPrimitivesOpenGLQueries;			///< "GL_CLIPPING_OUTPUT_PRIMITIVES_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfClippingOutputPrimitives"
  GLuint* mFragmentShaderInvocationsOpenGLQueries;		///< "GL_FRAGMENT_SHADER_INVOCATIONS_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfFragmentShaderInvocations"
  GLuint* mTessControlShaderPatchesOpenGLQueries;			///< "GL_TESS_CONTROL_SHADER_PATCHES_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfTessellationControlShaderInvocations"
  GLuint* mTesEvaluationShaderInvocationsOpenGLQueries;	///< "GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfTessellationEvaluationShaderInvocations"
  GLuint* mComputeShaderInvocationsOpenGLQueries;			///< "GL_COMPUTE_SHADER_INVOCATIONS_ARB", "RERHI::PipelineStatisticsQueryResult::numberOfComputeShaderInvocations"


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
