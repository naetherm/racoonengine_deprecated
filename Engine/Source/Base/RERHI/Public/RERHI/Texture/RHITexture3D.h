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
#include "RERHI/Texture/RHITexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract 3D texture interface
*
*  @remarks
*    Common use cases for 3D textures:
*    - 3D color correction lookup table (LUT)
*    - Volume rendering (medical area and nowadays in games as well)
*    - Light clusters 3D map
*/
class RHITexture3D : public RHITexture {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHITexture3D() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfTexture3Ds;
#endif
  }

  /**
  *  @brief
  *    Return the width of the texture
  *
  *  @return
  *    The width of the texture
  */
  [[nodiscard]] inline uint32_t getWidth() const {
    return mWidth;
  }

  /**
  *  @brief
  *    Return the height of the texture
  *
  *  @return
  *    The height of the texture
  */
  [[nodiscard]] inline uint32_t getHeight() const {
    return mHeight;
  }

  /**
  *  @brief
  *    Return the depth of the texture
  *
  *  @return
  *    The depth of the texture
  */
  [[nodiscard]] inline uint32_t getDepth() const {
    return mDepth;
  }

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rhi
  *    Owner RHI instance
  *  @param[in] width
  *    The width of the texture
  *  @param[in] height
  *    The height of the texture
  *  @param[in] depth
  *    The depth of the texture
  */
  inline RHITexture3D(RHIDynamicRHI &rhi, uint32_t width, uint32_t height, uint32_t depth
                      RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHITexture(ResourceType::TEXTURE_3D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
    mWidth(width),
    mHeight(height),
    mDepth(depth) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedTexture3Ds;
        ++rhi.getStatistics().currentNumberOfTexture3Ds;
#endif
  }

  explicit RHITexture3D(const RHITexture3D &source) = delete;

  RHITexture3D &operator=(const RHITexture3D &source) = delete;

  // Private data
private:
  uint32_t mWidth;  ///< The width of the texture
  uint32_t mHeight;  ///< The height of the texture
  uint32_t mDepth;  ///< The depth of the texture

};

typedef RECore::SmartRefCount<RHITexture3D> RHITexture3DPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
