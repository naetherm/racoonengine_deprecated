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
#include "RERHI/RHIResource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERHI {


class RHITexture1D;

class RHITexture1DArray;

class RHITexture2D;

class RHITexture2DArray;

class RHITexture3D;

class RHITextureCube;

class RHITextureCubeArray;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract texture manager interface
*
*  @remarks
*    The texture manager is responsible for managing fine granular instances of
*    - 1D texture ("Rhi::ITexture1D")
*    - 1D texture array ("Rhi::ITexture1DArray")
*    - 2D texture ("Rhi::ITexture2D")
*    - 2D texture array ("Rhi::ITexture2DArray")
*    - 3D texture ("Rhi::ITexture3D")
*    - Cube texture ("Rhi::ITextureCube")
*    - Cube texture array ("Rhi::ITextureCubeArray")
*/
class RHITextureManager : public RECore::RefCount<RHITextureManager> {

  // Public methods
public:
  /**
  *  @brief
  *    Destructor
  */
  inline virtual ~RHITextureManager() override
  {}

  /**
  *  @brief
  *    Return the owner RHI instance
  *
  *  @return
  *    The owner RHI instance, do not release the returned instance unless you added an own reference to it
  */
  [[nodiscard]] inline RHIDynamicRHI &getRhi() const {
    return mRhi;
  }

  // Public virtual Rhi::RHITextureManager methods
public:
  //[-------------------------------------------------------]
  //[ Resource creation                                     ]
  //[-------------------------------------------------------]
  /**
  *  @brief
  *    Create a 1D texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created 1D texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @note
  *    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
  */
  [[nodiscard]] virtual RHITexture1D *
  createTexture1D(RECore::uint32 width, TextureFormat::Enum textureFormat, const void *data = nullptr,
                  RECore::uint32 textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT
                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a 1D array texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] numberOfSlices
  *    Number of slices, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created 1D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @remarks
  *    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
  *    be in CRN-texture layout, which means organized in mip-major order, like this:
  *    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  *    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  *    (DDS-texture layout is using face-major order)
  *
  *  @note
  *    - Only supported if "Rhi::Capabilities::maximumNumberOf1DTextureArraySlices" is not 0
  */
  [[nodiscard]] virtual RHITexture1DArray *
  createTexture1DArray(RECore::uint32 width, RECore::uint32 numberOfSlices, TextureFormat::Enum textureFormat,
                       const void *data = nullptr, RECore::uint32 textureFlags = 0,
                       TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a 2D texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] height
  *    Texture height, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture data format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *  @param[in] numberOfMultisamples
  *    The number of multisamples per pixel (valid values: 1, 2, 4, 8)
  *  @param[in] optimizedTextureClearValue
  *    Optional optimized texture clear value
  *
  *  @return
  *    The created 2D texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @note
  *    - The following texture data layout is expected: Mip0, Mip1, Mip2, Mip3 ...
  */
  [[nodiscard]] virtual RHITexture2D *
  createTexture2D(RECore::uint32 width, RECore::uint32 height, TextureFormat::Enum textureFormat, const void *data = nullptr,
                  RECore::uint32 textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT,
                  RECore::uint8 numberOfMultisamples = 1,
                  const OptimizedTextureClearValue *optimizedTextureClearValue = nullptr
                  RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a 2D array texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] height
  *    Texture height, must be >0 else a null pointer is returned
  *  @param[in] numberOfSlices
  *    Number of slices, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created 2D array texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @remarks
  *    The texture array data consists of a sequence of texture slices. Each the texture slice data of a single texture slice has to
  *    be in CRN-texture layout, which means organized in mip-major order, like this:
  *    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
  *    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  *    (DDS-texture layout is using face-major order)
  *
  *  @note
  *    - Only supported if "Rhi::Capabilities::maximumNumberOf2DTextureArraySlices" is not 0
  */
  [[nodiscard]] virtual RHITexture2DArray *
  createTexture2DArray(RECore::uint32 width, RECore::uint32 height, RECore::uint32 numberOfSlices, TextureFormat::Enum textureFormat,
                       const void *data = nullptr, RECore::uint32 textureFlags = 0,
                       TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a 3D texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] height
  *    Texture height, must be >0 else a null pointer is returned
  *  @param[in] depth
  *    Texture depth, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created 3D texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @remarks
  *    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
  *    - Mip0: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5 (= positive X, negative X, positive Y, negative Y, positive Z, negative Z)
  *    - Mip1: Slice0, Slice1, Slice2, Slice3, Slice4, Slice5
  *    (DDS-texture layout is using face-major order)
  */
  [[nodiscard]] virtual RHITexture3D *
  createTexture3D(RECore::uint32 width, RECore::uint32 height, RECore::uint32 depth, TextureFormat::Enum textureFormat,
                  const void *data = nullptr, RECore::uint32 textureFlags = 0,
                  TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a cube texture instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created cube texture instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @remarks
  *    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
  *    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
  *    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
  *    (DDS-texture layout is using face-major order)
  */
  [[nodiscard]] virtual RHITextureCube *
  createTextureCube(RECore::uint32 width, TextureFormat::Enum textureFormat, const void *data = nullptr,
                    RECore::uint32 textureFlags = 0, TextureUsage textureUsage = TextureUsage::DEFAULT
                    RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  /**
  *  @brief
  *    Create a cube texture array instance
  *
  *  @param[in] width
  *    Texture width, must be >0 else a null pointer is returned
  *  @param[in] numberOfSlices
  *    Number of slices, must be >0 else a null pointer is returned
  *  @param[in] textureFormat
  *    Texture format
  *  @param[in] data
  *    Texture data, can be a null pointer, the data is internally copied and you have to free your memory if you no longer need it
  *  @param[in] textureFlags
  *    Texture flags, see "Rhi::TextureFlag::Enum"
  *  @param[in] textureUsage
  *    Indication of the texture usage
  *
  *  @return
  *    The created cube texture array instance, null pointer on error. Release the returned instance if you no longer need it.
  *
  *  @remarks
  *    The texture data has to be in CRN-texture layout, which means organized in mip-major order, like this:
  *    - Mip0: Face0, Face1, Face2, Face3, Face4, Face5
  *    - Mip1: Face0, Face1, Face2, Face3, Face4, Face5
  *    (DDS-texture layout is using face-major order)
  *
  *  @note
  *    - Only supported if "Rhi::Capabilities::maximumNumberOfCubeTextureArraySlices" is not 0
  */
  [[nodiscard]] virtual RHITextureCubeArray *
  createTextureCubeArray(RECore::uint32 width, RECore::uint32 numberOfSlices, TextureFormat::Enum textureFormat,
                         const void *data = nullptr, RECore::uint32 textureFlags = 0,
                         TextureUsage textureUsage = TextureUsage::DEFAULT RHI_RESOURCE_DEBUG_NAME_PARAMETER) = 0;

  // Protected methods
protected:
  /**
  *  @brief
  *    Constructor
  *
  *  @param[in] rhi
  *    Owner RHI instance
  */
  inline explicit RHITextureManager(RHIDynamicRHI &rhi) :
    mRhi(rhi) {}

  explicit RHITextureManager(const RHITextureManager &source) = delete;

  RHITextureManager &operator=(const RHITextureManager &source) = delete;

  // Private data
private:
  RHIDynamicRHI &mRhi;  ///< The owner RHI instance

};

typedef RECore::SmartRefCount<RHITextureManager> RHITextureManagerPtr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
