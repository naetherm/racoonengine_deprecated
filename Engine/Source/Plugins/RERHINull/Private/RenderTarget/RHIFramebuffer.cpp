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
#include "RERHINull/RenderTarget/RHIFramebuffer.h"
#include "RERHINull/RenderTarget/RHIRenderPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {

/**
*  @brief
*    Constructor
*
*  @param[in] renderPass
*    Render pass to use, the swap chain keeps a reference to the render pass
*/
Framebuffer::Framebuffer(RERHI::RHIRenderPass& renderPass RHI_RESOURCE_DEBUG_NAME_PARAMETER) :
RHIFramebuffer(renderPass RHI_RESOURCE_DEBUG_PASS_PARAMETER)
  {}

/**
*  @brief
*    Destructor
*/
Framebuffer::~Framebuffer()
{}


void Framebuffer::getWidthAndHeight(RECore::uint32& width, RECore::uint32& height) const
{
  // TODO(naetherm) Better implementation instead of just returning one (not that important, but would be nice)
  width  = 1;
  height = 1;
}


void Framebuffer::selfDestruct()
{
  RHI_DELETE(getRhi().getContext(), Framebuffer, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHINull
