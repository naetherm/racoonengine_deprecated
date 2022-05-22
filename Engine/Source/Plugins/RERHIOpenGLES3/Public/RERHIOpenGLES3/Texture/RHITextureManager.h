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
#include "RERHIOpenGLES3/RERHIOpenGLES3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHIOpenGLES3 {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class RHIDynamicRHI;
class IExtensions;


/**
*  @brief
*    OpenGL ES 3 texture manager interface
*/
class TextureManager final : public RERHI::RHITextureManager
{


  //[-------------------------------------------------------]
  //[ Public methods                                        ]
  //[-------------------------------------------------------]
public:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] openGLES3Rhi
  *    Owner OpenGL ES 3 RHI instance
  */
  explicit TextureManager(RHIDynamicRHI& openGLES3Rhi);

  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~TextureManager() override
  {}


  //[-------------------------------------------------------]
  //[ Public virtual RERHI::RHITextureManager methods           ]
  //[-------------------------------------------------------]
public:
  [[nodiscard]] virtual RERHI::RHITexture1D* createTexture1D(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITexture1DArray* createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITexture2D* createTexture2D(RECore::uint32 width, RECore::uint32 height, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT, [[maybe_unused]] RECore::uint8 numberOfMultisamples = 1, [[maybe_unused]] const RERHI::OptimizedTextureClearValue* optimizedTextureClearValue = nullptr RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITexture2DArray* createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITexture3D* createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITextureCube* createTextureCube(RECore::uint32 width, RERHI::TextureFormat::Enum textureFormat, const void* data = nullptr, RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;

  [[nodiscard]] virtual RERHI::RHITextureCubeArray* createTextureCubeArray([[maybe_unused]] RECore::uint32 width, [[maybe_unused]] RECore::uint32 numberOfSlices, [[maybe_unused]] RERHI::TextureFormat::Enum textureFormat, [[maybe_unused]] const void* data = nullptr, [[maybe_unused]] RECore::uint32 textureFlags = 0, [[maybe_unused]] RERHI::TextureUsage textureUsage = RERHI::TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) override;


  //[-------------------------------------------------------]
  //[ Protected virtual RERHI::RefCount methods               ]
  //[-------------------------------------------------------]
protected:
  inline virtual void selfDestruct() override
  {
    RHI_DELETE(getRhi().getContext(), TextureManager, this);
  }


  //[-------------------------------------------------------]
  //[ Private methods                                       ]
  //[-------------------------------------------------------]
private:
  explicit TextureManager(const TextureManager& source) = delete;
  TextureManager& operator =(const TextureManager& source) = delete;


  //[-------------------------------------------------------]
  //[ Private data                                          ]
  //[-------------------------------------------------------]
private:
  const IExtensions* mExtensions;	///< Extensions instance, always valid


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHIOpenGLES3
