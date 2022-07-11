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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHIOpenGLES3/Buffer/RHIVertexArray.h"
#include "RERHIOpenGLES3/Buffer/RHIVertexBuffer.h"
#include "RERHIOpenGLES3/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


VertexArray::VertexArray(RHIDynamicRHI& openGLES3Rhi, const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RHIVertexArray(openGLES3Rhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3VertexArray(0),
  mNumberOfVertexBuffers(numberOfVertexBuffers),
  mVertexBuffers((mNumberOfVertexBuffers > 0) ? RHI_MALLOC_TYPED(openGLES3Rhi.getContext(), VertexBuffer*, mNumberOfVertexBuffers) : nullptr),	// Guaranteed to be filled below, so we don't need to care to initialize the content in here
  mIndexBuffer(indexBuffer)
{
  // Create the OpenGL ES 3 vertex array
  glGenVertexArrays(1, &mOpenGLES3VertexArray);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Backup the currently bound OpenGL ES 3 array buffer
				GLint openGLES3ArrayBufferBackup = 0;
				glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &openGLES3ArrayBufferBackup);

				// Backup the currently bound OpenGL ES 3 element array buffer
				GLint openGLES3ElementArrayBufferBackup = 0;
				glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &openGLES3ElementArrayBufferBackup);

				// Backup the currently bound OpenGL ES 3 vertex array
				GLint openGLES3VertexArrayBackup = 0;
				glGetIntegerv(GL_VERTEX_ARRAY_BINDING_OES, &openGLES3VertexArrayBackup);
#endif

  // Bind this OpenGL ES 3 vertex array
  glBindVertexArray(mOpenGLES3VertexArray);

  { // Add a reference to the used vertex buffers
    VertexBuffer** currentVertexBuffers = mVertexBuffers;
    const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = vertexBuffers + mNumberOfVertexBuffers;
    for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = vertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer, ++currentVertexBuffers)
    {
      // Add a reference to the used vertex buffer
      // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
      *currentVertexBuffers = static_cast<VertexBuffer*>(vertexBuffer->vertexBuffer);
      (*currentVertexBuffers)->AddReference();
    }
  }

  { // Enable OpenGL ES 3 vertex attribute arrays
    // Loop through all attributes
    // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
    GLuint attributeLocation = 0;
    const RERHI::VertexAttribute* attributeEnd = vertexAttributes.attributes + vertexAttributes.numberOfAttributes;
    for (const RERHI::VertexAttribute* attribute = vertexAttributes.attributes; attribute < attributeEnd; ++attribute, ++attributeLocation)
    {
      // Set the OpenGL ES 3 vertex attribute pointer
      const RERHI::VertexArrayVertexBuffer& vertexArrayVertexBuffer = vertexBuffers[attribute->inputSlot];
      glBindBuffer(GL_ARRAY_BUFFER, static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLES3ArrayBuffer());
      if (Mapping::isOpenGLES3VertexAttributeFormatInteger(attribute->vertexAttributeFormat))
      {
        glVertexAttribIPointer(attributeLocation,
                               Mapping::getOpenGLES3Size(attribute->vertexAttributeFormat),
                               Mapping::getOpenGLES3Type(attribute->vertexAttributeFormat),
                               static_cast<GLsizei>(attribute->strideInBytes),
                               reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
      }
      else
      {
        glVertexAttribPointer(attributeLocation,
                              Mapping::getOpenGLES3Size(attribute->vertexAttributeFormat),
                              Mapping::getOpenGLES3Type(attribute->vertexAttributeFormat),
                              Mapping::isOpenGLES3VertexAttributeFormatNormalized(attribute->vertexAttributeFormat),
                              static_cast<GLsizei>(attribute->strideInBytes),
                              reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
      }

      // Set divisor
      if (attribute->instancesPerElement > 0)
      {
        glVertexAttribDivisor(attributeLocation, attribute->instancesPerElement);
      }

      // Enable OpenGL ES 3 vertex attribute array
      glEnableVertexAttribArray(attributeLocation);
    }

    // Set the used index buffer
    // -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
    if (nullptr != indexBuffer)
    {
      // Bind OpenGL ES 3 element array buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->getOpenGLES3ElementArrayBuffer());
    }
  }

#ifdef RHI_OPENGLES3_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL ES 3 vertex array
				glBindVertexArray(static_cast<GLuint>(openGLES3VertexArrayBackup));

				// Be polite and restore the previous bound OpenGL ES 3 element array buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(openGLES3ElementArrayBufferBackup));

				// Be polite and restore the previous bound OpenGL ES 3 array buffer
				glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(openGLES3ArrayBufferBackup));
#endif

  // Add a reference to the given index buffer
  if (nullptr != mIndexBuffer)
  {
    mIndexBuffer->AddReference();
  }

  // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
  if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
				{
					RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "VAO", 6)	// 6 = "VAO: " including terminating zero
					glObjectLabelKHR(GL_VERTEX_ARRAY_KHR, mOpenGLES3VertexArray, -1, detailedDebugName);
				}
#endif
}

/**
*  @brief
*    Destructor
*/
VertexArray::~VertexArray()
{
// Destroy the OpenGL ES 3 vertex array
// -> Silently ignores 0's and names that do not correspond to existing vertex array objects
  glDeleteVertexArrays(1, &mOpenGLES3VertexArray);

// Release the reference to the used vertex buffers
  RHIDynamicRHI& openGLES3Rhi = static_cast<RHIDynamicRHI&>(getRhi());
  if (nullptr != mVertexBuffers)
  {
// Release references
    VertexBuffer** vertexBuffersEnd = mVertexBuffers + mNumberOfVertexBuffers;
    for (VertexBuffer** vertexBuffer = mVertexBuffers; vertexBuffer < vertexBuffersEnd; ++vertexBuffer)
    {
      (*vertexBuffer)->Release();
    }

// Cleanup
    RHI_FREE(openGLES3Rhi.getContext(), mVertexBuffers);
  }

// Release the index buffer reference
  if (nullptr != mIndexBuffer)
  {
    mIndexBuffer->Release();
  }

// Free the unique compact vertex array ID
  openGLES3Rhi.VertexArrayMakeId.destroyID(getId());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
