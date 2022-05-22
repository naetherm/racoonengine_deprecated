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
#include "RERHIOpenGL/Buffer/RHIVertexArray.h"
#include "RERHIOpenGL/Buffer/RHIIndexBuffer.h"
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
VertexArray::~VertexArray() {
  // Release the index buffer reference
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->Release();
  }

  // Free the unique compact vertex array ID
  static_cast<RHIDynamicRHI &>(getRhi()).VertexArrayMakeId.destroyID(getId());
}

VertexArray::VertexArray(RHIDynamicRHI &openGLRhi, IndexBuffer *indexBuffer,
                         InternalResourceType::Enum internalResourceType, RECore::uint16 id
                         RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
  RHIVertexArray(openGLRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER),
  mIndexBuffer(indexBuffer),
  mInternalResourceType(internalResourceType) {
  // Add a reference to the given index buffer
  if (nullptr != mIndexBuffer) {
    mIndexBuffer->AddReference();
  }
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGL
