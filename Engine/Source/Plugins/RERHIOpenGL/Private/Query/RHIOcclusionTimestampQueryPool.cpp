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
#include "RERHIOpenGL/Query/RHIOcclusionTimestampQueryPool.h"
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
OcclusionTimestampQueryPool::OcclusionTimestampQueryPool(RHIDynamicRHI &openGLRhi, RERHI::QueryType queryType,
                                                         RECore::uint32 numberOfQueries RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  QueryPool(openGLRhi, queryType, numberOfQueries RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLQueries(RHI_MALLOC_TYPED(openGLRhi.getContext(), GLuint, numberOfQueries)) {
// If possible, use "glCreateQueries()" (OpenGL 4.5) in order to create the query instance at once
  if (nullptr != glCreateQueries) {
    switch (queryType) {
      case RERHI::QueryType::OCCLUSION:
        glCreateQueries(GL_SAMPLES_PASSED_ARB, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
        break;

      case RERHI::QueryType::PIPELINE_STATISTICS:
        RHI_ASSERT(false, "Use \"RHIDynamicRHI::PipelineStatisticsQueryPool\"")
        break;

      case RERHI::QueryType::TIMESTAMP:
        glCreateQueries(GL_TIMESTAMP, static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
        break;
    }
  } else {
    glGenQueriesARB(static_cast<GLsizei>(numberOfQueries), mOpenGLQueries);
  }

// Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLRhi.getExtensions().isGL_KHR_debug())
          {
            switch (queryType)
            {
              case RERHI::QueryType::OCCLUSION:
              {
                RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Occlusion query", 18)	// 18 = "Occlusion query: " including terminating zero
                for (RECore::uint32 i = 0; i < mNumberOfQueries; ++i)
                {
                  glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
                }
                break;
              }

              case RERHI::QueryType::PIPELINE_STATISTICS:
                RHI_ASSERT(false, "Use \"RHIDynamicRHI::PipelineStatisticsQueryPool\"")
                break;

              case RERHI::QueryType::TIMESTAMP:
              {
                RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "Timestamp query", 18)	// 18 = "Timestamp query: " including terminating zero
                for (RECore::uint32 i = 0; i < mNumberOfQueries; ++i)
                {
                  glObjectLabel(GL_QUERY, mOpenGLQueries[i], -1, detailedDebugName);
                }
                break;
              }
            }
          }
#endif
}

OcclusionTimestampQueryPool::~OcclusionTimestampQueryPool() {
  // Destroy the OpenGL queries
  // -> Silently ignores 0's and names that do not correspond to existing queries
  glDeleteQueriesARB(static_cast<GLsizei>(mNumberOfQueries), mOpenGLQueries);
  RHI_FREE(getRhi().getContext(), mOpenGLQueries);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
