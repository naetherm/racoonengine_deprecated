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
	*    Depth write mask
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_DEPTH_WRITE_MASK"-documentation for details
	*/
	enum class DepthWriteMask
	{
		ZERO = 0,
		ALL  = 1
	};

	/**
	*  @brief
	*    Stencil operation
	*
	*  @note
	*    - These constants directly map to Direct3D 10 & 11 & 12 constants, do not change them
	*
	*  @see
	*    - "D3D12_STENCIL_OP"-documentation for details
	*/
	enum class StencilOp
	{
		KEEP	 = 1,
		ZERO	 = 2,
		REPLACE	 = 3,
		INCR_SAT = 4,
		DECR_SAT = 5,
		INVERT	 = 6,
		INCREASE = 7,
		DECREASE = 8
	};

	/**
	*  @brief
	*    Depth stencil operation description
	*
	*  @note
	*    - This depth stencil operation description maps directly to Direct3D 10 & 11 & 12, do not change it
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
	*
	*  @see
	*    - "D3D12_DEPTH_STENCILOP_DESC"-documentation for details
	*/
	struct DepthStencilOpDesc final
	{
		StencilOp		stencilFailOp;		///< Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilDepthFailOp;	///< Default: "Rhi::StencilOp::KEEP"
		StencilOp		stencilPassOp;		///< Default: "Rhi::StencilOp::KEEP"
		ComparisonFunc	stencilFunc;		///< Default: "Rhi::ComparisonFunc::ALWAYS"
	};

	/**
	*  @brief
	*    Depth stencil state
	*
	*  @note
	*    - This depth stencil state maps directly to Direct3D 10 & 11 & 12, do not change it
	*    - This also means that "int" is used over "bool" because in Direct3D it's defined this way
	*    - If you want to know how the default values were chosen, have a look into the "Rhi::DepthStencilStateBuilder::getDefaultDepthStencilState()"-implementation
	*
	*  @see
	*    - "D3D12_DEPTH_STENCIL_DESC"-documentation for details
	*/
	struct DepthStencilState final
	{
		int					depthEnable;		///< Boolean value. Default: "true"
		DepthWriteMask		depthWriteMask;		///< Default: "Rhi::DepthWriteMask::ALL"
		ComparisonFunc		depthFunc;			///< Default: "Rhi::ComparisonFunc::GREATER" instead of "Rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
		int					stencilEnable;		///< Boolean value. Default: "false"
		RECore::uint8				stencilReadMask;	///< Default: "0xff"
		RECore::uint8				stencilWriteMask;	///< Default: "0xff"
		DepthStencilOpDesc	frontFace;			///< Default: See "Rhi::DepthStencilOpDesc"
		DepthStencilOpDesc	backFace;			///< Default: See "Rhi::DepthStencilOpDesc"
	};
	struct DepthStencilStateBuilder final
	{
		/**
		*  @brief
		*    Return the default depth stencil state
		*
		*  @return
		*    The default depth stencil state, see "Rhi::DepthStencilState" for the default values
		*/
		[[nodiscard]] static inline const DepthStencilState& getDefaultDepthStencilState()
		{
			// As default values, the one of Direct3D 11 and Direct 10 were chosen in order to make it easier for those RHI implementations
			// (choosing OpenGL default values would bring no benefit due to the design of the OpenGL API)
			// - Direct3D 11 "D3D11_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/ff476110%28v=vs.85%29.aspx
			// - Direct3D 10 "D3D10_DEPTH_STENCIL_DESC structure"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb205036%28v=vs.85%29.aspx
			// - Direct3D 9 "D3DRENDERSTATETYPE enumeration"-documentation at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb172599%28v=vs.85%29.aspx
			// - OpenGL & OpenGL ES 3: The official specifications (unlike Direct3D, OpenGL versions are more compatible to each other)

			// Return default values
			static constexpr DepthStencilState DEPTH_STENCIL_STATE =
			{																		//	Direct3D 11		Direct3D 10	Direct3D 9				OpenGL
				true,						// depthEnable (int)						"true"			"true"		"true"					TODO(naetherm)
				DepthWriteMask::ALL,		// depthWriteMask (Rhi::DepthWriteMask)		"ALL"			"ALL"		"ALL"					TODO(naetherm)
				ComparisonFunc::GREATER,	// depthFunc (Rhi::ComparisonFunc)			"LESS"			"LESS"		"LESS_EQUAL"			TODO(naetherm)	- Default: "Rhi::ComparisonFunc::GREATER" instead of "Rhi::ComparisonFunc::LESS" due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
				false,						// stencilEnable (int)						"false"			"false"		"false"					TODO(naetherm)
				0xff,						// stencilReadMask (RECore::uint8)				"0xff"			"0xff"		"0xffffffff"			TODO(naetherm)
				0xff,						// stencilWriteMask (RECore::uint8)				"0xff"			"0xff"		"0xffffffff"			TODO(naetherm)
				{ // sFrontFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)
				},
				{ // sBackFace (Rhi::DepthStencilOpDesc)
					StencilOp::KEEP,		// stencilFailOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					StencilOp::KEEP,		// stencilDepthFailOp (Rhi::StencilOp)		"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					StencilOp::KEEP,		// stencilPassOp (Rhi::StencilOp)			"KEEP"			"KEEP"		"KEEP" (both sides)		TODO(naetherm)
					ComparisonFunc::ALWAYS	// stencilFunc (Rhi::ComparisonFunc)		"ALWAYS"		"ALWAYS"	"ALWAYS" (both sides)	TODO(naetherm)
				}
			};
			return DEPTH_STENCIL_STATE;
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RERHI
