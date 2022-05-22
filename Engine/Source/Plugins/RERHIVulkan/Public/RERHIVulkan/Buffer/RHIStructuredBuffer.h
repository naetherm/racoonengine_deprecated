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
#include "RERHIVulkan/RERHIVulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


/**
*  @brief
*    Vulkan structured buffer object (SBO) interface
*/
class StructuredBuffer final : public RERHI::RHIStructuredBuffer
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] vulkanRhi
  *    Owner Vulkan RHI instance
  *  @param[in] numberOfBytes
  *    Number of bytes within the structured buffer, must be valid
  *  @param[in] data
  *    Structured buffer data, can be a null pointer (empty buffer)
  *  @param[in] bufferUsage
  *    Indication of the buffer usage
  *  @param[in] numberOfStructureBytes
  *    Number of structure bytes
  */
  StructuredBuffer(RHIDynamicRHI& vulkanRhi, RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~StructuredBuffer() override;

  /**
  *  @brief
  *    Return the Vulkan structured buffer
  *
  *  @return
  *    The Vulkan structured buffer
  */
  [[nodiscard]] inline VkBuffer getVkBuffer() const
  {
    return mVkBuffer;
  }

  /**
  *  @brief
  *    Return the Vulkan device memory
  *
  *  @return
  *    The Vulkan device memory
  */
  [[nodiscard]] inline VkDeviceMemory getVkDeviceMemory() const
  {
    return mVkDeviceMemory;
  }


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), StructuredBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit StructuredBuffer(const StructuredBuffer& source) = delete;
  StructuredBuffer& operator =(const StructuredBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VkBuffer	   mVkBuffer;		///< Vulkan uniform texel buffer
  VkDeviceMemory mVkDeviceMemory;	///< Vulkan uniform texel memory


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
