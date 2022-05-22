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
#include "RERenderer/Resource/CompositorNode/Pass/ICompositorResourcePass.h"

// Disable warnings in external headers, we can't fix them
PRAGMA_WARNING_PUSH
	PRAGMA_WARNING_DISABLE_MSVC(4127)	// warning C4127: conditional expression is constant
	PRAGMA_WARNING_DISABLE_MSVC(4201)	// warning C4201: nonstandard extension used: nameless struct/union
	PRAGMA_WARNING_DISABLE_MSVC(4464)	// warning C4464: relative include path contains '..'
	PRAGMA_WARNING_DISABLE_MSVC(4324)	// warning C4324: '<x>': structure was padded due to alignment specifier
	#include <glm/glm.hpp>
PRAGMA_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassClear final : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("Clear");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint32 getFlags() const	// Combination of "RERHI::ClearFlag"
		{
			return mFlags;
		}

		[[nodiscard]] inline const glm::vec4& getClearColor() const
		{
			return mColor;
		}

		[[nodiscard]] inline float getZ() const
		{
			return mZ;
		}

		[[nodiscard]] inline RECore::uint32 getStencil() const
		{
			return mStencil;
		}


	//[-------------------------------------------------------]
	//[ Public virtual RERenderer::ICompositorResourcePass methods ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline virtual CompositorPassTypeId getTypeId() const override
		{
			return TYPE_ID;
		}

		virtual void deserialize(RECore::uint32 numberOfBytes, const RECore::uint8* data) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit CompositorResourcePassClear(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget),
			mFlags(0),
			mColor(0.0f, 0.0f, 0.0f, 0.0f),
			mZ(0.0f),	// 0 instead of 1 due to usage of Reversed-Z (see e.g. https://developer.nvidia.com/content/depth-precision-visualized and https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/)
			mStencil(0)
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassClear() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassClear(const CompositorResourcePassClear&) = delete;
		CompositorResourcePassClear& operator=(const CompositorResourcePassClear&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint32  mFlags;	///< Combination of "RERHI::ClearFlag"
		glm::vec4 mColor;
		float	  mZ;
		RECore::uint32  mStencil;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
