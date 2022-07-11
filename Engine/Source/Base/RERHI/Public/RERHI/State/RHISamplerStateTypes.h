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
#include "RERHI/RHIDynamicRHITypes.h"


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
*    Filter mode
*
*  @note
*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb205060%28v=vs.85%29.aspx are used in here
*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
*    - "Point" = "nearest" in OpenGL terminology
*
*  @see
*    - "D3D12_FILTER"-documentation for details
*/
enum class FilterMode
{
  MIN_MAG_MIP_POINT						   = 0,		///< Use point sampling for minification, magnification, and mip-level sampling.
  MIN_MAG_POINT_MIP_LINEAR				   = 0x1,	///< Use point sampling for minification and magnification; use linear interpolation for mip-level sampling.
  MIN_POINT_MAG_LINEAR_MIP_POINT			   = 0x4,	///< Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling.
  MIN_POINT_MAG_MIP_LINEAR				   = 0x5,	///< Use point sampling for minification; use linear interpolation for magnification and mip-level sampling.
  MIN_LINEAR_MAG_MIP_POINT				   = 0x10,	///< Use linear interpolation for minification; use point sampling for magnification and mip-level sampling.
  MIN_LINEAR_MAG_POINT_MIP_LINEAR			   = 0x11,	///< Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling.
  MIN_MAG_LINEAR_MIP_POINT				   = 0x14,	///< Use linear interpolation for minification and magnification; use point sampling for mip-level sampling.
  MIN_MAG_MIP_LINEAR						   = 0x15,	///< Use linear interpolation for minification, magnification, and mip-level sampling.
  ANISOTROPIC								   = 0x55,	///< Use anisotropic interpolation for minification, magnification, and mip-level sampling.
  COMPARISON_MIN_MAG_MIP_POINT			   = 0x80,	///< Use point sampling for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_POINT_MIP_LINEAR		   = 0x81,	///< Use point sampling for minification and magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,	///< Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_POINT_MAG_MIP_LINEAR		   = 0x85,	///< Use point sampling for minification; use linear interpolation for magnification and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_LINEAR_MAG_MIP_POINT		   = 0x90,	///< Use linear interpolation for minification; use point sampling for magnification and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,	///< Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_LINEAR_MIP_POINT		   = 0x94,	///< Use linear interpolation for minification and magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
  COMPARISON_MIN_MAG_MIP_LINEAR			   = 0x95,	///< Use linear interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  COMPARISON_ANISOTROPIC					   = 0xd5,	///< Use anisotropic interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
  UNKNOWN									   = 0xd6	///< Unknown invalid setting
};

/**
*  @brief
*    Texture address mode
*
*  @note
*    - Original Direct3D comments from http://msdn.microsoft.com/en-us/library/windows/desktop/bb172483%28v=vs.85%29.aspx are used in here
*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
*
*  @see
*    - "D3D12_TEXTURE_ADDRESS_MODE"-documentation for details
*/
enum class TextureAddressMode
{
  WRAP		= 1,	///< Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
  MIRROR		= 2,	///< Flip the texture at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
  CLAMP		= 3,	///< Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
  BORDER		= 4,	///< Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in "SamplerState::borderColor".
  MIRROR_ONCE	= 5		///< Similar to "MIRROR" and "CLAMP". Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
};

/**
*  @brief
*    Sampler state
*
*  @remarks
*    == About mipmapping ==
*    The texture filter mode does not support explicitly disabling mipmapping. In case our texture does not have
*    any mipmaps, set "Rhi::SamplerState::maxLod" to zero in order to ensure a correct behaviour across the
*    difference graphics APIs. When not doing this you usually have no issues when using OpenGL, OpenGL ES 3, Direct 10,
*    Direct3D 11 or Direct3D 9 with the "ps_2_0"-profile, but when using Direct3D 9 with the "ps_3_0"-profile you might
*    get into trouble due to another internal graphics API behaviour.
*
*  @note
*    - This sampler state maps directly to Direct3D 10 & 11, do not change it
*    - If you want to know how the default values were chosen, have a look into the "Rhi::ISamplerState::getDefaultSamplerState()"-implementation
*
*  @see
*    - "D3D12_SAMPLER_DESC"-documentation for details
*/
struct SamplerState final
{
  FilterMode		   filter;			///< Default: "Rhi::FilterMode::MIN_MAG_MIP_LINEAR"
  TextureAddressMode addressU;		///< (also known as "S"), Default: "Rhi::TextureAddressMode::CLAMP"
  TextureAddressMode addressV;		///< (also known as "T"), Default: "Rhi::TextureAddressMode::CLAMP"
  TextureAddressMode addressW;		///< (also known as "R"), Default: "Rhi::TextureAddressMode::CLAMP"
  float			   mipLodBias;		///< Default: "0.0f"
  RECore::uint32		   maxAnisotropy;	///< Default: "16"
  ComparisonFunc	   comparisonFunc;	///< Default: "Rhi::ComparisonFunc::NEVER"
  float			   borderColor[4];	///< Default: 0.0f, 0.0f, 0.0f, 0.0f
  float			   minLod;			///< Default: -3.402823466e+38f (-FLT_MAX)
  float			   maxLod;			///< Default: 3.402823466e+38f (FLT_MAX)
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
