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
#include <RERHI/Buffer/RHIIndexBuffer.h>


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
*    Vulkan index buffer object (IBO) interface
*/
class IndexBuffer final : public RERHI::RHIIndexBuffer
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
  *    Number of bytes within the index buffer, must be valid
  *  @param[in] data
  *    Index buffer data, can be a null pointer (empty buffer)
  *  @param[in] bufferFlags
  *    Buffer flags, see "RERHI::BufferFlag"
  *  @param[in] bufferUsage
  *    Indication of the buffer usage
  *  @param[in] indexBufferFormat
  *    Index buffer data format
  */
  IndexBuffer(RHIDynamicRHI& vulkanRhi, RECore::uint32 numberOfBytes, const void* data, RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, RERHI::IndexBufferFormat::Enum indexBufferFormat RHI_RESOURCE_DEBUG_NAME_PARAMETER);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~IndexBuffer() override;

  /**
  *  @brief
  *    Return the Vulkan index type
  *
  *  @return
  *    The Vulkan index type
  */
  [[nodiscard]] inline VkIndexType getVkIndexType() const
  {
    return mVkIndexType;
  }

  /**
  *  @brief
  *    Return the Vulkan index buffer
  *
  *  @return
  *    The Vulkan index buffer
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
    RHI_DELETE(getRhi().getContext(), IndexBuffer, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit IndexBuffer(const IndexBuffer& source) = delete;
  IndexBuffer& operator =(const IndexBuffer& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  VkIndexType	   mVkIndexType;	///< Vulkan vertex type
  VkBuffer	   mVkBuffer;		///< Vulkan vertex buffer
  VkDeviceMemory mVkDeviceMemory;	///< Vulkan vertex memory


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
