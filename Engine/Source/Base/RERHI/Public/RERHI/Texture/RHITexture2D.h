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
namespace RERHI
{


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract 2D texture interface
*
*  @note
*    - Common use case for 2D texture: Pretty much every standard 2D texture
*/
class RHITexture2D : public RHITexture
{

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHITexture2D() override
  {
#ifdef RHI_STATISTICS
    // Update the statistics
				--getRhi().getStatistics().currentNumberOfTexture2Ds;
#endif
  }

  /**
  *  @brief
  *    Return the width of the texture
  *
  *  @return
  *    The width of the texture
  */
  [[nodiscard]] inline RECore::uint32 getWidth() const
  {
    return mWidth;
  }

  /**
  *  @brief
  *    Return the height of the texture
  *
  *  @return
  *    The height of the texture
  */
  [[nodiscard]] inline RECore::uint32 getHeight() const
  {
    return mHeight;
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
  */
  inline RHITexture2D(RHIDynamicRHI& rhi, RECore::uint32 width, RECore::uint32 height RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT) :
    RHITexture(ResourceType::TEXTURE_2D, rhi RHI_RESOURCE_DEBUG_PASS_PARAMETER),
    mWidth(width),
    mHeight(height)
  {
#ifdef RHI_STATISTICS
    // Update the statistics
				++rhi.getStatistics().numberOfCreatedTexture2Ds;
				++rhi.getStatistics().currentNumberOfTexture2Ds;
#endif
  }

  explicit RHITexture2D(const RHITexture2D& source) = delete;
  RHITexture2D& operator =(const RHITexture2D& source) = delete;

  // Private data
private:
  RECore::uint32 mWidth;	///< The width of the texture
  RECore::uint32 mHeight;	///< The height of the texture

};

typedef RECore::SmartRefCount<RHITexture2D> RHITexture2DPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
