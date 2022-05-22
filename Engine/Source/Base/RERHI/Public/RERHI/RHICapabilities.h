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
#include "RERHI/Texture/RHITextureTypes.h"


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
*    Capabilities class
*
*  @note
*    - The data is public by intent in order to make it easier to use this class,
*      no issues involved because the user only gets a constant instance
*/
class Capabilities final
{

  // Public data
public:
  char				deviceName[128];								///< UTF-8 device name of the used graphics card (e.g. "AMD Radeon R9 200 Series")
  TextureFormat::Enum preferredSwapChainColorTextureFormat;			///< Preferred swap chain color texture format
  TextureFormat::Enum preferredSwapChainDepthStencilTextureFormat;	///< Preferred swap chain depth stencil texture format
  RECore::uint32			maximumNumberOfViewports;						///< Maximum number of viewports (always at least 1)
  RECore::uint32			maximumNumberOfSimultaneousRenderTargets;		///< Maximum number of simultaneous render targets (if <1 render to texture is not supported)
  RECore::uint32			maximumTextureDimension;						///< Maximum texture dimension (usually 2048, 4096, 8192 or 16384)
  RECore::uint32			maximumNumberOf1DTextureArraySlices;			///< Maximum number of 1D texture array slices (usually 512 up to 8192, in case there's no support for 1D texture arrays it's 0)
  RECore::uint32			maximumNumberOf2DTextureArraySlices;			///< Maximum number of 2D texture array slices (usually 512 up to 8192, in case there's no support for 2D texture arrays it's 0)
  RECore::uint32			maximumNumberOfCubeTextureArraySlices;			///< Maximum number of cube texture array slices (usually 512 up to 8192, in case there's no support for cube texture arrays it's 0)
  RECore::uint32			maximumTextureBufferSize;						///< Maximum texture buffer (TBO) size in texel (>65536, typically much larger than that of one-dimensional texture, in case there's no support for texture buffer it's 0)
  RECore::uint32			maximumStructuredBufferSize;					///< Maximum structured buffer size in bytes (>65536, typically much larger than that of one-dimensional texture, in case there's no support for structured buffer it's 0)
  RECore::uint32			maximumIndirectBufferSize;						///< Maximum indirect buffer size in bytes
  RECore::uint32			maximumUniformBufferSize;						///< Maximum uniform buffer (UBO) size in bytes (usually at least 4096 *16 bytes, in case there's no support for uniform buffer it's 0)
  RECore::uint8				maximumNumberOfMultisamples;					///< Maximum number of multisamples (always at least 1, usually 8)
  RECore::uint8				maximumAnisotropy;								///< Maximum anisotropy (always at least 1, usually 16)
  bool				upperLeftOrigin;								///< Upper left origin (true for Vulkan, Direct3D, OpenGL with "GL_ARB_clip_control"-extension)
  bool				zeroToOneClipZ;									///< Zero-to-one clip Z (true for Vulkan, Direct3D, OpenGL with "GL_ARB_clip_control"-extension)
  bool				individualUniforms;								///< Individual uniforms ("constants" in Direct3D terminology) supported? If not, only uniform buffer objects are supported.
  bool				instancedArrays;								///< Instanced arrays supported? (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
  bool				drawInstanced;									///< Draw instanced supported? (shader model 4 feature, build in shader variable holding the current instance ID)
  bool				baseVertex;										///< Base vertex supported for draw calls?
  bool				nativeMultithreading;							///< Does the RHI support native multithreading? For example Direct3D 11 does meaning we can also create RHI resources asynchronous while for OpenGL we have to create an separate OpenGL context (less efficient, more complex to implement).
  bool				shaderBytecode;									///< Shader bytecode supported?
  // Graphics
  bool				vertexShader;									///< Is there support for vertex shaders (VS)?
  RECore::uint32			maximumNumberOfPatchVertices;					///< Tessellation-control-shader (TCS) stage and tessellation-evaluation-shader (TES) stage: Maximum number of vertices per patch (usually 0 for no tessellation support or 32 which is the maximum number of supported vertices per patch)
  RECore::uint32			maximumNumberOfGsOutputVertices;				///< Geometry-shader (GS) stage: Maximum number of vertices a geometry shader (GS) can emit (usually 0 for no geometry shader support or 1024)
  bool				fragmentShader;									///< Is there support for fragment shaders (FS)?
  bool				meshShader;										///< Is there support for task shaders (TS) and mesh shaders (MS)?
  // Compute
  bool				computeShader;									///< Is there support for compute shaders (CS)?

  // Public methods
public:
  /**
  *  @brief
  *    Default constructor
  */
  inline Capabilities() :
    deviceName{},
    preferredSwapChainColorTextureFormat(TextureFormat::Enum::UNKNOWN),
    preferredSwapChainDepthStencilTextureFormat(TextureFormat::Enum::UNKNOWN),
    maximumNumberOfViewports(0),
    maximumNumberOfSimultaneousRenderTargets(0),
    maximumTextureDimension(0),
    maximumNumberOf1DTextureArraySlices(0),
    maximumNumberOf2DTextureArraySlices(0),
    maximumNumberOfCubeTextureArraySlices(0),
    maximumTextureBufferSize(0),
    maximumStructuredBufferSize(0),
    maximumIndirectBufferSize(0),
    maximumUniformBufferSize(0),
    maximumNumberOfMultisamples(1),
    maximumAnisotropy(1),
    upperLeftOrigin(true),
    zeroToOneClipZ(true),
    individualUniforms(false),
    instancedArrays(false),
    drawInstanced(false),
    baseVertex(false),
    nativeMultithreading(false),
    shaderBytecode(false),
    vertexShader(false),
    maximumNumberOfPatchVertices(0),
    maximumNumberOfGsOutputVertices(0),
    fragmentShader(false),
    meshShader(false),
    computeShader(false)
  {}

  /**
  *  @brief
  *    Destructor
  */
  inline ~Capabilities()
  {}

  // Private methods
private:
  explicit Capabilities(const Capabilities& source) = delete;
  Capabilities& operator =(const Capabilities& source) = delete;

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
