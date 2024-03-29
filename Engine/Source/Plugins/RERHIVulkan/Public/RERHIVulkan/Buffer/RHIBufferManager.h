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


/**
*  @brief
*    Vulkan buffer manager interface
*/
class BufferManager final : public RERHI::RHIBufferManager
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
  */
  explicit BufferManager(RHIDynamicRHI& vulkanRhi);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~BufferManager() override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIBufferManager methods            ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RERHI::RHIVertexBuffer* createVertexBuffer(RECore::uint32 numberOfBytes, const void* data = nullptr, RECore::uint32 bufferFlags = 0, RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIIndexBuffer* createIndexBuffer(RECore::uint32 numberOfBytes, const void* data = nullptr, RECore::uint32 bufferFlags = 0, RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW, RERHI::IndexBufferFormat::Enum indexBufferFormat = RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIVertexArray* createVertexArray(const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITextureBuffer* createTextureBuffer(RECore::uint32 numberOfBytes, const void* data = nullptr, RECore::uint32 bufferFlags = RERHI::BufferFlag::SHADER_RESOURCE, RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW, RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIStructuredBuffer* createStructuredBuffer(RECore::uint32 numberOfBytes, const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, RERHI::BufferUsage bufferUsage, RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIIndirectBuffer* createIndirectBuffer(RECore::uint32 numberOfBytes, const void* data = nullptr, RECore::uint32 indirectBufferFlags = 0, RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIUniformBuffer* createUniformBuffer(RECore::uint32 numberOfBytes, const void* data = nullptr, RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), BufferManager, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit BufferManager(const BufferManager& source) = delete;
  BufferManager& operator =(const BufferManager& source) = delete;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIVulkan
