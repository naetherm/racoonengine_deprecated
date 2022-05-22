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
#include "RERHIOpenGL/Buffer/RHIVertexArrayVao.h"
#include "RERHIOpenGL/Buffer/RHIVertexBuffer.h"
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
VertexArrayVao::~VertexArrayVao() {
  // Destroy the OpenGL vertex array
  // -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mOpenGLVertexArray);

  // Release the reference to the used vertex buffers
  if (nullptr != mVertexBuffers) {
    // Release references
    VertexBuffer **vertexBuffersEnd = mVertexBuffers + mNumberOfVertexBuffers;
    for (VertexBuffer **vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer) {
      (*vertexBuffer)->Release();
    }

    // Cleanup
    RHI_FREE(getRhi().getContext(), mVertexBuffers);
  }
}

VertexArrayVao::VertexArrayVao(RHIDynamicRHI &openGLRhi, RECore::uint32 numberOfVertexBuffers,
                               const RERHI::VertexArrayVertexBuffer *vertexBuffers, IndexBuffer *indexBuffer,
                               RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  VertexArray(openGLRhi, indexBuffer, InternalResourceType::VAO, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLVertexArray(0),
  mNumberOfVertexBuffers(numberOfVertexBuffers),
  mVertexBuffers(nullptr) {
  // Add a reference to the used vertex buffers
  if (numberOfVertexBuffers > 0) {
    mVertexBuffers = RHI_MALLOC_TYPED(openGLRhi.getContext(), VertexBuffer*, numberOfVertexBuffers);

    // Loop through all vertex buffers
    VertexBuffer **currentVertexBuffers = mVertexBuffers;
    const RERHI::VertexArrayVertexBuffer *vertexBuffersEnd = vertexBuffers + numberOfVertexBuffers;
    for (const RERHI::VertexArrayVertexBuffer *vertexBuffer = vertexBuffers;
         vertexBuffer < vertexBuffersEnd; ++vertexBuffer, ++currentVertexBuffers) {
      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *currentVertexBuffers = static_cast<VertexBuffer *>(vertexBuffer->vertexBuffer);
      (*currentVertexBuffers)->AddReference();
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
