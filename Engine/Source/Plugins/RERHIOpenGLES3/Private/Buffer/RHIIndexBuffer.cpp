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
#include "RERHIOpenGLES3/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGLES3/RHIDynamicRHI.h"
#include "RERHIOpenGLES3/Extensions.h"
#include "RERHIOpenGLES3/Mapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


IndexBuffer::IndexBuffer(RHIDynamicRHI &openGLES3Rhi, RECore::uint32 numberOfBytes, const void *data,
                         RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat
                         RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
  RERHI::RHIIndexBuffer(openGLES3Rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mOpenGLES3ElementArrayBuffer(0),
  mOpenGLES3Type(GL_UNSIGNED_SHORT),
  mIndexSizeInBytes(RERHI::IndexBufferFormat::getNumberOfBytesPerElement(indexBufferFormat)),
  mBufferSize(numberOfBytes) {
  // "GL_UNSIGNED_INT" is only allowed when the "GL_OES_element_index_uint" extension is there
  if (RERHI::IndexBufferFormat::UNSIGNED_INT != indexBufferFormat ||
      openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_OES_element_index_uint()) {
    // Create the OpenGL ES 3 element array buffer
    glGenBuffers(1, &mOpenGLES3ElementArrayBuffer);

    // Set the OpenGL ES 3 index buffer data type
    mOpenGLES3Type = Mapping::getOpenGLES3Type(indexBufferFormat);

#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Backup the currently bound OpenGL ES 3 element array buffer
          GLint openGLES3ElementArrayBufferBackup = 0;
          glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &openGLES3ElementArrayBufferBackup);
#endif

    // Bind this OpenGL ES 3 element array buffer and upload the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOpenGLES3ElementArrayBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(numberOfBytes), data,
                 Mapping::getOpenGLES3Type(bufferUsage));

#ifdef RHI_OPENGLES3_STATE_CLEANUP
    // Be polite and restore the previous bound OpenGL ES 3 element array buffer
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(openGLES3ElementArrayBufferBackup));
#endif

    // Assign a default name to the resource for debugging purposes
#ifdef DEBUG
    if (openGLES3Rhi.getOpenGLES3Context().getExtensions().isGL_KHR_debug())
          {
            RHI_DECORATED_DEBUG_NAME(debugName, detailedDebugName, "IBO", 6)	// 6 = "IBO: " including terminating zero
            glObjectLabelKHR(GL_BUFFER_KHR, mOpenGLES3ElementArrayBuffer, -1, detailedDebugName);
          }
#endif
  } else {
    // Error!
    RHI_ASSERT(false,
               "\"GL_UNSIGNED_INT\" is only allowed in case the \"GL_OES_element_index_uint\" extension is there")
  }
}

/**
*  @brief
*    Destructor
*/
IndexBuffer::~IndexBuffer() {
// Destroy the OpenGL ES 3 element array buffer
// -> Silently ignores 0's and names that do not correspond to existing buffer objects
  glDeleteBuffers(1, &mOpenGLES3ElementArrayBuffer);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
