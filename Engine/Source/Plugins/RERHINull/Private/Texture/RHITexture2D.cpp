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
#include "RERHINull/Texture/RHITexture2D.h"
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
*  @param[in] width
*    The width of the texture
*  @param[in] height
*    The height of the texture
*/
Texture2D::Texture2D(RHIDynamicRHI& nullRhi, uint32_t width, uint32_t height RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
RHITexture2D(nullRhi, width, height RHI_RESOURCE_DEBUG_PASS_PARAMETER)
{}

/**
*  @brief
*    Destructor
*/
Texture2D::~Texture2D()
{}


void Texture2D::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), Texture2D, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
