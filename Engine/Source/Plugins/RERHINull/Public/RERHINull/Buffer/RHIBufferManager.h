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
#include "RERHINull/RERHINull.h"
#include <RERHI/Buffer/RHIBufferManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHINull {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null buffer manager interface
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
  *  @param[in] nullRhi
  *    Owner null RHI instance
  */
  explicit BufferManager(RHIDynamicRHI& nullRhi);

  /**
  *  @brief
  *    Destructor
  */
  virtual ~BufferManager() override;


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHIBufferManager methods            ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RERHI::RHIVertexBuffer* createVertexBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RECore::uint32 bufferFlags = 0, [[maybe_unused]] RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIIndexBuffer* createIndexBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RECore::uint32 bufferFlags = 0, [[maybe_unused]] RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW, [[maybe_unused]] RERHI::IndexBufferFormat::Enum indexBufferFormat = RERHI::IndexBufferFormat::UNSIGNED_SHORT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIVertexArray* createVertexArray([[maybe_unused]] const RERHI::VertexAttributes& vertexAttributes, RECore::uint32 numberOfVertexBuffers, const RERHI::VertexArrayVertexBuffer* vertexBuffers, RERHI::RHIIndexBuffer* indexBuffer = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITextureBuffer* createTextureBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RECore::uint32 bufferFlags = RERHI::BufferFlag::SHADER_RESOURCE, [[maybe_unused]] RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat = RERHI::TextureFormat::R32G32B32A32F RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIStructuredBuffer* createStructuredBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data, [[maybe_unused]] RECore::uint32 bufferFlags, [[maybe_unused]] RERHI::BufferUsage bufferUsage, [[maybe_unused]] RECore::uint32 numberOfStructureBytes RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIIndirectBuffer* createIndirectBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RECore::uint32 indirectBufferFlags = 0, [[maybe_unused]] RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHIUniformBuffer* createUniformBuffer([[maybe_unused]] RECore::uint32 numberOfBytes, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RERHI::BufferUsage bufferUsage = RERHI::BufferUsage::STATIC_DRAW RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  virtual void selfDestruct() override;


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
} // RERHINull
