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
#include "RERHINull/Buffer/RHIVertexArray.h"
#include "RERHINull/RHIDynamicRHI.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] nullRhi
*    Owner null RHI instance
*  @param[in] id
*    The unique compact vertex array ID
*/
VertexArray::VertexArray(RHIDynamicRHI& nullRhi, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIVertexArray(nullRhi, id RHI_RESOURCE_DEBUG_PASS_PARAMETER)
{}

/**
*  @brief
*    Destructor
*/
VertexArray::~VertexArray()
{
  // Free the unique compact vertex array ID
  static_cast<RHIDynamicRHI&>(getRhi()).VertexArrayMakeId.destroyID(getId());
}


void VertexArray::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), VertexArray, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
