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
#include "RERHIOpenGL/Query/PipelineStatisticsQueryPool.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"


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
PipelineStatisticsQueryPool::PipelineStatisticsQueryPool(RHIDynamicRHI &openGLRhi, RERHI::QueryType queryType,
                                                         RECore::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  QueryPool(openGLRhi, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mVerticesSubmittedOpenGLQueries(RHI_MALLOC_TYPED(openGLRhi.getContext(), GLuint, numberOfQueries * 11)),
  mPrimitivesSubmittedOpenGLQueries(mVerticesSubmittedOpenGLQueries + numberOfQueries),
  mVertexShaderInvocationsOpenGLQueries(mPrimitivesSubmittedOpenGLQueries + numberOfQueries),
  mGeometryShaderInvocationsOpenGLQueries(mVertexShaderInvocationsOpenGLQueries + numberOfQueries),
  mGeometryShaderPrimitivesEmittedOpenGLQueries(mGeometryShaderInvocationsOpenGLQueries + numberOfQueries),
  mClippingInputPrimitivesOpenGLQueries(mGeometryShaderPrimitivesEmittedOpenGLQueries + numberOfQueries),
  mClippingOutputPrimitivesOpenGLQueries(mClippingInputPrimitivesOpenGLQueries + numberOfQueries),
  mFragmentShaderInvocationsOpenGLQueries(mClippingOutputPrimitivesOpenGLQueries + numberOfQueries),
  mTessControlShaderPatchesOpenGLQueries(mFragmentShaderInvocationsOpenGLQueries + numberOfQueries),
  mTesEvaluationShaderInvocationsOpenGLQueries(mTessControlShaderPatchesOpenGLQueries + numberOfQueries),
  mComputeShaderInvocationsOpenGLQueries(mTesEvaluationShaderInvocationsOpenGLQueries + numberOfQueries) {
// "glCreateQueries()" (OpenGL 4.5) doesn't support "GL_ARB_pipeline_statistics_query"
  glGenQueriesARB(static_cast<GLsizei>(numberOfQueries) * 11, mVerticesSubmittedOpenGLQueries);

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            switch (queryType)
            {
              case RERHI::QueryType::OCCLUSION:
              case RERHI::QueryType::TIMESTAMP:
                RHI_ASSERT(false, "Use \"RHIDynamicRHI::OcclusionTimestampQueryPool\"")
                break;

              case RERHI::QueryType::PIPELINE_STATISTICS:
              {
                // Enforce instant query creation so we can set a debug name
                for (RECore::uint32 i = 0; i < numberOfQueries; ++i)
                {
                  beginQuery(i);
                  endQuery();
                }

                // Set debug name
                RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Pipeline statistics query", 28)	// 28 = "Pipeline statistics query: " including terminating zero
                for (RECore::uint32 i = 0; i < mNumberOfQueries * 11; ++i)
                {
                  glObjectLabel(GL_QUERY, mVerticesSubmittedOpenGLQueries[i], -1, detailedDebugName);
                }
                break;
              }
            }
          }
#endif
}

PipelineStatisticsQueryPool::~PipelineStatisticsQueryPool() {
  // Destroy the OpenGL queries
  // -> Silently ignores 0's and names that do not correspond to existing queries
  glDeleteQueriesARB(static_cast<GLsizei>(mNumberOfQueries) * 11, mVerticesSubmittedOpenGLQueries);
  RHI_FREE(getRhi().getContext(), mVerticesSubmittedOpenGLQueries);
}

void PipelineStatisticsQueryPool::beginQuery(RECore::uint32 queryIndex) const {
  glBeginQueryARB(GL_VERTICES_SUBMITTED_ARB, mVerticesSubmittedOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_PRIMITIVES_SUBMITTED_ARB, mPrimitivesSubmittedOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_VERTEX_SHADER_INVOCATIONS_ARB, mVertexShaderInvocationsOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_GEOMETRY_SHADER_INVOCATIONS, mGeometryShaderInvocationsOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB, mGeometryShaderPrimitivesEmittedOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_CLIPPING_INPUT_PRIMITIVES_ARB, mClippingInputPrimitivesOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB, mClippingOutputPrimitivesOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_FRAGMENT_SHADER_INVOCATIONS_ARB, mFragmentShaderInvocationsOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_TESS_CONTROL_SHADER_PATCHES_ARB, mTessControlShaderPatchesOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB, mTesEvaluationShaderInvocationsOpenGLQueries[queryIndex]);
  glBeginQueryARB(GL_COMPUTE_SHADER_INVOCATIONS_ARB, mComputeShaderInvocationsOpenGLQueries[queryIndex]);
}

void PipelineStatisticsQueryPool::endQuery() const {
  glEndQueryARB(GL_VERTICES_SUBMITTED_ARB);
  glEndQueryARB(GL_PRIMITIVES_SUBMITTED_ARB);
  glEndQueryARB(GL_VERTEX_SHADER_INVOCATIONS_ARB);
  glEndQueryARB(GL_GEOMETRY_SHADER_INVOCATIONS);
  glEndQueryARB(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB);
  glEndQueryARB(GL_CLIPPING_INPUT_PRIMITIVES_ARB);
  glEndQueryARB(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB);
  glEndQueryARB(GL_FRAGMENT_SHADER_INVOCATIONS_ARB);
  glEndQueryARB(GL_TESS_CONTROL_SHADER_PATCHES_ARB);
  glEndQueryARB(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB);
  glEndQueryARB(GL_COMPUTE_SHADER_INVOCATIONS_ARB);
}

bool PipelineStatisticsQueryPool::getQueryPoolResults(RECore::uint8 *data, RECore::uint32 firstQueryIndex, RECore::uint32 numberOfQueries,
                                                      RECore::uint32 strideInBytes, bool waitForResult) const {
  bool resultAvailable = true;

  // Define a helper macro
#define GET_QUERY_RESULT(openGLQueries, queryResult) \
        resultAvailable = getQueryPoolResult(openGLQueries[firstQueryIndex + i], waitForResult, currentPipelineStatisticsQueryResult->queryResult); \
        if (!resultAvailable) \
        { \
          break; \
        }

  // Get query pool results
  RERHI::PipelineStatisticsQueryResult *currentPipelineStatisticsQueryResult = reinterpret_cast<RERHI::PipelineStatisticsQueryResult *>(data);
  for (RECore::uint32 i = 0; i < numberOfQueries; ++i) {
    GET_QUERY_RESULT(mVerticesSubmittedOpenGLQueries, numberOfInputAssemblerVertices)
    GET_QUERY_RESULT(mPrimitivesSubmittedOpenGLQueries, numberOfInputAssemblerPrimitives)
    GET_QUERY_RESULT(mVertexShaderInvocationsOpenGLQueries, numberOfVertexShaderInvocations)
    GET_QUERY_RESULT(mGeometryShaderInvocationsOpenGLQueries, numberOfGeometryShaderInvocations)
    GET_QUERY_RESULT(mGeometryShaderPrimitivesEmittedOpenGLQueries, numberOfGeometryShaderOutputPrimitives)
    GET_QUERY_RESULT(mClippingInputPrimitivesOpenGLQueries, numberOfClippingInputPrimitives)
    GET_QUERY_RESULT(mClippingOutputPrimitivesOpenGLQueries, numberOfClippingOutputPrimitives)
    GET_QUERY_RESULT(mFragmentShaderInvocationsOpenGLQueries, numberOfFragmentShaderInvocations)
    GET_QUERY_RESULT(mTessControlShaderPatchesOpenGLQueries, numberOfTessellationControlShaderInvocations)
    GET_QUERY_RESULT(mTesEvaluationShaderInvocationsOpenGLQueries, numberOfTessellationEvaluationShaderInvocations)
    GET_QUERY_RESULT(mComputeShaderInvocationsOpenGLQueries, numberOfComputeShaderInvocations)
    currentPipelineStatisticsQueryResult += strideInBytes;
  }

  // Undefine helper macro
#undef GET_QUERY_RESULT

  // Done
  return resultAvailable;
}

bool
PipelineStatisticsQueryPool::getQueryPoolResult(GLuint openGLQuery, bool waitForResult, RECore::uint64 &queryResult) const {
  bool resultAvailable = true;
  GLuint openGLQueryResult = GL_FALSE;
  do {
    glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &openGLQueryResult);
  } while (waitForResult && GL_TRUE != openGLQueryResult);
  if (GL_TRUE == openGLQueryResult) {
    glGetQueryObjectuivARB(openGLQuery, GL_QUERY_RESULT_ARB, &openGLQueryResult);
    queryResult = static_cast<RECore::uint64>(openGLQueryResult);
  } else {
    // Result not ready
    resultAvailable = false;
  }

  // Done
  return resultAvailable;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
