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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RERenderer
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef RECore::uint32 MaterialTechniqueId;	///< Material technique identifier, result of hashing the material technique name via "RERenderer::StringId"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class CompositorResourcePassScene : public ICompositorResourcePass
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CompositorPassFactory;	// The only one allowed to create instances of this class


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static constexpr RECore::uint32 TYPE_ID = STRING_ID("Scene");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		[[nodiscard]] inline RECore::uint8 getMinimumRenderQueueIndex() const	///< Inclusive
		{
			return mMinimumRenderQueueIndex;
		}

		[[nodiscard]] inline RECore::uint8 getMaximumRenderQueueIndex() const	///< Inclusive
		{
			return mMaximumRenderQueueIndex;
		}

		[[nodiscard]] inline bool isTransparentPass() const
		{
			return mTransparentPass;
		}

		[[nodiscard]] inline MaterialTechniqueId getMaterialTechniqueId() const
		{
			return mMaterialTechniqueId;
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
		[[nodiscard]] virtual bool getRenderQueueIndexRange(RECore::uint8& minimumRenderQueueIndex, RECore::uint8& maximumRenderQueueIndex) const override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline explicit CompositorResourcePassScene(const CompositorTarget& compositorTarget) :
			ICompositorResourcePass(compositorTarget),
			mMinimumRenderQueueIndex(0),
			mMaximumRenderQueueIndex(255),
			mTransparentPass(false),
			mMaterialTechniqueId(RECore::getInvalid<MaterialTechniqueId>())
		{
			// Nothing here
		}

		inline virtual ~CompositorResourcePassScene() override
		{
			// Nothing here
		}

		explicit CompositorResourcePassScene(const CompositorResourcePassScene&) = delete;
		CompositorResourcePassScene& operator=(const CompositorResourcePassScene&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RECore::uint8				mMinimumRenderQueueIndex;	///< Inclusive
		RECore::uint8				mMaximumRenderQueueIndex;	///< Inclusive
		bool				mTransparentPass;
		MaterialTechniqueId	mMaterialTechniqueId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
