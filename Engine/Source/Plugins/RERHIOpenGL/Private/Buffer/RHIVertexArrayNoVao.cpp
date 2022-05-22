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
#include "RERHIOpenGL/Buffer/RHIVertexArrayNoVao.h"
#include "RERHIOpenGL/Buffer/RHIIndexBuffer.h"
#include "RERHIOpenGL/Buffer/RHIVertexBuffer.h"
#include "RERHIOpenGL/RHIDynamicRHI.h"
#include "RERHIOpenGL/Extensions.h"
#include "RERHIOpenGL/Mapping.h"


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
VertexArrayNoVao::VertexArrayNoVao(RHIDynamicRHI& openGLRhi, const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
VertexArray(openGLRhi, indexBuffer, InternalResourceType::NO_VAO, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
mNumberOfAttributes(vertexAttributes.numberOfAttributes),
mAttributes(mNumberOfAttributes ? RHI_MALLOC_TYPED(openGLRhi.getContext(), RERHI::VertexAttribute, mNumberOfAttributes) : nullptr),
mNumberOfVertexBuffers(numberOfVertexBuffers),
mVertexBuffers(numberOfVertexBuffers ? RHI_MALLOC_TYPED(openGLRhi.getContext(), RERHI::VertexArrayVertexBuffer, numberOfVertexBuffers) : nullptr),
mIsGL_ARB_instanced_arrays(openGLRhi.getExtensions().isGL_ARB_instanced_arrays())
{
  // Copy over the data
  if (nullptr != mAttributes)
  {
    memcpy(mAttributes, vertexAttributes.attributes, sizeof(RERHI::VertexAttribute) * mNumberOfAttributes);
  }
  if (nullptr != mVertexBuffers)
  {
    memcpy(mVertexBuffers, vertexBuffers, sizeof(RERHI::VertexArrayVertexBuffer) * mNumberOfVertexBuffers);
  }

  // Add a reference to the used vertex buffers
  const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
  for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
  {
    vertexBuffer->vertexBuffer->AddReference();
  }
}


VertexArrayNoVao::~VertexArrayNoVao()
{
  // Destroy the vertex array attributes
  const RERHI::RHIContext& context = getRhi().getContext();
  RHI_FREE(context, mAttributes);

  // Destroy the vertex array vertex buffers
  if (nullptr != mVertexBuffers)
  {
    // Release the reference to the used vertex buffers
    const RERHI::VertexArrayVertexBuffer* vertexBufferEnd = mVertexBuffers + mNumberOfVertexBuffers;
    for (const RERHI::VertexArrayVertexBuffer* vertexBuffer = mVertexBuffers; vertexBuffer < vertexBufferEnd; ++vertexBuffer)
    {
      vertexBuffer->vertexBuffer->Release();
    }

    // Cleanup
    RHI_FREE(context, mVertexBuffers);
  }
}

void VertexArrayNoVao::enableOpenGLVertexAttribArrays()
{
#ifdef RHI_OPENGL_STATE_CLEANUP
  // Backup the currently bound OpenGL array buffer
				// -> Using "GL_EXT_direct_state_access" this would not help in here because "glVertexAttribPointerARB" is not specified there :/
				GLint openGLArrayBufferBackup = 0;
				glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &openGLArrayBufferBackup);
#endif

  // Loop through all attributes
  // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
  GLuint attributeLocation = 0;
  const RERHI::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
  for (const RERHI::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation)
  {
    // Set the OpenGL vertex attribute pointer
    // TODO(naetherm) Add security check: Is the given resource one of the currently used RHI?
    const RERHI::VertexArrayVertexBuffer& vertexArrayVertexBuffer = mVertexBuffers[attribute->inputSlot];
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<VertexBuffer*>(vertexArrayVertexBuffer.vertexBuffer)->getOpenGLArrayBuffer());
    if (Mapping::isOpenGLVertexAttributeFormatInteger(attribute->vertexAttributeFormat))
    {
      glVertexAttribIPointer(attributeLocation,
                             Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
                             Mapping::getOpenGLType(attribute->vertexAttributeFormat),
                             static_cast<GLsizei>(attribute->strideInBytes),
                             reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
    }
    else
    {
      glVertexAttribPointerARB(attributeLocation,
                               Mapping::getOpenGLSize(attribute->vertexAttributeFormat),
                               Mapping::getOpenGLType(attribute->vertexAttributeFormat),
                               static_cast<GLboolean>(Mapping::isOpenGLVertexAttributeFormatNormalized(attribute->vertexAttributeFormat)),
                               static_cast<GLsizei>(attribute->strideInBytes),
                               reinterpret_cast<void*>(static_cast<uintptr_t>(attribute->alignedByteOffset)));
    }

    // Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
    if (attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays)
    {
      glVertexAttribDivisorARB(attributeLocation, attribute->instancesPerElement);
    }

    // Enable OpenGL vertex attribute array
    glEnableVertexAttribArrayARB(attributeLocation);
  }

#ifdef RHI_OPENGL_STATE_CLEANUP
  // Be polite and restore the previous bound OpenGL array buffer
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, static_cast<GLuint>(openGLArrayBufferBackup));
#endif

  // Set the used index buffer
  // -> In case of no index buffer we don't bind buffer 0, there's not really a point in it
  const IndexBuffer* indexBuffer = getIndexBuffer();
  if (nullptr != indexBuffer)
  {
    // Bind OpenGL element array buffer
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer->getOpenGLElementArrayBuffer());
  }
}

void VertexArrayNoVao::disableOpenGLVertexAttribArrays()
{
  // No previous bound OpenGL element array buffer restore, there's not really a point in it

  // Loop through all attributes
  // -> We're using "glBindAttribLocation()" when linking the program so we have known attribute locations (the vertex array can't know about the program)
  GLuint attributeLocation = 0;
  const RERHI::VertexAttribute* attributeEnd = mAttributes + mNumberOfAttributes;
  for (const RERHI::VertexAttribute* attribute = mAttributes; attribute < attributeEnd; ++attribute, ++attributeLocation)
  {
    // Disable OpenGL vertex attribute array
    glDisableVertexAttribArrayARB(attributeLocation);

    // Per-instance instead of per-vertex requires "GL_ARB_instanced_arrays"
    if (attribute->instancesPerElement > 0 && mIsGL_ARB_instanced_arrays)
    {
      glVertexAttribDivisorARB(attributeLocation, 0);
    }
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
