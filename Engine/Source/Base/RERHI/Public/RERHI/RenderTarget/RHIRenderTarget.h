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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RERHI/RERHI.h"
#include "RERHI/RHIResource.h"
#include "RERHI/RenderTarget/RHIRenderPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract render target interface
*/
class RHIRenderTarget : public RHIResource {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIRenderTarget() override
  {
    mRenderPass.Release();
  }

  /**
  *  @brief
  *    Return the render pass
  *
  *  @return
  *    Render pass, don't release the reference unless you add an own reference
  */
  [[nodiscard]] inline RHIRenderPass &getRenderPass() const {
    return mRenderPass;
  }

  // Public virtual Rhi::RHIRenderTarget methods
public:
  /**
  *  @brief
  *    Return the width and height of the render target
  *
  *  @param[out] width
  *    Receives the width of the render target, guaranteed to be never ever zero
  *  @param[out] height
  *    Receives the height of the render target, guaranteed to be never ever zero
  */
  virtual void getWidthAndHeight(RECore::uint32 &width, RECore::uint32 &height) const = 0;

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] resourceType
  *    The resource type
  *  @param[in] renderPass
  *    Render pass to use, the render target keeps a reference to the render pass
  */
  inline RHIRenderTarget(ResourceType resourceType, RHIRenderPass &renderPass
                         RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHIResource(resourceType, renderPass.getRhi() RHI_RESOURCE_DEBUG_PASS_PARAMETER),
    mRenderPass(renderPass) {
    mRenderPass.AddReference();
  }

  explicit RHIRenderTarget(const RHIRenderTarget &source) = delete;

  RHIRenderTarget &operator=(const RHIRenderTarget &source) = delete;

  // Private data
private:
  RHIRenderPass &mRenderPass;  ///< Render pass to use, the render target keeps a reference to the render pass

};

typedef RECore::SmartRefCount<RHIRenderTarget> RHIRenderTargetPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
