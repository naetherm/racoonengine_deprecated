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
#include "RERHI/RHIResourceTypes.h"
#include "RHIResourceTypes.h"
#include <RECore/Core/RefCount.h>
#include <RECore/Core/SmartRefCount.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract resource interface
*/
class RHIResource : public RECore::RefCount<RHIResource> {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHIResource() override
  {}

#ifdef DEBUG
  /**
      *  @brief
      *    Return the resource debug name
      *
      *  @return
      *    The resource debug name, never a null pointer and at least an empty string
      */
      [[nodiscard]] inline const char* getDebugName() const
      {
        return mDebugName;
      }
#endif

  /**
  *  @brief
  *    Return the resource type
  *
  *  @return
  *    The resource type
  */
  [[nodiscard]] inline ResourceType getResourceType() const {
    return mResourceType;
  }

  /**
  *  @brief
  *    Return the owner RHI instance
  *
  *  @return
  *    The owner RHI instance, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline RHIDynamicRHI &getRhi() const {
    return *mRhi;
  }

  // Public virtual Rhi::RHIResource methods
public:
  //[-------------------------------------------------------]
  //[ Internal                                              ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Return the RHI implementation specific resource handle (e.g. native Direct3D texture pointer or OpenGL texture ID)
  *
  *  @return
  *    The RHI implementation specific resource handle, can be a null pointer
  *
  *  @note
  *    - Don't use this RHI implementation specific method if you don't have to
  */
  [[nodiscard]] inline virtual void *getInternalResourceHandle() const {
    return nullptr;
  }

  // Protected methods
protected:
#ifdef DEBUG
  /**
      *  @brief
      *    Constructor
      *
      *  @param[in] resourceType
      *    The resource type
      *
      *  @note
      *    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
      */
      inline RHIResource(ResourceType resourceType, const char debugName[]) :
        mResourceType(resourceType),
        mRhi(nullptr)	// Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
      {
        ASSERT(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
        strncpy(mDebugName, debugName, 256);
        mDebugName[255] = '\0';
      }

      /**
      *  @brief
      *    Constructor
      *
      *  @param[in] resourceType
      *    The resource type
      *  @param[in] rhi
      *    Owner RHI instance
      */
      inline RHIResource(ResourceType resourceType, IRhi& rhi, const char debugName[]) :
        mResourceType(resourceType),
        mRhi(&rhi)
      {
        RE_LOG(strlen(debugName) < 256, "Resource debug name is not allowed to exceed 255 characters")
        strncpy(mDebugName, debugName, 256);
        mDebugName[255] = '\0';
      }
#else

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] resourceType
  *    The resource type
  *
  *  @note
  *    - Only used for rare border cases, use the constructor with the RHI reference whenever possible
  */
  inline explicit RHIResource(ResourceType resourceType) :
    mResourceType(resourceType),
    mRhi(
      nullptr)  // Only used for rare border cases, use the constructor with the RHI reference whenever possible. Normally the RHI pointer should never ever be a null pointer. So if you're in here, you're considered to be evil.
  {}

  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] resourceType
  *    The resource type
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline RHIResource(ResourceType resourceType, RHIDynamicRHI &rhi) :
    mResourceType(resourceType),
    mRhi(&rhi) {}

#endif

  explicit RHIResource(const RHIResource &source) = delete;

  RHIResource &operator=(const RHIResource &source) = delete;

  // Private data
private:
#ifdef DEBUG
  char mDebugName[256];	///< Debug name for easier resource identification when debugging, contains terminating zero, first member variable by intent to see it at once during introspection (debug memory layout change is no problem here)
#endif
  ResourceType mResourceType;  ///< The resource type
  RHIDynamicRHI *mRhi;      ///< The owner RHI instance, always valid

};

typedef RECore::SmartRefCount<RHIResource> RHIResourcePtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
