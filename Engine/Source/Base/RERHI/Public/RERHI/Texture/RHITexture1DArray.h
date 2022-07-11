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
*    Abstract 1D array texture interface
*
*  @note
*    - Common use case for 1D texture array: An 1D array texture is for example useful for storing multiple 1D Illuminating Engineering Society (IES) light profiles packed together in a single texture in order to avoid RHI state changes
*/
class RHITexture1DArray : public RHITexture {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHITexture1DArray() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
        --getRhi().getStatistics().currentNumberOfTexture1DArrays;
#endif
  }

  /**
  *  @brief
  *    Return the width of the texture
  *
  *  @return
  *    The width of the texture
  */
  [[nodiscard]] inline RECore::uint32 getWidth() const {
    return mWidth;
  }

  /**
  *  @brief
  *    Return the number of slices
  *
  *  @return
  *    The number of slices
  */
  [[nodiscard]] inline RECore::uint32 getNumberOfSlices() const {
    return mNumberOfSlices;
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
  *  @param[in] numberOfSlices
  *    The number of slices
  */
  inline RHITexture1DArray(RHIDynamicRHI &rhi, RECore::uint32 width, RECore::uint32 numberOfSlices
                           RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHITexture(ResourceType::TEXTURE_1D_ARRAY, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
    mWidth(width),
    mNumberOfSlices(numberOfSlices) {
#ifdef RHI_STATISTICS
    // Update the statistics
        ++rhi.getStatistics().numberOfCreatedTexture1DArrays;
        ++rhi.getStatistics().currentNumberOfTexture1DArrays;
#endif
  }

  explicit RHITexture1DArray(const RHITexture1DArray &source) = delete;

  RHITexture1DArray &operator=(const RHITexture1DArray &source) = delete;

  // Private data
private:
  RECore::uint32 mWidth;      ///< The width of the texture
  RECore::uint32 mNumberOfSlices;  ///< The number of slices

};

typedef RECore::SmartRefCount<RHITexture1DArray> RHITexture1DArrayPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
