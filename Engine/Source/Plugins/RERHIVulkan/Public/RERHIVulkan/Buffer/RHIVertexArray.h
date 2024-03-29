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
#include "RERHIVulkan/RERHIVulkan.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIVulkan {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class IndexBuffer;
class VertexBuffer;


/**
*  @brief
*    Vulkan vertex array interface
*/
class VertexArray final : public RERHI::RHIVertexArray
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
  *  @param[in] vertexAttributes
  *    Vertex attributes ("vertex declaration" in Direct3D 9 terminology, "input layout" in Direct3D 10 & 11 terminology)
  *  @param[in] numberOfVertexBuffers
  *    Number of vertex buffers, having zero vertex buffers is valid
  *  @param[in] vertexBuffers
  *    At least numberOfVertexBuffers instances of vertex array vertex buffers, can be a null pointer in case there are zero vertex buffers, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] indexBuffer
  *    Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
  *  @param[in] id
  *    The unique compact vertex array ID
  */
  VertexArray(RHIDynamicRHI& vulkanRhi, const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, IndexBuffer* indexBuffer, RECore::uint16 id RHI_RESOURCE_DEBUG_NAME_PARAMETER_NO_DEFAULT);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~VertexArray() override;

  /**
  *  @brief
  *    Return the used index buffer
  *
  *  @return
  *    The used index buffer, can be a null pointer, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline IndexBuffer* getIndexBuffer() const
  {
    return mIndexBuffer;
  }

  /**
  *  @brief
  *    Bind Vulkan buffers
  *
  *  @param[in] vkCommandBuffer
  *    Vulkan command buffer to write into
  */
  void bindVulkanBuffers(VkCommandBuffer vkCommandBuffer) const;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), VertexArray, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit VertexArray(const VertexArray& source) = delete;
  VertexArray& operator =(const VertexArray& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  IndexBuffer*   mIndexBuffer;		///< Optional index buffer to use, can be a null pointer, the vertex array instance keeps a reference to the index buffer
  // Vulkan input slots
  RECore::uint32	   mNumberOfSlots;		///< Number of used Vulkan input slots
  VkBuffer*	   mVertexVkBuffers;	///< Vulkan vertex buffers
  RECore::uint32*	   mStrides;			///< Strides in bytes, if "mVertexVkBuffers" is no null pointer this is no null pointer as well
  VkDeviceSize*  mOffsets;			///< Offsets in bytes, if "mVertexVkBuffers" is no null pointer this is no null pointer as well
  // For proper vertex buffer reference counter behaviour
  VertexBuffer** mVertexBuffers;		///< Vertex buffers (we keep a reference to it) used by this vertex array, can be a null pointer


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
